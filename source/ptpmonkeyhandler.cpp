#include "ptpmonkeyhandler.h"
#include "ptpmonkeyimplementation.h"
#include "ptpstructs.h"
#include "ptpparser.h"
#include <iostream>
#include <iomanip>
using namespace pml::ptpmonkey;


void PtpMonkeyHandler::SetCallbacks(const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload>)>& pSync,
                              const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload>)>& pFollowUp,
                              const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload>)>& pDelayRequest,
                              const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpDelayResponse>)>& pDelayResponse,
                              const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpAnnounce>)>& pAnnounce,
                              const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpManagement>)>& pManagement)
{
    m_pSync = pSync;
    m_pFollowUp = pFollowUp;
    m_pDelayRequest = pDelayRequest;
    m_pDelayResponse = pDelayResponse;
    m_pAnnounce = pAnnounce;
    m_pManagement = pManagement;
}

void PtpMonkeyHandler::HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)
{

    auto pPtpH = std::dynamic_pointer_cast<ptpV2Header>(pHeader);
    auto pPtpP = std::dynamic_pointer_cast<ptpV2Payload>(pPayload);
    if(pPtpP && pPtpH)
    {
        switch(static_cast<hdr::enumType>(pPtpH->nType))
        {
            case hdr::enumType::SYNC:
                if(m_pSync)
                {
                    m_pSync(pPtpH, pPtpP);
                }
                break;
            case hdr::enumType::FOLLOW_UP:
                if(m_pFollowUp)
                {
                    m_pFollowUp(pPtpH, pPtpP);
                }
                break;
            case hdr::enumType::DELAY_RESP:
                if(m_pDelayResponse)
                {
                    m_pDelayResponse(pPtpH, std::dynamic_pointer_cast<ptpDelayResponse>(pPtpP));
                }
                break;
            case hdr::enumType::ANNOUNCE:
                if(m_pAnnounce)
                {
                    m_pAnnounce(pPtpH, std::dynamic_pointer_cast<ptpAnnounce>(pPtpP));
                }
                break;
            case hdr::enumType::DELAY_REQ:
                if(m_pDelayRequest)
                {
                    m_pDelayRequest(pPtpH, pPtpP);
                }
                break;
            case hdr::enumType::MANAGEMENT:
                if(m_pManagement)
                {
                    m_pManagement(pPtpH, std::dynamic_pointer_cast<ptpManagement>(pPayload));
                }
                break;
            default:
                break;
        }
    }
}
