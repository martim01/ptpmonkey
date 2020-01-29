#pragma once
#include <vector>
#include <memory>
#include <list>

#ifdef _WIN32_WINNT
#include "winsock2.h"
#endif // _WIN32_WINNT
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

        virtual void ParseMessage(const timeval& tvSocket,const std::string& sSenderIp, std::vector<unsigned char> vMessage)=0;

    protected:

        std::list<std::shared_ptr<Handler>> m_lstHandler;
};
