#pragma once
#include "parser.h"
#include <string>
#include <memory>
#include "ptpstructs.h"

namespace ptpmonkey
{
    using ptpV1Message = std::pair<std::shared_ptr<ptpHeader>, std::shared_ptr<ptpPayload> >;
    using ptpV2Message = std::pair<std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload> >;

    class PtpParser : public Parser
    {
        public:
            PtpParser(std::shared_ptr<Handler> pHandler) : Parser(pHandler){};
            void ParseMessage(const time_s_ns& socketTime,const std::string& sSenderIp, std::vector<unsigned char> vMessage) override;

            static ptpV1Message ParseV1(const time_s_ns& socketTime, const std::string& sSenderIp, std::vector<unsigned char> vMessage);
            static ptpV2Message ParseV2(const time_s_ns& socketTime, const std::string& sSenderIp, std::vector<unsigned char> vMessage);
        protected:


    };
};
