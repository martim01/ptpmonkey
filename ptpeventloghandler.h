#pragma once
#include "ptpeventhander.h"

/** @class A simple class that handles PTP events and outputs details to cout
**/
class PtpEventLogHandler : public PtpEventHandler
{
    public:
        void ClockAdded(std::shared_ptr<PtpV2Clock> pClock) override;
        void ClockUpdated(std::shared_ptr<PtpV2Clock> pClock) override;
        void ClockRemoved(std::shared_ptr<PtpV2Clock> pClock) override;
        void ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock) override;
        void ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock) override;
        void ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock) override;
};

