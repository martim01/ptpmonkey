#include "ptpsync.h"
#include "ptpmonkey.h"
#include "log.h"
#include "ptpclock.h"
#include <sys/timex.h>
#include <sys/time.h>
#include "synceventhandler.h"
#include <cstring>
#include <fstream>

using namespace pml::ptpmonkey;

Sync::Sync()=default;

Sync::~Sync()=default;

void Sync::ClockRemoved(std::shared_ptr<PtpV2Clock>)
{
}

void Sync::ClockTimeCalculated(std::shared_ptr<PtpV2Clock>)
{
    TrySyncToPtp();
}

void Sync::ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock)
{
    SaveDetails();
}

void Sync::ClockBecomeSlave(std::shared_ptr<PtpV2Clock>  pClock)
{
    
}

void Sync::SaveGrandmasterDetails(std::shared_ptr<PtpV2Clock>  pClock)
{
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "SaveGrandmaster " << pClock->GetId();
    std::ofstream ofs;
    ofs.open("/var/run/ptpmonkey");
    if(ofs.is_open())
    {
        ofs << pClock->GetId() << std::endl;
        ofs << pClock->GetDomain() << std::endl;
        ofs.close();
    }
}

void Sync::SaveDetails() const
{
    std::ofstream ofs;
    ofs.open("/var/run/ptpmonkey");
    if(ofs.is_open())
    {
        ofs << "{ ";
        ofs << "\"Grandmaster\": {" ;
        if(auto pMaster = m_pMonkey->GetSyncMasterClock(); pMaster)
        {
            ofs << "\"Id\": \""        << pMaster->GetId() << "\", " ;
            ofs << "\"Ip\": \""        << pMaster->GetIpAddress() << "\", ";
            ofs << "\"Accuracy\": "    << +pMaster->GetAccuracy() << ", ";
            ofs << "\"Variance\": "    << pMaster->GetVariance() << ", ";
            ofs << "\"Class\": "       << +pMaster->GetClass() << ", ";
            ofs << "\"Priority1\": "   << +pMaster->GetPriority1() << ", ";
            ofs << "\"Priority2\": "   << +pMaster->GetPriority2() << ", ";
            ofs << "\"Source\": "      << +pMaster->GetTimeSource() << ", ";
            ofs << "\"Steps\": "       << +pMaster->GetStepsRemoved() << ", ";
            ofs << "\"Utc_Offset\": "  << +pMaster->GetUtcOffset() << ", ";


            auto flags = pMaster->GetFlags(ptpV2Header::ANNOUNCE);
            ofs << "\"Alternate_Master\": " << ((flags & ptpV2Header::enumFlags::ALTERNATE_MASTER) != 0) << ", ";
            ofs << "\"Two_Step\": " << ((flags & ptpV2Header::enumFlags::TWO_STEP) != 0) << ", ";
            ofs << "\"Unicast\": " << ((flags & ptpV2Header::enumFlags::UNICAST) != 0) << ", ";
            ofs << "\"Profile1\": " << ((flags & ptpV2Header::enumFlags::PROFILE1) != 0) << ", ";
            ofs << "\"Profile2\": " << ((flags & ptpV2Header::enumFlags::PROFILE2) != 0) << ", ";
            ofs << "\"LI_61\": " << ((flags & ptpV2Header::enumFlags::LI_61) != 0) << ", ";
            ofs << "\"LI_59\": " << ((flags & ptpV2Header::enumFlags::LI_59) != 0) << ", ";
            ofs << "\"UTC_Valid\": " << ((flags & ptpV2Header::enumFlags::UTC_OFFSET_VALID) != 0) << ", ";
            ofs << "\"Timescale\": " << ((flags & ptpV2Header::enumFlags::TIMESCALE) != 0) << ", ";
            ofs << "\"Time_Traceable\": " << ((flags & ptpV2Header::enumFlags::TIME_TRACEABLE) != 0) << ", ";
            ofs << "\"Freq_Traceable\": " << ((flags & ptpV2Header::enumFlags::FREQ_TRACEABLE) != 0);
            
        }
        ofs << " }, ";


        ofs << "\"Offset\": { ";
        if(m_pMonkey->GetLocalClock())
        {
            ofs << "\"current\": "      << m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::CURRENT).count() << ", ";
            ofs << "\"mean\": "         << m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::MEAN).count() << ", ";
            ofs << "\"sd\": "           << m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::SD).count() << ", ";
            ofs << "\"slope\": "        << m_pMonkey->GetLocalClock()->GetOffsetSlope() << ", ";
            ofs << "\"intersection\": " << m_pMonkey->GetLocalClock()->GetOffsetIntersection();
        }
        ofs << " }, "; 

        ofs << "\"Delay\": { ";
        if(m_pMonkey->GetLocalClock())
        {
            ofs << "\"current\": "      << m_pMonkey->GetLocalClock()->GetDelay(PtpV2Clock::CURRENT).count() << ", ";
            ofs << "\"mean\": "         << m_pMonkey->GetLocalClock()->GetDelay(PtpV2Clock::MEAN).count() << ", ";
            ofs << "\"sd\": "           << m_pMonkey->GetLocalClock()->GetDelay(PtpV2Clock::SD).count() << ", ";
            ofs << "\"slope\": "        << m_pMonkey->GetLocalClock()->GetDelaySlope() << ", ";
            ofs << "\"intersection\": " << m_pMonkey->GetLocalClock()->GetDelayIntersection();
        }
        ofs << " }, ";
            

        ofs << "\"Received\": { ";
        if(m_pMonkey->GetLocalClock())
        {
            ofs << "\"Announce\": "   << m_pMonkey->GetLocalClock()->GetCount(ptpV2Header::enumType::ANNOUNCE) << ", ";
            ofs << "\"Sync\": "       << m_pMonkey->GetLocalClock()->GetCount(ptpV2Header::enumType::SYNC) << ", ";
            ofs << "\"DelayReq\": "   << m_pMonkey->GetLocalClock()->GetCount(ptpV2Header::enumType::DELAY_REQ) << ", ";
            ofs << "\"DelayResp\": "  << m_pMonkey->GetLocalClock()->GetCount(ptpV2Header::enumType::DELAY_RESP) << ", ";
            ofs << "\"FollowUp\": "   << m_pMonkey->GetLocalClock()->GetCount(ptpV2Header::enumType::FOLLOW_UP) << ", ";
            ofs << "\"Signalling\": " << m_pMonkey->GetLocalClock()->GetCount(ptpV2Header::enumType::SIGNALLING) << ", ";
            ofs << "\"Management\": " << m_pMonkey->GetLocalClock()->GetCount(ptpV2Header::enumType::MANAGEMENT);
        }
        ofs << " }, ";
        ofs << "\"Interval\": { ";
        if(m_pMonkey->GetLocalClock())
        {
            ofs << "\"Announce\": "   << +m_pMonkey->GetLocalClock()->GetInterval(ptpV2Header::enumType::ANNOUNCE) << ", ";
            ofs << "\"Sync\": "       << +m_pMonkey->GetLocalClock()->GetInterval(ptpV2Header::enumType::SYNC) << ", ";
            ofs << "\"DelayReq\": "   << +m_pMonkey->GetLocalClock()->GetInterval(ptpV2Header::enumType::DELAY_REQ) << ", ";
            ofs << "\"DelayResp\": "  << +m_pMonkey->GetLocalClock()->GetInterval(ptpV2Header::enumType::DELAY_RESP) << ", ";
            ofs << "\"FollowUp\": "   << +m_pMonkey->GetLocalClock()->GetInterval(ptpV2Header::enumType::FOLLOW_UP) << ", ";
            ofs << "\"Signalling\": " << +m_pMonkey->GetLocalClock()->GetInterval(ptpV2Header::enumType::SIGNALLING) << ", ";
            ofs << "\"Management\": " << +m_pMonkey->GetLocalClock()->GetInterval(ptpV2Header::enumType::MANAGEMENT);
        }

        ofs << " } }" << std::endl;
    }

}

