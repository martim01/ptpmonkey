#include "ptploghandler.h"
#include "ptpstructs.h"
#include <iostream>
#include "timeutils.h"
#include "ptpparser.h"
#include "log.h"

using namespace ptpmonkey;
void PtpLogHandler::HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)
{
    std::shared_ptr<ptpV2Header> pPtpH = std::dynamic_pointer_cast<ptpV2Header>(pHeader);
    std::shared_ptr<ptpV2Payload> pPtpP = std::dynamic_pointer_cast<ptpV2Payload>(pPayload);
    if(pPtpP && pPtpH)
    {
        switch(pPtpH->nType)
        {
        case ptpV2Header::SYNC:
            pml::Log() << "PtpMonkey\tSync\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
            break;
        case ptpV2Header::FOLLOW_UP:
           pml::Log() << "PtpMonkey\tFollowUp\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
        case ptpV2Header::DELAY_REQ:
           pml::Log() << "PtpMonkey\tDelayReq\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
        case ptpV2Header::DELAY_RESP:
            pml::Log() << "PtpMonkey\tDelayRep\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
        case ptpV2Header::ANNOUNCE:
            pml::Log() << "PtpMonkey\t" << (int)pPtpH->nType << "\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId;
            break;
        }
    }
}
