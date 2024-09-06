#pragma once
#include <vector>
#include <string>
#include "timeutils.h"
#include "ptpdll.h"
#include <array>
#include <memory>
#include <map>

namespace pml
{
    namespace ptpmonkey
    {
        /** @struct header
        *   @brief abstract base class for the header of any message
        **/
        struct PTP_IMPEXPORT header
        {
            /** @brief Output the header values for debug purposes **/
            virtual void OutputValues()=0;
            virtual ~header()=default;
            std::string sIpAddress; ///< The ip address the message arrived from
        };

        /** @struct payload
        *   @brief abstract base class for the payload of any message
        **/
        struct PTP_IMPEXPORT payload
        {
            /** @brief Output the payload values for debug purposes **/
            virtual void OutputValues()=0;
            virtual ~payload()=default;
        };

        /** @struct ptpHeader
        *   @brief  base class for the header of a v1 and v2 ptp message
        **/
        struct PTP_IMPEXPORT ptpHeader : header
        {
            explicit ptpHeader(const std::chrono::nanoseconds& socketTime);
            ptpHeader()=default;
            

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
            ptpSource()=default;

            /** @brief Constructor
            *   @param vMessage a vector of unsigned char that contain the
            **/
            explicit ptpSource(const std::vector<unsigned char>& vMessage);
            void CreateMessage(std::vector<unsigned char>& vMessage, size_t nBegin) const;

            unsigned long long int nSourceId;
            std::string sSourceId;
            unsigned short nSourcePort;
        };

        struct PTP_IMPEXPORT ptpV2Header : public ptpHeader
        {
            ptpV2Header()=default;
            ptpV2Header(const std::chrono::nanoseconds& socketTime, const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            std::vector<unsigned char> CreateMessage() const;


            unsigned short nMessageLength;
            unsigned char nDomain;
            unsigned short nFlags;
            unsigned long long nCorrection;
            ptpSource source;
            unsigned short nSequenceId;
            unsigned char nControl;
            char nInterval;

            enum class enumType{SYNC=0x0, DELAY_REQ=0x1, PEER_DLEAY_REQ = 0x2, PEER_DELAY_RESP = 0x3, FOLLOW_UP=0x8, DELAY_RESP=0x9, PEER_DELAY_RESP_FOLLOW_UP = 0xA,
                    ANNOUNCE=0xB, SIGNALLING=0xC, MANAGEMENT=0xD};

            enum class enumFlags{ ALTERNATE_MASTER = 0x0100,
                            TWO_STEP         = 0x0200,
                            UNICAST          = 0x0400,
                            PROFILE1         = 0x2000,
                            PROFILE2         = 0x4000,
                            SECURITY         = 0x8000,
                            LI_61            = 0x0001,
                            LI_59            = 0x0002,
                            UTC_OFFSET_VALID = 0x0004,
                            TIMESCALE        = 0x0008,
                            TIME_TRACEABLE   = 0x0010,
                            FREQ_TRACEABLE   = 0x0020 };


        };



        struct PTP_IMPEXPORT ptpV2Payload : public ptpPayload
        {
            ptpV2Payload()=default;
            explicit ptpV2Payload(const std::vector<unsigned char>& vMessage);
            void OutputValues() override;
            std::vector<unsigned char> CreateMessage() const;


        };

        struct PTP_IMPEXPORT ptpAnnounce : public ptpV2Payload
        {
            explicit ptpAnnounce(const std::vector<unsigned char>& vMessage);
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


                enum class enumAccuracy { ACC_25NS = 0x20, ACC_100NS = 0x21, ACC_250NS = 0x22,
                                    ACC_1US = 0x23, ACC_2_5US = 0x24, ACC_10US = 0x25, ACC_25US = 0x26, ACC_100US = 0x27, ACC_250US = 0x28,
                                    ACC_1MS = 0x29, ACC_2_5MS = 0x2A, ACC_10MS = 0x2B, ACC_25MS = 0x2C, ACC_100MS = 0x2D, ACC_250MS = 0x2E,
                                    ACC_1S = 0x2F, ACC_10S = 0x30, ACC_OVER=0x31, ACC_UNKNOWN = 0xF3};

                enum class enumTimeSource {ATOMIC = 0x10, GPS = 0x20, RADIO = 0x30, PTP = 0x40, NTP = 0x50, HANDSET = 0x60, OTHER = 0x90, INTERNAL = 0xA0};
        };

        struct PTP_IMPEXPORT ptpDelayResponse : public ptpV2Payload
        {
            explicit ptpDelayResponse(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            ptpSource source;
        };


