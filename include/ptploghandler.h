#pragma once
#include "handler.h"

namespace pml
{
    namespace ptpmonkey
    {

        class PtpLogHandler : public Handler
        {
            public:
                PtpLogHandler(){}
                void HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload) override;
        };

    };
}