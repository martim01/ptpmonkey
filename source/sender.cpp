#include "sender.h"
#include "ptpstructs.h"
#include "timeutils.h"
#include "mac.h"
#include "ptpmonkeyimplementation.h"
#include <cmath>
#include "ptpparser.h"
#include "receiver.h"
#include "ptpmonkey.h"

#ifdef __GNU__
#include <linux/socket.h>
#include <linux/net_tstamp.h>
#include <time.h>
#endif // __GNU__
#include <iostream>
#include <iomanip>
#include "log.h"

#define FORCE_SO 1

using namespace pml::ptpmonkey;

Sender::Sender(PtpMonkeyImplementation& manager, std::shared_ptr<PtpParser> pParser, asio::io_context& io_context, const IpAddress& outboundIpAddress, const asio::ip::address& destination_address, unsigned short nPort, unsigned long nDomain, int nTimestampingSupported, bool bMulticast) : m_manager(manager),
          m_pParser(pParser),
          m_outboundIpAddress(outboundIpAddress),
          m_endpoint(destination_address, nPort),
          m_socket(io_context, m_endpoint.protocol()),
          m_timer(io_context),
          m_nDomain(nDomain),
          m_nTimestampingSupported(nTimestampingSupported),
          m_bMulticast(bMulticast)
{
}

void Sender::Stop()
{
    m_timer.cancel();
    m_bRun = false;
}

void Sender::Run()
{
    if(m_bMulticast)
    {
        asio::ip::multicast::outbound_interface option(asio::ip::address_v4::from_string(m_outboundIpAddress.Get()));
        m_socket.set_option(option);

        //lets not loop the message back it gets confusing.
        asio::ip::multicast::enable_loopback optionl(false);
        m_socket.set_option(optionl);
    }

    #ifdef __GNU__
    // @todo for some reason we get tx software timestamps even though pi says it doesn't suppport it. add this bodge for now to aid debugging
    #if FORCE_SO==1
    m_nTimestampingSupported |= port::enumTimestamping::TIMESTAMP_TX_SOFTWARE;
    pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "" << "Sender: Attempt to set tx software timestamping anyway.";
    #endif // FORCE_SO

    int nFlags(0);
    nFlags |= (m_nTimestampingSupported & port::enumTimestamping::TIMESTAMP_TX_HARDWARE) ? (SOF_TIMESTAMPING_TX_HARDWARE | SOF_TIMESTAMPING_RAW_HARDWARE) : 0;
    nFlags |= (m_nTimestampingSupported & port::enumTimestamping::TIMESTAMP_TX_SOFTWARE) ? (SOF_TIMESTAMPING_TX_SOFTWARE | SOF_TIMESTAMPING_SOFTWARE) : 0;

    if(nFlags != 0)
    {
        if(setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_TIMESTAMPING, &nFlags, sizeof(nFlags)) < 0)
        {
            pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "" << "Sender: Failed to set SO_TIMESTAMPING";
            m_bTimestampEnabled = (setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_TIMESTAMPNS, &nFlags, sizeof(nFlags)) >= 0);
            if(!m_bTimestampEnabled)
            {
                pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "" << "Sender: Failed to set SO_TIMESTAMPNS";
            }
            else
            {
                pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "" << "Sender: Set SO_TIMESTAMPNS timestamping";
            }
        }
        else
        {
            m_bTimestampEnabled = true;
            pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "" << "Sender: Set SO_TIMESTAMPING timestamping";
        }
    }
    #endif
    SendDelayRequest();
    DoTimeout();

}

void Sender::SendDelayRequest()
{
    if(m_bRun)
    {
        if(m_manager.GetSyncMasterClock() != nullptr)
        {
            auto vBuffer = CreateDelayRequest();

            m_socket.async_send_to(asio::buffer(vBuffer), m_endpoint, [this, vBuffer](std::error_code ec, std::size_t)
            {
                if (!ec)
                {
                    if(m_bTimestampEnabled)
                    {
                        GetTxTimestamp(vBuffer);
                    }
                    else
                    {
                        //approximate the timestamp.
                        ptpV2Message pMessage = m_pParser->ParseV2(Now(), IpAddress(""), vBuffer);
                        //tell the local client we've sent a delay request message
                        m_manager.DelayRequestSent(pMessage.first, pMessage.second);
                    }
                }
                else
                {
                    pmlLog(pml::LOG_ERROR, "pml::ptpmonkey") << "" << "Sender: Send failed: " << ec;
                }
            });
        }
    }
}

