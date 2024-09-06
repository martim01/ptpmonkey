#include "ptpstructs.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "timeutils.h"
#include "log.h"
#include <algorithm>

using namespace pml::ptpmonkey;

const std::array<std::string, 5> ptpManagement::MANAGEMENT_ACTION{"GET", "SET", "RESPONSE", "COMMAND", "ACKNOWLEDGE"};

const std::map<managementTlv::enumId, std::string> managementTlv::TLV_ID{{managementTlv::enumId::USER_DESCRIPTION, "USER_DESCRIPTION"}, {managementTlv::enumId::SAVE_IN_NON_VOLATILE_STORAGE, "SAVE_IN_NON_VOLATILE_STORAGE"},  {managementTlv::enumId::RESET_NON_VOLATILE_STORAGE, "RESET_NON_VOLATILE_STORAGE"},{managementTlv::enumId::INITIALIZE, "INITIALIZE"}, {managementTlv::enumId::FAULT_LOG, "FAULT_LOG"}, {managementTlv::enumId::FAULT_LOG_RESET, "FAULT_LOG_RESET"}, {managementTlv::enumId::DEFAULT_DATA_SET, "DEFAULT_DATA_SET"}, {managementTlv::enumId::CURRENT_DATA_SET, "CURRENT_DATA_SET"}, {managementTlv::enumId::PARENT_DATA_SET, "PARENT_DATA_SET"}, {managementTlv::enumId::TIME_PROPERTIES_DATA_SET,"TIME_PROPERTIES_DATA_SET"}, {managementTlv::enumId::PRIORITY1, "PRIORITY1"}, {managementTlv::enumId::PRIORITY2, "PRIORITY2"}, {managementTlv::enumId::DOMAIN, "DOMAIN"}, {managementTlv::enumId::SLAVE_ONLY, "SLAVE_ONLY"}, {managementTlv::enumId::TIME, "TIME"}, {managementTlv::enumId::CLOCK_ACCURACY, "CLOCK_ACCURACY"}, {managementTlv::enumId::UTC_PROPERTIES, "UTC_PROPERTIES"}, {managementTlv::enumId::TRACEABILITY_PROPERTIES, "TRACEABILITY_PROPERTIES"}, {managementTlv::enumId::TIMESCALE_PROPERTIES, "TIMESCALE_PROPERTIES"}, {managementTlv::enumId::PATH_TRACE_LIST, "PATH_TRACE_LIST"}, {managementTlv::enumId::PATH_TRACE_ENABLE, "PATH_TRACE_ENABLE"}, {managementTlv::enumId::GRANDMASTER_CLUSTER_TABLE, "GRANDMASTER_CLUSTER_TABLE"}, {managementTlv::enumId::ACCEPTABLE_MASTER_TABLE, "ACCEPTABLE_MASTER_TABLE"}, {managementTlv::enumId::ACCEPTABLE_MASTER_MAX_TABLE_SIZE, "ACCEPTABLE_MASTER_MAX_TABLE_SIZE"}, {managementTlv::enumId::ALTERNATE_TIME_OFFSET_ENABLE, "ALTERNATE_TIME_OFFSET_ENABLE"}, {managementTlv::enumId::ALTERNATE_TIME_OFFSET_NAME, "ALTERNATE_TIME_OFFSET_NAME"}, {managementTlv::enumId::ALTERNATE_TIME_OFFSET_MAX_KEY, "ALTERNATE_TIME_OFFSET_MAX_KEY"}, {managementTlv::enumId::ALTERNATE_TIME_OFFSET_PROPERTIES, "ALTERNATE_TIME_OFFSET_PROPERTIES"}, {managementTlv::enumId::EXTERNAL_PORT_CONFIGURATION_ENABLED, "EXTERNAL_PORT_CONFIGURATION_ENABLED"}, {managementTlv::enumId::HOLDOVER_UPGRADE_ENABLE, "HOLDOVER_UPGRADE_ENABLE"}, {managementTlv::enumId::TRANSPARENT_CLOCK_DEFAULT_DATA_SET, "TRANSPARENT_CLOCK_DEFAULT_DATA_SET"}, {managementTlv::enumId::PRIMARY_DOMAIN, "PRIMARY_DOMAIN"}, {managementTlv::enumId::TIME_STATUS_NP, "TIME_STATUS_NP"}, {managementTlv::enumId::GRANDMASTER_SETTINGS_NP, "GRANDMASTER_SETTINGS_NP"}, {managementTlv::enumId::SUBSCRIBE_EVENTS_NP, "SUBSCRIBE_EVENTS_NP"}, {managementTlv::enumId::SYNCHRONIZATION_UNCERTAIN_NP, "SYNCHRONIZATION_UNCERTAIN_NP"}, {managementTlv::enumId::NULL_MANAGEMENT, "NULL_MANAGEMENT"}, {managementTlv::enumId::CLOCK_DESCRIPTION, "CLOCK_DESCRIPTION"}, {managementTlv::enumId::PORT_DATA_SET, "PORT_DATA_SET"}, {managementTlv::enumId::LOG_ANNOUNCE_INTERVAL, "LOG_ANNOUNCE_INTERVAL"}, {managementTlv::enumId::ANNOUNCE_RECEIPT_TIMEOUT, "ANNOUNCE_RECEIPT_TIMEOUT"}, {managementTlv::enumId::LOG_SYNC_INTERVAL, "LOG_SYNC_INTERVAL"}, {managementTlv::enumId::VERSION_NUMBER, "VERSION_NUMBER"}, {managementTlv::enumId::ENABLE_PORT, "ENABLE_PORT"}, {managementTlv::enumId::DISABLE_PORT, "DISABLE_PORT"}, {managementTlv::enumId::UNICAST_NEGOTIATION_ENABLE, "UNICAST_NEGOTIATION_ENABLE"}, {managementTlv::enumId::UNICAST_MASTER_TABLE, "UNICAST_MASTER_TABLE"}, {managementTlv::enumId::UNICAST_MASTER_MAX_TABLE_SIZE, "UNICAST_MASTER_MAX_TABLE_SIZE"}, {managementTlv::enumId::ACCEPTABLE_MASTER_TABLE_ENABLED, "ACCEPTABLE_MASTER_TABLE_ENABLED"}, {managementTlv::enumId::ALTERNATE_MASTER, "ALTERNATE_MASTER"}, {managementTlv::enumId::MASTER_ONLY, "MASTER_ONLY"}, {managementTlv::enumId::EXT_PORT_CONFIG_PORT_DATA_SET, "EXT_PORT_CONFIG_PORT_DATA_SET"}, {managementTlv::enumId::TRANSPARENT_CLOCK_PORT_DATA_SET,"TRANSPARENT_CLOCK_PORT_DATA_SET"}, {managementTlv::enumId::DELAY_MECHANISM, "DELAY_MECHANISM"}, {managementTlv::enumId::LOG_MIN_PDELAY_REQ_INTERVAL, "LOG_MIN_PDELAY_REQ_INTERVAL"}, {managementTlv::enumId::PORT_DATA_SET_NP, "PORT_DATA_SET_NP"}, {managementTlv::enumId::PORT_PROPERTIES_NP, "PORT_PROPERTIES_NP"}, {managementTlv::enumId::PORT_STATS_NP, "PORT_STATS_NP"}, {managementTlv::enumId::PORT_SERVICE_STATS_NP, "PORT_SERVICE_STATS_NP"}, {managementTlv::enumId::UNICAST_MASTER_TABLE_NP, "UNICAST_MASTER_TABLE_NP"}, {managementTlv::enumId::PORT_HWCLOCK_NP, "PORT_HWCLOCK_NP"}, {managementTlv::enumId::POWER_PROFILE_SETTINGS_NP, "POWER_PROFILE_SETTINGS_NP"}, {managementTlv::enumId::CMLDS_INFO_NP, "CMLDS_INFO_NP"}};


