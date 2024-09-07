#include "ptpstructs.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "timeutils.h"
#include "log.h"
#include <algorithm>

using namespace pml::ptpmonkey;

const std::array<std::string, 5> ptpManagement::MANAGEMENT_ACTION{"GET", "SET", "RESPONSE", "COMMAND", "ACKNOWLEDGE"};

const std::map<mngmnt::enumId, std::string> managementTlv::TLV_ID{{mngmnt::enumId::USER_DESCRIPTION, "USER_DESCRIPTION"}, {mngmnt::enumId::SAVE_IN_NON_VOLATILE_STORAGE, "SAVE_IN_NON_VOLATILE_STORAGE"},  {mngmnt::enumId::RESET_NON_VOLATILE_STORAGE, "RESET_NON_VOLATILE_STORAGE"},{mngmnt::enumId::INITIALIZE, "INITIALIZE"}, {mngmnt::enumId::FAULT_LOG, "FAULT_LOG"}, {mngmnt::enumId::FAULT_LOG_RESET, "FAULT_LOG_RESET"}, {mngmnt::enumId::DEFAULT_DATA_SET, "DEFAULT_DATA_SET"}, {mngmnt::enumId::CURRENT_DATA_SET, "CURRENT_DATA_SET"}, {mngmnt::enumId::PARENT_DATA_SET, "PARENT_DATA_SET"}, {mngmnt::enumId::TIME_PROPERTIES_DATA_SET,"TIME_PROPERTIES_DATA_SET"}, {mngmnt::enumId::PRIORITY1, "PRIORITY1"}, {mngmnt::enumId::PRIORITY2, "PRIORITY2"}, {mngmnt::enumId::DOMAIN, "DOMAIN"}, {mngmnt::enumId::SLAVE_ONLY, "SLAVE_ONLY"}, {mngmnt::enumId::TIME, "TIME"}, {mngmnt::enumId::CLOCK_ACCURACY, "CLOCK_ACCURACY"}, {mngmnt::enumId::UTC_PROPERTIES, "UTC_PROPERTIES"}, {mngmnt::enumId::TRACEABILITY_PROPERTIES, "TRACEABILITY_PROPERTIES"}, {mngmnt::enumId::TIMESCALE_PROPERTIES, "TIMESCALE_PROPERTIES"}, {mngmnt::enumId::PATH_TRACE_LIST, "PATH_TRACE_LIST"}, {mngmnt::enumId::PATH_TRACE_ENABLE, "PATH_TRACE_ENABLE"}, {mngmnt::enumId::GRANDMASTER_CLUSTER_TABLE, "GRANDMASTER_CLUSTER_TABLE"}, {mngmnt::enumId::ACCEPTABLE_MASTER_TABLE, "ACCEPTABLE_MASTER_TABLE"}, {mngmnt::enumId::ACCEPTABLE_MASTER_MAX_TABLE_SIZE, "ACCEPTABLE_MASTER_MAX_TABLE_SIZE"}, {mngmnt::enumId::ALTERNATE_TIME_OFFSET_ENABLE, "ALTERNATE_TIME_OFFSET_ENABLE"}, {mngmnt::enumId::ALTERNATE_TIME_OFFSET_NAME, "ALTERNATE_TIME_OFFSET_NAME"}, {mngmnt::enumId::ALTERNATE_TIME_OFFSET_MAX_KEY, "ALTERNATE_TIME_OFFSET_MAX_KEY"}, {mngmnt::enumId::ALTERNATE_TIME_OFFSET_PROPERTIES, "ALTERNATE_TIME_OFFSET_PROPERTIES"}, {mngmnt::enumId::EXTERNAL_PORT_CONFIGURATION_ENABLED, "EXTERNAL_PORT_CONFIGURATION_ENABLED"}, {mngmnt::enumId::HOLDOVER_UPGRADE_ENABLE, "HOLDOVER_UPGRADE_ENABLE"}, {mngmnt::enumId::TRANSPARENT_CLOCK_DEFAULT_DATA_SET, "TRANSPARENT_CLOCK_DEFAULT_DATA_SET"}, {mngmnt::enumId::PRIMARY_DOMAIN, "PRIMARY_DOMAIN"}, {mngmnt::enumId::TIME_STATUS_NP, "TIME_STATUS_NP"}, {mngmnt::enumId::GRANDMASTER_SETTINGS_NP, "GRANDMASTER_SETTINGS_NP"}, {mngmnt::enumId::SUBSCRIBE_EVENTS_NP, "SUBSCRIBE_EVENTS_NP"}, {mngmnt::enumId::SYNCHRONIZATION_UNCERTAIN_NP, "SYNCHRONIZATION_UNCERTAIN_NP"}, {mngmnt::enumId::NULL_MANAGEMENT, "NULL_MANAGEMENT"}, {mngmnt::enumId::CLOCK_DESCRIPTION, "CLOCK_DESCRIPTION"}, {mngmnt::enumId::PORT_DATA_SET, "PORT_DATA_SET"}, {mngmnt::enumId::LOG_ANNOUNCE_INTERVAL, "LOG_ANNOUNCE_INTERVAL"}, {mngmnt::enumId::ANNOUNCE_RECEIPT_TIMEOUT, "ANNOUNCE_RECEIPT_TIMEOUT"}, {mngmnt::enumId::LOG_SYNC_INTERVAL, "LOG_SYNC_INTERVAL"}, {mngmnt::enumId::VERSION_NUMBER, "VERSION_NUMBER"}, {mngmnt::enumId::ENABLE_PORT, "ENABLE_PORT"}, {mngmnt::enumId::DISABLE_PORT, "DISABLE_PORT"}, {mngmnt::enumId::UNICAST_NEGOTIATION_ENABLE, "UNICAST_NEGOTIATION_ENABLE"}, {mngmnt::enumId::UNICAST_MASTER_TABLE, "UNICAST_MASTER_TABLE"}, {mngmnt::enumId::UNICAST_MASTER_MAX_TABLE_SIZE, "UNICAST_MASTER_MAX_TABLE_SIZE"}, {mngmnt::enumId::ACCEPTABLE_MASTER_TABLE_ENABLED, "ACCEPTABLE_MASTER_TABLE_ENABLED"}, {mngmnt::enumId::ALTERNATE_MASTER, "ALTERNATE_MASTER"}, {mngmnt::enumId::MASTER_ONLY, "MASTER_ONLY"}, {mngmnt::enumId::EXT_PORT_CONFIG_PORT_DATA_SET, "EXT_PORT_CONFIG_PORT_DATA_SET"}, {mngmnt::enumId::TRANSPARENT_CLOCK_PORT_DATA_SET,"TRANSPARENT_CLOCK_PORT_DATA_SET"}, {mngmnt::enumId::DELAY_MECHANISM, "DELAY_MECHANISM"}, {mngmnt::enumId::LOG_MIN_PDELAY_REQ_INTERVAL, "LOG_MIN_PDELAY_REQ_INTERVAL"}, {mngmnt::enumId::PORT_DATA_SET_NP, "PORT_DATA_SET_NP"}, {mngmnt::enumId::PORT_PROPERTIES_NP, "PORT_PROPERTIES_NP"}, {mngmnt::enumId::PORT_STATS_NP, "PORT_STATS_NP"}, {mngmnt::enumId::PORT_SERVICE_STATS_NP, "PORT_SERVICE_STATS_NP"}, {mngmnt::enumId::UNICAST_MASTER_TABLE_NP, "UNICAST_MASTER_TABLE_NP"}, {mngmnt::enumId::PORT_HWCLOCK_NP, "PORT_HWCLOCK_NP"}, {mngmnt::enumId::POWER_PROFILE_SETTINGS_NP, "POWER_PROFILE_SETTINGS_NP"}, {mngmnt::enumId::CMLDS_INFO_NP, "CMLDS_INFO_NP"}};


