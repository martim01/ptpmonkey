#pragma once
#include <vector>
#include <string>
#include "timeutils.h"
#include "ptpdll.h"
#include <array>
#include <memory>
#include <map>
#include "enums.h"
namespace pml::ptpmonkey
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

    };



    struct PTP_IMPEXPORT ptpV2Payload : public ptpPayload
    {
        ptpV2Payload()=default;
        explicit ptpV2Payload(const std::vector<unsigned char>& vMessage);
        void OutputValues() override;
        virtual std::vector<unsigned char> CreateMessage() const;

    };

    struct PTP_IMPEXPORT ptpAnnounce : public ptpV2Payload
    {
        explicit ptpAnnounce(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;

        unsigned short nUtcOffset;
        unsigned char nGrandmasterPriority1;
        unsigned char nGrandmasterClass;
        clck::enumAccuracy eGrandmasterAccuracy;
        unsigned short nGrandmasterVariance;
        unsigned char nGrandmasterPriority2;
        std::string sGrandmasterClockId;
        unsigned short nStepsRemoved;
        clck::enumTimeSource eTimeSource;

    };

    struct PTP_IMPEXPORT ptpDelayResponse : public ptpV2Payload
    {
        explicit ptpDelayResponse(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;

        ptpSource source;
    };


    struct PTP_IMPEXPORT portAddress {    
        uint16_t nProtocol{0};
        std::string sAddress;   //uint16_t length followed by letters
    };

    struct PTP_IMPEXPORT tlv
    {
        tlv()=default;
        explicit tlv(mngmnt::enumId id) : eId(id){} 
        virtual ~tlv();

        virtual void OutputValues()=0;
        virtual std::vector<unsigned char> CreateMessage() const=0;

        mngmnt::enumId eId;
        size_t nPos = 0;
    };

    struct PTP_IMPEXPORT tlvBasic : public tlv
    {
        tlvBasic(mngmnt::enumId id) : tlv(id){}

        explicit tlvBasic(mngmnt::enumId id, const std::vector<unsigned char>& vMessage);

        void OutputValues() override;
        std::vector<unsigned char> CreateMessage() const override;

        uint8_t nValue{0};
        uint8_t nReserved{0};
    };

    struct PTP_IMPEXPORT tlvClockAccuracy : public tlvBasic
    {
        tlvClockAccuracy() : tlvBasic(mngmnt::enumId::CLOCK_ACCURACY){}
        explicit tlvClockAccuracy(const std::vector<unsigned char>& vMessage);

        void OutputValues() override;
        
    };

    struct PTP_IMPEXPORT tlvTraceable : public tlvBasic
    {
        tlvTraceable() : tlvBasic(mngmnt::enumId::TRACEABILITY_PROPERTIES){}
        
        explicit tlvTraceable(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        bool bTimeTraceable;
        bool bFrequencyTraceable;
    };

    struct PTP_IMPEXPORT tlvTimescale : public tlvBasic
    {
        tlvTimescale() : tlvBasic(mngmnt::enumId::TIMESCALE_PROPERTIES){}
        explicit tlvTimescale(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        bool bPtpTimescale;
    };

    struct PTP_IMPEXPORT tlvVersion : public tlvBasic
    {
        tlvVersion() : tlvBasic(mngmnt::enumId::VERSION_NUMBER){}
        explicit tlvVersion(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        uint8_t nVersion;
    };

    struct PTP_IMPEXPORT tlvUserDescription : public tlv
    {
        tlvUserDescription() : tlv(mngmnt::enumId::USER_DESCRIPTION){}
        explicit tlvUserDescription(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        std::string sUserDescription;
    };

    struct PTP_IMPEXPORT tlvClockDescription : public tlv
    {
        tlvClockDescription() : tlv(mngmnt::enumId::CLOCK_DESCRIPTION){}
        explicit tlvClockDescription(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

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

    struct PTP_IMPEXPORT clockQuality
    {
        uint8_t  nClass{0};
        clck::enumAccuracy  eAccuracy{clck::enumAccuracy::ACC_NA};
        uint16_t  nOffsetScaledLogVariance{0};

        static const std::map<clck::enumAccuracy, std::string> ACCURACY;
        static const std::map<clck::enumTimeSource, std::string> SOURCE;
    };

    struct PTP_IMPEXPORT tlvDefaultDataSet : public tlv
    {
        tlvDefaultDataSet() : tlv(mngmnt::enumId::DEFAULT_DATA_SET){}
        explicit tlvDefaultDataSet(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        uint8_t nReserved1{0};
        uint16_t nNumberPorts{0};
        uint8_t  nPriority1{0};
        clockQuality  quality;
        uint8_t nPriority2{0};
        std::string sClockIdentity;
        uint8_t nDomain{0};
        uint8_t nReserved2{0};

        bool b2Step{false};
        bool bSlaveOnly{false};
    };

    struct subnano
    {
        std::chrono::nanoseconds nano{0};  //
        uint16_t sub{0};
    };

    struct PTP_IMPEXPORT tlvCurrentDataSet  : public tlv
    {
        tlvCurrentDataSet() : tlv(mngmnt::enumId::CURRENT_DATA_SET){}
        explicit tlvCurrentDataSet(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        uint16_t nStepsRemoved{0};
        subnano offsetFromMaster; //(uint64)
        subnano meanPathDelay; //(uint64)
    };

    struct PTP_IMPEXPORT tlvParentDataSet : public tlv 
    {
        tlvParentDataSet() : tlv(mngmnt::enumId::PARENT_DATA_SET){};
        explicit tlvParentDataSet(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        std::string  sParentPortIdentity;
        uint8_t nParentStats{0};
        uint8_t nReserved{0};
        uint16_t nObservedParentOffsetScaledLogVariance{0};
        uint32_t nObservedParentClockPhaseChangeRate{0};
        uint8_t  nGrandmasterPriority1{0};
        clockQuality grandmasterClockQuality;
        uint8_t nGrandmasterPriority2{0};
        std::string sGrandmasterIdentity;
    };

    struct PTP_IMPEXPORT tlvTimePropertiesDataSet : public tlv 
    {
        tlvTimePropertiesDataSet() : tlv(mngmnt::enumId::TIME_PROPERTIES_DATA_SET){};
        explicit tlvTimePropertiesDataSet(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        uint16_t nCurrentUtcOffset{0};
        clck::enumTimeSource eTimeSource{clck::enumTimeSource::NA};

        bool bLeap61{false};
        bool bLeap59{false};
        bool bUtcOffsetValid{false};
        bool bPtpTimescale{false};
        bool bTimeTraceable{false};
        bool bFreqTraceable{false};
    };

    struct PTP_IMPEXPORT tlvPortDataSet : public tlv
    { 
        tlvPortDataSet() : tlv(mngmnt::enumId::PORT_DATA_SET){};
        explicit tlvPortDataSet(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;


        std::string sPortIdentity;      // 8 bytes;
        uint8_t nPortState{0};
        uint8_t nLogMinDelayReqInterval{0};
        subnano peerMeanPathDelay;
        int8_t  nLogAnnounceInterval{0};
        uint8_t nAnnounceReceiptTimeout{0};
        int8_t  nLogSyncInterval{0};
        uint8_t nDelayMechanism{0};
        int8_t  nLogMinPdelayReqInterval{0};
        uint8_t nVersionNumber{0};

        static const std::array<std::string, 11> PORT_STATES; 
        static const std::array<std::string, 5> TIMESTAMPING;
    };


    struct PTP_IMPEXPORT tlvAlternateTimeOffsetName  : public tlv
    {
        tlvAlternateTimeOffsetName() : tlv(mngmnt::enumId::ALTERNATE_TIME_OFFSET_NAME){};
        explicit tlvAlternateTimeOffsetName(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        uint8_t nKeyField;
        std::string sName;
    };
    struct PTP_IMPEXPORT tlvAlternateTimeOffsetProperties  : public tlv
    {
        tlvAlternateTimeOffsetProperties() : tlv(mngmnt::enumId::ALTERNATE_TIME_OFFSET_PROPERTIES){};
        explicit tlvAlternateTimeOffsetProperties(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        uint8_t nKeyField{0};
        int32_t nCurrentOffset{0};
        int32_t nJumpSeconds{0};
        std::chrono::seconds nextJump{0};
    };

    struct PTP_IMPEXPORT tlvTimeStatusNP : public tlv
    {
        tlvTimeStatusNP() : tlv(mngmnt::enumId::TIME_STATUS_NP){};
        explicit tlvTimeStatusNP(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        struct scaledNs 
        {
            uint16_t nanoseconds_msb{0};
            uint64_t nanoseconds_lsb{0};
            uint16_t fractional_nanoseconds{0};
        };

        std::chrono::nanoseconds masterOffset{0}; /*nanoseconds*/
        std::chrono::nanoseconds ingressTime{0};  /*nanoseconds*/
        int32_t nCumulativeScaledRateOffset{0};
        int32_t nScaledLastGmPhaseChange{0};
        uint16_t nGmTimeBaseIndicator{0};
        scaledNs lastGmPhaseChange{0};
        bool bGmPresent{false};
        std::string sGmIdentity{0};
    };
    
    struct PTP_IMPEXPORT tlvGrandmasterSettingsNP : public tlv 
    {
        tlvGrandmasterSettingsNP() : tlv(mngmnt::enumId::GRANDMASTER_SETTINGS_NP){};
        explicit tlvGrandmasterSettingsNP(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        clockQuality quality;
        int16_t nUtcOffset{0};
        clck::enumTimeSource eTimeSource{clck::enumTimeSource::NA};

        bool bLeap61{false};
        bool bLeap59{false};
        bool bCurrentUtcOffsetValid{false};
        bool bPtpTimescale{false};
        bool bTimeTraceable{false};
        bool bFrequencyTraceable{false};
    };

    struct PTP_IMPEXPORT tlvPortPropertiesNP : public tlv 
    {
        tlvPortPropertiesNP() : tlv(mngmnt::enumId::PORT_PROPERTIES_NP){};
        explicit tlvPortPropertiesNP(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        std::string sPortIdentity; //8-2
        uint8_t nPortState{0};
        uint8_t nTimestamping{0};
        std::string sInterface{0};
    };

    struct PTP_IMPEXPORT tlvPortStatsNP : public tlv
    {
        tlvPortStatsNP() : tlv(mngmnt::enumId::PORT_STATS_NP){};
        explicit tlvPortStatsNP(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        std::string sPortIdentity; //8-2
        std::array<uint64_t, 16> rx;
        std::array<uint64_t, 16> tx;
    };

    struct PTP_IMPEXPORT tlvPortServiceStatsNP : public tlv
    {
        tlvPortServiceStatsNP() : tlv(mngmnt::enumId::PORT_SERVICE_STATS_NP){};
        explicit tlvPortServiceStatsNP(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        std::string sPortIdentity; //8-2
        uint64_t nAnnounceTimeout{0};
        uint64_t nSyncTimeout{0};
        uint64_t nDelayTimeout{0};
        uint64_t nUnicastServiceTimeout{0};
        uint64_t nUnicastRequestTimeout{0};
        uint64_t nMasterAnnounceTimeout{0};
        uint64_t nMasterSyncTimeout{0};
        uint64_t nQualificationTimeout{0};
        uint64_t nSyncMismatch{0};
        uint64_t nFollowupMismatch{0};

    };

    struct PTP_IMPEXPORT tlvPortDataSetNP : public tlv
    {
        tlvPortDataSetNP() : tlv(mngmnt::enumId::PORT_DATA_SET_NP){};
        explicit tlvPortDataSetNP(const std::vector<unsigned char>& vMessage);
        void OutputValues() final;
        std::vector<unsigned char> CreateMessage() const final;

        std::chrono::nanoseconds neighbourPropDelayThresh{0}; /*uint32_t */
	    bool bCapable{false}; //uint32_t
        
    };
    
    
    struct PTP_IMPEXPORT managementTlv 
    {
        virtual void OutputValues()=0;
        virtual void CreateMessage(std::vector<unsigned char>& vMessage) const = 0;

        size_t nPos = 0;
        mngmnt::enumType eType;
        uint16_t nLength{0}; /* must be even */
        mngmnt::enumId eId;
        std::shared_ptr<tlv> pData = nullptr;
        mngmnt::enumError eError{mngmnt::enumError::OKAY};

        static const std::map<mngmnt::enumId, std::string> TLV_ID;
    };

    struct PTP_IMPEXPORT managementTlvResponse : public managementTlv
    {
        explicit managementTlvResponse(const std::vector<unsigned char>& vMessage);
        managementTlvResponse(mngmnt::enumType eT, mngmnt::enumGet eI);
        managementTlvResponse(mngmnt::enumType eT, mngmnt::enumSet eI);

        ~managementTlvResponse()=default;

        void OutputValues() final;

        void ParseTlv(const std::vector<unsigned char>& vMessage);
        void CreateMessage(std::vector<unsigned char>& vMessage) const final;

        static const std::map<mngmnt::enumError, std::string> MNGMNT_ERROR;
    };

    
    struct PTP_IMPEXPORT ptpManagement : public ptpV2Payload
    {
        explicit ptpManagement(const std::vector<unsigned char>& vMessage);
        ptpManagement(mngmnt::enumGet id, uint8_t nHops, const std::string& sTargetPortId, uint16_t nTargetPortNumber);
        ptpManagement(mngmnt::enumSet id, uint8_t nHops, const std::string& sTargetPortId, uint16_t nTargetPortNumber);
        

        ~ptpManagement() final;
        void OutputValues() final;

        std::vector<unsigned char> CreateMessage() const final;

        std::string sTargetPortIdentity{"FF:FF:FF:FF:FF:FF:FF:FF-65535"};
        uint16_t nTargetPortId{0};
        uint8_t  nStartingBoundaryHops{0};
        uint8_t  nBoundaryHops{0};
        uint8_t  flags{0}; /* reserved | actionField */
        mngmnt::enumAction eAction;
        uint8_t  nReserved{0};
        std::shared_ptr<managementTlv> pTlv = nullptr;

        static const std::array<std::string, 5> MANAGEMENT_ACTION;
    };
   
}