const std::array<std::string, 11> tlvPortDataSet::PORT_STATES{"NONE","INITIALIZING","FAULTY","DISABLED","LISTENING","PRE_MASTER","MASTER","PASSIVE","UNCALIBRATED","SLAVE","GRAND_MASTER"};
const std::array<std::string, 5> tlvPortDataSet::TIMESTAMPING{"SOFTWARE", "HARDWARE", "LEGACY_HW", "ONESTEP", "P2P1STEP"};


uint8_t ToU8(const std::vector<unsigned char>& vMessage, size_t& nPos)
{
	auto value = vMessage[nPos];
	++nPos;
	return value;
}

int8_t To8(const std::vector<unsigned char>& vMessage, size_t& nPos)
{
	auto value = vMessage[nPos];
	++nPos;
	return static_cast<int8_t>(value);
}

uint16_t ToU16(const std::vector<unsigned char>& vMessage, size_t& nPos)
{
    auto value =  static_cast<uint16_t>((static_cast<uint16_t>(vMessage[nPos]) << 8) + static_cast<uint16_t>(vMessage[nPos+1]));
	nPos += 2;
	return value;
}

int16_t To16(const std::vector<unsigned char>& vMessage, size_t& nPos)
{
    auto value =  static_cast<int16_t>((static_cast<int16_t>(vMessage[nPos]) << 8) + static_cast<int16_t>(vMessage[nPos+1]));
	nPos += 2;
	return value;
}

uint32_t ToU32(const std::vector<unsigned char>& vMessage, size_t& nPos)
{
    auto value =  ((static_cast<uint32_t>(vMessage[nPos]) << 24) +
										(static_cast<uint32_t>(vMessage[nPos+1]) << 16) +
										(static_cast<uint32_t>(vMessage[nPos+2]) << 8) + 
										 static_cast<uint32_t>(vMessage[nPos+3]));
	nPos += 4;
	return value;
}

int32_t To32(const std::vector<unsigned char>& vMessage, size_t& nPos)
{
    auto value =  ((static_cast<int32_t>(vMessage[nPos]) << 24) +
										(static_cast<int32_t>(vMessage[nPos+1]) << 16) +
										(static_cast<int32_t>(vMessage[nPos+2]) << 8) + 
										 static_cast<int32_t>(vMessage[nPos+3]));
	nPos += 4;
	return value;
}


uint64_t ToU64(const std::vector<unsigned char>& vMessage, size_t& nPos)
{
    auto value =  ((static_cast<uint64_t>(vMessage[nPos]) << 56) +
										(static_cast<uint64_t>(vMessage[nPos+1]) << 48) +
										(static_cast<uint64_t>(vMessage[nPos+2]) << 40) +
										(static_cast<uint64_t>(vMessage[nPos+3]) << 32) +
										(static_cast<uint64_t>(vMessage[nPos+4]) << 24) +
										(static_cast<uint64_t>(vMessage[nPos+5]) << 16) +
										(static_cast<uint64_t>(vMessage[nPos+6]) << 8) + 
										 static_cast<uint64_t>(vMessage[nPos+7]));
	nPos += 8;
	return value;
}

int64_t To64(const std::vector<unsigned char>& vMessage, size_t& nPos)
{
    auto value =   ((static_cast<int64_t>(vMessage[nPos]) << 56) +
										(static_cast<int64_t>(vMessage[nPos+1]) << 48) +
										(static_cast<int64_t>(vMessage[nPos+2]) << 40) +
										(static_cast<int64_t>(vMessage[nPos+3]) << 32) +
										(static_cast<int64_t>(vMessage[nPos+4]) << 24) +
										(static_cast<int64_t>(vMessage[nPos+5]) << 16) +
										(static_cast<int64_t>(vMessage[nPos+6]) << 8) + 
										 static_cast<int64_t>(vMessage[nPos+7]));
	nPos += 8;
	return value;
}

