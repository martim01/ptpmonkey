#pragma once
#include "handler.h"

namespace ptpmonkey
{
    class LogHandler : public Handler
    {
        public:
            LogHandler(){}
            void HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload);
    };
}
