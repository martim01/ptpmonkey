#pragma once
#include "parser.h"
#include <string>
#include <memory>
#include "ptpstructs.h"

class PtpParser : public Parser
{
    public:
        PtpParser(std::shared_ptr<Handler> pHandler) : Parser(pHandler){};
        void ParseMessage(std::vector<unsigned char> vMessage) override;

        enum {SYNC=0, DELAY_REQ=1, FOLLOW_UP=8, DELAY_RESP=9, ANNOUNCE=11};

    protected:
        void ParseV1(std::vector<unsigned char> vMessage);
        void ParseV2(std::vector<unsigned char> vMessage);


};