std::string hex2str(const std::vector<unsigned char>& vMessage, size_t& nStart, size_t nLength)
{
	std::stringstream ssId;

    for(size_t i = 0; i < nLength; i++)
    {
        if(i != 0)
        {
            ssId << ":";
        }
        ssId << std::setw(2) << std::setfill('0') << std::hex;
        ssId << (int)vMessage[i+nStart];
    }
	nStart += nLength;

	return ssId.str();
}

std::string makestr(const std::vector<unsigned char>& vMessage, size_t& nStart, size_t nLength)
{
	if(nStart+nLength < vMessage.size())
	{
		return std::string(vMessage.begin()+nStart, vMessage.begin()+nStart+nLength);
	}
	nStart += nLength;
	return std::string();
}

std::string makestr16(const std::vector<unsigned char>& vMessage, size_t& nStart)
{
	uint16_t nLength = ToU16(vMessage, nStart);

	return makestr(vMessage, nStart, nLength);
}

std::string makestr8(const std::vector<unsigned char>& vMessage, size_t& nStart)
{
	uint8_t nLength = ToU8(vMessage, nStart);

	return makestr(vMessage, nStart, nLength);
}

std::string pid2str(const std::vector<unsigned char>& vMessage, size_t& nStart)
{
	return hex2str(vMessage, nStart, 8)+"-"+std::to_string(ToU16(vMessage, nStart));
}


ptpSource::ptpSource(const std::vector<unsigned char>& vMessage)
{
	size_t nPos = 0;
	sSourceId = hex2str(vMessage, nPos, 8);
	nSourcePort = ToU16(vMessage, nPos);
	sSourceId += "-"+std::to_string(nSourcePort);

    nSourceId = (static_cast<unsigned long long int>(vMessage[0]) << 56) |
                (static_cast<unsigned long long int>(vMessage[1]) << 48) |
                (static_cast<unsigned long long int>(vMessage[2]) << 40) |
                (static_cast<unsigned long long int>(vMessage[3]) << 32) |
                (static_cast<unsigned long long int>(vMessage[4]) << 24) |
                (static_cast<unsigned long long int>(vMessage[5]) << 16) |
                (static_cast<unsigned long long int>(vMessage[6]) << 8)  |
                 vMessage[7];

}

void ptpSource::CreateMessage(std::vector<unsigned char>& vMessage, size_t nBegin) const
{
    vMessage[nBegin+0] = static_cast<unsigned char>(nSourceId >> 56);
    vMessage[nBegin+1] = static_cast<unsigned char>(nSourceId >> 48);
    vMessage[nBegin+2] = static_cast<unsigned char>(nSourceId >> 40);
    vMessage[nBegin+3] = static_cast<unsigned char>(nSourceId >> 32);
    vMessage[nBegin+4] = static_cast<unsigned char>(nSourceId >> 24);
    vMessage[nBegin+5] = static_cast<unsigned char>(nSourceId >> 16);
    vMessage[nBegin+6] = static_cast<unsigned char>(nSourceId >> 8);
    vMessage[nBegin+7] = static_cast<unsigned char>(nSourceId & 0xFF);

    vMessage[nBegin+8] = static_cast<unsigned char>(nSourcePort >> 8);
    vMessage[nBegin+9] = static_cast<unsigned char>(nSourcePort & 0xFF);
}


ptpHeader::ptpHeader(const std::chrono::nanoseconds& socketTime) :
    timestamp(socketTime)
{

}

ptpV2Header::ptpV2Header(const std::chrono::nanoseconds& socketTime, const std::vector<unsigned char>& vMessage) : ptpHeader(socketTime),
    source(std::vector<unsigned char>(vMessage.begin()+20, vMessage.end()))
{
    nVersion = 2;
	
    nType = vMessage[0] & 0xF;

	size_t nPos = 2;
    nMessageLength =  ToU16(vMessage, nPos);
    
	nDomain = vMessage[4];
	nPos = 6;
    nFlags = ToU16(vMessage, nPos);

    nCorrection = (static_cast<unsigned long long int>(vMessage[8]) << 56)  |
                  (static_cast<unsigned long long int>(vMessage[9]) << 48)  |
                  (static_cast<unsigned long long int>(vMessage[10]) << 40) |
                  (static_cast<unsigned long long int>(vMessage[11]) << 32) |
                  (static_cast<unsigned long long int>(vMessage[12]) << 24) |
                  (static_cast<unsigned long long int>(vMessage[13]) << 16) |
                  (static_cast<unsigned long long int>(vMessage[14]) << 8)  |
                  vMessage[15];


	nPos = 30;
    nSequenceId = ToU16(vMessage, nPos);
    nControl = vMessage[32];
    nInterval = static_cast<char>(vMessage[33]);
}

std::vector<unsigned char> ptpV2Header::CreateMessage() const
{
    std::vector<unsigned char> vMessage(34,0);
    vMessage[0] = nType;
    vMessage[1] = 2;
    vMessage[2] = (nMessageLength >> 8) & 0xFF;
    vMessage[3] = nMessageLength & 0xFF;
    vMessage[4] = nDomain;
    vMessage[6] = (nFlags >> 8) & 0xFF;
    vMessage[7] = (nFlags & 0xFF);
    vMessage[8] = (nCorrection >> 56) & 0xFF;
    vMessage[9] = (nCorrection >> 48) & 0xFF;
    vMessage[10] = (nCorrection >> 40) & 0xFF;
    vMessage[11] = (nCorrection >> 32) & 0xFF;
    vMessage[12] = (nCorrection >> 24) & 0xFF;
    vMessage[13] = (nCorrection >> 16) & 0xFF;
    vMessage[14] = (nCorrection >> 8) & 0xFF;
    vMessage[15] = nCorrection & 0xFF;
    //source
    source.CreateMessage(vMessage, 20);

    vMessage[30] = (nSequenceId >> 8) & 0xFF;
    vMessage[31] = nSequenceId & 0xFF;
    vMessage[32] = nControl;
    vMessage[33] = nInterval;

    return vMessage;
 }


