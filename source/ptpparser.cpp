#include "ptpparser.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "ptpstructs.h"
#include "handler.h"


void PtpParser::ParseMessage(const std::string& sSenderIp, std::vector<unsigned char> vMessage)
{

    unsigned char nVersion = vMessage[1] & 0xF;
    switch(nVersion)
    {
    case 1:
        ParseV1(sSenderIp, vMessage);
        break;
    case 2:
        ParseV2(sSenderIp, vMessage);
    }
}

void PtpParser::ParseV1(const std::string& sSenderIp, std::vector<unsigned char> vMessage)
{

}



void PtpParser::ParseV2(const std::string& sSenderIp, std::vector<unsigned char> vMessage)
{
    //first byte is meesage type:
    std::shared_ptr<ptpHeader> pHeader = std::make_shared<ptpV2Header>(vMessage);
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
    for(auto pHandler : m_lstHandler)
    {
        pHandler->HandleParsedMessage(pHeader, pPayload);
    }
}

