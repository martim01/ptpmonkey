#include "ptpstructs.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "timeutils.h"
#include "log.h"
#include <algorithm>

using namespace pml::ptpmonkey;

const std::array<std::string, 5> ptpManagement::kManagementAction{"GET", "SET", "RESPONSE", "COMMAND", "ACKNOWLEDGE"};

const std::map<mngmnt::enumId, std::string> managementTlv::kTlvId{{mngmnt::enumId::kUserDescription, "USER_DESCRIPTION"}, {mngmnt::enumId::kSaveInNonVolatileStorage, "SAVE_IN_NON_VOLATILE_STORAGE"},  {mngmnt::enumId::kResetNonVolatileStorage, "RESET_NON_VOLATILE_STORAGE"},{mngmnt::enumId::kInitialize, "INITIALIZE"}, {mngmnt::enumId::kFaultLog, "FAULT_LOG"}, {mngmnt::enumId::kFaultLogReset, "FAULT_LOG_RESET"}, {mngmnt::enumId::kDefaultDataSet, "DEFAULT_DATA_SET"}, {mngmnt::enumId::kCurrentDataSet, "CURRENT_DATA_SET"}, {mngmnt::enumId::kParentDataSet, "PARENT_DATA_SET"}, {mngmnt::enumId::kTimePropertiesDataSet,"TIME_PROPERTIES_DATA_SET"}, {mngmnt::enumId::kPriority1, "PRIORITY1"}, {mngmnt::enumId::kPriority2, "PRIORITY2"}, {mngmnt::enumId::kDomain, "DOMAIN"}, {mngmnt::enumId::kSlaveOnly, "SLAVE_ONLY"}, {mngmnt::enumId::kTime, "TIME"}, {mngmnt::enumId::kClockAccuracy, "CLOCK_ACCURACY"}, {mngmnt::enumId::kUtcProperties, "UTC_PROPERTIES"}, {mngmnt::enumId::kTraceabilityProperties, "TRACEABILITY_PROPERTIES"}, {mngmnt::enumId::kTimescaleProperties, "TIMESCALE_PROPERTIES"}, {mngmnt::enumId::kPathTraceList, "PATH_TRACE_LIST"}, {mngmnt::enumId::kPathTraceEnable, "PATH_TRACE_ENABLE"}, {mngmnt::enumId::kGrandmasterClusterTable, "GRANDMASTER_CLUSTER_TABLE"}, {mngmnt::enumId::kAcceptableMasterTable, "ACCEPTABLE_MASTER_TABLE"}, {mngmnt::enumId::kAcceptableMasterMaxTableSize,"ACCEPTABLE_MASTER_MAX_TABLE_SIZE"}, {mngmnt::enumId::kAlternateTimeOffsetEnable, "ALTERNATE_TIME_OFFSET_ENABLE"}, {mngmnt::enumId::kAlternateTimeOffsetName, "ALTERNATE_TIME_OFFSET_NAME"}, {mngmnt::enumId::kAlternateTimeOffsetMaxKey, "ALTERNATE_TIME_OFFSET_MAX_KEY"}, {mngmnt::enumId::kAlternateTimeOffsetProperties, "ALTERNATE_TIME_OFFSET_PROPERTIES"}, {mngmnt::enumId::kExternalPortConfigurationEnabled, "EXTERNAL_PORT_CONFIGURATION_ENABLED"}, {mngmnt::enumId::kHoldoverUpgradeEnable, "HOLDOVER_UPGRADE_ENABLE"}, {mngmnt::enumId::kTransparentClockDefaultDataSet,"TRANSPARENT_CLOCK_DEFAULT_DATA_SET"}, {mngmnt::enumId::kPrimaryDomain, "PRIMARY_DOMAIN"}, {mngmnt::enumId::kTimeStatusNp, "TIME_STATUS_NP"}, {mngmnt::enumId::kGrandmasterSettingsNp, "GRANDMASTER_SETTINGS_NP"}, {mngmnt::enumId::kSubscribeEventsNp, "SUBSCRIBE_EVENTS_NP"}, {mngmnt::enumId::kSynchronizationUncertainNp, "SYNCHRONIZATION_UNCERTAIN_NP"}, {mngmnt::enumId::kNullManagement, "NULL_MANAGEMENT"}, {mngmnt::enumId::kClockDescription, "CLOCK_DESCRIPTION"}, {mngmnt::enumId::kPortDataSet, "PORT_DATA_SET"}, {mngmnt::enumId::kLogAnnounceInterval, "LOG_ANNOUNCE_INTERVAL"}, {mngmnt::enumId::kAnnounceReceiptTimeout, "ANNOUNCE_RECEIPT_TIMEOUT"}, {mngmnt::enumId::kLogSyncInterval, "LOG_SYNC_INTERVAL"}, {mngmnt::enumId::kVersionNumber, "VERSION_NUMBER"}, {mngmnt::enumId::kEnablePort, "ENABLE_PORT"}, {mngmnt::enumId::kDisablePort, "DISABLE_PORT"}, {mngmnt::enumId::kUnicastNegotiationEnable, "UNICAST_NEGOTIATION_ENABLE"}, {mngmnt::enumId::kUnicastMasterTable, "UNICAST_MASTER_TABLE"}, {mngmnt::enumId::kUnicastMasterMaxTableSize, "UNICAST_MASTER_MAX_TABLE_SIZE"}, {mngmnt::enumId::kAcceptableMasterTableEnabled, "ACCEPTABLE_MASTER_TABLE_ENABLED"}, {mngmnt::enumId::kAlternateMaster, "ALTERNATE_MASTER"}, {mngmnt::enumId::kMasterOnly, "MASTER_ONLY"}, {mngmnt::enumId::kExtPortConfigPortDataSet, "EXT_PORT_CONFIG_PORT_DATA_SET"}, {mngmnt::enumId::kTransparentClockPortDataSet,"TRANSPARENT_CLOCK_PORT_DATA_SET"}, {mngmnt::enumId::kDelayMechanism, "DELAY_MECHANISM"}, {mngmnt::enumId::kLogMinPdelayReqInterval, "LOG_MIN_PDELAY_REQ_INTERVAL"}, {mngmnt::enumId::kPortDataSetNp, "PORT_DATA_SET_NP"}, {mngmnt::enumId::kPortPropertiesNp, "PORT_PROPERTIES_NP"}, {mngmnt::enumId::kPortStatsNp, "PORT_STATS_NP"}, {mngmnt::enumId::kPortServiceStatsNp, "PORT_SERVICE_STATS_NP"}, {mngmnt::enumId::kUnicastMasterTableNp, "UNICAST_MASTER_TABLE_NP"}, {mngmnt::enumId::kPortHwclockNp, "PORT_HWCLOCK_NP"}, {mngmnt::enumId::kPowerProfileSettingsNp, "POWER_PROFILE_SETTINGS_NP"}, {mngmnt::enumId::kCmldsInfoNp, "CMLDS_INFO_NP"}};


const std::array<std::string, 11> tlvPortDataSet::kPortStates{"NONE","INITIALIZING","FAULTY","DISABLED","LISTENING","PRE_MASTER","MASTER","PASSIVE","UNCALIBRATED","SLAVE","GRAND_MASTER"};
const std::array<std::string, 5> tlvPortDataSet::kTimestamping{"SOFTWARE", "HARDWARE", "LEGACY_HW", "ONESTEP", "P2P1STEP"};

const std::map<clck::enumAccuracy, std::string> clockQuality::kAccuracy{ {clck::enumAccuracy::kNa, "NOT_AVAILABLE"}, {clck::enumAccuracy::k250Ns, "25ns"}, {clck::enumAccuracy::k100Ns, "100ns"}, {clck::enumAccuracy::k250Ns, "250ns"}, {clck::enumAccuracy::k1Us, "1us"}, {clck::enumAccuracy::k2_5Us, "2.5us"}, {clck::enumAccuracy::k10Us, "10us"}, {clck::enumAccuracy::k25Us, "25us"}, {clck::enumAccuracy::k100Us, "100us"}, {clck::enumAccuracy::k250Us, "250us"}, {clck::enumAccuracy::k1Ms, "1ms"}, {clck::enumAccuracy::k2_5Ms, "2 to 5ms"}, {clck::enumAccuracy::k10Ms, "10ms"}, {clck::enumAccuracy::k25Ms, "25ms"}, {clck::enumAccuracy::k100Ms, "100ms"}, {clck::enumAccuracy::k250Ms, "250ms"}, {clck::enumAccuracy::k1S, "1s"}, {clck::enumAccuracy::k10S, "10s"}, {clck::enumAccuracy::kOver, ">10s"}, {clck::enumAccuracy::kUnknown, "UNKNOWN"}};

