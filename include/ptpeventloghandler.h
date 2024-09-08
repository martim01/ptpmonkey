#pragma once
#include "ptpeventhander.h"

namespace pml
{
    namespace ptpmonkey
    {
        
        /** @class A simple class that handles PTP events and outputs details to cout
        **/
        class PtpEventLogHandler : public PtpEventHandler
        {

            public:
                PtpEventLogHandler(bool bVerbose = false) : m_bVerbose(bVerbose){}
                void ClockAdded(std::shared_ptr<PtpV2Clock> pClock) override;
                void ClockUpdated(std::shared_ptr<PtpV2Clock> pClock) override;
                void ClockRemoved(std::shared_ptr<PtpV2Clock> pClock) override;
                void ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock) override;
                void ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock) override;
                void ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock) override;

                void AnnounceSent(std::shared_ptr<PtpV2Clock> pClock) override;
                void SyncSent(std::shared_ptr<PtpV2Clock> pClock) override;
                void FollowUpSent(std::shared_ptr<PtpV2Clock> pClock) override;
                void DelayRequestSent(std::shared_ptr<PtpV2Clock> pClock) override;
                void DelayResponseSent(std::shared_ptr<PtpV2Clock> pClock) override;

                void ManagementMessageReceived(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpManagement> pMessage) override;
            private:
                bool m_bVerbose;
        };
    }
}