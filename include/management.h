#pragma once
#include <string>
#include "enums.h"
#include <chrono>
#include "ptpdll.h"
#include <functional>
#include "asio.hpp"

namespace pml::ptpmonkey
{
    class PtpMonkey;
    struct ptpManagement;

    class PTP_IMPEXPORT Manager
    {
        public:
            Manager& Hops(uint8_t nHops);
            Manager& Target(const std::string& sTargetPortId, uint16_t nTargetPortNumber);
            Manager& UsePtp4l(bool bPtp4l);
            
            bool Get(mngmnt::enumGet id);

            bool SetPriority1(uint8_t nPriority);
            bool SetPriority2(uint8_t nPriority);
            bool SetAlternateTimeOffsetEnable(uint8_t nKeyField, bool bEnable);
            bool SetAlternateTimeOffsetName(uint8_t nKeyField, const std::string& sName);
            bool SetAlternateTimeOffsetProperties(uint8_t nKeyField, uint32_t nCurrentOffset, uint32_t nJumpSeconds, const std::chrono::seconds& nextJump);
 
            bool SetGrandmasterSettings(uint8_t nClass, clck::enumAccuracy eAccuracy, uint16_t nOffsetScaledLogVariance, int16_t nUtcOffset, clck::enumTimeSource eTimeSource, bool bLeap61, bool bLeap59, bool bCurrentUtcOffsetValid, bool bPtpTimescale, bool bTimeTraceable, bool bFrequencyTraceable);

            bool SetSynchonizationUncertain(clck::enumSync eState);
            bool SetPortDataSetNP(const std::chrono::nanoseconds& neighbourPropDelayThresh, bool bCapable);


            bool SetUserDescription(const std::string& sName, const std::string& sLocation);   //max chars 128
            bool SetDomain(uint8_t nDomain);
            bool SetSlaveOnly(bool bSlaveOnly);
            bool SetAnnounceInterval(Rate eInterval);
            bool SetAnnounceReceiptTimeout(uint8_t nTimeout);   //2-255
            bool SetSyncInterval(Rate eInterval);
            bool SetVersionNumber(uint8_t nVersion);
            bool SetTime(const std::chrono::nanoseconds& timestamp, uint16_t nSubNano);

            bool TargetSet() const;
//            bool EnablePort();
//            bool DisablePort();


            

            //MID_SUBSCRIBE_EVENTS_NP
            //MID_POWER_PROFILE_SETTINGS_NP

        private:
            friend class PtpMonkey;

            Manager(const std::function<bool(const ptpManagement&)>& pSender) : m_pSender(pSender){}

            bool m_bPtp4l{false};
            uint8_t m_nHops{1};
            std::string m_sTargetPortId{"FF:FF:FF:FF:FF:FF:FF:FF"};
            uint16_t m_nTargetPortNumber{0xFFFF};

            std::function<bool(const ptpManagement&)> m_pSender;


            asio::io_context m_context;
            std::shared_ptr<asio::local::datagram_protocol::socket> m_pSocket = nullptr;
            uint16_t m_nSequence{0};

            static const std::string kTargetIdAll;
            static const uint16_t kTargetNumberAll;
    };
}