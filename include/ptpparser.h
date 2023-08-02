#pragma once
#include "parser.h"
#include <string>
#include <memory>
#include "ptpstructs.h"
#include <mutex>

namespace ptpmonkey
{
    using ptpV1Message = std::pair<std::shared_ptr<ptpHeader>, std::shared_ptr<ptpPayload> >;
    using ptpV2Message = std::pair<std::shared_ptr<ptpV2Header>, std::shared_ptr<ptpV2Payload> >;

    class PtpParser : public Parser
    {
        public:
            explicit PtpParser(std::shared_ptr<Handler> pHandler, unsigned char nDomain) : Parser(pHandler), m_nDomain(nDomain){};
            void ParseMessage(const rawMessage& aMessage) override;

            ptpV1Message ParseV1(const std::chrono::nanoseconds& socketTime, const IpAddress& ipSender, std::vector<unsigned char> vMessage);
            ptpV2Message ParseV2(const std::chrono::nanoseconds& socketTime, const IpAddress& ipSender, std::vector<unsigned char> vMessage);

            void SetDomain(unsigned char nDomain);
        
        private:
            unsigned char m_nDomain;
            std::mutex m_mutex;
    };
};