const std::map<clck::enumTimeSource, std::string> clockQuality::kSource{{clck::enumTimeSource::kNa, "NOT_AVAILABLE"}, {clck::enumTimeSource::kAtomic, "ATOMIC_CLOCK"}, {clck::enumTimeSource::kGps, "GPS"}, {clck::enumTimeSource::kRadio, "TERRESTRIAL_RADIO"}, {clck::enumTimeSource::kPtp, "PTP"}, {clck::enumTimeSource::kNtp, "NTP"}, {clck::enumTimeSource::kHandset, "HANDSET"}, {clck::enumTimeSource::kOther, "OTHER"}, {clck::enumTimeSource::kInternal, "INTERNAL_OSCILLATOR"}};

const std::map<mngmnt::enumError, std::string> managementTlvResponse::kError{{mngmnt::enumError::kResponseTooBig, "RESPONSE_TOO_BIG"},
																      {mngmnt::enumError::kNoSuchId, "NO_SUCH_ID"},
																	  {mngmnt::enumError::kWrongLength, "WRONG_LENGTH"},
																	  {mngmnt::enumError::kWrongValue, "WRONG_VALUE"},
																	  {mngmnt::enumError::kNotSetable, "NOT_SETABLE"},
																	  {mngmnt::enumError::kNotSupported, "NOT_SUPPORTED"},
																	  {mngmnt::enumError::kGeneralError, "GENERAL_ERROR"}};




std::vector<std::string> SplitString(std::string_view str, char cSplit)
{
    std::vector<std::string> vSplit;
    std::istringstream f(str.data());
    std::string s;

    while (getline(f, s, cSplit))
    {
        if(s.empty() == false)
        {
			vSplit.push_back(s);
        }
    }
    return vSplit;
}


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

subnano ToSubNano(const std::vector<unsigned char>& vMessage, size_t& nPos)
{
	subnano sub;

    sub.nano =  std::chrono::nanoseconds((static_cast<int64_t>(vMessage[nPos]) << 40) +
				   (static_cast<int64_t>(vMessage[nPos+1]) << 32) +
					(static_cast<int64_t>(vMessage[nPos+2]) << 24) +
					(static_cast<int64_t>(vMessage[nPos+3]) << 16) +
					(static_cast<int64_t>(vMessage[nPos+4]) << 8) +
					(static_cast<int64_t>(vMessage[nPos+5])));

	nPos += 6;
	sub.sub = To16(vMessage, nPos);
	return sub;
}

std::chrono::nanoseconds ToSecNano(const std::vector<unsigned char>& vMessage, size_t& nPos)
{
	std::chrono::seconds sec((static_cast<int64_t>(vMessage[nPos]) << 40) +
				   (static_cast<int64_t>(vMessage[nPos+1]) << 32) +
					(static_cast<int64_t>(vMessage[nPos+2]) << 24) +
					(static_cast<int64_t>(vMessage[nPos+3]) << 16) +
					(static_cast<int64_t>(vMessage[nPos+4]) << 8) +
					(static_cast<int64_t>(vMessage[nPos+5])));
	std::chrono::nanoseconds nano((static_cast<int64_t>(vMessage[nPos+6]) << 24) +
					(static_cast<int64_t>(vMessage[nPos+7]) << 16) +
					(static_cast<int64_t>(vMessage[nPos+8]) << 8) +
					(static_cast<int64_t>(vMessage[nPos+9])));

	nano += std::chrono::duration_cast<std::chrono::nanoseconds>(sec);
	nPos += 10;
	return nano;
}

void FromSecNano(const std::chrono::nanoseconds& timestamp, std::vector<unsigned char>& vMessage)
{
	auto [sec, nano] = Split(timestamp);

    vMessage.push_back((sec.count()>>40) & 0xFF);
    vMessage.push_back((sec.count()>>32) & 0xFF);
    vMessage.push_back((sec.count()>>24) & 0xFF);
    vMessage.push_back((sec.count()>>16) & 0xFF);
    vMessage.push_back((sec.count()>> 8) & 0xFF);
    vMessage.push_back((sec.count()) & 0xFF);
    vMessage.push_back((nano.count()>>24) & 0xFF);
    vMessage.push_back((nano.count()>>16) & 0xFF);
    vMessage.push_back((nano.count()>> 8) & 0xFF);
    vMessage.push_back((nano.count()) & 0xFF);
}

void FromU8(uint8_t nValue, std::vector<unsigned char>& vMessage)
{
	vMessage.push_back(nValue);
}

void FromU16(uint16_t nValue, std::vector<unsigned char>& vMessage)
{
	vMessage.push_back((nValue >> 8) & 0xFF);
    vMessage.push_back(nValue & 0xFF);
}

void FromSubNano(const subnano& sub, std::vector<unsigned char>& vMessage)
{
	vMessage.push_back((sub.nano.count()>>40) & 0xFF);
    vMessage.push_back((sub.nano.count()>>32) & 0xFF);
    vMessage.push_back((sub.nano.count()>>24) & 0xFF);
    vMessage.push_back((sub.nano.count()>>16) & 0xFF);
    vMessage.push_back((sub.nano.count()>> 8) & 0xFF);
    vMessage.push_back(sub.nano.count() & 0xFF);

	FromU16(sub.sub, vMessage);
}





void FromU32(uint32_t nValue, std::vector<unsigned char>& vMessage)
{
	vMessage.push_back((nValue>>24) & 0xFF);
    vMessage.push_back((nValue>>16) & 0xFF);
    vMessage.push_back((nValue>> 8) & 0xFF);
    vMessage.push_back( nValue      & 0xFF);
}

void FromU64(uint64_t nValue, std::vector<unsigned char>& vMessage)
{
	vMessage.push_back((nValue>>56) & 0xFF);
	vMessage.push_back((nValue>>48) & 0xFF);
	vMessage.push_back((nValue>>40) & 0xFF);
	vMessage.push_back((nValue>>32) & 0xFF);
	vMessage.push_back((nValue>>24) & 0xFF);
    vMessage.push_back((nValue>>16) & 0xFF);
    vMessage.push_back((nValue>> 8) & 0xFF);
    vMessage.push_back( nValue      & 0xFF);
}

std::string ToHexStr(const std::vector<unsigned char>& vMessage, size_t& nStart, size_t nLength)
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

std::string ToHexStr16(const std::vector<unsigned char>& vMessage, size_t& nStart)
{
	auto nLength = ToU16(vMessage, nStart);
	return ToHexStr(vMessage, nStart, nLength);
}

std::string ToDecStr(const std::vector<unsigned char>& vMessage, size_t& nStart, size_t nLength)
{
	std::stringstream ssId;

    for(size_t i = 0; i < nLength; i++)
    {
        if(i != 0)
        {
            ssId << ".";
        }
        ssId << (int)vMessage[i+nStart];
    }
	nStart += nLength;

	return ssId.str();
}

std::string ToDecStr16(const std::vector<unsigned char>& vMessage, size_t& nStart)
{
	auto nLength = ToU16(vMessage, nStart);
	return ToDecStr(vMessage, nStart, nLength);
}


std::string ToStr(const std::vector<unsigned char>& vMessage, size_t& nStart, size_t nLength)
{
	if(nStart+nLength < vMessage.size())
	{
		auto str = std::string(vMessage.begin()+nStart, vMessage.begin()+nStart+nLength);
		nStart += nLength;
		return str;
	}
	else
	{
		nStart += nLength;
		return std::string("?");
	}
}

std::string ToStr16(const std::vector<unsigned char>& vMessage, size_t& nStart)
{
	uint16_t nLength = ToU16(vMessage, nStart);

	return ToStr(vMessage, nStart, nLength);
}

std::string ToStr8(const std::vector<unsigned char>& vMessage, size_t& nStart)
{
	uint8_t nLength = ToU8(vMessage, nStart);

	return ToStr(vMessage, nStart, nLength);
}



std::string ToPidStr(const std::vector<unsigned char>& vMessage, size_t& nStart)
{
	auto str = ToHexStr(vMessage, nStart, 8);
	str+= "-"+std::to_string(ToU16(vMessage, nStart));
	return str;
}

