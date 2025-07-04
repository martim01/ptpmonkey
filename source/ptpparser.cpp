#include "ptpparser.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "ptpstructs.h"
#include "handler.h"
#include "log.h"

using namespace pml::ptpmonkey;




ptpV1Message PtpParser::ParseV1(const std::chrono::nanoseconds& socketTime,const IpAddress& ipSender, std::vector<unsigned char> vMessage)
{
    return std::make_pair(nullptr, nullptr);
}



ptpV2Message PtpParser::ParseV2(const std::chrono::nanoseconds& socketTime, const IpAddress& ipSender, std::vector<unsigned char> vMessage)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    //first byte is meesage type:
    auto pHeader = std::make_shared<ptpV2Header>(socketTime, vMessage);
    std::shared_ptr<ptpV2Payload> pPayload = nullptr;

    if(pHeader->nDomain == m_nDomain)
    {
        pHeader->sIpAddress = ipSender.Get();

        switch(static_cast<hdr::enumType>(pHeader->nType))
        {
            case hdr::enumType::SYNC:
                pPayload = std::make_shared<ptpV2Payload>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
                break;
            case hdr::enumType::DELAY_RESP:
                pPayload = std::make_shared<ptpDelayResponse>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
                break;
            case hdr::enumType::DELAY_REQ:
                pPayload = std::make_shared<ptpV2Payload>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
                break;
            case hdr::enumType::FOLLOW_UP:
                pPayload = std::make_shared<ptpV2Payload>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
                break;
            case hdr::enumType::ANNOUNCE:
                pPayload = std::make_shared<ptpAnnounce>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
                break;
            case hdr::enumType::MANAGEMENT:
                pPayload = std::make_shared<ptpManagement>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
        }
    }
    return std::make_pair(pHeader, pPayload);
}



void PtpParser::ParseMessage(const rawMessage& aMessage)
{
    if(aMessage.vBuffer.size() < 34)
        return;


    unsigned char nVersion = aMessage.vBuffer[1] & 0xF;
    switch(nVersion)
    {
    case 1:
        ParseV1(aMessage.timestamp, aMessage.ipSender, aMessage.vBuffer);
        break;
    case 2:
        {
            ptpV2Message pMessage = ParseV2(aMessage.timestamp, aMessage.ipSender, aMessage.vBuffer);
            if(pMessage.first && pMessage.second)
            {
                for(auto pHandler : m_lstHandler)
                {
                    pHandler->HandleParsedMessage(pMessage.first, pMessage.second);
                }
            }
        }
        break;
    default:
        {
            pml::log::Stream ls(pml::log::Level::kWarning);
            ls << "Unknown version\n" << std::hex << std::setfill('0') << std::setw(2);
            for(const auto& byte : aMessage.vBuffer)
            {
                ls << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned int>(byte) << " ";
            }
        }

    }

}

void PtpParser::SetDomain(unsigned char nDomain)
 {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_nDomain = nDomain;
}
