#pragma once
#include <vector>
#include <string>
#include "timeutils.h"


/** @struct header
*   @brief abstract base class for the header of any message
**/
struct header
{
    /** @brief Output the header values for debug purposes **/
    virtual void OutputValues()=0;
    std::string sIpAddress; ///< The ip address the message arrived from
};

/** @struct payload
*   @brief abstract base class for the payload of any message
**/
struct payload
{
    /** @brief Output the payload values for debug purposes **/
    virtual void OutputValues()=0;
};

/** @struct ptpHeader
*   @brief  base class for the header of a v1 and v2 ptp message
**/
struct ptpHeader : header
{
    ptpHeader();

    unsigned char nVersion; ///< The ptp version
    unsigned char nType;    ///< The type of message the payload contains
    time_s_ns timestamp;    ///< The time the message was recevied by the application

};

/** @struct ptpPayload
*   @brief  base class for the payload of a v1 and v2 ptp message
**/
struct ptpPayload : payload
{
    time_s_ns originTime;   ///< The time the message was sent by the ptp clock
};

/** @struct ptpPayload
*   @brief  contains the source id and port of a ptp clock
**/
struct ptpSource
{
    ///< default constructor
    ptpSource(){}

    /** @brief Constructor
    *   @param vMessage a vector of unsigned char that contain the
    **/
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