void ptpV2Header::OutputValues()
{
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << std::dec;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Type = " << (int)nType;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Version = " << (int)nVersion;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Length = " << (int)nMessageLength ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "SubDomain = " << (int)nDomain ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Flags = 0x" << std::hex << nFlags << std::dec ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Correction = " << nCorrection ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "SourceId = " << source.sSourceId ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "SourcePort = " << source.nSourcePort ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Sequence = " << nSequenceId ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Control = " << (int)nControl ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Interval = " << (int)nInterval ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "timestamp = " << TimeToIsoString(timestamp) ;
}


ptpV2Payload::ptpV2Payload(const std::vector<unsigned char>& vMessage)
{
    auto seconds = std::chrono::seconds((static_cast<unsigned long long int>(vMessage[0])<<40) | (static_cast<unsigned long long int>(vMessage[1])<<32) | (static_cast<unsigned long long int>(vMessage[2])<<24) | (static_cast<unsigned long long int>(vMessage[3])<<16) | (static_cast<unsigned long long int>(vMessage[4])<<8) | (vMessage[5]));
    auto nano = std::chrono::nanoseconds((static_cast<unsigned long long int>(vMessage[6])<<24) | (static_cast<unsigned long long int>(vMessage[7])<<16) | (static_cast<unsigned long long int>(vMessage[8])<<8) | (vMessage[9]));

    originTime = std::chrono::duration_cast<std::chrono::nanoseconds>(seconds)+nano;
}

std::vector<unsigned char> ptpV2Payload::CreateMessage() const
{
    auto split = Split(originTime);

    std::vector<unsigned char> vMessage(10,0);
    vMessage[0] = (split.first.count()>>40) & 0xFF;
    vMessage[1] = (split.first.count()>>32) & 0xFF;
    vMessage[2] = (split.first.count()>>24) & 0xFF;
    vMessage[3] = (split.first.count()>>16) & 0xFF;
    vMessage[4] = (split.first.count()>> 8) & 0xFF;
    vMessage[5] = (split.first.count()) & 0xFF;

    vMessage[6] = (split.second.count()>>24) & 0xFF;
    vMessage[7] = (split.second.count()>>16) & 0xFF;
    vMessage[8] = (split.second.count()>> 8) & 0xFF;
    vMessage[9] = (split.second.count()) & 0xFF;

    return vMessage;

}

void ptpV2Payload::OutputValues()
{
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Timestamp = " << TimeToString(originTime) ;
}


ptpDelayResponse::ptpDelayResponse(const std::vector<unsigned char>& vMessage) : ptpV2Payload(vMessage),
source(std::vector<unsigned char>(vMessage.begin()+10, vMessage.end()))
{

}

void ptpDelayResponse::OutputValues()
{
    ptpV2Payload::OutputValues();
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Source = " << source.sSourceId ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "SourcePort = " << source.nSourcePort ;
}



ptpAnnounce::ptpAnnounce(const std::vector<unsigned char>& vMessage) : ptpV2Payload(vMessage)
{
	size_t nPos = 10;
    nUtcOffset = ToU16(vMessage, nPos);
    nGrandmasterPriority1 =  vMessage[13];
    nGrandmasterClass = vMessage[14];
    nGrandmasterAccuracy = vMessage[15];

	nPos = 16;
    nGrandmasterVariance = ToU16(vMessage, nPos);
    nGrandmasterPriority2  =  vMessage[18];

	nPos = 19;
	sGrandmasterClockId = hex2str(vMessage, nPos, 8);

	nPos = 27;
    nStepsRemoved = ToU16(vMessage, nPos);
    nTimeSource = vMessage[29];
}

void ptpAnnounce::OutputValues()
{
    ptpV2Payload::OutputValues();
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "UTC Offset = " << (int)nUtcOffset ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GrandmasterPriority1 = " << (int)nGrandmasterPriority1 ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GrandmasterClass = " << (int)nGrandmasterClass ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GrandmasterAccuracy = " << (int)nGrandmasterAccuracy ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GrandmasterVariance = " << (int)nGrandmasterVariance ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GrandmasterPriority2 = " << (int)nGrandmasterPriority2 ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster ClockId = " << sGrandmasterClockId ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Steps = " << (int)nStepsRemoved ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Source = " << (int)nTimeSource ;
}


ptpManagement::ptpManagement(const std::vector<unsigned char>& vMessage) : ptpV2Payload(vMessage)
{
	size_t nPos = 0;

	sTargetPortIdentity = pid2str(vMessage, nPos);
    nStartingBoundaryHops = ToU8(vMessage, nPos);
    nBoundaryHops = ToU8(vMessage, nPos);
    flags = ToU8(vMessage, nPos);
    nAction = flags & 0x0f;
    //13 is reserved
    if(vMessage.size() > 19)    //means we have a tlv
    {
		pTlv = std::make_unique<managementTlv>(std::vector<unsigned char>(vMessage.begin()+14, vMessage.end()));
	}
}

ptpManagement::~ptpManagement()=default;

void ptpManagement::OutputValues()
{
	ptpV2Payload::OutputValues();
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Target Port = " << sTargetPortIdentity;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Starting Boundary Hops = " << nStartingBoundaryHops;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Boundary Hops = " << nBoundaryHops;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Action = " << MANAGEMENT_ACTION[nAction];

	if(pTlv)
	{
		pTlv->OutputValues();
	}
}

void managementTlv::OutputValues()
{
	auto itId = TLV_ID.find(static_cast<enumId>(nId));
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "ID = " << (itId != TLV_ID.end() ? itId->second : std::to_string(nId));

	if(nType == enumType::MANAGEMENT)
	{
		if(pData)
		{
			pData->OutputValues();
		}
		else
		{
			pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "No Data";
		}
	}
	else if(nType == enumType::MANAGEMENT_ERROR_STATUS)
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Management error";
	}
	else
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Unknown TLV " << nType;
	}
}

