#pragma once
#include <vector>
#include <memory>
#include <list>
#include "timeutils.h"
#include "namedtypes.h"

namespace ptpmonkey
{
    class Handler;

    /** @struct raw message
    *   @brief struct holding rx timestamp and raw message data
    **/
    struct rawMessage
    {
        IpAddress ipSender;
        std::chrono::nanoseconds timestamp;
        std::vector<unsigned char> vBuffer;
    };

    class Parser
    {
        public:
            explicit Parser(std::shared_ptr<Handler> pHandler)
            {
                m_lstHandler.push_back(pHandler);
            }

            void AddHandler(std::shared_ptr<Handler> pHandler)
            {
                m_lstHandler.push_back(pHandler);
            }

            virtual void ParseMessage(const rawMessage& aMessage)=0;

        protected:

            std::list<std::shared_ptr<Handler>> m_lstHandler;
    };
};
