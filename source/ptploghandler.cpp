#include "ptploghandler.h"
#include "ptpstructs.h"
#include <iostream>
#include "timeutils.h"
#include "ptpparser.h"
#include "log.h"
#include "enums.h"

using namespace pml::ptpmonkey;
void PtpLogHandler::HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)
{
    std::shared_ptr<ptpV2Header> pPtpH = std::dynamic_pointer_cast<ptpV2Header>(pHeader);
    std::shared_ptr<ptpV2Payload> pPtpP = std::dynamic_pointer_cast<ptpV2Payload>(pPayload);
    if(pPtpP && pPtpH)
    {
        
        switch(static_cast<hdr::enumType>(pPtpH->nType))
        {
            case hdr::enumType::SYNC:
          //      pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Sync\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
                break;
            case hdr::enumType::FOLLOW_UP:
            //    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "FollowUp\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
            case hdr::enumType::DELAY_REQ:
              //  pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "DelayReq\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
            case hdr::enumType::DELAY_RESP:
            //    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "DelayRep\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
            case hdr::enumType::ANNOUNCE:
            ///    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Announce\t" <<  TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId;
                break;
            case hdr::enumType::MANAGEMENT:
            pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "===============================================================================================";
                pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Management\t" <<  TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId;
                pPtpP->OutputValues();
                pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "===============================================================================================";
                break;
        }
        
    }
}