const std::array<std::string, 11> tlvPortDataSet::PORT_STATES{"NONE","INITIALIZING","FAULTY","DISABLED","LISTENING","PRE_MASTER","MASTER","PASSIVE","UNCALIBRATED","SLAVE","GRAND_MASTER"};
const std::array<std::string, 5> tlvPortDataSet::TIMESTAMPING{"SOFTWARE", "HARDWARE", "LEGACY_HW", "ONESTEP", "P2P1STEP"};

const std::map<clck::enumAccuracy, std::string> clockQuality::ACCURACY{ {clck::enumAccuracy::ACC_UNKNOWN, "NOT_AVAILABLE"}, {clck::enumAccuracy::ACC_25NS, "25ns"}, {clck::enumAccuracy::ACC_100NS, "100ns"}, {clck::enumAccuracy::ACC_250NS, "250ns"}, {clck::enumAccuracy::ACC_1US, "1us"}, {clck::enumAccuracy::ACC_2_5US, "2.5us"}, {clck::enumAccuracy::ACC_10US, "10us"}, {clck::enumAccuracy::ACC_25US, "25us"}, {clck::enumAccuracy::ACC_100US, "100us"}, {clck::enumAccuracy::ACC_250US, "250us"}, {clck::enumAccuracy::ACC_1MS, "1ms"}, {clck::enumAccuracy::ACC_2_5MS, "2 to 5ms"}, {clck::enumAccuracy::ACC_10MS, "10ms"}, {clck::enumAccuracy::ACC_25MS, "25ms"}, {clck::enumAccuracy::ACC_100MS, "100ms"}, {clck::enumAccuracy::ACC_250MS, "250ms"}, {clck::enumAccuracy::ACC_1S, "1s"}, {clck::enumAccuracy::ACC_10S, "10s"}, {clck::enumAccuracy::ACC_OVER, ">10s"}, {clck::enumAccuracy::ACC_UNKNOWN, "UNKNOWN"}};

const std::map<clck::enumTimeSource, std::string> clockQuality::SOURCE{{clck::enumTimeSource::NA, "NOT_AVAILABLE"}, {clck::enumTimeSource::ATOMIC, "ATOMIC_CLOCK"}, {clck::enumTimeSource::GPS, "GPS"}, {clck::enumTimeSource::RADIO, "TERRESTRIAL_RADIO"}, {clck::enumTimeSource::PTP, "PTP"}, {clck::enumTimeSource::NTP, "NTP"}, {clck::enumTimeSource::HANDSET, "HANDSET"}, {clck::enumTimeSource::OTHER, "OTHER"}, {clck::enumTimeSource::INTERNAL, "INTERNAL_OSCILLATOR"}};

