#include "ptpmonkeyhandler.h"
#include "ptpmonkeyimplementation.h"
#include "ptpstructs.h"
#include "ptpparser.h"
#include <iostream>
#include <iomanip>

void PtpMonkeyHandler::HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)
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
            case PtpParser::DELAY_REQ:
                m_manager.DelayRequest(pPtpH, pPtpP);
                break;

        }
    }
}