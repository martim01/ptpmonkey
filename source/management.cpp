#include "management.h"
#include "ptpstructs.h"
#include <memory>
#include "log.h"
using namespace pml::ptpmonkey;

const std::string Manager::TARGET_ID_ALL = "FF:FF:FF:FF:FF:FF:FF:FF";
const uint16_t Manager::TARGET_NUMBER_ALL = 0xFFFF;

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

bool Manager::Get(mngmnt::enumGet id)
{
    return m_pSender(ptpManagement(id, m_nHops, m_sTargetPortId, m_nTargetPortNumber));
}

bool Manager::SetPriority1(uint8_t nPriority)
{
    if(m_sTargetPortId == TARGET_ID_ALL || m_nTargetPortNumber == TARGET_NUMBER_ALL)
    {
        pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "Attempting management SET but target not configured";
        return false;
    }

    auto message = ptpManagement(mngmnt::enumSet::PRIORITY1, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::PRIORITY1);
    ptlv->nValue = nPriority;
    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

bool Manager::SetPriority2(uint8_t nPriority)
{
    if(m_sTargetPortId == TARGET_ID_ALL || m_nTargetPortNumber == TARGET_NUMBER_ALL)
    {
        pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "Attempting management SET but target not configured";
        return false;
    }

    auto message = ptpManagement(mngmnt::enumSet::PRIORITY2, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::PRIORITY2);
    ptlv->nValue = nPriority;
    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

bool Manager::SetAlternateTimeOffsetEnable(uint8_t nKeyField, bool bEnable)
{
    if(m_sTargetPortId == TARGET_ID_ALL || m_nTargetPortNumber == TARGET_NUMBER_ALL)
    {
        pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "Attempting management SET but target not configured";
        return false;
    }

    auto message = ptpManagement(mngmnt::enumSet::ALTERNATE_TIME_OFFSET_ENABLE, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::ALTERNATE_TIME_OFFSET_ENABLE);
    ptlv->nValue = nKeyField;
    ptlv->nReserved = bEnable;
    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

bool Manager::SetAlternateTimeOffsetName(uint8_t nKeyField, const std::string& sName)
{
    if(m_sTargetPortId == TARGET_ID_ALL || m_nTargetPortNumber == TARGET_NUMBER_ALL)
    {
        pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "Attempting management SET but target not configured";
        return false;
    }

    auto message = ptpManagement(mngmnt::enumSet::ALTERNATE_TIME_OFFSET_NAME, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvAlternateTimeOffsetName>();
    ptlv->nKeyField = nKeyField;
    ptlv->sName = sName.substr(0,10);   //max 10 chars I think
    message.pTlv->pData = ptlv;

    return m_pSender(message);
}

bool Manager::SetAlternateTimeOffsetProperties(uint8_t nKeyField, uint32_t nCurrentOffset, uint32_t nJumpSeconds, const std::chrono::seconds& nextJump)
{
    if(m_sTargetPortId == TARGET_ID_ALL || m_nTargetPortNumber == TARGET_NUMBER_ALL)
    {
        pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "Attempting management SET but target not configured";
        return false;
    }


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
    if(m_sTargetPortId == TARGET_ID_ALL || m_nTargetPortNumber == TARGET_NUMBER_ALL)
    {
        pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "Attempting management SET but target not configured";
        return false;
    }


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
    if(m_sTargetPortId == TARGET_ID_ALL || m_nTargetPortNumber == TARGET_NUMBER_ALL)
    {
        pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "Attempting management SET but target not configured";
        return false;
    }

    auto message = ptpManagement(mngmnt::enumSet::SYNCHRONIZATION_UNCERTAIN_NP, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvBasic>(mngmnt::enumId::SYNCHRONIZATION_UNCERTAIN_NP);
    ptlv->nValue = static_cast<uint8_t>(eState);
    
    message.pTlv->pData = ptlv;
    return m_pSender(message);
}

bool Manager::SetPortDataSetNP(const std::chrono::nanoseconds& neighbourPropDelayThresh, bool bCapable)
{
    if(m_sTargetPortId == TARGET_ID_ALL || m_nTargetPortNumber == TARGET_NUMBER_ALL)
    {
        pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "Attempting management SET but target not configured";
        return false;
    }
    
    auto message = ptpManagement(mngmnt::enumSet::PORT_DATA_SET_NP, m_nHops, m_sTargetPortId, m_nTargetPortNumber);
    auto ptlv = std::make_shared<tlvPortDataSetNP>();
    ptlv->bCapable = bCapable;
    ptlv->neighbourPropDelayThresh = neighbourPropDelayThresh;
    
    message.pTlv->pData = ptlv;

    return m_pSender(message);
}