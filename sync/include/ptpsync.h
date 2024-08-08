#pragma once
#include <optional>
#include <string>
#include <memory>
#include "namedtypes.h"
#include "timeutils.h"
#include "ptpmonkey.h"

namespace pml
{
    namespace ptpmonkey
    {
        class PtpV2Clock;

        class Sync
        {
            public:
                Sync();
                ~Sync();

                bool Run(const IpInterface& interface, unsigned char nDomain, Mode mode, int nHwC=-1);
                bool Run(const IpAddress& address, unsigned char nDomain, Mode mode, int nHwC=-1);

                bool IsPtpFrequencyLocked() const {return m_bPtpLock;}


                void ClockRemoved(std::shared_ptr<PtpV2Clock> pClock);
                void ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock);
                void ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock);
                void ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock);
        
            private:

                void SaveDetails() const;
                void SaveGrandmasterDetails(std::shared_ptr<PtpV2Clock>  pClock);

                bool TrySyncToPtp();
                

                bool HardCrash(const std::chrono::nanoseconds& offset, int nClockId);
                bool AdjustFrequency(double slope, int nClockId);
                bool AdjustTime(std::chrono::nanoseconds offset, const std::chrono::nanoseconds& utc, int nClockId);

                std::optional<long> SetGetFrequency(std::optional<long> setFreq, int nClockId);

                void OpenHwClock(int nHwC);

                size_t m_nPtpDomain = 0;
                bool m_bUseTai = false;
                unsigned long m_nSyncCount = 0;
                bool m_bPtpLock = false;
                size_t m_nPtpSamples = 0;
                size_t m_nMinSamplSize = 10;

                int m_nHwC = -1;
                long m_nFrequency;

                IpInterface m_interface;
                IpAddress  m_address;
                std::unique_ptr<PtpMonkey> m_pMonkey = nullptr;
        };
    }
}
