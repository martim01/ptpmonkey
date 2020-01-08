#include "ptpmanagerhandler.h"
#include "ptpmanager.h"
#include "ptpstructs.h"
#include "ptpparser.h"

void PtpManagerHandler::HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)
{

    std::shared_ptr<ptpV2Header> pPtpH = std::dynamic_pointer_cast<ptpV2Header>(pHeader);
    std::shared_ptr<ptpV2Payload> pPtpP = std::dynamic_pointer_cast<ptpV2Payload>(pPayload);
    if(pPtpP && pPtpH)
    {
        switch(pPtpH->nType)
        {
            case PtpParser::SYNC:
                m_manager.Sync(pPtpH, pPtpP);
                break;
            case PtpParser::DELAY_RESP:
                m_manager.DelayResponse(pPtpH, std::dynamic_pointer_cast<ptpDelayResponse>(pPtpP));
                break;
            case PtpParser::ANNOUNCE:
                m_manager.Announce(pPtpH, std::dynamic_pointer_cast<ptpAnnounce>(pPtpP));
                break;

        }
    }
}