managementTlv::managementTlv(const std::vector<unsigned char>& vMessage)
{
	nType = ToU16(vMessage, nPos);
	nLength = ToU16(vMessage, nPos);
    

	if(nType == enumType::MANAGEMENT)
	{
		ParseTlv(vMessage);
	}
}

void managementTlv::ParseTlv(const std::vector<unsigned char>& vMessage)
{
	nId = ToU16(vMessage, nPos);

	switch(nId)
	{
		case enumId::CLOCK_DESCRIPTION:
			pData = std::make_unique<tlvClockDescription>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::USER_DESCRIPTION:
			break;
		case enumId::DEFAULT_DATA_SET:
			pData = std::make_unique<tlvDefaultDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::CURRENT_DATA_SET:
			pData = std::make_unique<tlvCurrentDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::PORT_DATA_SET:
			pData = std::make_unique<tlvPortDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::PARENT_DATA_SET:
			pData = std::make_unique<tlvParentDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::TIME_PROPERTIES_DATA_SET:
			pData = std::make_unique<tlvTimePropertiesDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::PRIORITY1:
		case enumId::PRIORITY2:
		case enumId::DOMAIN:
		case enumId::SLAVE_ONLY:
		case enumId::CLOCK_ACCURACY:
		case enumId::MASTER_ONLY:
		case enumId::SYNCHRONIZATION_UNCERTAIN_NP:
		case enumId::LOG_ANNOUNCE_INTERVAL:
		case enumId::LOG_MIN_PDELAY_REQ_INTERVAL:
		case enumId::DELAY_MECHANISM:
		case enumId::LOG_SYNC_INTERVAL:
		case enumId::ANNOUNCE_RECEIPT_TIMEOUT:
		case enumId::ALTERNATE_TIME_OFFSET_ENABLE:
			pData = std::make_unique<tlvBasic>(nId, std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::TRACEABILITY_PROPERTIES:
			pData = std::make_unique<tlvTraceable>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::TIMESCALE_PROPERTIES:
			pData = std::make_unique<tlvTimescale>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::VERSION_NUMBER:
			pData = std::make_unique<tlvVersion>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::ALTERNATE_TIME_OFFSET_NAME:
			pData = std::make_unique<tlvAlternateTimeOffsetName>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::ALTERNATE_TIME_OFFSET_PROPERTIES:
			pData = std::make_unique<tlvAlternateTimeOffsetProperties>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::TIME_STATUS_NP:
			pData = std::make_unique<tlvTimeStatusNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::GRANDMASTER_SETTINGS_NP:
			pData = std::make_unique<tlvGrandmasterSettingsNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::SUBSCRIBE_EVENTS_NP:
			// @todo
			break;
		case enumId::PORT_PROPERTIES_NP:
			pData = std::make_unique<tlvPortPropertiesNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case enumId::PORT_STATS_NP:
			pData = std::make_unique<tlvPortStatsNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		/*
	case enumId::PORT_SERVICE_STATS_NP:
		pssp = (struct port_service_stats_np *) mgt->data;
		fprintf(fp, "PORT_SERVICE_STATS_NP "
		IFMT "portIdentity              %s"
		IFMT "announce_timeout          %" PRIu64
		IFMT "sync_timeout              %" PRIu64
		IFMT "delay_timeout             %" PRIu64
		IFMT "unicast_service_timeout   %" PRIu64
		IFMT "unicast_request_timeout   %" PRIu64
		IFMT "master_announce_timeout   %" PRIu64
		IFMT "master_sync_timeout       %" PRIu64
		IFMT "qualification_timeout     %" PRIu64
		IFMT "sync_mismatch             %" PRIu64
		IFMT "followup_mismatch         %" PRIu64,
		pid2str(&pssp->portIdentity),
		pssp->stats.announce_timeout,
		pssp->stats.sync_timeout,
		pssp->stats.delay_timeout,
		pssp->stats.unicast_service_timeout,
		pssp->stats.unicast_request_timeout,
		pssp->stats.master_announce_timeout,
		pssp->stats.master_sync_timeout,
		pssp->stats.qualification_timeout,
		pssp->stats.sync_mismatch,
		pssp->stats.followup_mismatch);
		break;
	case enumId::UNICAST_MASTER_TABLE_NP:
		umtn = (struct unicast_master_table_np *) mgt->data;
		fprintf(fp, "UNICAST_MASTER_TABLE_NP "
			IFMT "actual_table_size %hu",
			umtn->actual_table_size);
		buf = (uint8_t *) umtn->unicast_masters;
		// table header
		fprintf(fp,
			IFMT "%s  %-24s %-34s %-9s %s %s %s %s  %s",
			"BM", "identity", "address", "state",
			"clockClass", "clockQuality", "offsetScaledLogVariance",
			"p1", "p2");
		for (i = 0; i < umtn->actual_table_size; i++) {
			ume = (struct unicast_master_entry *) buf;
			pmc_show_unicast_master_entry(ume, fp);
			buf += sizeof(*ume) + ume->address.addressLength;
		}
		break;
	case enumId::PORT_HWCLOCK_NP:
		phn = (struct port_hwclock_np *) mgt->data;
		fprintf(fp, "PORT_HWCLOCK_NP "
			IFMT "portIdentity            %s"
			IFMT "phcIndex                %d"
			IFMT "flags                   %hhu",
			pid2str(&phn->portIdentity),
			phn->phc_index,
			phn->flags);
		break;
	case enumId::POWER_PROFILE_SETTINGS_NP:
		pwr = (struct ieee_c37_238_settings_np *) mgt->data;
		fprintf(fp, "POWER_PROFILE_SETTINGS_NP "
			IFMT "version                   %hu"
			IFMT "grandmasterID             0x%04hx"
			IFMT "grandmasterTimeInaccuracy %u"
			IFMT "networkTimeInaccuracy     %u"
			IFMT "totalTimeInaccuracy       %u",
			pwr->version,
			pwr->grandmasterID,
			pwr->grandmasterTimeInaccuracy,
			pwr->networkTimeInaccuracy,
			pwr->totalTimeInaccuracy);
		break;
	case enumId::CMLDS_INFO_NP:
		cmlds = (struct cmlds_info_np *) mgt->data;
		fprintf(fp, "CMLDS_INFO_NP "
			IFMT "meanLinkDelay           %" PRId64
			IFMT "scaledNeighborRateRatio %" PRId32
			IFMT "as_capable              %" PRIu32,
			cmlds->meanLinkDelay >> 16,
			cmlds->scaledNeighborRateRatio,
			cmlds->as_capable);
		break;
		*/
	}
}

tlv::~tlv() = default;


tlvBasic::tlvBasic(uint16_t id, const std::vector<unsigned char>& vMessage) : tlv(id)
{
	nValue = ToU8(vMessage,nPos);
	nReserved = ToU8(vMessage, nPos);
}

void tlvBasic::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Value = " << nValue;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Reserved = " << nReserved;
}