        struct portAddress {    
            uint16_t nProtocol;
            std::string sAddress;   //uint16_t length followed by letters
	    };

        struct tlv
        {
            explicit tlv(uint16_t id) : nId(id){} 
            virtual ~tlv();

            virtual void OutputValues()=0;

            uint16_t nId;
            size_t nPos = 0;
        };
 
        struct tlvBasic : public tlv
        {
            explicit tlvBasic(uint16_t id, const std::vector<unsigned char>& vMessage);

            void OutputValues() override;
            uint8_t nValue;
            uint8_t nReserved;
        };

        struct tlvTraceable : public tlvBasic
        {
            explicit tlvTraceable(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            bool bTimeTraceable;
            bool bFrequencyTraceable;
        };

        struct tlvTimescale : public tlvBasic
        {
            explicit tlvTimescale(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;
            bool bPtpTimescale;
        };

        struct tlvVersion : public tlvBasic
        {
            explicit tlvVersion(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            uint8_t nVersion;
        };

        struct tlvClockDescription : public tlv
        {

            explicit tlvClockDescription(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            bool bClockOrdinary;
            bool bClockBoundary;
            bool bClockTranparentP2P;
            bool bClockTranparentE2E;
            bool bClockManagement;
            std::string sPhysicalLayerProtocol;  //length (byte) followed by letters
            std::string sPhysicalAddress;  //length (byte) followed by letters
            portAddress protocolAddress;
            std::string sManufacturerIdentity;
            std::string sProductDescription;
            std::string sRevisionData;
            std::string sUserDescription;
            std::string sProfileIdentity;
        };

        struct clockQuality
        {
            uint8_t  nClass;
            uint8_t  nAccuracy;
            uint16_t  nOffsetScaledLogVariance;
        };

        struct tlvDefaultDataSet : public tlv
        {
            explicit tlvDefaultDataSet(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            uint8_t nFlags;
            uint8_t nReserved1;
            uint16_t nNumberPorts;
            uint8_t  nPriority1;
            clockQuality  quality;
            uint8_t nPriority2;
            std::string sClockIdentity;
            uint8_t nDomain;
            uint8_t nReserved2;

            bool b2Step;
            bool bSlaveOnly;
        };

        struct tlvCurrentDataSet  : public tlv
        {
            explicit tlvCurrentDataSet(const std::vector<unsigned char>& vMessage);
	        void OutputValues() final;

            uint16_t nStepsRemoved;
            std::chrono::nanoseconds offsetFromMaster; //(uint64)
	        std::chrono::nanoseconds meanPathDelay; //(uint64)
        };

        struct tlvParentDataSet : public tlv 
        {
            explicit tlvParentDataSet(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            std::string  sParentPortIdentity;
            uint8_t nParentStats;
            uint8_t nReserved;
            uint16_t nObservedParentOffsetScaledLogVariance;
            uint32_t nObservedParentClockPhaseChangeRate;
            uint8_t  nGrandmasterPriority1;
            clockQuality grandmasterClockQuality;
            uint8_t nGrandmasterPriority2;
            std::string sGrandmasterIdentity;
        };

        struct tlvTimePropertiesDataSet : public tlv 
        {
            explicit tlvTimePropertiesDataSet(const std::vector<unsigned char>& vMessage);
	        void OutputValues() final;

            uint16_t nCurrentUtcOffset;
	        uint8_t nFlags;
	        uint8_t nTimeSource;

            bool bLeap61;
            bool bLeap59;
            bool bUtcOffsetValid;
            bool bPtpTimescale;
            bool bTimeTraceable;
            bool bFreqTraceable;
        };

        struct tlvPortDataSet : public tlv
        { 
            explicit tlvPortDataSet(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            std::string sPortIdentity;      // 8 bytes;
            uint8_t nPortState;
            uint8_t nLogMinDelayReqInterval;
            std::chrono::nanoseconds peerMeanPathDelay;
            int8_t  nLogAnnounceInterval;
            uint8_t nAnnounceReceiptTimeout;
            int8_t  nLogSyncInterval;
            uint8_t nDelayMechanism;
            int8_t  nLogMinPdelayReqInterval;
            uint8_t nVersionNumber;

            static const std::array<std::string, 11> PORT_STATES; 
            static const std::array<std::string, 5> TIMESTAMPING;
        };


        struct tlvAlternateTimeOffsetName  : public tlv
        {
            explicit tlvAlternateTimeOffsetName(const std::vector<unsigned char>& vMessage);
	        void OutputValues() final;