const std::map<mngmnt::enumError, std::string> managementTlvResponse::MNGMNT_ERROR{{mngmnt::enumError::RESPONSE_TOO_BIG, "RESPONSE_TOO_BIG"},
																      {mngmnt::enumError::NO_SUCH_ID, "NO_SUCH_ID"},
																	  {mngmnt::enumError::WRONG_LENGTH, "WRONG_LENGTH"},
																	  {mngmnt::enumError::WRONG_VALUE, "WRONG_VALUE"},
																	  {mngmnt::enumError::NOT_SETABLE, "NOT_SETABLE"},
																	  {mngmnt::enumError::NOT_SUPPORTED, "NOT_SUPPORTED"},
																	  {mngmnt::enumError::GENERAL_ERROR, "GENERAL_ERROR"}};




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

void ptpV2Payload::OutputValues()
{
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Timestamp = " << originTime.count();
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

void ptpAnnounce::OutputValues()
{
    ptpV2Payload::OutputValues();

	auto itAccuracy = clockQuality::ACCURACY.find(eGrandmasterAccuracy);

    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "UTC Offset = " << (int)nUtcOffset ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GrandmasterPriority1 = " << (int)nGrandmasterPriority1 ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GrandmasterClass = " << (int)nGrandmasterClass ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GrandmasterAccuracy = " << (itAccuracy != clockQuality::ACCURACY.end() ? itAccuracy->second : std::to_string(static_cast<int>(eGrandmasterAccuracy)));
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GrandmasterVariance = " << (int)nGrandmasterVariance ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GrandmasterPriority2 = " << (int)nGrandmasterPriority2 ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster ClockId = " << sGrandmasterClockId ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Steps = " << (int)nStepsRemoved ;

	auto itSource = clockQuality::SOURCE.find(eTimeSource);
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Source = " << (itSource != clockQuality::SOURCE.end() ? itSource->second : std::to_string(static_cast<int>(eTimeSource)));
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
		if(eAction == mngmnt::enumAction::RESPONSE)    //means we have a tlv
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
	eAction = mngmnt::enumAction::GET;
	pTlv = std::make_shared<managementTlvResponse>(mngmnt::enumType::MANAGEMENT, static_cast<mngmnt::enumId>(id));
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

void ptpManagement::OutputValues()
{
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Target Port            = " << sTargetPortIdentity;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Starting Boundary Hops = " << (int)nStartingBoundaryHops;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Boundary Hops          = " << (int)nBoundaryHops;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Action                 = " << MANAGEMENT_ACTION[static_cast<int>(eAction)];

	if(pTlv)
	{
		pTlv->OutputValues();
	}
}

void managementTlvResponse::OutputValues()
{
	auto itId = TLV_ID.find(eId);
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "ID = " << (itId != TLV_ID.end() ? itId->second : std::to_string(static_cast<int>(eId)));

	if(eType == mngmnt::enumType::MANAGEMENT)
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
	else if(eType == mngmnt::enumType::MANAGEMENT_ERROR_STATUS)
	{
		auto itError = MNGMNT_ERROR.find(eError);
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << (itError != MNGMNT_ERROR.end() ? itError->second : std::to_string(static_cast<uint16_t>(eError)));
	}
	else
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Unknown TLV " << static_cast<int>(eType);
	}
}

managementTlvResponse::managementTlvResponse(const std::vector<unsigned char>& vMessage)
{
	eType = static_cast<mngmnt::enumType>(ToU16(vMessage, nPos));
	nLength = ToU16(vMessage, nPos);
    

	if(eType == mngmnt::enumType::MANAGEMENT)
	{
		ParseTlv(vMessage);
	}
	else if(eType == mngmnt::enumType::MANAGEMENT_ERROR_STATUS)
	{
		eError = static_cast<mngmnt::enumError>(ToU16(vMessage, nPos));
		eId = static_cast<mngmnt::enumId>(ToU16(vMessage, nPos));
	}
}

managementTlvResponse::managementTlvResponse(mngmnt::enumType eT, mngmnt::enumId eI)
{
	eType = eT;
	eId = eI;
	switch(eId)
	{
		case mngmnt::enumId::CLOCK_DESCRIPTION:
			pData = std::make_shared<tlvClockDescription>();
			break;
		case mngmnt::enumId::USER_DESCRIPTION:
			pData = std::make_shared<tlvUserDescription>();
			break;
		case mngmnt::enumId::DEFAULT_DATA_SET:
			pData = std::make_shared<tlvDefaultDataSet>();
			break;
		case mngmnt::enumId::CURRENT_DATA_SET:
			pData = std::make_shared<tlvCurrentDataSet>();
			break;
		case mngmnt::enumId::PORT_DATA_SET:
			pData = std::make_shared<tlvPortDataSet>();
			break;
		case mngmnt::enumId::PARENT_DATA_SET:
			pData = std::make_shared<tlvParentDataSet>();
			break;
		case mngmnt::enumId::TIME_PROPERTIES_DATA_SET:
			pData = std::make_shared<tlvTimePropertiesDataSet>();
			break;
		case mngmnt::enumId::PRIORITY1:
		case mngmnt::enumId::PRIORITY2:
		case mngmnt::enumId::DOMAIN:
		case mngmnt::enumId::SLAVE_ONLY:
		case mngmnt::enumId::MASTER_ONLY:
		case mngmnt::enumId::SYNCHRONIZATION_UNCERTAIN_NP:
		case mngmnt::enumId::LOG_ANNOUNCE_INTERVAL:
		case mngmnt::enumId::LOG_MIN_PDELAY_REQ_INTERVAL:
		case mngmnt::enumId::DELAY_MECHANISM:
		case mngmnt::enumId::LOG_SYNC_INTERVAL:
		case mngmnt::enumId::ANNOUNCE_RECEIPT_TIMEOUT:
		case mngmnt::enumId::ALTERNATE_TIME_OFFSET_ENABLE:
			pData = std::make_shared<tlvBasic>(eId);
			break;
		case mngmnt::enumId::CLOCK_ACCURACY:
			pData = std::make_shared<tlvClockAccuracy>();
			break;
		case mngmnt::enumId::TRACEABILITY_PROPERTIES:
			pData = std::make_shared<tlvTraceable>();
			break;
		case mngmnt::enumId::TIMESCALE_PROPERTIES:
			pData = std::make_shared<tlvTimescale>();
			break;
		case mngmnt::enumId::VERSION_NUMBER:
			pData = std::make_shared<tlvVersion>();
			break;
		case mngmnt::enumId::ALTERNATE_TIME_OFFSET_NAME:
			pData = std::make_shared<tlvAlternateTimeOffsetName>();
			break;
		case mngmnt::enumId::ALTERNATE_TIME_OFFSET_PROPERTIES:
			pData = std::make_shared<tlvAlternateTimeOffsetProperties>();
			break;
		case mngmnt::enumId::TIME_STATUS_NP:
			pData = std::make_shared<tlvTimeStatusNP>();
			break;
		case mngmnt::enumId::GRANDMASTER_SETTINGS_NP:
			pData = std::make_shared<tlvGrandmasterSettingsNP>();
			break;
		case mngmnt::enumId::SUBSCRIBE_EVENTS_NP:
			// @todo
			break;
		case mngmnt::enumId::PORT_PROPERTIES_NP:
			pData = std::make_shared<tlvPortPropertiesNP>();
			break;
		case mngmnt::enumId::PORT_STATS_NP:
			pData = std::make_shared<tlvPortStatsNP>();
			break;
		case mngmnt::enumId::PORT_SERVICE_STATS_NP:
			pData = std::make_shared<tlvPortServiceStatsNP>();
			break;
		default:
			pData = nullptr;
	}
}

void managementTlvResponse::CreateMessage(std::vector<unsigned char>& vMessage) const
{
	FromU16(static_cast<uint16_t>(mngmnt::enumType::MANAGEMENT), vMessage);
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
		case mngmnt::enumId::CLOCK_DESCRIPTION:
			pData = std::make_shared<tlvClockDescription>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::USER_DESCRIPTION:
			pData = std::make_shared<tlvUserDescription>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::DEFAULT_DATA_SET:
			pData = std::make_shared<tlvDefaultDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::CURRENT_DATA_SET:
			pData = std::make_shared<tlvCurrentDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::PORT_DATA_SET:
			pData = std::make_shared<tlvPortDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::PARENT_DATA_SET:
			pData = std::make_shared<tlvParentDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::TIME_PROPERTIES_DATA_SET:
			pData = std::make_shared<tlvTimePropertiesDataSet>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::PRIORITY1:
		case mngmnt::enumId::PRIORITY2:
		case mngmnt::enumId::DOMAIN:
		case mngmnt::enumId::SLAVE_ONLY:
		case mngmnt::enumId::MASTER_ONLY:
		case mngmnt::enumId::SYNCHRONIZATION_UNCERTAIN_NP:
		case mngmnt::enumId::LOG_ANNOUNCE_INTERVAL:
		case mngmnt::enumId::LOG_MIN_PDELAY_REQ_INTERVAL:
		case mngmnt::enumId::DELAY_MECHANISM:
		case mngmnt::enumId::LOG_SYNC_INTERVAL:
		case mngmnt::enumId::ANNOUNCE_RECEIPT_TIMEOUT:
		case mngmnt::enumId::ALTERNATE_TIME_OFFSET_ENABLE:
			pData = std::make_shared<tlvBasic>(eId, std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::CLOCK_ACCURACY:
			pData = std::make_shared<tlvClockAccuracy>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::TRACEABILITY_PROPERTIES:
			pData = std::make_shared<tlvTraceable>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::TIMESCALE_PROPERTIES:
			pData = std::make_shared<tlvTimescale>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::VERSION_NUMBER:
			pData = std::make_shared<tlvVersion>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::ALTERNATE_TIME_OFFSET_NAME:
			pData = std::make_shared<tlvAlternateTimeOffsetName>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::ALTERNATE_TIME_OFFSET_PROPERTIES:
			pData = std::make_shared<tlvAlternateTimeOffsetProperties>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::TIME_STATUS_NP:
			pData = std::make_shared<tlvTimeStatusNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::GRANDMASTER_SETTINGS_NP:
			pData = std::make_shared<tlvGrandmasterSettingsNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::SUBSCRIBE_EVENTS_NP:
			// @todo
			break;
		case mngmnt::enumId::PORT_PROPERTIES_NP:
			pData = std::make_shared<tlvPortPropertiesNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::PORT_STATS_NP:
			pData = std::make_shared<tlvPortStatsNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
		case mngmnt::enumId::PORT_SERVICE_STATS_NP:
			pData = std::make_shared<tlvPortServiceStatsNP>(std::vector<unsigned char>(vMessage.begin()+6, vMessage.end()));
			break;
			
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

void tlvBasic::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Value    = " << (int)nValue;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Reserved = " << (int)nReserved;

}

std::vector<unsigned char> tlvBasic::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU8(nValue, vMessage);
	FromU8(nReserved, vMessage);

	return vMessage;
}

tlvClockAccuracy::tlvClockAccuracy(const std::vector<unsigned char>& vMessage) : tlvBasic(mngmnt::enumId::CLOCK_ACCURACY, vMessage)
{

}

void tlvClockAccuracy::OutputValues()
{
	if(auto itAccuracy = clockQuality::ACCURACY.find(static_cast<clck::enumAccuracy>(nValue)); itAccuracy != clockQuality::ACCURACY.end())
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Accuracy =  " << itAccuracy->second;
	}
	else
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Accuracy =  " << std::to_string(nValue);
	}
}

