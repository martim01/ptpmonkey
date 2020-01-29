#pragma once
#include "parser.h"
#include <string>
#include <memory>
#include "ptpstructs.h"

class PtpParser : public Parser
{
    public:
        PtpParser(std::shared_ptr<Handler> pHandler) : Parser(pHandler){};
        void ParseMessage(const timeval& tvSocket,const std::string& sSenderIp, std::vector<unsigned char> vMessage) override;




    protected:
        void ParseV1(const timeval& tvSocket,const std::string& sSenderIp, std::vector<unsigned char> vMessage);
        void ParseV2(const timeval& tvSocket,const std::string& sSenderIp, std::vector<unsigned char> vMessage);


};