void FromPidStr(const std::string& sPortId, std::vector<unsigned char>& vMessage)
{
	
	auto vPortNumber = SplitString(sPortId, '-');
	
	auto vId = vPortNumber.empty() ? std::vector<std::string>(8) : SplitString(vPortNumber[0], ':');
	for(size_t i = 0; i < 8; ++i)
	{
		if(i < vId.size())
		{
			try
			{
				vMessage.push_back(static_cast<uint8_t>(std::stoul(vId[i], nullptr, 16)));
			}
			catch(const std::exception& e)
			{
				vMessage.push_back(0x0);
			}
		}
		else
		{
			vMessage.push_back(0x0);
		}
	}
	if(vPortNumber.size() > 1)
	{
		try
		{
			auto nValue = static_cast<uint16_t>(std::stoul(vPortNumber[1]));
			FromU16(nValue, vMessage);
		}
		catch(const std::exception& e)
		{
			vMessage.push_back(0xFF);
			vMessage.push_back(0xFF);
		}
	}
	else
	{
		vMessage.push_back(0xFF);
		vMessage.push_back(0xFF);
	}
}

void FromHexStr16(const std::string& str, std::vector<unsigned char>& vMessage)
{
	auto vId = SplitString(str, ':');
	
	auto nLength = static_cast<uint16_t>(vId.size());
	FromU16(nLength, vMessage);

	for(size_t i = 0; i < nLength; ++i)
	{
		try
		{
			vMessage.push_back(static_cast<uint8_t>(std::stoul(vId[i], nullptr, 16)));
		}
		catch(const std::exception& e)
		{
			vMessage.push_back(0x0);
		}
	}
}

void FromHexStr8(const std::string& str, std::vector<unsigned char>& vMessage)
{
	auto vId = SplitString(str, ':');
	
	auto nLength = static_cast<uint8_t>(vId.size());
	FromU16(nLength, vMessage);

	for(size_t i = 0; i < nLength; ++i)
	{
		try
		{
			vMessage.push_back(static_cast<uint8_t>(std::stoul(vId[i], nullptr, 16)));
		}
		catch(const std::exception& e)
		{
			vMessage.push_back(0x0);
		}
	}
}

void FromHexStr(const std::string& str, std::vector<unsigned char>& vMessage, uint16_t nLength)
{
	auto vId = SplitString(str, ':');
	
	for(size_t i = 0; i < nLength; ++i)
	{
		if(i < vId.size())
		{
			try
			{
				vMessage.push_back(static_cast<uint8_t>(std::stoul(vId[i], nullptr, 16)));
			}
			catch(const std::exception& e)
			{
				vMessage.push_back(0x0);
			}
		}
		else
		{
			vMessage.push_back(0x0);
		}
	}
}

uint8_t FromStr(const std::string& str, std::vector<unsigned char>& vMessage)
{
	auto nLength = static_cast<uint8_t>(str.length());
	vMessage.push_back(nLength);
	for(uint8_t i = 0; i < nLength; ++i)
	{
		vMessage.push_back(static_cast<uint8_t>(str[i]));
	}
	return nLength+1;
}

void FromDecStr16(const std::string& str, std::vector<unsigned char>& vMessage)
{
	auto vId = SplitString(str, '.');
	
	auto nLength = static_cast<uint16_t>(vId.size());
	FromU16(nLength, vMessage);

	for(size_t i = 0; i < nLength; ++i)
	{
		try
		{
			vMessage.push_back(static_cast<uint8_t>(std::stoul(vId[i])));
		}
		catch(const std::exception& e)
		{
			vMessage.push_back(0x0);
		}
	}
}


ptpSource::ptpSource(const std::vector<unsigned char>& vMessage)
{
	size_t nPos = 0;
	sSourceId = ToHexStr(vMessage, nPos, 8);
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


void ptpV2Header::OutputValues() const
{
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << std::dec;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Type = " << (int)nType;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Version = " << (int)nVersion;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Length = " << (int)nMessageLength ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "SubDomain = " << (int)nDomain ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Flags = 0x" << std::hex << nFlags << std::dec ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Correction = " << nCorrection ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "SourceId = " << source.sSourceId ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "SourcePort = " << source.nSourcePort ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Sequence = " << nSequenceId ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Control = " << (int)nControl ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Interval = " << (int)nInterval ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "timestamp = " << TimeToIsoString(timestamp) ;
}


ptpV2Payload::ptpV2Payload(const std::vector<unsigned char>& vMessage)
{
    auto seconds = std::chrono::seconds((static_cast<uint64_t>(vMessage[0])<<40) | (static_cast<uint64_t>(vMessage[1])<<32) | (static_cast<uint64_t>(vMessage[2])<<24) | (static_cast<uint64_t>(vMessage[3])<<16) | (static_cast<uint64_t>(vMessage[4])<<8) | (vMessage[5]));
    auto nano = std::chrono::nanoseconds((static_cast<uint64_t>(vMessage[6])<<24) | (static_cast<uint64_t>(vMessage[7])<<16) | (static_cast<uint64_t>(vMessage[8])<<8) | (vMessage[9]));

    originTime = std::chrono::duration_cast<std::chrono::nanoseconds>(seconds)+nano;
}

std::vector<unsigned char> ptpV2Payload::CreateMessage() const
{
    auto [sec, nano] = Split(originTime);

    std::vector<unsigned char> vMessage(10,0);
    vMessage[0] = (sec.count()>>40) & 0xFF;
    vMessage[1] = (sec.count()>>32) & 0xFF;
    vMessage[2] = (sec.count()>>24) & 0xFF;
    vMessage[3] = (sec.count()>>16) & 0xFF;
    vMessage[4] = (sec.count()>> 8) & 0xFF;
    vMessage[5] = (sec.count()) & 0xFF;

    vMessage[6] = (nano.count()>>24) & 0xFF;
    vMessage[7] = (nano.count()>>16) & 0xFF;
    vMessage[8] = (nano.count()>> 8) & 0xFF;
    vMessage[9] = (nano.count()) & 0xFF;

    return vMessage;

}

void ptpV2Payload::OutputValues() const
{
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Timestamp = " << originTime.count();
}


ptpDelayResponse::ptpDelayResponse(const std::vector<unsigned char>& vMessage) : ptpV2Payload(vMessage),
source(std::vector<unsigned char>(vMessage.begin()+10, vMessage.end()))
{

}

void ptpDelayResponse::OutputValues() const
{
    ptpV2Payload::OutputValues();
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Source = " << source.sSourceId ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "SourcePort = " << source.nSourcePort ;
}



ptpAnnounce::ptpAnnounce(const std::vector<unsigned char>& vMessage) : ptpV2Payload(vMessage)
{
	size_t nPos = 10;
    nUtcOffset = ToU16(vMessage, nPos);
    nGrandmasterPriority1 =  vMessage[13];
    nGrandmasterClass = vMessage[14];
    eGrandmasterAccuracy = static_cast<clck::enumAccuracy>(vMessage[15]);

	nPos = 16;
    nGrandmasterVariance = ToU16(vMessage, nPos);
    nGrandmasterPriority2  =  vMessage[18];

	nPos = 19;
	sGrandmasterClockId = ToHexStr(vMessage, nPos, 8);

	nPos = 27;
    nStepsRemoved = ToU16(vMessage, nPos);
    eTimeSource = static_cast<clck::enumTimeSource>(vMessage[29]);
}

void ptpAnnounce::OutputValues() const
{
    ptpV2Payload::OutputValues();

	auto itAccuracy = clockQuality::kAccuracy.find(eGrandmasterAccuracy);

    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "UTC Offset = " << (int)nUtcOffset ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "GrandmasterPriority1 = " << (int)nGrandmasterPriority1 ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "GrandmasterClass = " << (int)nGrandmasterClass ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "GrandmasterAccuracy = " << (itAccuracy != clockQuality::kAccuracy.end() ? itAccuracy->second : std::to_string(static_cast<int>(eGrandmasterAccuracy)));
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "GrandmasterVariance = " << (int)nGrandmasterVariance ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "GrandmasterPriority2 = " << (int)nGrandmasterPriority2 ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Grandmaster ClockId = " << sGrandmasterClockId ;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Steps = " << (int)nStepsRemoved ;

	auto itSource = clockQuality::kSource.find(eTimeSource);
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Time Source = " << (itSource != clockQuality::kSource.end() ? itSource->second : std::to_string(static_cast<int>(eTimeSource)));
}


ptpManagement::ptpManagement(const std::vector<unsigned char>& vMessage) : ptpV2Payload(vMessage)
{
	size_t nPos = 0;

	sTargetPortIdentity = ToPidStr(vMessage, nPos);
    nStartingBoundaryHops = ToU8(vMessage, nPos);
    nBoundaryHops = ToU8(vMessage, nPos);
    flags = ToU8(vMessage, nPos);
    eAction = static_cast<mngmnt::enumAction>(flags & 0x0f);
    //13 is reserved
    if(vMessage.size() > 19)
	{
		if(eAction == mngmnt::enumAction::kResponse)    //means we have a tlv
    	{
			pTlv = std::make_shared<managementTlvResponse>(std::vector<unsigned char>(vMessage.begin()+14, vMessage.end()));
		}
	}
}

ptpManagement::ptpManagement(mngmnt::enumGet id, uint8_t nHops, const std::string& sTargetPortId, uint16_t nTargetPortNumber)
{
	sTargetPortIdentity = sTargetPortId+"-"+std::to_string(nTargetPortNumber);
	nStartingBoundaryHops = nHops;
	nBoundaryHops = nHops;
	eAction = mngmnt::enumAction::kGet;
	pTlv = std::make_shared<managementTlvResponse>(mngmnt::enumType::kManagement, id);
}

ptpManagement::ptpManagement(mngmnt::enumSet id, uint8_t nHops, const std::string& sTargetPortId, uint16_t nTargetPortNumber)
{
	sTargetPortIdentity = sTargetPortId+"-"+std::to_string(nTargetPortNumber);
	nStartingBoundaryHops = nHops;
	nBoundaryHops = nHops;
	eAction = mngmnt::enumAction::kSet;
	pTlv = std::make_shared<managementTlvResponse>(mngmnt::enumType::kManagement, id);
}

std::vector<unsigned char> ptpManagement::CreateMessage() const
{
	std::vector<unsigned char> vMessage;

	FromPidStr(sTargetPortIdentity, vMessage);
	FromU8(nStartingBoundaryHops, vMessage);
	FromU8(nBoundaryHops, vMessage);
	FromU8(static_cast<uint8_t>(eAction), vMessage);
	FromU8(0x0, vMessage);

	if(pTlv)
	{
		pTlv->CreateMessage(vMessage);
	}

	return vMessage;
}

ptpManagement::~ptpManagement()=default;

void ptpManagement::OutputValues() const
{
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Target Port            = " << sTargetPortIdentity;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Starting Boundary Hops = " << (int)nStartingBoundaryHops;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Boundary Hops          = " << (int)nBoundaryHops;
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Action                 = " << kManagementAction[static_cast<int>(eAction)];

	if(pTlv)
	{
		pTlv->OutputValues();
	}
}

void managementTlvResponse::OutputValues() const
{
	auto itId = kTlvId.find(eId);
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "ID = " << (itId != kTlvId.end() ? itId->second : std::to_string(static_cast<int>(eId)));

	if(eType == mngmnt::enumType::kManagement)
	{
		if(pData)
		{
			pData->OutputValues();
		}
		else
		{
			pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "No Data";
		}
	}
	else if(eType == mngmnt::enumType::kManagementErrorStatus)
	{
		auto itError = kError.find(eError);
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << (itError != kError.end() ? itError->second : std::to_string(static_cast<uint16_t>(eError)));

		if(sError.empty() == false)
		{
			pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << sError;
		}
	}
	else
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Unknown TLV " << static_cast<int>(eType);
	}
}

