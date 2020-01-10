#pragma once
#include "handler.h"

class PtpMonkey;

class PtpMonkeyHandler : public Handler
{
    public:
        PtpMonkeyHandler(PtpMonkey& manager) : m_manager(manager){}
        void HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload) override;

    private:
        PtpMonkey& m_manager;
};


