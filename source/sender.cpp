#include "sender.h"
#include "ptpstructs.h"
#include "timeutils.h"
#include "mac.h"
#include "ptpmonkeyimplementation.h"
#include <cmath>

#ifdef __GNU__
#include <linux/socket.h>
#endif // __GNU__

using namespace ptpmonkey;

Sender::Sender(PtpMonkeyImplementation& manager, asio::io_context& io_context, const IpAddress& outboundIpAddress, const asio::ip::address& multicast_address,
        unsigned short nPort) : m_manager(manager),
          m_outboundIpAddress(outboundIpAddress),
          m_endpoint(multicast_address, nPort),
          m_socket(io_context, m_endpoint.protocol()),
          m_timer(io_context),
          m_nSequence(0)
{

}

void Sender::Run()
{

    int socket = m_socket.native_handle();
    int opt = 1;
    int nError = setsockopt(socket, SOL_SOCKET, SO_TIMESTAMPING, (void*)&opt, sizeof(opt));


    asio::ip::multicast::outbound_interface option(asio::ip::address_v4::from_string(m_outboundIpAddress.Get()));
    m_socket.set_option(option);

    do_send();
}

void Sender::do_send()
{

    if(m_manager.GetMasterClock() != nullptr)
    {
        m_socket.async_send_to(asio::buffer(CreateRequest()), m_endpoint,
        [this](std::error_code ec, std::size_t /*length*/)
        {

            if (!ec)
            {
                do_timeout();
            }
            else
            {
                std::cout << ec << std::endl;
            }
        });
    }
    else
    {
        do_timeout();
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


    m_manager.AddDelayRequest(m_nSequence, theHeader.timestamp);

    m_nSequence++;
    return vMessage;
}


void Sender::do_timeout()
{
    m_timer.expires_after(std::chrono::milliseconds(static_cast<unsigned long>(1000.0*std::pow(2, static_cast<float>(m_manager.GetDelayRate())))));
    m_timer.async_wait(
    [this](std::error_code ec)
    {
        if (!ec)
        {
            do_send();
        }
    });
}
