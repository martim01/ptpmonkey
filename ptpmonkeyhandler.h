#pragma once
#include "handler.h"

class PtpMonkeyImplementation;

class PtpMonkeyHandler : public Handler
{
    public:
        PtpMonkeyHandler(PtpMonkeyImplementation& manager) : m_manager(manager){}
        void HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload) override;

    private:
        PtpMonkeyImplementation& m_manager;
};