tlvTraceable::tlvTraceable(const std::vector<unsigned char>& vMessage) : tlvBasic(mngmnt::enumId::TRACEABILITY_PROPERTIES, vMessage)
{
	bTimeTraceable = nValue & static_cast<uint8_t>(hdr::enumFlags::TIME_TRACEABLE);
	bFrequencyTraceable = nValue & static_cast<uint8_t>(hdr::enumFlags::FREQ_TRACEABLE);
}

std::vector<unsigned char> tlvTraceable::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	uint8_t nValue = (bTimeTraceable ? static_cast<uint8_t>(hdr::enumFlags::TIME_TRACEABLE) : 0) + 
					 (bFrequencyTraceable ? static_cast<uint8_t>(hdr::enumFlags::TIME_TRACEABLE) : 0);
	
	FromU8(nValue, vMessage);
	FromU8(0, vMessage);
	return vMessage;
}

void tlvTraceable::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Traceable = " << (bTimeTraceable ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Freq Traceable = " << (bFrequencyTraceable ? "true" : "false");
}

tlvTimescale::tlvTimescale(const std::vector<unsigned char>& vMessage) : tlvBasic(mngmnt::enumId::TIMESCALE_PROPERTIES, vMessage)
{
	bPtpTimescale = nValue & static_cast<uint8_t>(hdr::enumFlags::TIMESCALE);
}


