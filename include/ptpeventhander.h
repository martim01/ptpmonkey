#pragma once
#include <memory>

namespace pml::ptpmonkey
{

    class PtpV2Clock;
    struct ptpManagement;
    struct ptpV2Header;

    /** @class Abstract class that handles PTP events sent to it from PtpMonkey
    **/
    class PtpEventHandler
    {
        public:
            virtual ~PtpEventHandler()=default;

            /** @brief Sent when a new clock is detected in the domain
            *   @param pClock a shared_ptr to the PtpV2Clock that defines the newly detected device
            **/
            virtual void ClockAdded(std::shared_ptr<PtpV2Clock> pClock)=0;

            /** @brief Sent when a possible master clock already detected in the domain changes some of its settings (usually by an announce message)
            *   @param pClock a shared_ptr to the PtpV2Clock
            **/
            virtual void ClockUpdated(std::shared_ptr<PtpV2Clock> pClock)=0;

            /** @brief Sent after a certain period of time when a clock in the domain stops sending messages
            *   @param pClock a shared_ptr to the PtpV2Clock at the state when it last sent a message
            **/
            virtual void ClockRemoved(std::shared_ptr<PtpV2Clock> pClock)=0;

            /** @brief Sent once the local machine has enough information to calculate the time of the Ptp Grand Master for the domain
            *   @param pClock a shared_ptr to the GrandMaster clock
            **/
            virtual void ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock)=0;

            /** @brief Sent when a clock becomes GrandMaster
            *   @param pClock a shared_ptr to the GrandMaster clock
            **/
            virtual void ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock)=0;

            /** @brief Sent when a clock stops being GrandMaster
            *   @param pClock a shared_ptr to the clock that was GrandMaster and no longer is
            **/
            virtual void ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock)=0;


            virtual void AnnounceSent(std::shared_ptr<PtpV2Clock> pClock)=0;
            virtual void SyncSent(std::shared_ptr<PtpV2Clock> pClock)=0;
            virtual void FollowUpSent(std::shared_ptr<PtpV2Clock> pClock)=0;
            virtual void DelayRequestSent(std::shared_ptr<PtpV2Clock> pClock)=0;
            virtual void DelayResponseSent(std::shared_ptr<PtpV2Clock> pClock)=0;

            virtual void ManagementMessageReceived(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpManagement> pMessage)=0;

    };
}
