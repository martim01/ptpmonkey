#pragma once
#include <memory>

namespace ptpmonkey
{
    struct header;
    struct payload;

    class Handler
    {
        public:
            Handler(){}
            virtual void HandleParsedMessage(std::shared_ptr<header> pHeader, std::shared_ptr<payload> pPayload)=0;
    };


};
