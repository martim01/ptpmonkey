#pragma once
#include <vector>
#include <memory>
class Handler;

class Parser
{
    public:
        Parser(std::shared_ptr<Handler> pHandler) : m_pHandler(pHandler){};
        virtual void ParseMessage(const std::string& sSenderIp, std::vector<unsigned char> vMessage)=0;

    protected:

        std::shared_ptr<Handler> m_pHandler;
};
