#pragma once
#include <chrono>
#include <list>
#include <optional>
#include <array>
#include <string>

class Offset
{
    public:
        enum enumAction {WAITING, CLOCK_ADJ, FREQ_ADJ, CRASHED, SLEWING, ERROR};
        
        static const std::array<std::string,6> STR_ACTION;

        Offset();
        enumAction Add(std::chrono::nanoseconds offset);
        void ClearData();

        bool IsSynced() const { return m_bSynced;}

        double GetIntercept() const { return m_alpha;}
        double GetSlope() const { return m_beta;}
        std::chrono::microseconds GetAdjusting() const;
        unsigned long GetFrequencyAdjust() const { return m_nFreqAdjust;}
        size_t GetBufferSize() const { return m_lstOffset.size();}
    private:
        void WorkoutLR();
        double GetAverage();

        void CrashTime(double dOffset) const;

        std::list<double> m_lstOffset;
        std::list<double> m_lstFrame;

        bool m_bSlewing = false;
        bool m_bSynced = false;

        enumAction m_eAction = enumAction::WAITING;
        double m_alpha;
        double m_beta;

        timeval m_tvAdjusting;
        unsigned long m_nFreqAdjust;
};