void Sender::SendManagementMessage(std::shared_ptr<ptpManagement> pMessage)
{
    if(m_bRun)
    {
        auto vBuffer = CreateManagement(pMessage);
        m_socket.async_send_to(asio::buffer(vBuffer), m_endpoint, [this, vBuffer](std::error_code ec, std::size_t)
        {
            if (ec)
            {
                pmlLog(pml::LOG_ERROR, "pml::ptpmonkey") << "" << "Sender: Send failed: " << ec;
            }
        });
    }
}

void Sender::Queue(const ptpManagement& message)
{
    m_qManagement.push(message);
}

std::vector<unsigned char> Sender::CreateManagement(std::shared_ptr<ptpManagement> pMessage)
{
    auto itSequence = m_mSequence.try_emplace(hdr::enumType::MANAGEMENT, 0).first;

    auto vPayload = pMessage->CreateMessage();

    auto theHeader = CreateHeader(hdr::enumType::MANAGEMENT, itSequence->second, 34+vPayload.size(), 0x7f);

    auto vBuffer = theHeader.CreateMessage();
    std::copy(vPayload.begin(), vPayload.end(), std::back_inserter(vBuffer));

    itSequence->second++;
    return vBuffer;
}

ptpV2Header Sender::CreateHeader(hdr::enumType eType, uint16_t nSequence, uint16_t nLength, uint8_t nInterval)
{
    ptpV2Header theHeader;

    theHeader.nVersion = 2;
    theHeader.nType = static_cast<uint8_t>(eType);
    theHeader.timestamp = Now();

    theHeader.nMessageLength = nLength;
    theHeader.nDomain = static_cast<unsigned char>(m_nDomain);
    theHeader.nFlags = 0;
    if(!m_bMulticast)
    {
        theHeader.nFlags |= static_cast<unsigned short>(hdr::enumFlags::UNICAST);
    }
    theHeader.nCorrection = 0;
    theHeader.source.nSourceId = GenerateClockIdentity(m_outboundIpAddress);
    theHeader.source.nSourcePort = 1;
    theHeader.nSequenceId = nSequence;
    theHeader.nControl = 1;
    theHeader.nInterval = nInterval;
    return theHeader;
}

std::vector<unsigned char> Sender::CreateDelayRequest()
{
    auto itSequence = m_mSequence.try_emplace(hdr::enumType::DELAY_REQ, 0).first;
    auto theHeader = CreateHeader(hdr::enumType::DELAY_REQ, itSequence->second, 44, static_cast<char>(m_manager.GetDelayRate()));

    ptpV2Payload thePayload;
    m_lastDelayReq = Now();
    thePayload.originTime = m_lastDelayReq;

    auto vBuffer = theHeader.CreateMessage();
    auto vPayload =  thePayload.CreateMessage();

    std::copy(vPayload.begin(), vPayload.end(), std::back_inserter(vBuffer));

    itSequence->second++;
    return vBuffer;
}


void Sender::DoTimeout()
{
    m_timer.expires_after(std::chrono::milliseconds(100));
    m_timer.async_wait([this](std::error_code ec)
    {
        if (!ec)
        {
            auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            if(m_manager.SendDelayRequests() && now - m_lastDelayReq > m_manager.GetDelayReqGap())
            {
                SendDelayRequest();
            }

            while(auto pMessage = m_qManagement.pop())
            {
                SendManagementMessage(pMessage);
            }
            DoTimeout();
        }
    });
}


void Sender::GetTxTimestamp(const std::vector<unsigned char>& vBuffer)
{
    #ifdef __GNU__
    rawMessage aMessage = Receiver::NativeReceive(m_socket, MSG_ERRQUEUE);
    if(vBuffer.size() >= 34)
    {
        ptpV2Message pMessage = m_pParser->ParseV2(aMessage.timestamp, IpAddress(""), vBuffer);
        //tell the local client what the actual timestamp for this message a
        m_manager.DelayRequestSent(pMessage.first, pMessage.second);
    }
    #endif // __GNU__
}

void Sender::ChangeEndpoint(const asio::ip::address& destination)
{
    m_endpoint = asio::ip::udp::endpoint(destination, 319);
}

void Sender::SetDomain(unsigned char nDomain)
{
    m_nDomain = nDomain;
}