tlvTraceable::tlvTraceable(const std::vector<unsigned char>& vMessage) : tlvBasic(managementTlv::enumId::TRACEABILITY_PROPERTIES, vMessage)
{
	bTimeTraceable = nValue & (1<<4);
	bFrequencyTraceable = nValue & (1<<5);
}

void tlvTraceable::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Traceable = " << (bTimeTraceable ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Freq Traceable = " << (bFrequencyTraceable ? "true" : "false");
}

tlvTimescale::tlvTimescale(const std::vector<unsigned char>& vMessage) : tlvBasic(managementTlv::enumId::TIMESCALE_PROPERTIES, vMessage)
{
	bPtpTimescale = nValue & (1<<3);
}

void tlvTimescale::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PTP Timescale = " << (bPtpTimescale ? "true" : "false");
}

tlvVersion::tlvVersion(const std::vector<unsigned char>& vMessage) : tlvBasic(managementTlv::enumId::VERSION_NUMBER, vMessage)
{
	nVersion = nValue & 0x0f;
}

void tlvVersion::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Version = " << nVersion;
}



tlvClockDescription::tlvClockDescription(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::CLOCK_DESCRIPTION)
{
	auto nFlags = ToU16(vMessage, nPos);
	
	bClockOrdinary = 0x8000 & nFlags;
	bClockBoundary = 0x4000 & nFlags;
	bClockTranparentP2P = 0x2000 & nFlags;
	bClockTranparentE2E = 0x1000 & nFlags;
	bClockManagement = 0x800 & nFlags;

	sPhysicalLayerProtocol = makestr8(vMessage, nPos);

	sPhysicalAddress = makestr16(vMessage, nPos);
	protocolAddress.nProtocol = ToU16(vMessage, nPos);
	

	protocolAddress.sAddress = makestr16(vMessage, nPos);
	
	sManufacturerIdentity = makestr(vMessage, nPos, 3);
	++nPos; //reserved

	sProductDescription = makestr8(vMessage, nPos);
	sRevisionData = makestr8(vMessage, nPos);
	sUserDescription = makestr8(vMessage, nPos);

	sProfileIdentity = hex2str(vMessage, nPos, 6);
}

void tlvClockDescription::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Physical Layer Protocol = " << sPhysicalLayerProtocol;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Physical Address = " << sPhysicalAddress;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Protocol Address = " << protocolAddress.sAddress;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Protocol = " << protocolAddress.nProtocol;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Manufacturer Id = " << sManufacturerIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Product Description = " << sProductDescription;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Revision = " << sRevisionData;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "User Description = " << sUserDescription;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Profile Id = " << sProfileIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock: Ordinary =  " << (bClockOrdinary ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock: Boundary =  " << (bClockBoundary ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock: Transparant P2P =  " << (bClockTranparentP2P ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock: Transparant E2E =  " << (bClockTranparentE2E ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock: Managemenbt =  " << (bClockManagement ? "true" : "false");
}


tlvDefaultDataSet::tlvDefaultDataSet(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::DEFAULT_DATA_SET)
{
	nFlags = ToU8(vMessage, nPos);
	nReserved1 = ToU8(vMessage, nPos);
	nNumberPorts = ToU16(vMessage, nPos);
	nPriority1 = ToU8(vMessage, nPos);
	quality.nClass = ToU8(vMessage, nPos);
	quality.nAccuracy = ToU8(vMessage, nPos);
	quality.nOffsetScaledLogVariance = ToU16(vMessage, nPos);
	nPriority2 = ToU8(vMessage, nPos);
	sClockIdentity = hex2str(vMessage, nPos, 8);
	nDomain = ToU8(vMessage, nPos);
	nReserved2 = ToU8(vMessage, nPos);

	b2Step = nFlags & (1<<0);
	bSlaveOnly = nFlags & (1<<1);
}

void tlvDefaultDataSet::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock ID =  " << sClockIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Domain =  " << nDomain;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "2-Step = " << (b2Step ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Slave Only = " << (bSlaveOnly ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Ports =  " << nNumberPorts;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Priority1 =  " << nPriority1;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Priority2 =  " << nPriority2;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Class =  " << quality.nClass;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Accuracy =  " << quality.nAccuracy;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Offset Log Variance =  " << quality.nOffsetScaledLogVariance;
}

tlvCurrentDataSet::tlvCurrentDataSet(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::CURRENT_DATA_SET)
{
	nStepsRemoved = ToU16(vMessage, nPos);
	offsetFromMaster = std::chrono::nanoseconds(To64(vMessage, nPos));
	meanPathDelay = std::chrono::nanoseconds(To64(vMessage, nPos));
}

void tlvCurrentDataSet::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Steps Removed = " << nStepsRemoved;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Offset = " << offsetFromMaster.count() << " ns";
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Mean Delay = " << meanPathDelay.count() << " ns";
}

