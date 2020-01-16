#pragma once
#include "handler.h"

class PtpLogHandler : public Handler
{
    public:
        PtpLogHandler(){}
        void HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload) override;
};