bool Sync::Run(const IpInterface& interface, unsigned char nDomain, Mode mode)
{
    if(m_pMonkey == nullptr || interface.Get() != m_interface.Get() || mode != m_pMonkey->GetMode())
    {
        m_interface = interface;

        m_pMonkey = std::make_unique<PtpMonkey>(m_interface, nDomain, 2, mode, Rate::EVERY_1_SEC);
        m_pMonkey->AddEventHandler(std::make_shared<SyncEventHandler>(this));
        m_pMonkey->Run(false);
    }
    else if(nDomain != m_pMonkey->GetDomain())
    {
        m_pMonkey->SetDomain(nDomain);
    }

    TrySyncToPtp();

    return true;
}

bool Sync::Run(const IpAddress& address, unsigned char nDomain, Mode mode)
{
    if(m_pMonkey == nullptr || address.Get() != m_address.Get() || mode != m_pMonkey->GetMode())
    {
        m_address = address;

        m_pMonkey = std::make_unique<PtpMonkey>(m_address, nDomain, 2, mode, Rate::EVERY_1_SEC);
        m_pMonkey->AddEventHandler(std::make_shared<SyncEventHandler>(this));
        m_pMonkey->Run(false);
    }
    else if(nDomain != m_pMonkey->GetDomain())
    {
        m_pMonkey->SetDomain(nDomain);
    }

    TrySyncToPtp();

    return true;
}



std::optional<long> Sync::SetGetFrequency(std::optional<long> setFreq)
{
    timex buf;
    memset(&buf, 0,sizeof(buf));
    if(setFreq)
    {
        buf.freq = (*setFreq);
        buf.modes = ADJ_FREQUENCY;
        pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Set frequency to: " << (*setFreq);
    }

    if(adjtimex(&buf) == -1)
    {
        pmlLog(pml::LOG_ERROR, "pml::ptpmonkey") << "Failed to read/write frequency " <<strerror(errno);
        return {};
    }
    return buf.freq;
}