tlvParentDataSet::tlvParentDataSet(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::PARENT_DATA_SET)
{
	sParentPortIdentity = pid2str(vMessage, nPos);
	nParentStats = ToU8(vMessage, nPos);
	nReserved = ToU8(vMessage, nPos);
	nObservedParentOffsetScaledLogVariance = ToU16(vMessage, nPos);
	nObservedParentClockPhaseChangeRate = ToU32(vMessage, nPos);
	nGrandmasterPriority1 = ToU8(vMessage, nPos);
	grandmasterClockQuality.nClass = ToU8(vMessage, nPos);
	grandmasterClockQuality.nAccuracy = ToU8(vMessage, nPos);

	grandmasterClockQuality.nOffsetScaledLogVariance = ToU16(vMessage, nPos);

	nGrandmasterPriority2 = ToU8(vMessage, nPos);
	sGrandmasterIdentity = hex2str(vMessage, nPos, 8);
}

void tlvParentDataSet::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Parent Port Id = " << sParentPortIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Parent Stats = " << nParentStats;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Observered Parent Offset Log Variance = " << nObservedParentOffsetScaledLogVariance;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Observered Parent Offset Log Variance = " << nObservedParentClockPhaseChangeRate;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Id = " << sGrandmasterIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Priority1 = " << nGrandmasterPriority1;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Priority2 = " << nGrandmasterPriority2;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Clock Class = " << grandmasterClockQuality.nClass;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Clock Accuracy = " << grandmasterClockQuality.nAccuracy;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Offset Log Variance = " << grandmasterClockQuality.nOffsetScaledLogVariance;
}

tlvTimePropertiesDataSet::tlvTimePropertiesDataSet(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::TIME_PROPERTIES_DATA_SET)
{
	nCurrentUtcOffset = ToU16(vMessage, nPos);
	nFlags = ToU8(vMessage, nPos);
	nTimeSource = ToU8(vMessage, nPos);

	bLeap61 		= nFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::LI_61);
	bLeap59 		= nFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::LI_59);
	bUtcOffsetValid = nFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::UTC_OFFSET_VALID);
	bPtpTimescale 	= nFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::TIMESCALE);
	bTimeTraceable 	= nFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::TIME_TRACEABLE);
	bFreqTraceable	= nFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::FREQ_TRACEABLE);
};

void tlvTimePropertiesDataSet::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Current UTC Offset  = " << nCurrentUtcOffset;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Source  = " << nTimeSource;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Leap 61  = " << (bLeap61 ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Leap 59  = " << (bLeap59 ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "UTC Offset Valid  = " << (bUtcOffsetValid ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PTP Timescale  = " << (bPtpTimescale ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Traceable  = " << (bTimeTraceable ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Freq Traceable  = " << (bFreqTraceable ? "true" : "false");
}


tlvPortDataSet::tlvPortDataSet(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::PORT_DATA_SET)
{
	sPortIdentity  = pid2str(vMessage, nPos);
	nPortState = ToU8(vMessage, nPos);
	nLogMinDelayReqInterval = ToU8(vMessage, nPos);
	peerMeanPathDelay = std::chrono::nanoseconds(ToU64(vMessage, nPos));
	nLogAnnounceInterval = To8(vMessage, nPos);
	nAnnounceReceiptTimeout = ToU8(vMessage, nPos);
	nLogSyncInterval = To8(vMessage, nPos);
	nDelayMechanism = ToU8(vMessage, nPos);
	nLogMinPdelayReqInterval = To8(vMessage, nPos);
	nVersionNumber = ToU8(vMessage, nPos) & 0x0f;
}

void tlvPortDataSet::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Port Id = " << sPortIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "State = " << PORT_STATES[nPortState];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Min Delay Req Interval =  " << nLogMinDelayReqInterval;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Peer Mean Path Delay =  " << peerMeanPathDelay.count();
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Announce Interval =  " << nLogAnnounceInterval;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Announce Timeout =  " << nAnnounceReceiptTimeout;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Sync Interval =  " << nLogSyncInterval;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Delay Mechanism =  " << nDelayMechanism;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "P-Delay_Req Interval =  " << nLogMinPdelayReqInterval;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Version =  " << nVersionNumber;
}

tlvAlternateTimeOffsetName::tlvAlternateTimeOffsetName(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::ALTERNATE_TIME_OFFSET_NAME)
{
	nKeyField = ToU8(vMessage, nPos);
	
	sName = makestr8(vMessage, nPos);
}

void tlvAlternateTimeOffsetName::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Key Field = " << nKeyField;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Name = " << sName;
}


tlvAlternateTimeOffsetProperties::tlvAlternateTimeOffsetProperties(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::ALTERNATE_TIME_OFFSET_PROPERTIES)
{
	nKeyField = ToU8(vMessage, nPos);
	nCurrentOffset = To32(vMessage, nPos);
	nJumpSeconds = To32(vMessage, nPos);

	uint64_t nNext = ToU16(vMessage, nPos);
	nNext <<= 32;
	nNext |=ToU32(vMessage, nPos);
	
	nextJump = std::chrono::seconds(nNext);
}

void tlvAlternateTimeOffsetProperties::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Key Field = " << nKeyField;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Current Offset = " << nCurrentOffset;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Jump Seconds = " << nJumpSeconds;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Next Jump = " << nextJump.count();
}