managementTlvResponse::managementTlvResponse(const std::vector<unsigned char>& vMessage)
{
	eType = static_cast<mngmnt::enumType>(ToU16(vMessage, nPos));
	nLength = ToU16(vMessage, nPos);
    

	if(eType == mngmnt::enumType::kManagement)
	{
		ParseTlv(vMessage);
	}
	else if(eType == mngmnt::enumType::kManagementErrorStatus)
	{
		eError = static_cast<mngmnt::enumError>(ToU16(vMessage, nPos));
		eId = static_cast<mngmnt::enumId>(ToU16(vMessage, nPos));
		nReserved = ToU32(vMessage, nPos);
		sError = ToStr8(vMessage, nPos);
	}
}

managementTlvResponse::managementTlvResponse(mngmnt::enumType eT, mngmnt::enumSet eI)
{
	eType = eT;
	eId = static_cast<mngmnt::enumId>(eI);
}

managementTlvResponse::managementTlvResponse(mngmnt::enumType eT, mngmnt::enumGet eI)
{
	eType = eT;
	eId = static_cast<mngmnt::enumId>(eI);
	switch(eId)
	{
		case mngmnt::enumId::kClockDescription:
			pData = std::make_shared<tlvClockDescription>();
			break;
		case mngmnt::enumId::kUserDescription:
			pData = std::make_shared<tlvUserDescription>();
			break;
		case mngmnt::enumId::kDefaultDataSet:
			pData = std::make_shared<tlvDefaultDataSet>();
			break;
		case mngmnt::enumId::kCurrentDataSet:
			pData = std::make_shared<tlvCurrentDataSet>();
			break;
		case mngmnt::enumId::kPortDataSet:
			pData = std::make_shared<tlvPortDataSet>();
			break;
		case mngmnt::enumId::kParentDataSet:
			pData = std::make_shared<tlvParentDataSet>();
			break;
		case mngmnt::enumId::kTimePropertiesDataSet:
			pData = std::make_shared<tlvTimePropertiesDataSet>();
			break;
		case mngmnt::enumId::kTime:
			pData = std::make_shared<tlvTime>();
			break;
		case mngmnt::enumId::kPriority1:
		case mngmnt::enumId::kPriority2:
		case mngmnt::enumId::kDomain:
		case mngmnt::enumId::kSlaveOnly:
		case mngmnt::enumId::kMasterOnly:
		case mngmnt::enumId::kSynchronizationUncertainNp:
		case mngmnt::enumId::kLogAnnounceInterval:
		case mngmnt::enumId::kLogMinPdelayReqInterval:
		case mngmnt::enumId::kDelayMechanism:
		case mngmnt::enumId::kLogSyncInterval:
		case mngmnt::enumId::kAnnounceReceiptTimeout:
		case mngmnt::enumId::kAlternateTimeOffsetEnable:
			pData = std::make_shared<tlvBasic>(eId);
			break;
		case mngmnt::enumId::kClockAccuracy:
			pData = std::make_shared<tlvClockAccuracy>();
			break;
		case mngmnt::enumId::kTraceabilityProperties:
			pData = std::make_shared<tlvTraceable>();
			break;
		case mngmnt::enumId::kTimescaleProperties:
			pData = std::make_shared<tlvTimescale>();
			break;
		case mngmnt::enumId::kVersionNumber:
			pData = std::make_shared<tlvVersion>();
			break;
		case mngmnt::enumId::kAlternateTimeOffsetName:
			pData = std::make_shared<tlvAlternateTimeOffsetName>();
			break;
		case mngmnt::enumId::kAlternateTimeOffsetProperties:
			pData = std::make_shared<tlvAlternateTimeOffsetProperties>();
			break;
		case mngmnt::enumId::kTimeStatusNp:
			pData = std::make_shared<tlvTimeStatusNP>();
			break;
		case mngmnt::enumId::kGrandmasterSettingsNp:
			pData = std::make_shared<tlvGrandmasterSettingsNP>();
			break;
		case mngmnt::enumId::kSubscribeEventsNp:
			// @todo
			break;
		case mngmnt::enumId::kPortPropertiesNp:
			pData = std::make_shared<tlvPortPropertiesNP>();
			break;
		case mngmnt::enumId::kPortStatsNp:
			pData = std::make_shared<tlvPortStatsNP>();
			break;
		case mngmnt::enumId::kPortServiceStatsNp:
			pData = std::make_shared<tlvPortServiceStatsNP>();
			break;
		case mngmnt::enumId::kPortDataSetNp:
			pData = std::make_shared<tlvPortDataSetNP>();
		default:
			pData = nullptr;
	}
}

void managementTlvResponse::CreateMessage(std::vector<unsigned char>& vMessage) const
{
	FromU16(static_cast<uint16_t>(mngmnt::enumType::kManagement), vMessage);
	if(pData)
	{
		auto vTlv = pData->CreateMessage();

		if(vTlv.size() % 2 != 0)
		{
			vTlv.push_back(0x0); //pad
		}

		FromU16(static_cast<uint16_t>(vTlv.size()+2), vMessage);
		FromU16(static_cast<uint16_t>(eId), vMessage);
		std::copy(vTlv.begin(), vTlv.end(), std::back_insert_iterator(vMessage));
	}
	else
	{
		FromU16(0, vMessage);
		FromU16(static_cast<uint16_t>(eId), vMessage);
	}

}