bool Sync::TrySyncToPtp()
{
    m_nPtpSamples++;

    if(auto pLocal = m_pMonkey->GetLocalClock(); pLocal)
    {
        SaveDetails();

        pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Try to sync";

        auto pMaster = m_pMonkey->GetSyncMasterClock();
        auto offset = pLocal->GetOffset(PtpV2Clock::CURRENT);
        auto slope = pLocal->GetOffsetSlope()*1e6;   //slope in ppm

        auto utc = std::chrono::nanoseconds(0);
        if(m_bUseTai)
        {
            utc = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(pMaster->GetUtcOffset()));
            offset += utc;
        }

        if(HardCrash(offset))
        {
            return true;
        }

        if(!m_bPtpLock && m_nPtpSamples < m_nMinSamplSize)
        {   //waiting for more info
            pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Try to sync - need more samples";
            return true;
        }

        if(AdjustFrequency(slope))
        {
            return true;
        }
        
        return AdjustTime(offset, utc);
            
    }
    else
    {
        pmlLog(pml::LOG_WARN, "pml::ptpmonkey") << "Not synced to PTP master";
    }
    return false;
}

bool Sync::HardCrash(const std::chrono::nanoseconds& offset)
{
    if(m_nPtpSamples > 1 && std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(offset).count()) > 5000)
    {
        auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
        auto hardSetM = now-offset;
        auto [sec, nano] = Split(hardSetM);

        timespec tv;
        tv.tv_sec = sec.count();
        tv.tv_nsec = nano.count();

        if(clock_settime(CLOCK_REALTIME, &tv) != 0)
        {
            pmlLog(pml::LOG_ERROR, "pml::ptpmonkey") << "Failed to hard crash " <<strerror(errno);
        }
        else
        {
            pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Hard crashed to " << TimeToIsoString(hardSetM);
            clock_gettime(CLOCK_REALTIME, &tv);
            pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Current time now: " << ctime(&tv.tv_sec);
        }
        m_pMonkey->ResetLocalClockStats();

        m_nPtpSamples = 0;
        m_bPtpLock = false;
        return true;
    }
    return false;
}

bool Sync::AdjustFrequency(double slope)
{
    if(false && !m_bPtpLock && (slope > 0.2 || slope < -0.2))
    {
        auto origin = m_pMonkey->GetLocalClock()->GetOffsetIntersection();

        auto setFreq = SetGetFrequency({});
        if(!setFreq)
        {
            return false;
        }

        double dOffsetFreq = slope*65535.0;
        auto nFrequency = static_cast<long>((*setFreq)-dOffsetFreq); //this is the frequency to keep the clock in sync

        //auto nOffsetPPM = std::chrono::duration_cast<std::chrono::microseconds>(offset).count();
        //auto nNewFreq = m_nFrequency - nOffsetPPM;

        m_pMonkey->ResetLocalClockStats();
        m_nPtpSamples = 0;

        SetGetFrequency(nFrequency);

        pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "mean " << m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::MEAN).count() 
                << "ns\tsd " << m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::SD).count() 
                << "ns\tslope " << m_pMonkey->GetLocalClock()->GetOffsetSlope()
                << "ppm\tintersection " << m_pMonkey->GetLocalClock()->GetOffsetIntersection();

        return true;
    }
    
    return false;
}

bool Sync::AdjustTime(std::chrono::nanoseconds offset, const std::chrono::nanoseconds& utc)
{
    m_nPtpSamples = 0;

    m_bPtpLock = true;

    auto mean = m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::MEAN);

    auto sd = m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::SD);
    auto maxBand = mean+sd+utc;
    auto minBand = mean-sd+utc;

    offset = std::max(minBand, std::min(maxBand, offset));

    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "mean " << std::chrono::duration_cast<std::chrono::microseconds>(mean).count() 
             << "us\tsd " << std::chrono::duration_cast<std::chrono::microseconds>(sd).count() 
             << "us\tmax " << std::chrono::duration_cast<std::chrono::microseconds>(maxBand).count() 
             << "us\tmin " << std::chrono::duration_cast<std::chrono::microseconds>(minBand).count()
             << "us\toffset " << std::chrono::duration_cast<std::chrono::microseconds>(offset).count()
             << "\tslope " << m_pMonkey->GetLocalClock()->GetOffsetSlope()
             << "us\tintersection " << m_pMonkey->GetLocalClock()->GetOffsetIntersection();

    auto [sec, nsec] = Split(-offset);
    timeval tv;
    tv.tv_sec = sec.count();
    tv.tv_usec = nsec.count()/1000;
    while(tv.tv_usec < 0)
    {
        tv.tv_sec -= 1;
        tv.tv_usec += 1000000;
    }

    timeval tvOld;
    adjtime(nullptr, &tvOld);


    if(adjtime(&tv, nullptr) != 0)
    {
        pmlLog(pml::LOG_ERROR, "pml::ptpmonkey") << "Could not set time: " <<strerror(errno);
        return false;
    }
    
    return true;
}