tlvTimeStatusNP::tlvTimeStatusNP(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::TIME_STATUS_NP)
{
	masterOffset = std::chrono::nanoseconds(ToU64(vMessage, nPos));
	ingressTime = std::chrono::nanoseconds(ToU64(vMessage, nPos));
	nCumulativeScaledRateOffset = To32(vMessage, nPos);
	nScaledLastGmPhaseChange =To32(vMessage, nPos);
	nGmTimeBaseIndicator = ToU16(vMessage, nPos);
	lastGmPhaseChange.nanoseconds_msb = ToU16(vMessage, nPos);
	lastGmPhaseChange.nanoseconds_lsb = ToU64(vMessage, nPos);
	lastGmPhaseChange.fractional_nanoseconds = ToU16(vMessage, nPos);

	bGmPresent = (To32(vMessage, nPos) != 0);
	sGmIdentity = hex2str(vMessage, nPos, 8);
}

void tlvTimeStatusNP::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Master Offset = " << masterOffset.count();
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Ingress Time = " << ingressTime.count();
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Last GM Phase Change = " << nScaledLastGmPhaseChange;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GM Time Base = " << nGmTimeBaseIndicator;
	//@todo lastphasechange
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Rate Offset = " << nCumulativeScaledRateOffset;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GM Present = " << (bGmPresent ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GM Id = " << sGmIdentity;
}

tlvGrandmasterSettingsNP::tlvGrandmasterSettingsNP(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::GRANDMASTER_SETTINGS_NP)
{
	quality.nClass = ToU8(vMessage, nPos);
	quality.nAccuracy = ToU8(vMessage, nPos);
	quality.nOffsetScaledLogVariance = ToU16(vMessage, nPos);

	nUtcOffset = To16(vMessage, nPos);
	nTimeFlags = ToU8(vMessage, nPos);
	nTimeSource = ToU8(vMessage, nPos);

	bLeap61 				= nTimeFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::LI_61);
	bLeap59 				= nTimeFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::LI_59);
	bCurrentUtcOffsetValid  = nTimeFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::UTC_OFFSET_VALID);
	bPtpTimescale 			= nTimeFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::TIMESCALE);
	bTimeTraceable 			= nTimeFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::TIME_TRACEABLE);
	bFrequencyTraceable		= nTimeFlags & static_cast<uint8_t>(ptpV2Header::enumFlags::FREQ_TRACEABLE);
}


void tlvGrandmasterSettingsNP::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Class = " << quality.nClass;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Accuracy = " << quality.nAccuracy;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Offset Log Variance = " << quality.nOffsetScaledLogVariance;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "UTC Offset = " << nUtcOffset;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Source = " << nTimeSource;

	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Leap 61  = " << (bLeap61 ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Leap 59  = " << (bLeap59 ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "UTC Offset Valid  = " << (bCurrentUtcOffsetValid ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PTP Timescale  = " << (bPtpTimescale ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Traceable  = " << (bTimeTraceable ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Freq Traceable  = " << (bFrequencyTraceable ? "true" : "false");
}


tlvPortPropertiesNP::tlvPortPropertiesNP(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::PORT_PROPERTIES_NP)
{
	sPortIdentity = pid2str(vMessage, nPos);

	nPortState = To8(vMessage, nPos);
	nTimestamping = To8(vMessage, nPos);
	sInterface = makestr8(vMessage, nPos);
}

void tlvPortPropertiesNP::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Port ID  = " << sPortIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Port State  = " << tlvPortDataSet::PORT_STATES[nPortState];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Timestamping = " << tlvPortDataSet::TIMESTAMPING[nTimestamping];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Interface = " << sInterface;
}

tlvPortStatsNP::tlvPortStatsNP(const std::vector<unsigned char>& vMessage) : tlv(managementTlv::enumId::PORT_STATS_NP)
{
	sPortIdentity = pid2str(vMessage, nPos);
	for(auto& value : rx)
	{
		value = ToU64(vMessage, nPos);
	}
	for(auto& value : tx)
	{
		value = ToU64(vMessage, nPos);
	}
}

void tlvPortStatsNP::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Port ID                    = " << sPortIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Delay_Req               = " << rx[static_cast<size_t>(ptpV2Header::enumType::DELAY_REQ)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Delay_Resp              = " << rx[static_cast<size_t>(ptpV2Header::enumType::DELAY_RESP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Follow_Up               = " << rx[static_cast<size_t>(ptpV2Header::enumType::FOLLOW_UP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Management              = " << rx[static_cast<size_t>(ptpV2Header::enumType::MANAGEMENT)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX P_Delay_Resp            = " << rx[static_cast<size_t>(ptpV2Header::enumType::PEER_DELAY_RESP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX P_Delay_Resp_Follow_Up  = " << rx[static_cast<size_t>(ptpV2Header::enumType::PEER_DELAY_RESP_FOLLOW_UP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX P_Delay_Req             = " << rx[static_cast<size_t>(ptpV2Header::enumType::PEER_DLEAY_REQ)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Signalling              = " << rx[static_cast<size_t>(ptpV2Header::enumType::SIGNALLING)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Sync                    = " << rx[static_cast<size_t>(ptpV2Header::enumType::SYNC)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Delay_Req               = " << tx[static_cast<size_t>(ptpV2Header::enumType::DELAY_REQ)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Delay_Resp              = " << tx[static_cast<size_t>(ptpV2Header::enumType::DELAY_RESP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Follow_Up               = " << tx[static_cast<size_t>(ptpV2Header::enumType::FOLLOW_UP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Management              = " << tx[static_cast<size_t>(ptpV2Header::enumType::MANAGEMENT)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX P_Delay_Resp            = " << tx[static_cast<size_t>(ptpV2Header::enumType::PEER_DELAY_RESP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX P_Delay_Resp_Follow_Up  = " << tx[static_cast<size_t>(ptpV2Header::enumType::PEER_DELAY_RESP_FOLLOW_UP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX P_Delay_Req             = " << tx[static_cast<size_t>(ptpV2Header::enumType::PEER_DLEAY_REQ)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Signalling              = " << tx[static_cast<size_t>(ptpV2Header::enumType::SIGNALLING)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Sync                    = " << tx[static_cast<size_t>(ptpV2Header::enumType::SYNC)];
}