std::vector<unsigned char> tlvTimescale::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU8(bPtpTimescale ? static_cast<uint8_t>(hdr::enumFlags::TIMESCALE) : 0, vMessage);
	FromU8(0, vMessage);
	return vMessage;
}

void tlvTimescale::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PTP Timescale = " << (bPtpTimescale ? "true" : "false");
}

tlvVersion::tlvVersion(const std::vector<unsigned char>& vMessage) : tlvBasic(mngmnt::enumId::VERSION_NUMBER, vMessage)
{
	nVersion = nValue & 0x0f;
}

void tlvVersion::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Version = " << (int)nVersion;
}

std::vector<unsigned char> tlvVersion::CreateMessage() const
{
	std::vector<unsigned char> vMessage;

	FromU8(nVersion, vMessage);
	FromU8(nReserved, vMessage);
	return vMessage;
}

tlvUserDescription::tlvUserDescription(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::USER_DESCRIPTION)
{
	sUserDescription = ToStr8(vMessage, nPos);
}

void tlvUserDescription::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "User Description        = " << sUserDescription;
}

std::vector<unsigned char> tlvUserDescription::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromStr(sUserDescription, vMessage);
	return vMessage;
}


tlvClockDescription::tlvClockDescription(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::CLOCK_DESCRIPTION)
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
	uint16_t nFlags = bClockOrdinary      ? (static_cast<uint16_t>(clck::enumType::ORDINARY)) : 0 +
					  bClockBoundary      ? (static_cast<uint16_t>(clck::enumType::BOUNDARY)) : 0 +
					  bClockTranparentP2P ? (static_cast<uint16_t>(clck::enumType::TRANSPARENT_P2P)) : 0 +
					  bClockTranparentE2E ? (static_cast<uint16_t>(clck::enumType::TRANSPARENT_E2E)) : 0 +
					  bClockManagement    ? (static_cast<uint16_t>(clck::enumType::MANAGEMENT)) : 0;
					  
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

