#pragma once
#include "handler.h"

class PtpManager;

class PtpManagerHandler : public Handler
{
    public:
        PtpManagerHandler(PtpManager& manager) : m_manager(manager){}
        void HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload) override;

    private:
        PtpManager& m_manager;
};


