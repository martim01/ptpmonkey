#include "management.h"
#include "ptpstructs.h"
#include <memory>
#include "log.h"
using namespace pml::ptpmonkey;

const std::string Manager::TARGET_ID_ALL = "FF:FF:FF:FF:FF:FF:FF:FF";
const uint16_t Manager::TARGET_NUMBER_ALL = 0xFFFF;

ptpV2Header CreateHeader(hdr::enumType eType, uint16_t nSequence, uint16_t nLength, uint8_t nInterval, uint8_t nDomain)
{
    ptpV2Header theHeader;

    theHeader.nVersion = 2;
    theHeader.nType = static_cast<uint8_t>(eType);
    theHeader.timestamp = Now();

    theHeader.nMessageLength = nLength;
    theHeader.nDomain = nDomain;
    theHeader.nFlags = 0;
    theHeader.nCorrection = 0;

    auto proc_id = getpid();
    theHeader.source.nSourceId = proc_id;
    //theHeader.source.nSourceId += ((proc_id & 0xFF000000) >> 24);
//	theHeader.source.nSourceId += ((proc_id & 0x00FF0000) >> 16);
    theHeader.source.nSourcePort = proc_id & 0xFFFF;
    theHeader.nSequenceId = nSequence;
    theHeader.nControl = 1;
    theHeader.nInterval = nInterval;
    return theHeader;
}


std::vector<unsigned char> CreateManagement(const ptpManagement& message, uint16_t& nSequence, uint8_t nDomain)
{
    auto vPayload = message.CreateMessage();

    auto theHeader = CreateHeader(hdr::enumType::MANAGEMENT, nSequence, 34+vPayload.size(), 0x7f, nDomain);

    auto vBuffer = theHeader.CreateMessage();
    std::copy(vPayload.begin(), vPayload.end(), std::back_inserter(vBuffer));

    ++nSequence;
    return vBuffer;
}


Manager& Manager::Hops(uint8_t nHops)
{
    m_nHops = nHops;
    return *this;
}

Manager& Manager::Target(const std::string& sTargetPortId, uint16_t nTargetPortNumber)
{
    m_sTargetPortId = sTargetPortId;
    m_nTargetPortNumber = nTargetPortNumber;

    return *this;
}

Manager& Manager::UsePtp4l(bool bPtp4l)
{
    m_bPtp4l = bPtp4l;
    return *this;
}

bool Manager::Get(mngmnt::enumGet id)
{
    if(!m_bPtp4l)
    {
        return m_pSender(ptpManagement(id, m_nHops, m_sTargetPortId, m_nTargetPortNumber));
    }
    else
    {
        
        auto sock = socket(AF_UNIX, SOCK_DGRAM, 0);
        sockaddr_un name;
        name.sun_family = AF_UNIX;
        strcpy(name.sun_path, ("/var/run/ptpmonkeyexample."+std::to_string(getpid())).c_str());
        if(bind(sock, (sockaddr*)&name, sizeof(sockaddr_un)))
        {
            pmlLog(pml::LOG_ERROR, "pml::ptpmonkey") << "Failed to bind " << strerror(errno);
            return false;
        }
        
        sockaddr_un server;
        server.sun_family = AF_UNIX;
        strcpy(name.sun_path, "/run/ptp4l");
        
        auto vBuffer = CreateManagement(ptpManagement(id, 0, "", 0xFFFF),m_nSequence, 0);
        if(sendto(sock, vBuffer.data(), vBuffer.size(), 0, (sockaddr*)&name, sizeof(sockaddr_un)) < 0)
        {
            pmlLog(pml::LOG_ERROR, "pml::ptpmonkey") << "Failed to send " << strerror(errno);
            return false;
        }
        
        
        std::array<unsigned char, 1024> data;
        if(auto nRead = read(sock, data.data(), data.size()); nRead < 0)
        {
            pmlLog(pml::LOG_ERROR, "pml::ptpmonkey") << "Failed to read " << strerror(errno);
            return false;
        }    
        else
        {
            auto str = std::string(data.begin(), data.begin()+nRead);
            pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Received: '" << str << "'";
            return true;
        }
    }
    return false;
}

