#include "ptpparser.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "ptpstructs.h"
#include "handler.h"


void PtpParser::ParseMessage(std::vector<unsigned char> vMessage)
{
    unsigned char nVersion = vMessage[1] & 0xF;
    switch(nVersion)
    {
    case 1:
        ParseV1(vMessage);
        break;
    case 2:
        ParseV2(vMessage);
    }
}

void PtpParser::ParseV1(std::vector<unsigned char> vMessage)
{

}



void PtpParser::ParseV2(std::vector<unsigned char> vMessage)
{
    //first byte is meesage type:
    std::shared_ptr<ptpHeader> pHeader = std::make_shared<ptpV2Header>(vMessage);

    switch(pHeader->nType)
    {
        case SYNC:
            m_pHandler->HandleParsedMessage(pHeader, std::make_shared<ptpV2Payload>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end())));
            break;
        case DELAY_RESP:
            m_pHandler->HandleParsedMessage(pHeader, std::make_shared<ptpDelayResponse>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end())));
            break;
        case FOLLOW_UP:
            m_pHandler->HandleParsedMessage(pHeader, std::make_shared<ptpV2Payload>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end())));
            break;
        case ANNOUNCE:
            m_pHandler->HandleParsedMessage(pHeader, std::make_shared<ptpAnnounce>(std::vector<unsigned char>(vMessage.begin()+34, vMessage.end())));
            break;

    }
}

