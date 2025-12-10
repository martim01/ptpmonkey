#include "ptpsync.h"
#include "ptpmonkey.h"
#include "log.h"
#include "ptpclock.h"
#include <sys/timex.h>
#include <sys/time.h>
#include "synceventhandler.h"
#include <cstring>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>

#define CLOCKFD 3
#define FD_TO_CLOCKID(fd)	((clockid_t) ((((unsigned int) ~fd) << 3) | CLOCKFD))
#define CLOCKID_TO_FD(clk)	((unsigned int) ~((clk) >> 3))

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
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "SaveGrandmaster " << pClock->GetId();
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
            //ofs << "\"Accuracy\": "    << +pMaster->GetAccuracy() << ", ";
            ofs << "\"Variance\": "    << pMaster->GetVariance() << ", ";
            ofs << "\"Class\": "       << +pMaster->GetClass() << ", ";
            ofs << "\"Priority1\": "   << +pMaster->GetPriority1() << ", ";
            ofs << "\"Priority2\": "   << +pMaster->GetPriority2() << ", ";
            //ofs << "\"Source\": "      << +pMaster->GetTimeSource() << ", ";
            ofs << "\"Steps\": "       << +pMaster->GetStepsRemoved() << ", ";
            ofs << "\"Utc_Offset\": "  << +pMaster->GetUtcOffset() << ", ";


            auto flags = pMaster->GetFlags(hdr::enumType::kSync);
	        ofs << "\"Flags\": " << flags << ", ";
            ofs << "\"Alternate_Master\": " << ((flags & static_cast<unsigned short>(hdr::enumFlags::kAlternateMaster)) != 0) << ", ";
            ofs << "\"Two_Step\": " <<         ((flags & static_cast<unsigned short>(hdr::enumFlags::kTwoStep)) != 0) << ", ";
            ofs << "\"Unicast\": " <<          ((flags & static_cast<unsigned short>(hdr::enumFlags::kUnicast)) != 0) << ", ";
            ofs << "\"Profile1\": " <<         ((flags & static_cast<unsigned short>(hdr::enumFlags::kProfile1)) != 0) << ", ";
            ofs << "\"Profile2\": " <<         ((flags & static_cast<unsigned short>(hdr::enumFlags::kProfile2)) != 0) << ", ";
            ofs << "\"LI_61\": " <<            ((flags & static_cast<unsigned short>(hdr::enumFlags::kLI_61)) != 0) << ", ";
            ofs << "\"LI_59\": " <<            ((flags & static_cast<unsigned short>(hdr::enumFlags::kLI_59)) != 0) << ", ";
            ofs << "\"UTC_Valid\": " <<        ((flags & static_cast<unsigned short>(hdr::enumFlags::kUtcOffsetValid)) != 0) << ", ";
            ofs << "\"Timescale\": " <<        ((flags & static_cast<unsigned short>(hdr::enumFlags::kTimescale)) != 0) << ", ";
            ofs << "\"Time_Traceable\": " <<   ((flags & static_cast<unsigned short>(hdr::enumFlags::kTimeTraceable)) != 0) << ", ";
            ofs << "\"Freq_Traceable\": " <<   ((flags & static_cast<unsigned short>(hdr::enumFlags::kFreqTraceable)) != 0);
            
        }
        ofs << " }, ";


        ofs << "\"Offset\": { ";
        if(m_pMonkey->GetLocalClock())
        {
            ofs << "\"current\": "      << m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::enumCalc::CURRENT).count() << ", ";
            ofs << "\"mean\": "         << m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::enumCalc::MEAN).count() << ", ";
            ofs << "\"sd\": "           << m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::enumCalc::SD).count() << ", ";
            ofs << "\"slope\": "        << m_pMonkey->GetLocalClock()->GetOffsetSlope() << ", ";
            ofs << "\"intersection\": " << m_pMonkey->GetLocalClock()->GetOffsetIntersection();
        }
        ofs << " }, "; 

        ofs << "\"Delay\": { ";
        if(m_pMonkey->GetLocalClock())
        {
            ofs << "\"current\": "      << m_pMonkey->GetLocalClock()->GetDelay(PtpV2Clock::enumCalc::CURRENT).count() << ", ";
            ofs << "\"mean\": "         << m_pMonkey->GetLocalClock()->GetDelay(PtpV2Clock::enumCalc::MEAN).count() << ", ";
            ofs << "\"sd\": "           << m_pMonkey->GetLocalClock()->GetDelay(PtpV2Clock::enumCalc::SD).count() << ", ";
            ofs << "\"slope\": "        << m_pMonkey->GetLocalClock()->GetDelaySlope() << ", ";
            ofs << "\"intersection\": " << m_pMonkey->GetLocalClock()->GetDelayIntersection();
        }
        ofs << " }, ";
            

        ofs << "\"Received\": { ";
        if(m_pMonkey->GetLocalClock())
        {
            ofs << "\"Announce\": "   << m_pMonkey->GetLocalClock()->GetCount(hdr::enumType::kAnnounce) << ", ";
            ofs << "\"Sync\": "       << m_pMonkey->GetLocalClock()->GetCount(hdr::enumType::kSync) << ", ";
            ofs << "\"DelayReq\": "   << m_pMonkey->GetLocalClock()->GetCount(hdr::enumType::kDelayReq) << ", ";
            ofs << "\"DelayResp\": "  << m_pMonkey->GetLocalClock()->GetCount(hdr::enumType::kDelayResp) << ", ";
            ofs << "\"FollowUp\": "   << m_pMonkey->GetLocalClock()->GetCount(hdr::enumType::kFollowUp) << ", ";
            ofs << "\"Signalling\": " << m_pMonkey->GetLocalClock()->GetCount(hdr::enumType::kSignalling) << ", ";
            ofs << "\"Management\": " << m_pMonkey->GetLocalClock()->GetCount(hdr::enumType::kManagement);
        }
        ofs << " }, ";
        ofs << "\"Interval\": { ";
        if(m_pMonkey->GetLocalClock())
        {
            ofs << "\"Announce\": "   << +m_pMonkey->GetLocalClock()->GetInterval(hdr::enumType::kAnnounce) << ", ";
            ofs << "\"Sync\": "       << +m_pMonkey->GetLocalClock()->GetInterval(hdr::enumType::kSync) << ", ";
            ofs << "\"DelayReq\": "   << +m_pMonkey->GetLocalClock()->GetInterval(hdr::enumType::kDelayReq) << ", ";
            ofs << "\"DelayResp\": "  << +m_pMonkey->GetLocalClock()->GetInterval(hdr::enumType::kDelayResp) << ", ";
            ofs << "\"FollowUp\": "   << +m_pMonkey->GetLocalClock()->GetInterval(hdr::enumType::kFollowUp) << ", ";
            ofs << "\"Signalling\": " << +m_pMonkey->GetLocalClock()->GetInterval(hdr::enumType::kSignalling) << ", ";
            ofs << "\"Management\": " << +m_pMonkey->GetLocalClock()->GetInterval(hdr::enumType::kManagement);
        }

        ofs << " } }" << std::endl;
    }

}

