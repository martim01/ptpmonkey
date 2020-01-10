#include "ptploghandler.h"
#include "ptpstructs.h"
#include <iostream>
#include "timeutils.h"
#include "ptpParser.h"
void PtpLogHandler::HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)
{
    std::shared_ptr<ptpV2Header> pPtpH = std::dynamic_pointer_cast<ptpV2Header>(pHeader);
    std::shared_ptr<ptpV2Payload> pPtpP = std::dynamic_pointer_cast<ptpV2Payload>(pPayload);
    if(pPtpP && pPtpH)
    {
        switch(pPtpH->nType)
        {
        case PtpParser::SYNC:
            std::cout << "Sync\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime) << std::endl;
            break;
        case PtpParser::FOLLOW_UP:
           std::cout << "FollowUp\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime) << std::endl;
        case PtpParser::DELAY_REQ:
               std::cout << "DelayReq\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime) << std::endl;
        case PtpParser::DELAY_RESP:
            std::cout << "DelayRep\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime) << std::endl;
        case PtpParser::ANNOUNCE:
            std::cout << (int)pPtpH->nType << "\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << std::endl;
            break;
        }
    }
}
