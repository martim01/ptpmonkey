#include "sender.h"
#include "ptpstructs.h"
#include "timeutils.h"
#include "mac.h"
#include "ptpmonkeyimplementation.h"
#include <cmath>
#include "ptpparser.h"
#include "receiver.h"
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
    int flags = SOF_TIMESTAMPING_TX_SOFTWARE | SOF_TIMESTAMPING_TX_HARDWARE | SOF_TIMESTAMPING_SOFTWARE | SOF_TIMESTAMPING_RAW_HARDWARE;
    if(setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_TIMESTAMPING, &flags, sizeof(flags)) < 0)
    {
        m_bTimestampEnabled = (setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_TIMESTAMPNS, &flags, sizeof(flags)) >= 0);
    }
    else
    {
        m_bTimestampEnabled = true;
    }
    #endif
    DoSend();

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
    rawMessage aMessage = Receiver::NativeReceive(m_socket, MSG_ERRQUEUE);
    if(aMessage.vBuffer.size() >= 34)
    {
        ptpV2Message pMessage = PtpParser::ParseV2(aMessage.timestamp, "", aMessage.vBuffer);
        //tell the local client what the actual timestamp for this message a
        m_manager.UpdateDelayRequestTimestamp(pMessage.first->nSequenceId, aMessage.timestamp);
    }
}