void tlvClockDescription::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Physical Layer Protocol = " << sPhysicalLayerProtocol;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Physical Address        = " << sPhysicalAddress;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Protocol Address        = " << protocolAddress.sAddress;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Protocol                = " << (int)protocolAddress.nProtocol;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Manufacturer Id         = " << sManufacturerIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Product Description     = " << sProductDescription;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Revision                = " << sRevisionData;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "User Description        = " << sUserDescription;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Profile Id              = " << sProfileIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock: Ordinary         =  " << (bClockOrdinary ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock: Boundary         =  " << (bClockBoundary ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock: Transparant P2P  =  " << (bClockTranparentP2P ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock: Transparant E2E  =  " << (bClockTranparentE2E ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock: Managemenbt      =  " << (bClockManagement ? "true" : "false");
}




tlvDefaultDataSet::tlvDefaultDataSet(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::DEFAULT_DATA_SET)
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

void tlvDefaultDataSet::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock ID                  =  " << sClockIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Domain                    =  " << (int)nDomain;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "2-Step                    = " << (b2Step ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Slave Only                = " << (bSlaveOnly ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Ports                     =  " << nNumberPorts;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Priority1                 =  " << (int)nPriority1;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Priority2                 =  " << (int)nPriority2;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Class               =  " << (int)quality.nClass;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Class               =  " << (int)quality.nClass;
	if(auto itAccuracy = clockQuality::ACCURACY.find(quality.eAccuracy); itAccuracy != clockQuality::ACCURACY.end())
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Accuracy            =  " << itAccuracy->second;
	}
	else
	{	
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Accuracy            =  " << std::to_string(static_cast<int>(quality.eAccuracy));
	}
	
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Offset Log Variance =  " << std::hex << "0x" << quality.nOffsetScaledLogVariance;
}

tlvCurrentDataSet::tlvCurrentDataSet(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::CURRENT_DATA_SET)
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

void tlvCurrentDataSet::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Steps Removed = " << nStepsRemoved;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Offset        = " << offsetFromMaster.nano.count() << "." << offsetFromMaster.sub;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Mean Delay    = " << meanPathDelay.nano.count() << "." << offsetFromMaster.sub;
}

tlvParentDataSet::tlvParentDataSet(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::PARENT_DATA_SET)
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

void tlvParentDataSet::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Parent Port Id                        = " << sParentPortIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Parent Stats                          = " << (int)nParentStats;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Observered Parent Offset Log Variance = " << nObservedParentOffsetScaledLogVariance;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Observered Parent Offset Log Variance = " << nObservedParentClockPhaseChangeRate;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Id                        = " << sGrandmasterIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Priority1                 = " << (int)nGrandmasterPriority1;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Priority2                 = " << (int)nGrandmasterPriority2;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Clock Class               = " << (int)grandmasterClockQuality.nClass;
	
	if(auto itAccuracy = clockQuality::ACCURACY.find(grandmasterClockQuality.eAccuracy); itAccuracy != clockQuality::ACCURACY.end())
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Accuracy                        =  " << itAccuracy->second;	
	}
	else
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Accuracy                        =  " << std::to_string(static_cast<int>(grandmasterClockQuality.eAccuracy));	
	}
	
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Grandmaster Offset Log Variance       = " << std::hex << "0x" << (int)grandmasterClockQuality.nOffsetScaledLogVariance;
}

tlvTimePropertiesDataSet::tlvTimePropertiesDataSet(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::TIME_PROPERTIES_DATA_SET)
{
	nCurrentUtcOffset = ToU16(vMessage, nPos);
	auto nFlags = ToU8(vMessage, nPos);
	eTimeSource = static_cast<clck::enumTimeSource>(ToU8(vMessage, nPos));

	bLeap61 		= nFlags & static_cast<uint8_t>(hdr::enumFlags::LI_61);
	bLeap59 		= nFlags & static_cast<uint8_t>(hdr::enumFlags::LI_59);
	bUtcOffsetValid = nFlags & static_cast<uint8_t>(hdr::enumFlags::UTC_OFFSET_VALID);
	bPtpTimescale 	= nFlags & static_cast<uint8_t>(hdr::enumFlags::TIMESCALE);
	bTimeTraceable 	= nFlags & static_cast<uint8_t>(hdr::enumFlags::TIME_TRACEABLE);
	bFreqTraceable	= nFlags & static_cast<uint8_t>(hdr::enumFlags::FREQ_TRACEABLE);
};

std::vector<unsigned char> tlvTimePropertiesDataSet::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU16(nCurrentUtcOffset, vMessage);

	uint8_t nFlags = bLeap61 		 ? static_cast<uint8_t>(hdr::enumFlags::LI_61) : 0 +
					 bLeap59 		 ? static_cast<uint8_t>(hdr::enumFlags::LI_59) : 0 +
					 bUtcOffsetValid ? static_cast<uint8_t>(hdr::enumFlags::UTC_OFFSET_VALID) : 0 +
					 bPtpTimescale 	 ? static_cast<uint8_t>(hdr::enumFlags::TIMESCALE) : 0 +
					 bTimeTraceable  ? static_cast<uint8_t>(hdr::enumFlags::TIME_TRACEABLE) : 0 +
					 bFreqTraceable	 ? static_cast<uint8_t>(hdr::enumFlags::FREQ_TRACEABLE) : 0;

	FromU8(nFlags, vMessage);
	FromU8(static_cast<uint8_t>(eTimeSource), vMessage);
	return vMessage;
}

