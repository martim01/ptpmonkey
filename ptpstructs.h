#pragma once
#include <vector>
#include <string>
#include "timeutils.h"

struct header
{
    virtual void OutputValues()=0;
};
struct payload
{
    virtual void OutputValues()=0;
};

struct ptpHeader : header
{
    ptpHeader();

    unsigned char nVersion;
    unsigned char nType;
    time_s_ns timestamp;

};

struct ptpPayload : payload
{
    time_s_ns originTime;
};

struct ptpSource
{
    ptpSource(){}
    ptpSource(const std::vector<unsigned char>& vMessage);
    void CreateMessage(std::vector<unsigned char>& vMessage, size_t nBegin);

    unsigned long long int nSourceId;
    std::string sSourceId;
    unsigned short nSourcePort;
};

struct ptpV2Header : public ptpHeader
{
    ptpV2Header(){}
    ptpV2Header(const std::vector<unsigned char>& vMessage);
    void OutputValues() final;

    std::vector<unsigned char> CreateMessage();


    unsigned short nMessageLength;
    unsigned char nDomain;
    unsigned short nFlags;
    unsigned long long nCorrection;
    ptpSource source;
    unsigned short nSequenceId;
    unsigned char nControl;
    unsigned char nInterval;


};



struct ptpV2Payload : public ptpPayload
{
    ptpV2Payload(){}
    ptpV2Payload(const std::vector<unsigned char>& vMessage);
    void OutputValues() override;
    std::vector<unsigned char> CreateMessage();


};

struct ptpAnnounce : public ptpV2Payload
{
    ptpAnnounce(const std::vector<unsigned char>& vMessage);
    void OutputValues() final;

    unsigned short nUtcOffset;
    unsigned char nGrandmasterPriority1;
    unsigned char nGrandmasterClass;
    unsigned char nGrandmasterAccuracy;
    unsigned short nGrandmasterVariance;
    unsigned char nGrandmasterPriority2;
    std::string sClockId;
    unsigned short nStepsRemoved;
    unsigned char nTimeSource;
};

struct ptpDelayResponse : public ptpV2Payload
{
    ptpDelayResponse(const std::vector<unsigned char>& vMessage);
    void OutputValues() final;

    ptpSource source;
};

