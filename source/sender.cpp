#include "sender.h"
#include "ptpstructs.h"
#include "timeutils.h"
#include "mac.h"
#include "ptpmonkeyimplementation.h"
#include <cmath>
#include "ptpparser.h"

#ifdef __GNU__
#include <linux/socket.h>
#include <linux/net_tstamp.h>
#include <time.h>
#endif // __GNU__
#include <iostream>
#include <iomanip>


using namespace ptpmonkey;

Sender::Sender(PtpMonkeyImplementation& manager, asio::io_context& io_context, const IpAddress& outboundIpAddress, const asio::ip::address& multicast_address,
        unsigned short nPort) : m_manager(manager),
          m_outboundIpAddress(outboundIpAddress),
          m_endpoint(multicast_address, nPort),
          m_socket(io_context, m_endpoint.protocol()),
          m_timer(io_context),
          m_nSequence(0),
          m_bTimestampEnabled(false)
{

}

void Sender::Run()
{

    asio::ip::multicast::outbound_interface option(asio::ip::address_v4::from_string(m_outboundIpAddress.Get()));
    m_socket.set_option(option);

    #ifdef __GNU__

    //@todo need to do more checking that we support this before doing it.
    int flags = SOF_TIMESTAMPING_TX_SOFTWARE | SOF_TIMESTAMPING_TX_HARDWARE;
    int nError = setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_TIMESTAMPING, &flags, sizeof(flags));
    int nError2 = setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_TIMESTAMPNS, &flags, sizeof(flags));

    std::cout << "TX Timestamp: " << nError << " " << nError2 << std::endl;
    m_bTimestampEnabled = (nError == 0 && nError2 == 0);

    DoSend();
    #endif
}

void Sender::DoSend()
{
    bool bDebug = true;

    if(m_manager.GetMasterClock() != nullptr || bDebug)
    {

        m_socket.async_send_to(asio::buffer(CreateRequest()), m_endpoint,
        [this](std::error_code ec, std::size_t /*length*/)
        {

            if (!ec)
            {
                if(m_bTimestampEnabled)
                {
                    GetTxTimestamp();
                }
                DoTimeout();
            }
            else
            {
                std::cout << ec << std::endl;
            }
        });
    }
    else
    {
        DoTimeout();
    }
}

std::vector<unsigned char> Sender::CreateRequest()
{
    ptpV2Header theHeader;
    ptpV2Payload thePayload;

    theHeader.nVersion = 2;
    theHeader.nType = 1;
    theHeader.timestamp = TimeNow();

    theHeader.nMessageLength = 44;
    theHeader.nDomain = 0;  //@todo need to set the domain
    theHeader.nFlags = 0;
    theHeader.nCorrection = 0;
    theHeader.source.nSourceId = GenerateClockIdentity(m_outboundIpAddress);
    theHeader.source.nSourcePort = 1;
    theHeader.nSequenceId = m_nSequence;
    theHeader.nControl = 1;
    theHeader.nInterval = static_cast<unsigned char>(m_manager.GetDelayRate());

    thePayload.originTime = TimeNow();

    std::vector<unsigned char> vMessage(theHeader.CreateMessage());
    std::vector<unsigned char> vPayload(thePayload.CreateMessage());

    std::copy(vPayload.begin(), vPayload.end(), std::back_inserter(vMessage));

    m_nSequence++;
    return vMessage;
}


void Sender::DoTimeout()
{
    m_timer.expires_after(std::chrono::milliseconds(static_cast<unsigned long>(1000.0*std::pow(2, static_cast<float>(m_manager.GetDelayRate())))));
    m_timer.async_wait(
    [this](std::error_code ec)
    {
        if (!ec)
        {
            DoSend();
        }
    });
}


void Sender::GetTxTimestamp()
{
    unsigned char data[512];
    msghdr msg;
    iovec entry;
    sockaddr_in from_addr;
    struct { cmsghdr cm;
             char control[512];
         } control;

    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = &entry;
    msg.msg_iovlen = 1;
    entry.iov_base = data;
    entry.iov_len = sizeof(data);
    msg.msg_name = (caddr_t)&from_addr;
    msg.msg_namelen = sizeof(from_addr);
    msg.msg_control = &control;
    msg.msg_controllen = sizeof(control);
    int recvmsg_flags = MSG_ERRQUEUE;//| MSG_DONTWAIT;

    int res = recvmsg(m_socket.native_handle(), &msg, recvmsg_flags);

    if(res > 76 && (recvmsg_flags & MSG_ERRQUEUE))
    {
        //this should be the timestamp ,sg
        for(cmsghdr* cmsg = CMSG_FIRSTHDR(&msg); cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg))
        {

            if(cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SO_TIMESTAMPNS)
            {
                //Get the hardware/software timestamp
                timespec* ts = (timespec*)CMSG_DATA(cmsg);
                time_s_ns now;
                now.first = std::chrono::seconds(ts->tv_sec);
                now.second = std::chrono::nanoseconds(ts->tv_nsec);

                //decode the message that was sent and get the sequence number
                unsigned char* pData = (unsigned char*)msg.msg_iov->iov_base;
                ptpV2Message pMessage = PtpParser::ParseV2(now, "", std::vector<unsigned char>(pData+42, pData+(res+1)));//vBuffer);

                //tell the local client what the actual timestamp for this message was
                m_manager.UpdateDelayRequestTimestamp(pMessage.first->nSequenceId, now);
                break;
            }
        }
    }
}
