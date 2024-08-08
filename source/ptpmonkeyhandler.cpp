#include "ptpmonkeyhandler.h"
#include "ptpmonkeyimplementation.h"
#include "ptpstructs.h"
#include "ptpparser.h"
#include <iostream>
#include <iomanip>
using namespace pml::ptpmonkey;

void PtpMonkeyHandler::HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)
{

    auto pPtpH = std::dynamic_pointer_cast<ptpV2Header>(pHeader);
    auto pPtpP = std::dynamic_pointer_cast<ptpV2Payload>(pPayload);
    if(pPtpP && pPtpH)
    {
        switch(static_cast<ptpV2Header::enumType>(pPtpH->nType))
        {
            case ptpV2Header::enumType::SYNC:
                m_manager.Sync(pPtpH, pPtpP);
                break;
            case ptpV2Header::enumType::FOLLOW_UP:
                m_manager.FollowUp(pPtpH, pPtpP);
                break;
            case ptpV2Header::enumType::DELAY_RESP:
                m_manager.DelayResponse(pPtpH, std::dynamic_pointer_cast<ptpDelayResponse>(pPtpP));
                break;
            case ptpV2Header::enumType::ANNOUNCE:
                m_manager.Announce(pPtpH, std::dynamic_pointer_cast<ptpAnnounce>(pPtpP));
                break;
            case ptpV2Header::enumType::DELAY_REQ:
                m_manager.DelayRequest(pPtpH, pPtpP);
                break;

        }
    }
}
