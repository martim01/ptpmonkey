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
            case hdr::enumType::kSync:
          //      pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Sync\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
                break;
            case hdr::enumType::kFollowUp:
            //    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "FollowUp\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
            case hdr::enumType::kDelayReq:
              //  pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "DelayReq\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
            case hdr::enumType::kDelayResp:
            //    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "DelayRep\t" << TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId << " " << TimeToString(pPtpP->originTime);
            case hdr::enumType::kAnnounce:
            ///    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Announce\t" <<  TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId;
                break;
            case hdr::enumType::kManagement:
            pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "===============================================================================================";
                pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Management\t" <<  TimeToString(pPtpH->timestamp) << "\t" << pPtpH->source.sSourceId;
                pPtpP->OutputValues();
                pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "===============================================================================================";
                break;
        }
        
    }
}