void managementTlvResponse::ParseTlv(const std::vector<unsigned char>& vMessage)
{
	eId = static_cast<mngmnt::enumId>(ToU16(vMessage, nPos));

	switch(eId)
	{
		case mngmnt::enumId::kClockDescription:
			pData = std::make_shared<tlvClockDescription>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kUserDescription:
			pData = std::make_shared<tlvUserDescription>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kDefaultDataSet:
			pData = std::make_shared<tlvDefaultDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kCurrentDataSet:
			pData = std::make_shared<tlvCurrentDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kPortDataSet:
			pData = std::make_shared<tlvPortDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kParentDataSet:
			pData = std::make_shared<tlvParentDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kTimePropertiesDataSet:
			pData = std::make_shared<tlvTimePropertiesDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kTime:
			pData = std::make_shared<tlvTime>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kPriority1:
		case mngmnt::enumId::kPriority2:
		case mngmnt::enumId::kDomain:
		case mngmnt::enumId::kSlaveOnly:
		case mngmnt::enumId::kMasterOnly:
		case mngmnt::enumId::kSynchronizationUncertainNp:
		case mngmnt::enumId::kLogAnnounceInterval:
		case mngmnt::enumId::kLogMinPdelayReqInterval:
		case mngmnt::enumId::kDelayMechanism:
		case mngmnt::enumId::kLogSyncInterval:
		case mngmnt::enumId::kAnnounceReceiptTimeout:
		case mngmnt::enumId::kAlternateTimeOffsetEnable:
			pData = std::make_shared<tlvBasic>(eId, std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kClockAccuracy:
			pData = std::make_shared<tlvClockAccuracy>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kTraceabilityProperties:
			pData = std::make_shared<tlvTraceable>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kTimescaleProperties:
			pData = std::make_shared<tlvTimescale>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kVersionNumber:
			pData = std::make_shared<tlvVersion>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kAlternateTimeOffsetName:
			pData = std::make_shared<tlvAlternateTimeOffsetName>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kAlternateTimeOffsetProperties:
			pData = std::make_shared<tlvAlternateTimeOffsetProperties>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kTimeStatusNp:
			pData = std::make_shared<tlvTimeStatusNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kGrandmasterSettingsNp:
			pData = std::make_shared<tlvGrandmasterSettingsNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kSubscribeEventsNp:
			// @todo
			break;
		case mngmnt::enumId::kPortPropertiesNp:
			pData = std::make_shared<tlvPortPropertiesNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kPortStatsNp:
			pData = std::make_shared<tlvPortStatsNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kPortServiceStatsNp:
			pData = std::make_shared<tlvPortServiceStatsNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::kPortDataSetNp:
			pData = std::make_shared<tlvPortDataSetNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			
		/*
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
			ToPidStr(&phn->portIdentity),
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


tlvBasic::tlvBasic(mngmnt::enumId id, const std::vector<unsigned char>& vMessage) : tlv(id)
{
	nValue = ToU8(vMessage,nPos);
	nReserved = ToU8(vMessage, nPos);
}

void tlvBasic::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Value    = " << (int)nValue;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Reserved = " << (int)nReserved;

}

std::vector<unsigned char> tlvBasic::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU8(nValue, vMessage);
	FromU8(nReserved, vMessage);

	return vMessage;
}

tlvClockAccuracy::tlvClockAccuracy(const std::vector<unsigned char>& vMessage) : tlvBasic(mngmnt::enumId::kClockAccuracy, vMessage)
{

}

void tlvClockAccuracy::OutputValues() const
{
	if(auto itAccuracy = clockQuality::kAccuracy.find(static_cast<clck::enumAccuracy>(nValue)); itAccuracy != clockQuality::kAccuracy.end())
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Accuracy =  " << itAccuracy->second;
	}
	else
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Accuracy =  " << std::to_string(nValue);
	}
}

tlvTraceable::tlvTraceable(const std::vector<unsigned char>& vMessage) : tlvBasic(mngmnt::enumId::kTraceabilityProperties, vMessage)
{
	bTimeTraceable = nValue & static_cast<uint8_t>(hdr::enumFlags::kTimeTraceable);
	bFrequencyTraceable = nValue & static_cast<uint8_t>(hdr::enumFlags::kFreqTraceable);
}

std::vector<unsigned char> tlvTraceable::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	uint8_t nValue = (bTimeTraceable ? static_cast<uint8_t>(hdr::enumFlags::kTimeTraceable) : 0) + 
					 (bFrequencyTraceable ? static_cast<uint8_t>(hdr::enumFlags::kTimeTraceable) : 0);
	
	FromU8(nValue, vMessage);
	FromU8(0, vMessage);
	return vMessage;
}

void tlvTraceable::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Time Traceable = " << (bTimeTraceable ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Freq Traceable = " << (bFrequencyTraceable ? "true" : "false");
}

tlvTimescale::tlvTimescale(const std::vector<unsigned char>& vMessage) : tlvBasic(mngmnt::enumId::kTimescaleProperties, vMessage)
{
	bPtpTimescale = nValue & static_cast<uint8_t>(hdr::enumFlags::kTimescale);
}


std::vector<unsigned char> tlvTimescale::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU8(bPtpTimescale ? static_cast<uint8_t>(hdr::enumFlags::kTimescale) : 0, vMessage);
	FromU8(0, vMessage);
	return vMessage;
}

void tlvTimescale::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "PTP Timescale = " << (bPtpTimescale ? "true" : "false");
}

tlvVersion::tlvVersion(const std::vector<unsigned char>& vMessage) : tlvBasic(mngmnt::enumId::kVersionNumber, vMessage)
{
	nVersion = nValue & 0x0f;
}

void tlvVersion::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Version = " << (int)nVersion;
}

std::vector<unsigned char> tlvVersion::CreateMessage() const
{
	std::vector<unsigned char> vMessage;

	FromU8(nVersion, vMessage);
	FromU8(nReserved, vMessage);
	return vMessage;
}

tlvUserDescription::tlvUserDescription(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kUserDescription)
{
	auto vSplit = SplitString(ToStr8(vMessage, nPos), ';');
	if(vSplit.size() > 1)
	{
		sLocation = vSplit[1];
	}
	if(vSplit.empty() == false)
	{
		sName = vSplit[0];
	}
}

void tlvUserDescription::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Name        = " << sName;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Location    = " << sLocation;
}

std::vector<unsigned char> tlvUserDescription::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	auto str = sName+";"+sLocation;

	FromStr(str.substr(0, 128), vMessage);
	return vMessage;
}


tlvClockDescription::tlvClockDescription(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kClockDescription)
{
	auto nFlags = ToU16(vMessage, nPos);
	
	bClockOrdinary = 0x8000 & nFlags;
	bClockBoundary = 0x4000 & nFlags;
	bClockTranparentP2P = 0x2000 & nFlags;
	bClockTranparentE2E = 0x1000 & nFlags;
	bClockManagement = 0x800 & nFlags;

	sPhysicalLayerProtocol = ToStr8(vMessage, nPos);

	sPhysicalAddress = ToHexStr16(vMessage, nPos);
	protocolAddress.nProtocol = ToU16(vMessage, nPos);
	
	if(protocolAddress.nProtocol == 1)	//IPV4
	{
		protocolAddress.sAddress = ToDecStr16(vMessage, nPos);
	}
	else
	{
		protocolAddress.sAddress = ToHexStr16(vMessage, nPos);
	}
	
	sManufacturerIdentity = ToHexStr(vMessage, nPos, 3);
	++nPos; //reserved

	sProductDescription = ToStr8(vMessage, nPos);
	sRevisionData = ToStr8(vMessage, nPos);
	sUserDescription = ToStr8(vMessage, nPos);

	sProfileIdentity = ToHexStr(vMessage, nPos, 6);
}

std::vector<unsigned char> tlvClockDescription::CreateMessage() const
{
	std::vector<unsigned char> vMessage;

	//create flag
	uint16_t nFlags = bClockOrdinary      ? (static_cast<uint16_t>(clck::enumType::kOrdinary)) : 0 +
					  bClockBoundary      ? (static_cast<uint16_t>(clck::enumType::kBoundary)) : 0 +
					  bClockTranparentP2P ? (static_cast<uint16_t>(clck::enumType::kTransparentP2P)) : 0 +
					  bClockTranparentE2E ? (static_cast<uint16_t>(clck::enumType::kTransparentE2E)) : 0 +
					  bClockManagement    ? (static_cast<uint16_t>(clck::enumType::kManagement)) : 0;
					  
	FromU16(nFlags, vMessage);
	FromStr(sPhysicalLayerProtocol, vMessage);
	FromHexStr16(sPhysicalAddress, vMessage);
	FromU16(protocolAddress.nProtocol, vMessage);
	if(protocolAddress.nProtocol == 1)
	{
		FromDecStr16(protocolAddress.sAddress, vMessage);
	}
	else
	{
		FromHexStr16(protocolAddress.sAddress, vMessage);
	}
	FromHexStr(sManufacturerIdentity, vMessage, 3);

	FromU8(0, vMessage);	//reserved

	FromStr(sProductDescription, vMessage);
	FromStr(sRevisionData, vMessage);
	FromStr(sUserDescription, vMessage);
	FromHexStr(sProfileIdentity, vMessage, 6);

	return vMessage;
}

void tlvClockDescription::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Physical Layer Protocol = " << sPhysicalLayerProtocol;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Physical Address        = " << sPhysicalAddress;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Protocol Address        = " << protocolAddress.sAddress;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Protocol                = " << (int)protocolAddress.nProtocol;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Manufacturer Id         = " << sManufacturerIdentity;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Product Description     = " << sProductDescription;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Revision                = " << sRevisionData;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "User Description        = " << sUserDescription;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Profile Id              = " << sProfileIdentity;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock: Ordinary         =  " << (bClockOrdinary ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock: Boundary         =  " << (bClockBoundary ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock: Transparant P2P  =  " << (bClockTranparentP2P ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock: Transparant E2E  =  " << (bClockTranparentE2E ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock: Managemenbt      =  " << (bClockManagement ? "true" : "false");
}




tlvDefaultDataSet::tlvDefaultDataSet(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kDefaultDataSet)
{
	auto nFlags = ToU8(vMessage, nPos);
	nReserved1 = ToU8(vMessage, nPos);
	nNumberPorts = ToU16(vMessage, nPos);
	nPriority1 = ToU8(vMessage, nPos);
	quality.nClass = ToU8(vMessage, nPos);
	quality.eAccuracy = static_cast<clck::enumAccuracy>(ToU8(vMessage, nPos));
	quality.nOffsetScaledLogVariance = ToU16(vMessage, nPos);
	nPriority2 = ToU8(vMessage, nPos);
	sClockIdentity = ToHexStr(vMessage, nPos, 8);
	nDomain = ToU8(vMessage, nPos);
	nReserved2 = ToU8(vMessage, nPos);

	b2Step = nFlags & (1<<0);
	bSlaveOnly = nFlags & (1<<1);
}

std::vector<unsigned char> tlvDefaultDataSet::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	uint8_t nFlags = (b2Step ? 1 : 0) + (bSlaveOnly ? 2 : 0);
	
	FromU8(nFlags, vMessage);
	FromU8(nReserved1, vMessage);
	FromU8(quality.nClass, vMessage);
	FromU8(static_cast<uint8_t>(quality.eAccuracy), vMessage);
	FromU16(quality.nOffsetScaledLogVariance, vMessage);
	FromU8(nPriority2, vMessage);
	FromHexStr(sClockIdentity, vMessage, 8);
	FromU8(nDomain, vMessage);
	FromU8(nReserved2, vMessage);
	
	return vMessage;
}

void tlvDefaultDataSet::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock ID                  =  " << sClockIdentity;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Domain                    =  " << (int)nDomain;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "2-Step                    = " << (b2Step ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Slave Only                = " << (bSlaveOnly ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Ports                     =  " << nNumberPorts;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Priority1                 =  " << (int)nPriority1;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Priority2                 =  " << (int)nPriority2;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Class               =  " << (int)quality.nClass;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Class               =  " << (int)quality.nClass;
	if(auto itAccuracy = clockQuality::kAccuracy.find(quality.eAccuracy); itAccuracy != clockQuality::kAccuracy.end())
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Accuracy            =  " << itAccuracy->second;
	}
	else
	{	
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Accuracy            =  " << std::to_string(static_cast<int>(quality.eAccuracy));
	}
	
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Offset Log Variance =  " << std::hex << "0x" << quality.nOffsetScaledLogVariance;
}

tlvCurrentDataSet::tlvCurrentDataSet(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kCurrentDataSet)
{
	nStepsRemoved = ToU16(vMessage, nPos);
	offsetFromMaster = ToSubNano(vMessage, nPos);
	meanPathDelay = ToSubNano(vMessage, nPos);
}

std::vector<unsigned char> tlvCurrentDataSet::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU16(nStepsRemoved, vMessage);
	FromSubNano(offsetFromMaster, vMessage);
	FromSubNano(meanPathDelay, vMessage);
	return vMessage;
}

void tlvCurrentDataSet::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Steps Removed = " << nStepsRemoved;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Offset        = " << offsetFromMaster.nano.count() << "." << offsetFromMaster.sub;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Mean Delay    = " << meanPathDelay.nano.count() << "." << offsetFromMaster.sub;
}

tlvParentDataSet::tlvParentDataSet(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kParentDataSet)
{
	sParentPortIdentity = ToPidStr(vMessage, nPos);
	nParentStats = ToU8(vMessage, nPos);
	nReserved = ToU8(vMessage, nPos);
	nObservedParentOffsetScaledLogVariance = ToU16(vMessage, nPos);
	nObservedParentClockPhaseChangeRate = ToU32(vMessage, nPos);
	nGrandmasterPriority1 = ToU8(vMessage, nPos);
	grandmasterClockQuality.nClass = ToU8(vMessage, nPos);
	grandmasterClockQuality.eAccuracy =  static_cast<clck::enumAccuracy>(ToU8(vMessage, nPos));

	grandmasterClockQuality.nOffsetScaledLogVariance = ToU16(vMessage, nPos);

	nGrandmasterPriority2 = ToU8(vMessage, nPos);
	sGrandmasterIdentity = ToHexStr(vMessage, nPos, 8);
}

std::vector<unsigned char> tlvParentDataSet::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromPidStr(sParentPortIdentity, vMessage);
	FromU8(nParentStats, vMessage);
	FromU8(nReserved,vMessage);
	FromU16(nObservedParentOffsetScaledLogVariance, vMessage);
	FromU32(nObservedParentClockPhaseChangeRate, vMessage);
	FromU8(nGrandmasterPriority1, vMessage);
	FromU8(grandmasterClockQuality.nClass, vMessage);
	FromU8(static_cast<uint8_t>(grandmasterClockQuality.eAccuracy), vMessage);
	FromU16(grandmasterClockQuality.nOffsetScaledLogVariance, vMessage);
	FromU8(nGrandmasterPriority2, vMessage);
	FromHexStr(sGrandmasterIdentity, vMessage, 8);
	return vMessage;
}

void tlvParentDataSet::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Parent Port Id                        = " << sParentPortIdentity;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Parent Stats                          = " << (int)nParentStats;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Observered Parent Offset Log Variance = " << nObservedParentOffsetScaledLogVariance;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Observered Parent Offset Log Variance = " << nObservedParentClockPhaseChangeRate;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Grandmaster Id                        = " << sGrandmasterIdentity;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Grandmaster Priority1                 = " << (int)nGrandmasterPriority1;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Grandmaster Priority2                 = " << (int)nGrandmasterPriority2;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Grandmaster Clock Class               = " << (int)grandmasterClockQuality.nClass;
	
	if(auto itAccuracy = clockQuality::kAccuracy.find(grandmasterClockQuality.eAccuracy); itAccuracy != clockQuality::kAccuracy.end())
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Accuracy                        =  " << itAccuracy->second;	
	}
	else
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Accuracy                        =  " << std::to_string(static_cast<int>(grandmasterClockQuality.eAccuracy));	
	}
	
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Grandmaster Offset Log Variance       = " << std::hex << "0x" << (int)grandmasterClockQuality.nOffsetScaledLogVariance;
}

tlvTimePropertiesDataSet::tlvTimePropertiesDataSet(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kTimePropertiesDataSet)
{
	nCurrentUtcOffset = ToU16(vMessage, nPos);
	auto nFlags = ToU8(vMessage, nPos);
	eTimeSource = static_cast<clck::enumTimeSource>(ToU8(vMessage, nPos));

	bLeap61 		= nFlags & static_cast<uint8_t>(hdr::enumFlags::kLI_61);
	bLeap59 		= nFlags & static_cast<uint8_t>(hdr::enumFlags::kLI_59);
	bUtcOffsetValid = nFlags & static_cast<uint8_t>(hdr::enumFlags::kUtcOffsetValid);
	bPtpTimescale 	= nFlags & static_cast<uint8_t>(hdr::enumFlags::kTimescale);
	bTimeTraceable 	= nFlags & static_cast<uint8_t>(hdr::enumFlags::kTimeTraceable);
	bFreqTraceable	= nFlags & static_cast<uint8_t>(hdr::enumFlags::kFreqTraceable);
};

std::vector<unsigned char> tlvTimePropertiesDataSet::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU16(nCurrentUtcOffset, vMessage);

	uint8_t nFlags = bLeap61 		 ? static_cast<uint8_t>(hdr::enumFlags::kLI_61) : 0 +
					 bLeap59 		 ? static_cast<uint8_t>(hdr::enumFlags::kLI_59) : 0 +
					 bUtcOffsetValid ? static_cast<uint8_t>(hdr::enumFlags::kUtcOffsetValid) : 0 +
					 bPtpTimescale 	 ? static_cast<uint8_t>(hdr::enumFlags::kTimescale) : 0 +
					 bTimeTraceable  ? static_cast<uint8_t>(hdr::enumFlags::kTimeTraceable) : 0 +
					 bFreqTraceable	 ? static_cast<uint8_t>(hdr::enumFlags::kFreqTraceable) : 0;

	FromU8(nFlags, vMessage);
	FromU8(static_cast<uint8_t>(eTimeSource), vMessage);
	return vMessage;
}

void tlvTimePropertiesDataSet::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Current UTC Offset  = " << nCurrentUtcOffset;
	if(auto itSource = clockQuality::kSource.find(eTimeSource); itSource != clockQuality::kSource.end())
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Time Source               = " << itSource->second;
	}
	else
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Time Source               = " << std::hex << "0x" << static_cast<int>(eTimeSource);
	}

	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Leap 61             = " << (bLeap61 ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Leap 59             = " << (bLeap59 ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "UTC Offset Valid    = " << (bUtcOffsetValid ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "PTP Timescale       = " << (bPtpTimescale ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Time Traceable      = " << (bTimeTraceable ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Freq Traceable      = " << (bFreqTraceable ? "true" : "false");
}


tlvPortDataSet::tlvPortDataSet(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kPortDataSet)
{
	sPortIdentity  = ToPidStr(vMessage, nPos);
	nPortState = ToU8(vMessage, nPos);
	nLogMinDelayReqInterval = ToU8(vMessage, nPos);
	peerMeanPathDelay = ToSubNano(vMessage, nPos);
	nLogAnnounceInterval = To8(vMessage, nPos);
	nAnnounceReceiptTimeout = ToU8(vMessage, nPos);
	nLogSyncInterval = To8(vMessage, nPos);

	eDelayMechanism = static_cast<clck::enumDelay>(ToU8(vMessage, nPos));
	nLogMinPdelayReqInterval = To8(vMessage, nPos);
	nVersionNumber = ToU8(vMessage, nPos) & 0x0f;
}

std::vector<unsigned char> tlvPortDataSet::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromPidStr(sPortIdentity, vMessage);
	FromU8(nPortState, vMessage);
	FromU8(nLogMinDelayReqInterval, vMessage);
	FromSubNano(peerMeanPathDelay, vMessage);
	FromU8(nLogAnnounceInterval, vMessage);
	FromU8(nAnnounceReceiptTimeout, vMessage);
	FromU8(nLogSyncInterval, vMessage);
	FromU8(static_cast<uint8_t>(eDelayMechanism), vMessage);
	FromU8(nLogMinPdelayReqInterval, vMessage);
	FromU8(nVersionNumber, vMessage);

	return vMessage;
}

void tlvPortDataSet::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Port Id                = " << sPortIdentity;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "State                  = " << kPortStates[nPortState];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Min Delay Req Interval =  " << (int)nLogMinDelayReqInterval;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Peer Mean Path Delay   =  " << peerMeanPathDelay.nano.count() << "." << peerMeanPathDelay.sub;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Announce Interval      =  " << (int)nLogAnnounceInterval;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Announce Timeout       =  " << (int)nAnnounceReceiptTimeout;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Sync Interval          =  " << (int)nLogSyncInterval;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Delay Mechanism        =  " << (int)eDelayMechanism;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "P-Delay_Req Interval   =  " << (int)nLogMinPdelayReqInterval;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Version =  " << (int)nVersionNumber;
}

tlvAlternateTimeOffsetName::tlvAlternateTimeOffsetName(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kAlternateTimeOffsetName)
{
	nKeyField = ToU8(vMessage, nPos);
	
	sName = ToStr8(vMessage, nPos);
}

std::vector<unsigned char> tlvAlternateTimeOffsetName::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU8(nKeyField, vMessage);
	FromStr(sName, vMessage);
	return vMessage;
}

void tlvAlternateTimeOffsetName::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Key Field = " << (int)nKeyField;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Name      = " << sName;
}


tlvAlternateTimeOffsetProperties::tlvAlternateTimeOffsetProperties(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kAlternateTimeOffsetProperties)
{
	nKeyField = ToU8(vMessage, nPos);
	nCurrentOffset = To32(vMessage, nPos);
	nJumpSeconds = To32(vMessage, nPos);

	uint64_t nNext = ToU16(vMessage, nPos);
	nNext <<= 32;
	nNext |=ToU32(vMessage, nPos);
	
	nextJump = std::chrono::seconds(nNext);
}

std::vector<unsigned char> tlvAlternateTimeOffsetProperties::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	//@TODO
	return vMessage;
}

void tlvAlternateTimeOffsetProperties::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Key Field      = " << (int)nKeyField;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Current Offset = " << nCurrentOffset;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Jump Seconds   = " << nJumpSeconds;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Next Jump      = " << nextJump.count();
}


tlvTimeStatusNP::tlvTimeStatusNP(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kTimeStatusNp)
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
	sGmIdentity = ToHexStr(vMessage, nPos, 8);
}

std::vector<unsigned char> tlvTimeStatusNP::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU64(masterOffset.count(), vMessage);
	FromU64(ingressTime.count(), vMessage);
	FromU32(nCumulativeScaledRateOffset, vMessage);
	FromU32(nScaledLastGmPhaseChange, vMessage);
	FromU16(nGmTimeBaseIndicator, vMessage);
	FromU16(lastGmPhaseChange.nanoseconds_msb, vMessage);
	FromU64(lastGmPhaseChange.nanoseconds_lsb, vMessage);
	FromU16(lastGmPhaseChange.fractional_nanoseconds, vMessage);
	FromU32(bGmPresent ? 1 : 0 , vMessage);
	FromHexStr(sGmIdentity, vMessage, 8);

	return vMessage;
}

void tlvTimeStatusNP::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Master Offset        = " << masterOffset.count();
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Ingress Time         = " << ingressTime.count();
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Last GM Phase Change = " << nScaledLastGmPhaseChange;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "GM Time Base         = " << nGmTimeBaseIndicator;
	//@todo lastphasechange
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Rate Offset          = " << nCumulativeScaledRateOffset;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "GM Present           = " << (bGmPresent ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "GM Id                = " << sGmIdentity;
}

tlvGrandmasterSettingsNP::tlvGrandmasterSettingsNP(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kGrandmasterSettingsNp)
{
	quality.nClass = ToU8(vMessage, nPos);
	quality.eAccuracy = static_cast<clck::enumAccuracy>(ToU8(vMessage, nPos));
	quality.nOffsetScaledLogVariance = ToU16(vMessage, nPos);

	nUtcOffset = To16(vMessage, nPos);
	auto nFlags = ToU8(vMessage, nPos);
	eTimeSource = static_cast<clck::enumTimeSource>(ToU8(vMessage, nPos));

	bLeap61 				= nFlags & static_cast<uint8_t>(hdr::enumFlags::kLI_61);
	bLeap59 				= nFlags & static_cast<uint8_t>(hdr::enumFlags::kLI_59);
	bCurrentUtcOffsetValid  = nFlags & static_cast<uint8_t>(hdr::enumFlags::kUtcOffsetValid);
	bPtpTimescale 			= nFlags & static_cast<uint8_t>(hdr::enumFlags::kTimescale);
	bTimeTraceable 			= nFlags & static_cast<uint8_t>(hdr::enumFlags::kTimeTraceable);
	bFrequencyTraceable		= nFlags & static_cast<uint8_t>(hdr::enumFlags::kFreqTraceable);
}

std::vector<unsigned char> tlvGrandmasterSettingsNP::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU8(quality.nClass, vMessage);
	FromU8(static_cast<uint8_t>(quality.eAccuracy), vMessage);
	FromU16(quality.nOffsetScaledLogVariance, vMessage);
	FromU16(nUtcOffset, vMessage);

	uint8_t nFlags = bLeap61 		         ? static_cast<uint8_t>(hdr::enumFlags::kLI_61) : 0 +
					 bLeap59 		         ? static_cast<uint8_t>(hdr::enumFlags::kLI_59) : 0 +
					 bCurrentUtcOffsetValid  ? static_cast<uint8_t>(hdr::enumFlags::kUtcOffsetValid) : 0 +
					 bPtpTimescale 	         ? static_cast<uint8_t>(hdr::enumFlags::kTimescale) : 0 +
					 bTimeTraceable          ? static_cast<uint8_t>(hdr::enumFlags::kTimeTraceable) : 0 +
					 bFrequencyTraceable	 ? static_cast<uint8_t>(hdr::enumFlags::kFreqTraceable) : 0;

	FromU8(nFlags, vMessage);
	FromU8(static_cast<uint8_t>(eTimeSource), vMessage);

	return vMessage;
}

void tlvGrandmasterSettingsNP::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Class               = " << (int)quality.nClass;
	
	if(auto itAccuracy = clockQuality::kAccuracy.find(quality.eAccuracy); itAccuracy != clockQuality::kAccuracy.end())
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Accuracy            =  " << itAccuracy->second;	
	}
	else
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Accuracy            =  " << std::to_string(static_cast<int>(quality.eAccuracy));
	}

	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Clock Offset Log Variance = " << quality.nOffsetScaledLogVariance;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "UTC Offset                = " << nUtcOffset;
	if(auto itSource = clockQuality::kSource.find(eTimeSource); itSource != clockQuality::kSource.end())
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Time Source               = " << itSource->second;
	}
	else
	{
		pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Time Source               = " << std::hex << "0x" << static_cast<int>(eTimeSource);
	}
	
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Leap 61                   = " << (bLeap61 ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Leap 59                   = " << (bLeap59 ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "UTC Offset Valid          = " << (bCurrentUtcOffsetValid ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "PTP Timescale             = " << (bPtpTimescale ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Time Traceable            = " << (bTimeTraceable ? "true" : "false");
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Freq Traceable            = " << (bFrequencyTraceable ? "true" : "false");
}


tlvPortPropertiesNP::tlvPortPropertiesNP(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kPortPropertiesNp)
{
	sPortIdentity = ToPidStr(vMessage, nPos);

	nPortState = To8(vMessage, nPos);
	nTimestamping = To8(vMessage, nPos);
	sInterface = ToStr8(vMessage, nPos);
}

std::vector<unsigned char> tlvPortPropertiesNP::CreateMessage() const
{
	std::vector<unsigned char> vMessage;

	FromPidStr(sPortIdentity, vMessage);
	FromU8(nPortState, vMessage);
	FromU8(nTimestamping,vMessage);
	FromStr(sInterface, vMessage);

	return vMessage;
}



void tlvPortPropertiesNP::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Port ID      = " << sPortIdentity;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Port State   = " << tlvPortDataSet::kPortStates[nPortState];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Timestamping = " << tlvPortDataSet::kTimestamping[nTimestamping];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Interface    = " << sInterface;
}

tlvPortStatsNP::tlvPortStatsNP(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kPortStatsNp)
{
	sPortIdentity = ToPidStr(vMessage, nPos);
	for(auto& value : rx)
	{
		value = ToU64(vMessage, nPos);
	}
	for(auto& value : tx)
	{
		value = ToU64(vMessage, nPos);
	}
}

std::vector<unsigned char> tlvPortStatsNP::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	return vMessage;
}



void tlvPortStatsNP::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Port ID                    = " << sPortIdentity;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "RX Delay_Req               = " << rx[static_cast<size_t>(hdr::enumType::kDelayReq)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "RX Delay_Resp              = " << rx[static_cast<size_t>(hdr::enumType::kDelayResp)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "RX Follow_Up               = " << rx[static_cast<size_t>(hdr::enumType::kFollowUp)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "RX Management              = " << rx[static_cast<size_t>(hdr::enumType::kManagement)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "RX P_Delay_Resp            = " << rx[static_cast<size_t>(hdr::enumType::kPeerDelayResp)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "RX P_Delay_Resp_Follow_Up  = " << rx[static_cast<size_t>(hdr::enumType::kPeerDelayRespFollowUp)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "RX P_Delay_Req             = " << rx[static_cast<size_t>(hdr::enumType::kPeerDelayReq)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "RX Signalling              = " << rx[static_cast<size_t>(hdr::enumType::kSignalling)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "RX Sync                    = " << rx[static_cast<size_t>(hdr::enumType::kSync)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "TX Delay_Req               = " << tx[static_cast<size_t>(hdr::enumType::kDelayReq)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "TX Delay_Resp              = " << tx[static_cast<size_t>(hdr::enumType::kDelayResp)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "TX Follow_Up               = " << tx[static_cast<size_t>(hdr::enumType::kFollowUp)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "TX Management              = " << tx[static_cast<size_t>(hdr::enumType::kManagement)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "TX P_Delay_Resp            = " << tx[static_cast<size_t>(hdr::enumType::kPeerDelayResp)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "TX P_Delay_Resp_Follow_Up  = " << tx[static_cast<size_t>(hdr::enumType::kPeerDelayRespFollowUp)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "TX P_Delay_Req             = " << tx[static_cast<size_t>(hdr::enumType::kPeerDelayReq)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "TX Signalling              = " << tx[static_cast<size_t>(hdr::enumType::kSignalling)];
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "TX Sync                    = " << tx[static_cast<size_t>(hdr::enumType::kSync)];
}

tlvPortServiceStatsNP::tlvPortServiceStatsNP(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::kPortServiceStatsNp)
{
	sPortIdentity = ToPidStr(vMessage, nPos);	
	nAnnounceTimeout = ToU64(vMessage, nPos);
	nSyncTimeout = ToU64(vMessage, nPos);
	nDelayTimeout = ToU64(vMessage, nPos);
	nUnicastServiceTimeout = ToU64(vMessage, nPos);
	nUnicastRequestTimeout = ToU64(vMessage, nPos);
	nMasterAnnounceTimeout = ToU64(vMessage, nPos);
	nMasterSyncTimeout = ToU64(vMessage, nPos);
	nQualificationTimeout = ToU64(vMessage, nPos);
	nSyncMismatch = ToU64(vMessage, nPos);
	nFollowupMismatch = ToU64(vMessage, nPos);
}

std::vector<unsigned char> tlvPortServiceStatsNP::CreateMessage() const
{
	std::vector<unsigned char> vMessage;


	FromPidStr(sPortIdentity, vMessage);	
	FromU64(nAnnounceTimeout, vMessage);
	FromU64(nSyncTimeout,vMessage);
	FromU64(nDelayTimeout, vMessage);
	FromU64(nUnicastServiceTimeout, vMessage);
	FromU64(nUnicastRequestTimeout, vMessage);
	FromU64(nMasterAnnounceTimeout, vMessage);
	FromU64(nMasterSyncTimeout, vMessage);
	FromU64(nQualificationTimeout, vMessage);
	FromU64(nSyncMismatch, vMessage);
	FromU64(nFollowupMismatch, vMessage);

	return vMessage;
}


void tlvPortServiceStatsNP::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Port ID                 = " << sPortIdentity; //8-2
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Announce Timeout        = " << nAnnounceTimeout;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Sync Timeout            = " << nSyncTimeout;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Delay Timeout           = " << nDelayTimeout;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Unicast Service Timeout = " << nUnicastServiceTimeout;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Unicast Request Timeout = " << nUnicastRequestTimeout;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Master Announce Timeout = " << nMasterAnnounceTimeout;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Master Sync Timeout     = " << nMasterSyncTimeout;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Qualification Timeout   = " << nQualificationTimeout;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Sync Mismatch           = " << nSyncMismatch;
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Followup Mismatch       = " << nFollowupMismatch;
}

tlvPortDataSetNP::tlvPortDataSetNP(const std::vector<unsigned char>& vMessage)
{
	neighbourPropDelayThresh = std::chrono::nanoseconds(ToU32(vMessage, nPos));
	bCapable = To32(vMessage, nPos)==1;
}

void tlvPortDataSetNP::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Neighbour PropDeley Threshold = " << neighbourPropDelayThresh.count();
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Capable                       = " << (bCapable ? "true" : "false");
}

std::vector<unsigned char> tlvPortDataSetNP::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU32(static_cast<uint32_t>(neighbourPropDelayThresh.count()), vMessage);
	FromU32(bCapable, vMessage);
	return vMessage;
}

tlvTime::tlvTime(const std::vector<unsigned char>& vMessage)
{
	timestamp = ToSecNano(vMessage, nPos);
}

void tlvTime::OutputValues() const
{
	pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Time        = " << timestamp.count() << " " << TimeToIsoString(timestamp);
}

std::vector<unsigned char> tlvTime::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromSecNano(timestamp, vMessage);
	return vMessage;
}