            uint8_t nKeyField;
	        std::string sName;
        };
        struct tlvAlternateTimeOffsetProperties  : public tlv
        {
            explicit tlvAlternateTimeOffsetProperties(const std::vector<unsigned char>& vMessage);
	        void OutputValues() final;

            uint8_t nKeyField;
            int32_t nCurrentOffset;
	        int32_t nJumpSeconds;
            std::chrono::seconds nextJump;
        };

        struct tlvTimeStatusNP : public tlv
        {
            explicit tlvTimeStatusNP(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            struct scaledNs 
            {
                uint16_t nanoseconds_msb;
                uint64_t nanoseconds_lsb;
                uint16_t fractional_nanoseconds;
            };

            std::chrono::nanoseconds masterOffset; /*nanoseconds*/
            std::chrono::nanoseconds ingressTime;  /*nanoseconds*/
            int32_t nCumulativeScaledRateOffset;
            int32_t nScaledLastGmPhaseChange;
            uint16_t nGmTimeBaseIndicator;
            scaledNs lastGmPhaseChange;
            bool bGmPresent;
            std::string sGmIdentity;
        };
        
        struct tlvGrandmasterSettingsNP : public tlv 
        {
            explicit tlvGrandmasterSettingsNP(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

	        clockQuality quality;
	        int16_t nUtcOffset;
	        uint8_t nTimeFlags;
            uint8_t nTimeSource;

            bool bLeap61;
			bool bLeap59;
			bool bCurrentUtcOffsetValid;
			bool bPtpTimescale;
			bool bTimeTraceable;
			bool bFrequencyTraceable;
        };

        struct tlvPortPropertiesNP : public tlv 
        {
            explicit tlvPortPropertiesNP(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            std::string sPortIdentity; //8-2
            uint8_t nPortState;
	        uint8_t nTimestamping;
	        std::string sInterface;
        };

        struct tlvPortStatsNP : public tlv
        {
            explicit tlvPortStatsNP(const std::vector<unsigned char>& vMessage);
            void OutputValues() final;

            std::string sPortIdentity; //8-2
            std::array<uint64_t, 16> rx;
            std::array<uint64_t, 16> tx;
        };

























