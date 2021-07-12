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
            void ParseMessage(const std::string& sSenderIp, const rawMessage& aMessage) override;

            static ptpV1Message ParseV1(const std::chrono::nanoseconds& socketTime, const std::string& sSenderIp, std::vector<unsigned char> vMessage);
            static ptpV2Message ParseV2(const std::chrono::nanoseconds& socketTime, const std::string& sSenderIp, std::vector<unsigned char> vMessage);
        protected:


    };
};
