#pragma once
#include "handler.h"
#include <functional>

namespace pml::ptpmonkey
{
    class PtpMonkeyImplementation;
    struct ptpV2Header;
    struct ptpV2Payload;
    struct ptpDelayResponse;        
    struct ptpAnnounce;
    struct ptpManagement;


    class PtpMonkeyHandler : public Handler
    {
        public:
            PtpMonkeyHandler()=default;
           
            void SetCallbacks(const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload>)>& pSync,
                              const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload>)>& pFollowUp,
                              const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload>)>& pDelayRequest,
                              const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpDelayResponse>)>& pDelayResponse,
                              const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpAnnounce>)>& pAnnounce,
                              const std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpManagement>)>& pManagement);

            void HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload) override;
        private:
            
            std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload>)> m_pSync = nullptr;
            std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload>)> m_pFollowUp = nullptr;
            std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload>)> m_pDelayRequest = nullptr;
            std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpDelayResponse>)> m_pDelayResponse;
            std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpAnnounce>)> m_pAnnounce = nullptr;
            std::function<void(std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpManagement>)> m_pManagement = nullptr;
    };

}
