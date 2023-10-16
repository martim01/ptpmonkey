#pragma once
#include "ptpeventhander.h"
#include <set>

namespace pml
{
    namespace ptpmonkey
    {
        class PtpV2Clock;
        class Sync;
    
        class SyncEventHandler : public PtpEventHandler
        {
            public:
                explicit SyncEventHandler(Sync* pSync) : m_pSync(pSync){}
                ~SyncEventHandler() final=default;

                void ClockAdded(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;
                void ClockUpdated(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;
                void ClockRemoved(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;
                void ClockTimeCalculated(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;
                void ClockBecomeMaster(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;
                void ClockBecomeSlave(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;

                void AnnounceSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;
                void SyncSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;
                void FollowUpSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;
                void DelayRequestSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;
                void DelayResponseSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock) final;

            private:
                Sync* m_pSync;
        };
    }
}




