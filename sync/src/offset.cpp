#include "offset.h"
#include "linearregression.h"
#include "log.h"
#include <sys/timex.h>
#include <cstring>
#include <cmath>
#include <algorithm>
#include "ltcutils.h"
#include "vamutils.h"

using namespace pml::vam;
const std::array<std::string,6> Offset::STR_ACTION = {"WAITING", "ADJUSTING CLOCK", "ADJUSTING FREQUENCY", "CRASHED CLOCK", "SLEWING", "ERROR"};

Offset::Offset()=default;

Offset::enumAction Offset::Add(std::chrono::nanoseconds offset)
{
    if(m_lstFrame.size() == 500)
    {
        WorkoutLR();
        ClearData();
        m_nFrame = 0;

    }
    
    m_lstOffset.push_back(static_cast<double>(offset.count())/1e6);
    m_lstFrame.push_back(static_cast<double>(m_lstFrame.size()));
    return m_eAction;
}

void Offset::ClearData()
{
    m_lstOffset.clear();
    m_lstFrame.clear();
}

void Offset::WorkoutLR()
{
    auto [alpha, beta] = GetSlopeAndIntercept(m_lstFrame, m_lstOffset);
    m_alpha = alpha*1e6; //ppm
    m_beta= beta*m_dFPS;
   
    if(m_beta > -0.8 && m_beta < 0.8)
    {
        auto av = -GetAverage();
        if(av < -0.5 || av > 0.5)
        {
            CrashTime(av);
            ClearData();
            m_nFrame = 0;
            m_bSlewing = true;
            m_eAction = enumAction::CRASHED;
        }
        else
        {
            double dSec;
            double dMicro = modf(-av, &dSec);
            dMicro*=1e6;
            timeval tv;
            tv.tv_sec = static_cast<int>(dSec);
            tv.tv_usec = static_cast<int>(dMicro);
            if(adjtime(&tv, &m_tvAdjusting) != 0)
            {
                pml::log::log(pml::log::Level::kError, "pml::vam::ltc") << "Failed to adjtime " <<strerror(errno);
                m_eAction = enumAction::ERROR;
                return;
            }
            else
            {
                pml::log::log(pml::log::Level::kDebug, "pml::vam::ltc") << "Time adjusted by " << tv.tv_sec << "s and " << tv.tv_usec << "us" << "\tLeft " << m_tvAdjusting.tv_sec << "s and " << m_tvAdjusting.tv_usec << "us";
            }
            m_bSlewing = true;
            m_eAction = enumAction::CLOCK_ADJ;
        }
    }
    else if(!m_bSlewing)
    {
        //change the frequency to get the slope to 0
        timex buf;
        memset(&buf, 0,sizeof(buf));
        if(adjtimex(&buf) == -1)
        {
            pml::log::log(pml::log::Level::kError, "pml::vam::ltc") << "Failed to read frequency " <<strerror(errno);
            m_eAction = enumAction::ERROR;
            return;
        }
 
        double dOffsetFreq = m_beta*65535.0;
        buf.freq += static_cast<long>(dOffsetFreq);
        buf.modes = ADJ_FREQUENCY;

        m_nFreqAdjust = buf.freq;

        if(adjtimex(&buf) == -1)
        {
            pml::log::log(pml::log::Level::kError, "pml::vam::ltc") << "Failed to set frequency " <<strerror(errno);
            m_eAction = enumAction::ERROR;
            return;
        }
        m_eAction = enumAction::FREQ_ADJ;
    }
    else
    {
        pml::log::log(pml::log::Level::kDebug, "pml::vam::ltc") << "Slewing - ignore this data set";

        if(adjtime(nullptr, &m_tvAdjusting) != 0)
        {
            pml::log::log(pml::log::Level::kError, "pml::vam::ltc") << "Failed to read offset " <<strerror(errno);
            m_eAction = enumAction::ERROR;
            return;
        }
        if(m_tvAdjusting.tv_sec == 0 && m_tvAdjusting.tv_usec == 0)
        {
            m_bSlewing = false;
        }
        else
        {
            pml::log::log(pml::log::Level::kDebug, "pml::vam::ltc") << "Still adjusting "  << m_tvAdjusting.tv_sec << "s and " << m_tvAdjusting.tv_usec << "us";
        }
        m_eAction = enumAction::SLEWING;
    }

    m_bSynced = (m_alpha > -1000.0 && m_alpha < 1000.0 && m_beta > -1.0 && m_beta < 1.0);
}

double Offset::GetAverage()
{
    return std::accumulate(m_lstOffset.begin(), m_lstOffset.end(),0.0)/static_cast<double>(m_lstOffset.size());
}


void Offset::CrashTime(double dOffset) const
{
    auto now = std::chrono::system_clock::now();
    now += DoubleToMicro(-dOffset);

    timespec ts;
    ts.tv_sec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    ts.tv_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() % 1000000000;


    if(clock_settime(CLOCK_REALTIME, &ts) != 0)
    {
        pml::log::log(pml::log::Level::kError, "pml::vam::ltc") << "Failed to hard crash " <<strerror(errno);
        return;
    }

    pml::log::log(pml::log::Level::kDebug, "pml::vam::ltc") << "Hard crashed to " << ConvertTimeToIsoString(std::chrono::system_clock::now());

}

std::chrono::microseconds Offset::GetAdjusting() const
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(m_tvAdjusting.tv_sec))+std::chrono::microseconds(m_tvAdjusting.tv_usec);
}