        struct PTP_IMPEXPORT managementTlv 
        {
            enum enumType {MANAGEMENT = 0x0001, MANAGEMENT_ERROR_STATUS = 0x0002, ORGANIZATION_EXTENSION = 0x0003, REQUEST_UNICAST_TRANSMISSION = 0x0004, GRANT_UNICAST_TRANSMISSION = 0x0005, CANCEL_UNICAST_TRANSMISSION = 0x0006, ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION = 0x0007, PATH_TRACE = 0x0008, ALTERNATE_TIME_OFFSET_INDICATOR = 0x0009, AUTHENTICATION_2008 = 0x2000, AUTHENTICATION_CHALLENGE = 0x2001, SECURITY_ASSOCIATION_UPDATE = 0x2002, CUM_FREQ_SCALE_FACTOR_OFFSET = 0x2003, PTPMON_REQ = 0x21FE, PTPMON_RESP = 0x21FF, ORGANIZATION_EXTENSION_PROPAGATE = 0x4000, ENHANCED_ACCURACY_METRICS = 0x4001, O, RGANIZATION_EXTENSION_DO_NOT_PROPAGATE = 0x8000, L1_SYNC = 0x8001, PORT_COMMUNICATION_AVAILABILITY = 0x8002, PROTOCOL_ADDRESS = 0x8003, SLAVE_RX_SYNC_TIMING_DATA = 0x8004, SLAVE_RX_SYNC_COMPUTED_DATA = 0x8005,SLAVE_TX_EVENT_TIMESTAMPS = 0x8006, SLAVE_DELAY_TIMING_DATA_NP = 0x7F00, CUMULATIVE_RATE_RATIO = 0x8007, PAD = 0x8008, AUTHENTICATION = 0x8009 };
            enum enumId {USER_DESCRIPTION = 0x0002, SAVE_IN_NON_VOLATILE_STORAGE = 0x0003, RESET_NON_VOLATILE_STORAGE = 0x0004, INITIALIZE = 0x0005, FAULT_LOG = 0x0006, FAULT_LOG_RESET = 0x0007, DEFAULT_DATA_SET = 0x2000, CURRENT_DATA_SET = 0x2001, PARENT_DATA_SET = 0x2002, TIME_PROPERTIES_DATA_SET = 0x2003, PRIORITY1 = 0x2005, PRIORITY2 = 0x2006, DOMAIN = 0x2007, SLAVE_ONLY = 0x2008, TIME = 0x200F, CLOCK_ACCURACY = 0x2010, UTC_PROPERTIES = 0x2011, TRACEABILITY_PROPERTIES = 0x2012, TIMESCALE_PROPERTIES = 0x2013, PATH_TRACE_LIST = 0x2015, PATH_TRACE_ENABLE = 0x2016, GRANDMASTER_CLUSTER_TABLE = 0x2017, ACCEPTABLE_MASTER_TABLE = 0x201A, ACCEPTABLE_MASTER_MAX_TABLE_SIZE = 0x201C, ALTERNATE_TIME_OFFSET_ENABLE = 0x201E, ALTERNATE_TIME_OFFSET_NAME = 0x201F, ALTERNATE_TIME_OFFSET_MAX_KEY = 0x2020, ALTERNATE_TIME_OFFSET_PROPERTIES = 0x2021, EXTERNAL_PORT_CONFIGURATION_ENABLED = 0x3000, HOLDOVER_UPGRADE_ENABLE = 0x3002, TRANSPARENT_CLOCK_DEFAULT_DATA_SET = 0x4000, PRIMARY_DOMAIN = 0x4002, TIME_STATUS_NP = 0xC000, GRANDMASTER_SETTINGS_NP = 0xC001, SUBSCRIBE_EVENTS_NP = 0xC003, SYNCHRONIZATION_UNCERTAIN_NP = 0xC006, NULL_MANAGEMENT = 0x0000, CLOCK_DESCRIPTION = 0x0001, PORT_DATA_SET = 0x2004, LOG_ANNOUNCE_INTERVAL = 0x2009, ANNOUNCE_RECEIPT_TIMEOUT = 0x200A, LOG_SYNC_INTERVAL = 0x200B, VERSION_NUMBER = 0x200C, ENABLE_PORT = 0x200D, DISABLE_PORT = 0x200E, UNICAST_NEGOTIATION_ENABLE = 0x2014, UNICAST_MASTER_TABLE = 0x2018, UNICAST_MASTER_MAX_TABLE_SIZE = 0x2019, ACCEPTABLE_MASTER_TABLE_ENABLED = 0x201B, ALTERNATE_MASTER = 0x201D, MASTER_ONLY = 0x3001, EXT_PORT_CONFIG_PORT_DATA_SET = 0x3003, TRANSPARENT_CLOCK_PORT_DATA_SET = 0x4001, DELAY_MECHANISM = 0x6000, LOG_MIN_PDELAY_REQ_INTERVAL = 0x6001, PORT_DATA_SET_NP = 0xC002, PORT_PROPERTIES_NP = 0xC004, PORT_STATS_NP = 0xC005, PORT_SERVICE_STATS_NP = 0xC007, UNICAST_MASTER_TABLE_NP = 0xC008, PORT_HWCLOCK_NP = 0xC009, POWER_PROFILE_SETTINGS_NP = 0xC00A, CMLDS_INFO_NP = 0xC00B };

            explicit managementTlv(const std::vector<unsigned char>& vMessage);
            ~managementTlv()=default;

            void OutputValues();

            void ParseTlv(const std::vector<unsigned char>& vMessage);

            size_t nPos = 0;
	        uint16_t nType;
	        uint16_t nLength; /* must be even */
            uint16_t nId;
            std::unique_ptr<tlv> pData;

            static const std::map<enumId, std::string> TLV_ID;
        };

        struct PTP_IMPEXPORT ptpManagement : public ptpV2Payload
        {
            explicit ptpManagement(const std::vector<unsigned char>& vMessage);
            ~ptpManagement() final;
            void OutputValues() final;

            std::string sTargetPortIdentity;
            uint16_t nTargetPortId;
            uint8_t  nStartingBoundaryHops;
            uint8_t  nBoundaryHops;
            uint8_t  flags; /* reserved | actionField */
            uint8_t  nAction;
            uint8_t  nReserved;
            std::unique_ptr<managementTlv> pTlv = nullptr;


            enum enumAction {GET, SET, RESPONSE, COMMAND, ACKNOWLEDGE};
            enum enumError{RESPONSE_TOO_BIG=1, NO_SUCH_ID, WRONG_LENGTH, WRONG_VALUE, NOT_SETABLE, NOT_SUPPORTED, GENERAL_ERROR	 = 0xFFFE };

            static const std::array<std::string, 5> MANAGEMENT_ACTION;
        };

        
    }
}
