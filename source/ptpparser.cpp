#include "ptpparser.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "ptpstructs.h"
#include "handler.h"

using namespace ptpmonkey;




ptpV1Message PtpParser::ParseV1(const std::chrono::nanoseconds& socketTime, const std::string& sSenderIp, std::vector<unsigned char> vMessage)
{
    return std::make_pair(nullptr, nullptr);
}



ptpV2Message PtpParser::ParseV2(const std::chrono::nanoseconds& socketTime, const std::string& sSenderIp, std::vector<unsigned char> vMessage)
{
    //first byte is meesage type:
    std::shared_ptr<ptpV2Header> pHeader = std::make_shared<ptpV2Header>(socketTime, vMessage);
    std::shared_ptr<ptpV2Payload> pPayload(nullptr);

    pHeader->sIpAddress = sSenderIp;

    switch(pHeader->nType)
    {
        case ptpV2Header::SYNC:
            pPayload = std::make_shared<ptpV2Payload>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
            break;
        case ptpV2Header::DELAY_RESP:
            pPayload = std::make_shared<ptpDelayResponse>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
            break;
        case ptpV2Header::DELAY_REQ:
            pPayload = std::make_shared<ptpV2Payload>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
            break;
        case ptpV2Header::FOLLOW_UP:
            pPayload = std::make_shared<ptpV2Payload>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
            break;
        case ptpV2Header::ANNOUNCE:
            pPayload = std::make_shared<ptpAnnounce>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end()));
            break;
    }
    return std::make_pair(pHeader, pPayload);

}



void PtpParser::ParseMessage(const std::string& sSenderIp, const rawMessage& aMessage)
{
    if(aMessage.vBuffer.size() < 34)
        return;


    unsigned char nVersion = aMessage.vBuffer[1] & 0xF;
    switch(nVersion)
    {
    case 1:
        ParseV1(aMessage.timestamp, sSenderIp, aMessage.vBuffer);
        break;
    case 2:
        {
            ptpV2Message pMessage = ParseV2(aMessage.timestamp, sSenderIp, aMessage.vBuffer);
            for(auto pHandler : m_lstHandler)
            {
                pHandler->HandleParsedMessage(pMessage.first, pMessage.second);
            }
        }
    }
}
