#pragma once
#include <vector>
#include <string>
#include "timeutils.h"
#include "ptpdll.h"

namespace ptpmonkey
{



    /** @struct header
    *   @brief abstract base class for the header of any message
    **/
    struct PTP_IMPEXPORT header
    {
        /** @brief Output the header values for debug purposes **/
        virtual void OutputValues()=0;
        std::string sIpAddress; ///< The ip address the message arrived from
    };

    /** @struct payload
    *   @brief abstract base class for the payload of any message
    **/
    struct PTP_IMPEXPORT payload
    {
        /** @brief Output the payload values for debug purposes **/
        virtual void OutputValues()=0;
    };

    /** @struct ptpHeader
    *   @brief  base class for the header of a v1 and v2 ptp message
    **/
    struct PTP_IMPEXPORT ptpHeader : header
    {
        ptpHeader(const std::chrono::nanoseconds& socketTime);
        ptpHeader(){};


        unsigned char nVersion; ///< The ptp version
        unsigned char nType;    ///< The type of message the payload contains
        std::chrono::nanoseconds timestamp;    ///< The time the message was recevied by the application

    };

    /** @struct ptpPayload
    *   @brief  base class for the payload of a v1 and v2 ptp message
    **/
    struct PTP_IMPEXPORT ptpPayload : payload
    {
        std::chrono::nanoseconds originTime;   ///< The time the message was sent by the ptp clock
    };

    /** @struct ptpPayload
    *   @brief  contains the source id and port of a ptp clock
    **/
    struct PTP_IMPEXPORT ptpSource
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

    struct PTP_IMPEXPORT ptpV2Header : public ptpHeader
    {
        ptpV2Header(){}
        ptpV2Header(const std::chrono::nanoseconds& socketTime, const std::vector<unsigned char>& vMessage);
        void OutputValues() final;

        std::vector<unsigned char> CreateMessage();


        unsigned short nMessageLength;
        unsigned char nDomain;
        unsigned short nFlags;
        unsigned long long nCorrection;
        ptpSource source;
        unsigned short nSequenceId;
        unsigned char nControl;
        char nInterval;

        enum enumType{SYNC=0x0, DELAY_REQ=0x1, PEER_DLEAY_REQ = 0x2, PEER_DELAY_RESP = 0x3, FOLLOW_UP=0x8, DELAY_RESP=0x9, PEER_DELAY_RESP_FOLLOW_UP = 0xA,
                ANNOUNCE=0xB, SIGNALLING=0xC, MANAGEMENT=0xD};

            enum enumFlags{ ALTERNATE_MASTER = 0x0100,
                            TWO_STEP         = 0x0200,
                            UNICAST          = 0x0400,
                            PROFILE1         = 0x2000,
                            PROFILE2         = 0x4000,
                            LI_61            = 0x0001,
                            LI_59            = 0x0002,
                            UTC_OFFSET_VALID = 0x0004,
                            TIMESCALE        = 0x0008,
                            TIME_TRACEABLE   = 0x0010,
                            FREQ_TRACEABLE   = 0x0020 };


    };



    struct PTP_IMPEXPORT ptpV2Payload : public ptpPayload
    {
        ptpV2Payload(){}
        ptpV2Payload(const std::vector<unsigned char>& vMessage);
        void OutputValues() override;
        std::vector<unsigned char> CreateMessage();


    };

    struct PTP_IMPEXPORT ptpAnnounce : public ptpV2Payload
    {
        ptpAnnounce(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;

        unsigned short nUtcOffset;
        unsigned char nGrandmasterPriority1;
        unsigned char nGrandmasterClass;
        unsigned char nGrandmasterAccuracy;
        unsigned short nGrandmasterVariance;
        unsigned char nGrandmasterPriority2;
        std::string sGrandmasterClockId;
        unsigned short nStepsRemoved;
        unsigned char nTimeSource;


            enum enumAccuracy { ACC_25NS = 0x20, ACC_100NS = 0x21, ACC_250NS = 0x22,
                                ACC_1US = 0x23, ACC_2_5US = 0x24, ACC_10US = 0x25, ACC_25US = 0x26, ACC_100US = 0x27, ACC_250US = 0x28,
                                ACC_1MS = 0x29, ACC_2_5MS = 0x2A, ACC_10MS = 0x2B, ACC_25MS = 0x2C, ACC_100MS = 0x2D, ACC_250MS = 0x2E,
                                ACC_1S = 0x2F, ACC_10S = 0x30, ACC_OVER=0x31, ACC_UNKNOWN = 0xF3};

            enum enumTimeSource {ATOMIC = 0x10, GPS = 0x20, RADIO = 0x30, PTP = 0x40, NTP = 0x50, HANDSET = 0x60, OTHER = 0x90, INTERNAL = 0xA0};
    };

    struct PTP_IMPEXPORT ptpDelayResponse : public ptpV2Payload
    {
        ptpDelayResponse(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;

        ptpSource source;
    };



};
