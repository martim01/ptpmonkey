#pragma once
#include <vector>
#include <memory>
#include <list>
#include "timeutils.h"

class Handler;

class Parser
{
    public:
        Parser(std::shared_ptr<Handler> pHandler)
        {
            m_lstHandler.push_back(pHandler);
        }

        void AddHandler(std::shared_ptr<Handler> pHandler)
        {
            m_lstHandler.push_back(pHandler);
        }

        virtual void ParseMessage(const time_s_ns& socketTime,const std::string& sSenderIp, std::vector<unsigned char> vMessage)=0;

    protected:

        std::list<std::shared_ptr<Handler>> m_lstHandler;
};
