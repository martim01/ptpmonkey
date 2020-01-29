#pragma once
#include "parser.h"
#include <string>
#include <memory>
#include "ptpstructs.h"

namespace ptpmonkey
{

    class PtpParser : public Parser
    {
        public:
            PtpParser(std::shared_ptr<Handler> pHandler) : Parser(pHandler){};
            void ParseMessage(const time_s_ns& socketTime,const std::string& sSenderIp, std::vector<unsigned char> vMessage) override;




        protected:
            void ParseV1(const time_s_ns& socketTime,const std::string& sSenderIp, std::vector<unsigned char> vMessage);
            void ParseV2(const time_s_ns& socketTime,const std::string& sSenderIp, std::vector<unsigned char> vMessage);


    };
};