bool Manager::SetPriority1(uint8_t nPriority)
{
    if(!TargetSet()) { return false; }

    auto message = ptpManagement(mngmnt::enumSet::PRIORITY1, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::PRIORITY1);
    ptlv->nValue = nPriority;
    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

bool Manager::SetPriority2(uint8_t nPriority)
{
    if(!TargetSet()) { return false; }

    auto message = ptpManagement(mngmnt::enumSet::PRIORITY2, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::PRIORITY2);
    ptlv->nValue = nPriority;
    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

bool Manager::SetAlternateTimeOffsetEnable(uint8_t nKeyField, bool bEnable)
{
    if(!TargetSet()) { return false; }

    auto message = ptpManagement(mngmnt::enumSet::ALTERNATE_TIME_OFFSET_ENABLE, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::ALTERNATE_TIME_OFFSET_ENABLE);
    ptlv->nValue = nKeyField;
    ptlv->nReserved = bEnable;
    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

bool Manager::SetAlternateTimeOffsetName(uint8_t nKeyField, const std::string& sName)
{
    if(!TargetSet()) { return false; }

    auto message = ptpManagement(mngmnt::enumSet::ALTERNATE_TIME_OFFSET_NAME, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvAlternateTimeOffsetName>();
    ptlv->nKeyField = nKeyField;
    ptlv->sName = sName.substr(0,10);   //max 10 chars I think
    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

bool Manager::SetAlternateTimeOffsetProperties(uint8_t nKeyField, uint32_t nCurrentOffset, uint32_t nJumpSeconds, const std::chrono::seconds& nextJump)
{
    if(!TargetSet()) { return false; }


    auto message = ptpManagement(mngmnt::enumSet::ALTERNATE_TIME_OFFSET_PROPERTIES, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    
    auto ptlv = std::make_shared<tlvAlternateTimeOffsetProperties>();
    ptlv->nKeyField = nKeyField;
    ptlv->nCurrentOffset = nCurrentOffset;
    ptlv->nJumpSeconds = nJumpSeconds;
    ptlv->nextJump = nextJump;

    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

bool Manager::SetGrandmasterSettings(uint8_t nClass, clck::enumAccuracy eAccuracy, uint16_t nOffsetScaledLogVariance, int16_t nUtcOffset, clck::enumTimeSource eTimeSource, bool bLeap61, bool bLeap59, bool bCurrentUtcOffsetValid, bool bPtpTimescale, bool bTimeTraceable, bool bFrequencyTraceable)
{
    if(!TargetSet()) { return false; }


    auto message = ptpManagement(mngmnt::enumSet::GRANDMASTER_SETTINGS_NP, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvGrandmasterSettingsNP>();
    ptlv->quality.nClass = nClass;
    ptlv->quality.eAccuracy = eAccuracy;
    ptlv->quality.nOffsetScaledLogVariance = nOffsetScaledLogVariance;
    ptlv->nUtcOffset = nUtcOffset;
    ptlv->eTimeSource = eTimeSource;
    ptlv->bLeap61 = bLeap61;
    ptlv->bLeap59 = bLeap59;
    ptlv->bCurrentUtcOffsetValid = bCurrentUtcOffsetValid;
    ptlv->bPtpTimescale = bPtpTimescale;
    ptlv->bTimeTraceable = bTimeTraceable;
    ptlv->bFrequencyTraceable = bFrequencyTraceable;

    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

bool Manager::SetSynchonizationUncertain(clck::enumSync eState)
{
    if(!TargetSet()) { return false; }

    auto message = ptpManagement(mngmnt::enumSet::SYNCHRONIZATION_UNCERTAIN_NP, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::SYNCHRONIZATION_UNCERTAIN_NP);
    ptlv->nValue = static_cast<uint8_t>(eState);
    
    message.pTlv->pData = ptlv;
    return m_pSender(message);
}

bool Manager::SetPortDataSetNP(const std::chrono::nanoseconds& neighbourPropDelayThresh, bool bCapable)
{
   if(!TargetSet()) { return false; } 
    
    auto message = ptpManagement(mngmnt::enumSet::PORT_DATA_SET_NP, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvPortDataSetNP>();
    ptlv->bCapable = bCapable;
    ptlv->neighbourPropDelayThresh = neighbourPropDelayThresh;
    
    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

//max chars 128
bool Manager::SetUserDescription(const std::string& sName, const std::string& sLocation)
{
    if(!TargetSet()) { return false; }

    auto message = ptpManagement(mngmnt::enumSet::USER_DESCRIPTION, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvUserDescription>();
    ptlv->sName = sName;
    ptlv->sLocation = sLocation;
       
    message.pTlv->pData = ptlv;
    return m_pSender(message);
}

bool Manager::SetDomain(uint8_t nDomain)
{
    if(!TargetSet()) { return false; }

    auto message = ptpManagement(mngmnt::enumSet::DOMAIN, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::DOMAIN);
    ptlv->nValue = nDomain;

    message.pTlv->pData = ptlv;
    return m_pSender(message);
}

bool Manager::SetSlaveOnly(bool bSlaveOnly)
{
    if(!TargetSet()) { return false; }

    auto message = ptpManagement(mngmnt::enumSet::SLAVE_ONLY, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::SLAVE_ONLY);
    ptlv->nValue = bSlaveOnly;
    message.pTlv->pData = ptlv;
    return m_pSender(message);
}

bool Manager::SetAnnounceInterval(Rate eInterval)
{
    if(!TargetSet()) { return false; }

    auto message = ptpManagement(mngmnt::enumSet::LOG_ANNOUNCE_INTERVAL, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::LOG_ANNOUNCE_INTERVAL);
    ptlv->nValue = static_cast<uint8_t>(eInterval);
    message.pTlv->pData = ptlv;
    return m_pSender(message);
}

bool Manager::SetAnnounceReceiptTimeout(uint8_t nTimeout)
{
    if(!TargetSet()) { return false; }
       
    auto message = ptpManagement(mngmnt::enumSet::ANNOUNCE_RECEIPT_TIMEOUT, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::ANNOUNCE_RECEIPT_TIMEOUT);
    ptlv->nValue = nTimeout;
    message.pTlv->pData = ptlv;
    return m_pSender(message);
}   //2-255

bool Manager::SetSyncInterval(Rate eInterval)
{
    if(!TargetSet()) { return false; }
       
    auto message = ptpManagement(mngmnt::enumSet::LOG_SYNC_INTERVAL, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::LOG_SYNC_INTERVAL);
    ptlv->nValue = static_cast<uint8_t>(eInterval);
    
    message.pTlv->pData = ptlv;
    return m_pSender(message);
}

bool Manager::SetVersionNumber(uint8_t nVersion)
{
    if(!TargetSet()) { return false; }

    auto message = ptpManagement(mngmnt::enumSet::VERSION_NUMBER, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::VERSION_NUMBER);
    ptlv->nValue = nVersion;
    message.pTlv->pData = ptlv;
    return m_pSender(message);
}

bool Manager::SetTime(const std::chrono::nanoseconds& timestamp, uint16_t nSubNano)
{
    if(!TargetSet()) { return false; }
    
    auto message = ptpManagement(mngmnt::enumSet::TIME, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvTime>();
    ptlv->timestamp = timestamp;
    
    message.pTlv->pData = ptlv;
    return m_pSender(message);
}


bool Manager::TargetSet() const
{
    if(m_sTargetPortId == TARGET_ID_ALL || m_nTargetPortNumber == TARGET_NUMBER_ALL)
    {
        pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "Attempting management SET but target not configured";
        return false;
    }
    return true;
}