bool Sync::Run(const IpInterface& interface, unsigned char nDomain, Mode mode, int nHwC)
{
    if(m_pMonkey == nullptr || interface.Get() != m_interface.Get() || mode != m_pMonkey->GetMode())
    {
        m_interface = interface;
        
        m_pMonkey = std::make_unique<PtpMonkey>(m_interface, nDomain, 2, mode, Rate::kEvery1Sec);
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

bool Sync::Run(const IpAddress& address, unsigned char nDomain, Mode mode, int nHwC)
{
    if(m_pMonkey == nullptr || address.Get() != m_address.Get() || mode != m_pMonkey->GetMode())
    {
        m_address = address;
        
        m_pMonkey = std::make_unique<PtpMonkey>(m_address, nDomain, 2, mode, Rate::kEvery1Sec);
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



std::optional<long> Sync::SetGetFrequency(std::optional<long> setFreq, int nClockId)
{
    timex buf;
    memset(&buf, 0,sizeof(buf));
    if(setFreq)
    {
        buf.freq = (*setFreq);
        buf.modes = ADJ_FREQUENCY;
        pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Set frequency to: " << (*setFreq);
    }

    if(clock_adjtime(nClockId, &buf) == -1)
    {
        pml::log::log(pml::log::Level::kError, "pml::ptpmonkey") << "Failed to read/write frequency " <<strerror(errno);
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

        pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Try to sync";

        auto pMaster = m_pMonkey->GetSyncMasterClock();
        auto offset = pLocal->GetOffset(PtpV2Clock::CURRENT);
        auto slope = pLocal->GetOffsetSlope()*1e6;   //slope in ppm

        auto utc = std::chrono::nanoseconds(0);
        if(m_bUseTai)
        {
            utc = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(pMaster->GetUtcOffset()));
            offset += utc;
        }

        if(HardCrash(offset, m_nHwC))
        {
            return true;
        }

        if(!m_bPtpLock && m_nPtpSamples < m_nMinSamplSize)
        {   //waiting for more info
            pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Try to sync - need more samples";
            return true;
        }

        if(AdjustFrequency(slope, m_nHwC))
        {
            return true;
        }
        
        return AdjustTime(offset, utc, m_nHwC);
            
    }
    else
    {
        pml::log::log(pml::log::Level::kWarning, "pml::ptpmonkey") << "Not synced to PTP master";
    }
    return false;
}

bool Sync::HardCrash(const std::chrono::nanoseconds& offset, int nClockId)
{
    if(m_nPtpSamples > 1 && std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(offset).count()) > 500)
    {
        auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
        auto hardSetM = now-offset;
        auto [sec, nano] = Split(hardSetM);

        timespec tv;
        tv.tv_sec = sec.count();
        tv.tv_nsec = nano.count();

        if(clock_settime(nClockId, &tv) != 0)
        {
            pml::log::log(pml::log::Level::kError, "pml::ptpmonkey") << "Clock: " << nClockId << ". Failed to hard crash " <<strerror(errno);
        }
        else
        {
            pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Clock: " << nClockId << ". Hard crashed to " << TimeToIsoString(hardSetM);
            clock_gettime(nClockId, &tv);
            pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Clock: " << nClockId << ". Current time now: " << ctime(&tv.tv_sec);
        }
        m_pMonkey->ResetLocalClockStats();

        m_nPtpSamples = 0;
        m_bPtpLock = false;
        return true;
    }
    return false;
}

bool Sync::AdjustFrequency(double slope, int nClockId)
{
    if(!m_bPtpLock && (slope > 0.2 || slope < -0.2))
    {
        auto origin = m_pMonkey->GetLocalClock()->GetOffsetIntersection();

        auto setFreq = SetGetFrequency({}, nClockId);
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

        SetGetFrequency(nFrequency, nClockId);

        pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "mean " << m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::MEAN).count() 
                << "ns\tsd " << m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::SD).count() 
                << "ns\tslope " << m_pMonkey->GetLocalClock()->GetOffsetSlope()
                << "ppm\tintersection " << m_pMonkey->GetLocalClock()->GetOffsetIntersection();

        return true;
    }
    
    return false;
}