void tlvTimePropertiesDataSet::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Current UTC Offset  = " << nCurrentUtcOffset;
	if(auto itSource = clockQuality::SOURCE.find(eTimeSource); itSource != clockQuality::SOURCE.end())
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Source               = " << itSource->second;
	}
	else
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Source               = " << std::hex << "0x" << static_cast<int>(eTimeSource);
	}

	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Leap 61             = " << (bLeap61 ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Leap 59             = " << (bLeap59 ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "UTC Offset Valid    = " << (bUtcOffsetValid ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PTP Timescale       = " << (bPtpTimescale ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Traceable      = " << (bTimeTraceable ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Freq Traceable      = " << (bFreqTraceable ? "true" : "false");
}


tlvPortDataSet::tlvPortDataSet(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::PORT_DATA_SET)
{
	sPortIdentity  = ToPidStr(vMessage, nPos);
	nPortState = ToU8(vMessage, nPos);
	nLogMinDelayReqInterval = ToU8(vMessage, nPos);
	peerMeanPathDelay = ToSubNano(vMessage, nPos);
	nLogAnnounceInterval = To8(vMessage, nPos);
	nAnnounceReceiptTimeout = ToU8(vMessage, nPos);
	nLogSyncInterval = To8(vMessage, nPos);
	nDelayMechanism = ToU8(vMessage, nPos);
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
	FromU8(nDelayMechanism, vMessage);
	FromU8(nLogMinPdelayReqInterval, vMessage);
	FromU8(nVersionNumber, vMessage);

	return vMessage;
}

void tlvPortDataSet::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Port Id                = " << sPortIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "State                  = " << PORT_STATES[nPortState];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Min Delay Req Interval =  " << (int)nLogMinDelayReqInterval;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Peer Mean Path Delay   =  " << peerMeanPathDelay.nano.count() << "." << peerMeanPathDelay.sub;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Announce Interval      =  " << (int)nLogAnnounceInterval;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Announce Timeout       =  " << (int)nAnnounceReceiptTimeout;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Sync Interval          =  " << (int)nLogSyncInterval;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Delay Mechanism        =  " << (int)nDelayMechanism;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "P-Delay_Req Interval   =  " << (int)nLogMinPdelayReqInterval;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Version =  " << (int)nVersionNumber;
}

tlvAlternateTimeOffsetName::tlvAlternateTimeOffsetName(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::ALTERNATE_TIME_OFFSET_NAME)
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

void tlvAlternateTimeOffsetName::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Key Field = " << (int)nKeyField;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Name      = " << sName;
}


tlvAlternateTimeOffsetProperties::tlvAlternateTimeOffsetProperties(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::ALTERNATE_TIME_OFFSET_PROPERTIES)
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

void tlvAlternateTimeOffsetProperties::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Key Field      = " << (int)nKeyField;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Current Offset = " << nCurrentOffset;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Jump Seconds   = " << nJumpSeconds;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Next Jump      = " << nextJump.count();
}


tlvTimeStatusNP::tlvTimeStatusNP(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::TIME_STATUS_NP)
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

void tlvTimeStatusNP::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Master Offset        = " << masterOffset.count();
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Ingress Time         = " << ingressTime.count();
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Last GM Phase Change = " << nScaledLastGmPhaseChange;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GM Time Base         = " << nGmTimeBaseIndicator;
	//@todo lastphasechange
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Rate Offset          = " << nCumulativeScaledRateOffset;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GM Present           = " << (bGmPresent ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "GM Id                = " << sGmIdentity;
}

tlvGrandmasterSettingsNP::tlvGrandmasterSettingsNP(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::GRANDMASTER_SETTINGS_NP)
{
	quality.nClass = ToU8(vMessage, nPos);
	quality.eAccuracy = static_cast<clck::enumAccuracy>(ToU8(vMessage, nPos));
	quality.nOffsetScaledLogVariance = ToU16(vMessage, nPos);

	nUtcOffset = To16(vMessage, nPos);
	auto nFlags = ToU8(vMessage, nPos);
	eTimeSource = static_cast<clck::enumTimeSource>(ToU8(vMessage, nPos));

	bLeap61 				= nFlags & static_cast<uint8_t>(hdr::enumFlags::LI_61);
	bLeap59 				= nFlags & static_cast<uint8_t>(hdr::enumFlags::LI_59);
	bCurrentUtcOffsetValid  = nFlags & static_cast<uint8_t>(hdr::enumFlags::UTC_OFFSET_VALID);
	bPtpTimescale 			= nFlags & static_cast<uint8_t>(hdr::enumFlags::TIMESCALE);
	bTimeTraceable 			= nFlags & static_cast<uint8_t>(hdr::enumFlags::TIME_TRACEABLE);
	bFrequencyTraceable		= nFlags & static_cast<uint8_t>(hdr::enumFlags::FREQ_TRACEABLE);
}

std::vector<unsigned char> tlvGrandmasterSettingsNP::CreateMessage() const
{
	std::vector<unsigned char> vMessage;
	FromU8(quality.nClass, vMessage);
	FromU8(static_cast<uint8_t>(quality.eAccuracy), vMessage);
	FromU16(quality.nOffsetScaledLogVariance, vMessage);
	FromU16(nUtcOffset, vMessage);

	uint8_t nFlags = bLeap61 		         ? static_cast<uint8_t>(hdr::enumFlags::LI_61) : 0 +
					 bLeap59 		         ? static_cast<uint8_t>(hdr::enumFlags::LI_59) : 0 +
					 bCurrentUtcOffsetValid  ? static_cast<uint8_t>(hdr::enumFlags::UTC_OFFSET_VALID) : 0 +
					 bPtpTimescale 	         ? static_cast<uint8_t>(hdr::enumFlags::TIMESCALE) : 0 +
					 bTimeTraceable          ? static_cast<uint8_t>(hdr::enumFlags::TIME_TRACEABLE) : 0 +
					 bFrequencyTraceable	 ? static_cast<uint8_t>(hdr::enumFlags::FREQ_TRACEABLE) : 0;

	FromU8(nFlags, vMessage);
	FromU8(static_cast<uint8_t>(eTimeSource), vMessage);

	return vMessage;
}

