#pragma once
#include <memory>
class PtpV2Clock;

class PtpEventHandler
{
    public:
        virtual void ClockAdded(std::shared_ptr<PtpV2Clock> pClock)=0;
        virtual void ClockUpdated(std::shared_ptr<PtpV2Clock> pClock)=0;
        virtual void ClockRemoved(std::shared_ptr<PtpV2Clock> pClock)=0;
        virtual void ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock)=0;

        virtual void ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock)=0;
        virtual void ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock)=0;
};