bool Sync::AdjustTime(std::chrono::nanoseconds offset, const std::chrono::nanoseconds& utc, int nClockId)
{
    m_nPtpSamples = 0;

    m_bPtpLock = true;

    auto mean = m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::MEAN);

    auto sd = m_pMonkey->GetLocalClock()->GetOffset(PtpV2Clock::SD);
    auto maxBand = mean+sd+utc;
    auto minBand = mean-sd+utc;

    offset = std::max(minBand, std::min(maxBand, offset));

    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "mean " << std::chrono::duration_cast<std::chrono::microseconds>(mean).count() 
             << "us\tsd " << std::chrono::duration_cast<std::chrono::microseconds>(sd).count() 
             << "us\tmax " << std::chrono::duration_cast<std::chrono::microseconds>(maxBand).count() 
             << "us\tmin " << std::chrono::duration_cast<std::chrono::microseconds>(minBand).count()
             << "us\toffset " << std::chrono::duration_cast<std::chrono::microseconds>(offset).count()
             << "\tslope " << m_pMonkey->GetLocalClock()->GetOffsetSlope()
             << "us\tintersection " << m_pMonkey->GetLocalClock()->GetOffsetIntersection();

    auto [sec, nsec] = Split(-offset);
    
    timex tx{};
	
    tx.modes = ADJ_SETOFFSET | ADJ_NANO;
	tx.time.tv_sec  = sec.count();
	tx.time.tv_usec = nsec.count();
	/*
	 * The value of a timeval is the sum of its fields, but the
	 * field tv_usec must always be non-negative.
	 */
	if (tx.time.tv_usec < 0) 
    {
		tx.time.tv_sec  -= 1;
		tx.time.tv_usec += 1000000000;
	}

    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") <<  "Clock: " << nClockId << ". AdjustTime: " << tx.time.tv_sec << ":" << tx.time.tv_usec;

	if (clock_adjtime(nClockId, &tx) < 0) 
    {
        pml::log::log(pml::log::Level::kError, "pml::ptpmonkey") << "Clock: " << nClockId << ". Could not set time: " <<strerror(errno);
		
		return false;
	}
    
    return true;
}


void Sync::OpenHwClock(int nHwC)
{
    timespec ts{};
	timex tx{};
	
    std::string sPath = "/dev/ptp"+std::to_string(nHwC);

    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Get ClockId for " << sPath;

	auto fd = open(sPath.c_str(), O_RDWR);

	if (fd < 0)
	{
        pml::log::log(pml::log::Level::kError, "pml::ptpmonkey") << "Could not open " << sPath;
        m_nHwC = CLOCK_REALTIME;
    }
    else
    {
	    m_nHwC = FD_TO_CLOCKID(fd);
        /* check if clkid is valid */
        if (clock_gettime(m_nHwC, &ts)) 
        {
            pml::log::log(pml::log::Level::kError, "pml::ptpmonkey") << "Could not get time from " << sPath;
            close(fd);
            m_nHwC = CLOCK_REALTIME;
 
        }
	    else if (clock_adjtime(m_nHwC, &tx)) 
        {
            pml::log::log(pml::log::Level::kError, "pml::ptpmonkey") << "Could not adjtime " << sPath;
		    close(fd);
            m_nHwC = CLOCK_REALTIME;
    	}
    }

    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "ClockId for " << sPath << " is " << m_nHwC;

}