void tlvGrandmasterSettingsNP::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Class               = " << (int)quality.nClass;
	
	if(auto itAccuracy = clockQuality::ACCURACY.find(quality.eAccuracy); itAccuracy != clockQuality::ACCURACY.end())
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Accuracy            =  " << itAccuracy->second;	
	}
	else
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Accuracy            =  " << std::to_string(static_cast<int>(quality.eAccuracy));
	}

	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Clock Offset Log Variance = " << quality.nOffsetScaledLogVariance;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "UTC Offset                = " << nUtcOffset;
	if(auto itSource = clockQuality::SOURCE.find(eTimeSource); itSource != clockQuality::SOURCE.end())
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Source               = " << itSource->second;
	}
	else
	{
		pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Source               = " << std::hex << "0x" << static_cast<int>(eTimeSource);
	}
	
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Leap 61                   = " << (bLeap61 ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Leap 59                   = " << (bLeap59 ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "UTC Offset Valid          = " << (bCurrentUtcOffsetValid ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PTP Timescale             = " << (bPtpTimescale ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Time Traceable            = " << (bTimeTraceable ? "true" : "false");
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Freq Traceable            = " << (bFrequencyTraceable ? "true" : "false");
}


tlvPortPropertiesNP::tlvPortPropertiesNP(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::PORT_PROPERTIES_NP)
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



void tlvPortPropertiesNP::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Port ID      = " << sPortIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Port State   = " << tlvPortDataSet::PORT_STATES[nPortState];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Timestamping = " << tlvPortDataSet::TIMESTAMPING[nTimestamping];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Interface    = " << sInterface;
}

tlvPortStatsNP::tlvPortStatsNP(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::PORT_STATS_NP)
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



void tlvPortStatsNP::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Port ID                    = " << sPortIdentity;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Delay_Req               = " << rx[static_cast<size_t>(hdr::enumType::DELAY_REQ)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Delay_Resp              = " << rx[static_cast<size_t>(hdr::enumType::DELAY_RESP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Follow_Up               = " << rx[static_cast<size_t>(hdr::enumType::FOLLOW_UP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Management              = " << rx[static_cast<size_t>(hdr::enumType::MANAGEMENT)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX P_Delay_Resp            = " << rx[static_cast<size_t>(hdr::enumType::PEER_DELAY_RESP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX P_Delay_Resp_Follow_Up  = " << rx[static_cast<size_t>(hdr::enumType::PEER_DELAY_RESP_FOLLOW_UP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX P_Delay_Req             = " << rx[static_cast<size_t>(hdr::enumType::PEER_DLEAY_REQ)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Signalling              = " << rx[static_cast<size_t>(hdr::enumType::SIGNALLING)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "RX Sync                    = " << rx[static_cast<size_t>(hdr::enumType::SYNC)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Delay_Req               = " << tx[static_cast<size_t>(hdr::enumType::DELAY_REQ)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Delay_Resp              = " << tx[static_cast<size_t>(hdr::enumType::DELAY_RESP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Follow_Up               = " << tx[static_cast<size_t>(hdr::enumType::FOLLOW_UP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Management              = " << tx[static_cast<size_t>(hdr::enumType::MANAGEMENT)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX P_Delay_Resp            = " << tx[static_cast<size_t>(hdr::enumType::PEER_DELAY_RESP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX P_Delay_Resp_Follow_Up  = " << tx[static_cast<size_t>(hdr::enumType::PEER_DELAY_RESP_FOLLOW_UP)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX P_Delay_Req             = " << tx[static_cast<size_t>(hdr::enumType::PEER_DLEAY_REQ)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Signalling              = " << tx[static_cast<size_t>(hdr::enumType::SIGNALLING)];
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "TX Sync                    = " << tx[static_cast<size_t>(hdr::enumType::SYNC)];
}

tlvPortServiceStatsNP::tlvPortServiceStatsNP(const std::vector<unsigned char>& vMessage) : tlv(mngmnt::enumId::PORT_SERVICE_STATS_NP)
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


void tlvPortServiceStatsNP::OutputValues()
{
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Port ID                 = " << sPortIdentity; //8-2
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Announce Timeout        = " << nAnnounceTimeout;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Sync Timeout            = " << nSyncTimeout;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Delay Timeout           = " << nDelayTimeout;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Unicast Service Timeout = " << nUnicastServiceTimeout;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Unicast Request Timeout = " << nUnicastRequestTimeout;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Master Announce Timeout = " << nMasterAnnounceTimeout;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Master Sync Timeout     = " << nMasterSyncTimeout;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Qualification Timeout   = " << nQualificationTimeout;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Sync Mismatch           = " << nSyncMismatch;
	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Followup Mismatch       = " << nFollowupMismatch;
}



