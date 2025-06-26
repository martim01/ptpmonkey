#include "ptpclock.h"
#include <iostream>
#include "log.h"
#include "linearregression.h"
#include <cmath>
using namespace pml::ptpmonkey;

PtpV2Clock::PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce) :
    m_nDomain(pHeader->nDomain),
    m_nUtcOffset(pAnnounce->nUtcOffset),
    m_nGrandmasterPriority1(pAnnounce->nGrandmasterPriority1),
    m_nGrandmasterClass(pAnnounce->nGrandmasterClass),
    m_eGrandmasterAccuracy(pAnnounce->eGrandmasterAccuracy),
    m_nGrandmasterVariance(pAnnounce->nGrandmasterVariance),
    m_nGrandmasterPriority2(pAnnounce->nGrandmasterPriority2),
    m_sClockId(pHeader->source.sSourceId),
    m_nStepsRemoved(pAnnounce->nStepsRemoved),
    m_eTimeSource(pAnnounce->eTimeSource),
    m_bGrandMaster(false),
    m_bSyncMaster(false),
    m_nSampleSize(10),
    m_sIpAddress(pHeader->sIpAddress),
    m_nt1s(0),
    m_nt1r(0),
    m_calculatedFirst(TIMEZERO),
    m_bT1Valid(false),
    m_lastMessageTime(pHeader->timestamp)
{
    m_mFlags[hdr::enumType::ANNOUNCE] = pHeader->nFlags;
    m_mInterval[hdr::enumType::ANNOUNCE] = pHeader->nInterval;
    m_mCount[hdr::enumType::ANNOUNCE].value++;

}

PtpV2Clock::PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload) :
    m_nDomain(pHeader->nDomain),
    m_nUtcOffset(0),
    m_nGrandmasterPriority1(0),
    m_nGrandmasterClass(0),
    m_eGrandmasterAccuracy(clck::enumAccuracy::ACC_NA),
    m_nGrandmasterVariance(0),
    m_nGrandmasterPriority2(0),
    m_sClockId(pHeader->source.sSourceId),
    m_nStepsRemoved(0),
    m_eTimeSource(clck::enumTimeSource::NA),
    m_bGrandMaster(false),
    m_bSyncMaster(false),
    m_nSampleSize(10),
    m_sIpAddress(pHeader->sIpAddress),
    m_nt1s(0),
    m_nt1r(0),
    m_calculatedFirst(TIMEZERO),
    m_bT1Valid(false),
    m_lastMessageTime(pHeader->timestamp)
{
    m_mFlags[static_cast<hdr::enumType>(pHeader->nType)] = pHeader->nFlags;
}

void PtpV2Clock::SyncFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_lastMessageTime = pHeader->timestamp;

    m_mInterval[hdr::enumType::SYNC] = pHeader->nInterval;
    m_mCount[hdr::enumType::SYNC].value++;

    m_mFlags[hdr::enumType::SYNC] = pHeader->nFlags;
    m_bSyncMaster = true;

}

void PtpV2Clock::SyncTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_bSyncMaster = false;
    m_mCount[hdr::enumType::SYNC].value++;
    if((pHeader->nFlags & static_cast<unsigned short>(hdr::enumFlags::TWO_STEP)) != 0)   //2-step
    {
        m_nt1r = TimeToNano(pHeader->timestamp);
        m_nFollowUpSequence = pHeader->nSequenceId;
        m_bT1Valid = false;
    }
    else
    {
        m_nt1s = TimeToNano(pPayload->originTime);
        m_nt1r = TimeToNano(pHeader->timestamp);
        m_nFollowUpSequence = 0;
        m_bT1Valid = true;
    }
}


void PtpV2Clock::FollowUpFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_lastMessageTime = pHeader->timestamp;
    m_mInterval[hdr::enumType::FOLLOW_UP] = pHeader->nInterval;
    m_mCount[hdr::enumType::FOLLOW_UP].value++;
    m_mFlags[hdr::enumType::FOLLOW_UP] = pHeader->nFlags;

    m_bSyncMaster = true;


}

void PtpV2Clock::FollowUpTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_bSyncMaster = false;
    m_nt1s = TimeToNano(pPayload->originTime);

    //check the follow up sequence is correct
    m_bT1Valid = (pHeader->nSequenceId == m_nFollowUpSequence);

    m_mCount[hdr::enumType::FOLLOW_UP].value++;

}


void PtpV2Clock::DelayRequest(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{

    m_lastMessageTime = pHeader->timestamp;
    m_mInterval[hdr::enumType::DELAY_REQ] = pHeader->nInterval;
    m_mCount[hdr::enumType::DELAY_REQ].value++;
    m_mFlags[hdr::enumType::DELAY_REQ] = pHeader->nFlags;


    // if we've already had a tx timestamp set for this message workout the os delay
    auto itRequest = m_mDelayRequest.find(pHeader->nSequenceId);
    if(itRequest != m_mDelayRequest.end())
    {
        pml::log::log(pml::log::Level::kWarning, "pml::ptpmonkey") << "PtpMonkey\tDelayRequest: Sequence Id repeated: " << std::dec << pHeader->nSequenceId;
    }
    else
    {
        m_mDelayRequest.insert(make_pair(pHeader->nSequenceId, pHeader->timestamp));
    }
}

void PtpV2Clock::DelayResponseFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload)
{
    m_lastMessageTime = pHeader->timestamp;
    m_mInterval[hdr::enumType::DELAY_RESP] = pHeader->nInterval;
    m_mCount[hdr::enumType::DELAY_RESP].value++;
    m_mFlags[hdr::enumType::DELAY_RESP] = pHeader->nFlags;
    m_bSyncMaster = true;

}

bool PtpV2Clock::DelayResponseTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload)
{
    m_mInterval[hdr::enumType::DELAY_RESP] = pHeader->nInterval;
    m_mCount[hdr::enumType::DELAY_RESP].value++;
    m_mFlags[hdr::enumType::DELAY_RESP] = pHeader->nFlags;

    bool bSyncChange(false);
    auto request = m_mDelayRequest.find(pHeader->nSequenceId);
    if(request != m_mDelayRequest.end())
    {
        if(m_bT1Valid)
        {
            long long int nt2s = TimeToNano(request->second);
            long long int nt2r = TimeToNano(pPayload->originTime);
        

            long long int nOffsetNano1 = m_nt1r-m_nt1s;
            long long int nOffsetNano2 = -nt2r+nt2s;
            long long int nOffsetNano = (nOffsetNano1+nOffsetNano2)/2;

            long long int nDelayNano1 = nOffsetNano1;
            long long int nDelayNano2 = nt2r-nt2s;

            long long int nDelayNano = (nDelayNano1+nDelayNano2)/2;

            m_calculatedAt = pHeader->timestamp;
            if(m_calculatedAt != TIMEZERO)
            {

                m_calculatedPtp = NanoToTime(nDelayNano)+pPayload->originTime;
                if(m_calculatedFirst == TIMEZERO)
                {
                    m_calculatedFirst = m_calculatedAt;
                }

                DoStats(nDelayNano, m_calculatedAt, m_delay);
                if(m_offset.lstValues.size() < m_nSampleSize || NanoToTime(nDelayNano) < m_delay.stat[MEAN]+m_delay.stat[SD])
                {
                    bSyncChange = DoStats(nOffsetNano, m_calculatedAt, m_offset);
                }
                else
                {   //delayed message just pretend its the mean offset here
                    bSyncChange = DoStats(TimeToNano(m_offset.stat[MEAN]), m_calculatedAt, m_offset);
                }
            }
        }
        m_mDelayRequest.erase(request);
    }
    return bSyncChange;
}

bool PtpV2Clock::DoStats(unsigned long long int nCurrent, std::chrono::nanoseconds calcAt, stats& theStats) const
{

    theStats.stat[CURRENT] = NanoToTime(nCurrent);

    theStats.dTotal = theStats.dTotal+TimeToDouble(theStats.stat[CURRENT]);

    theStats.lstValues.push_back(theStats.stat[CURRENT]);
    theStats.lstTimesLinReg.push_back(TimeToDouble(calcAt-m_calculatedFirst));
    theStats.lstValuesLinReg.push_back(TimeToDouble(theStats.stat[CURRENT]));


    if(theStats.lstValues.size() > STATS_HISTORY_SIZE)
    {
        theStats.dTotal -= TimeToDouble(theStats.lstValues.front());
        theStats.lstValues.pop_front();
        theStats.lstTimesLinReg.pop_front();
        theStats.lstValuesLinReg.pop_front();
    }
    theStats.stat[MEAN] = DoubleToTime(theStats.dTotal/static_cast<double>(theStats.lstValues.size()));

    if(theStats.lstValues.size() >= m_nSampleSize)   //got enough to create an offset
    {
        theStats.stat[MIN] = TIMEZERO;
        theStats.stat[MAX] = TIMEZERO;
        theStats.stat[SD] = TIMEZERO;


        double dTotal = 0.0;
        for(auto value : theStats.lstValues)
        {
            dTotal += TimeToDouble(value);
        }
        double dMean =dTotal / static_cast<double>(theStats.lstValues.size());

        double dVariance(0.0);
        for(auto value : theStats.lstValues)
        {
            if(theStats.stat[MIN] == TIMEZERO || theStats.stat[MIN] > value)
            {
                theStats.stat[MIN] = value;
            }
            if(theStats.stat[MAX] == TIMEZERO || theStats.stat[MAX] < value)
            {
                theStats.stat[MAX] = value;
            }
            double dVar = TimeToDouble(value)-dMean;

            dVariance += (dVar*dVar);

        }
        theStats.stat[MEAN] = DoubleToTime(dMean);

        theStats.stat[SD] = DoubleToTime(std::sqrt(dVariance/static_cast<double>(theStats.lstValues.size()-1)));
        theStats.stat[SE] = DoubleToTime(std::sqrt(dVariance/static_cast<double>(theStats.lstValues.size()-1))/std::sqrt(static_cast<double>(theStats.lstValues.size())));
        //std::cout << TimeToNano(theStats.stat[SD]) << std::endl;

        theStats.m_c = GetSlopeAndIntercept(theStats.lstTimesLinReg, theStats.lstValuesLinReg);

        theStats.bSet = true;
        return true;
    }
    else
    {
        return false;
    }
}

void PtpV2Clock::ResyncToMaster()
{
    m_offset.bSet = false;
    m_delay.bSet = false;
}

std::chrono::nanoseconds PtpV2Clock::GetPtpTime()  const
{
    return (Now()-m_calculatedAt)+m_calculatedPtp;//m_offset.stat[SET];

}

std::chrono::nanoseconds PtpV2Clock::GetOffset(enumCalc eCalc) const
{
    return m_offset.stat[eCalc];
}

std::chrono::nanoseconds PtpV2Clock::GetDelay(enumCalc eCalc) const
{
    return m_delay.stat[eCalc];
}

bool PtpV2Clock::UpdateAnnounce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce)
{
    m_lastMessageTime = pHeader->timestamp;
    m_mInterval[hdr::enumType::ANNOUNCE] = pHeader->nInterval;
    m_mCount[hdr::enumType::ANNOUNCE].value++;

    bool bChanged(false);
    if(m_nDomain != pHeader->nDomain)
    {
        m_nDomain = pHeader->nDomain;
        bChanged = true;
    }

    if(m_mFlags[hdr::enumType::ANNOUNCE] != pHeader->nFlags)
    {
        m_mFlags[hdr::enumType::ANNOUNCE] = pHeader->nFlags;
        bChanged = true;
    }

    if(m_nUtcOffset != pAnnounce->nUtcOffset)
    {
        m_nUtcOffset = pAnnounce->nUtcOffset;
        bChanged = true;
    }

    if(m_nGrandmasterPriority1 != pAnnounce->nGrandmasterPriority1)
    {
        m_nGrandmasterPriority1 = pAnnounce->nGrandmasterPriority1;
        bChanged = true;
    }

    if(m_nGrandmasterClass != pAnnounce->nGrandmasterClass)
    {
        m_nGrandmasterClass = pAnnounce->nGrandmasterClass;
        bChanged = true;
    }

    if(m_eGrandmasterAccuracy != pAnnounce->eGrandmasterAccuracy)
    {
        m_eGrandmasterAccuracy = pAnnounce->eGrandmasterAccuracy;
        bChanged = true;
    }

    if(m_nGrandmasterVariance != pAnnounce->nGrandmasterVariance)
    {
        m_nGrandmasterVariance = pAnnounce->nGrandmasterVariance;
        bChanged = true;
    }

    if(m_nGrandmasterPriority2 != pAnnounce->nGrandmasterPriority2)
    {
        m_nGrandmasterPriority2 = pAnnounce->nGrandmasterPriority2;
        bChanged = true;
    }

    if(m_sGrandmasterClockId != pAnnounce->sGrandmasterClockId)
    {
        m_sGrandmasterClockId = pAnnounce->sGrandmasterClockId;
        auto nColon = m_sClockId.find(':');
        m_bGrandMaster = (m_sGrandmasterClockId == m_sClockId.substr(0,nColon));
        bChanged = true;
    }

    if(m_sClockId != pHeader->source.sSourceId)
    {
        m_sClockId = pHeader->source.sSourceId;
        bChanged = true;
    }

    if(m_nStepsRemoved != pAnnounce->nStepsRemoved)
    {
        m_nStepsRemoved = pAnnounce->nStepsRemoved;
        bChanged = true;
    }

    if(m_eTimeSource != pAnnounce->eTimeSource)
    {
        m_eTimeSource = pAnnounce->eTimeSource;
        bChanged = true;
    }
    return bChanged;
}

unsigned char PtpV2Clock::GetInterval(hdr::enumType eType) const
{
    auto itInterval = m_mInterval.find(eType);
    if(itInterval != m_mInterval.end())
    {
        return itInterval->second;
    }
    return 126;
}

unsigned long long int PtpV2Clock::GetCount(hdr::enumType eType) const
{
    auto itCount = m_mCount.find(eType);
    if(itCount != m_mCount.end())
    {
        return itCount->second.value;
    }
    return 0;
}


unsigned short PtpV2Clock::GetFlags(hdr::enumType eType) const
{
    auto itReturn = m_mFlags.find(eType);
    if(itReturn != m_mFlags.end())
    {
        return itReturn->second;
    }
    return 0;
}

bool PtpV2Clock::IsSynced() const
{
    return m_offset.bSet;
}


double PtpV2Clock::GetOffsetSlope() const
{
    return m_offset.m_c.first;
}

double PtpV2Clock::GetOffsetIntersection() const
{
    return m_offset.m_c.second;
}


std::vector<std::pair<double,double>> PtpV2Clock::GetOffsetData() const
{
    std::vector<std::pair<double,double>> vData;
    vData.reserve(m_offset.lstTimesLinReg.size());
    auto itY = m_offset.lstValuesLinReg.begin();
    for(auto x : m_offset.lstTimesLinReg)
    {
        vData.push_back(std::make_pair(x, (*itY)));
        ++itY;
    }
    return vData;
}

double PtpV2Clock::GetDelaySlope() const
{
    return m_delay.m_c.first;
}

double PtpV2Clock::GetDelayIntersection() const
{
    return m_delay.m_c.second;
}


std::vector<std::pair<double,double>> PtpV2Clock::GetDelayData() const
{
    std::vector<std::pair<double,double>> vData;
    vData.reserve(m_delay.lstTimesLinReg.size());
    auto itY = m_delay.lstValuesLinReg.begin();
    for(auto x : m_delay.lstTimesLinReg)
    {
        vData.push_back(std::make_pair(x, (*itY)));
        ++itY;
    }
    return vData;
}

void PtpV2Clock::ClearStats()
{
    ClearStats(m_offset);
    ClearStats(m_delay);
}

void PtpV2Clock::ClearStats(stats& theStats)
{
    theStats.dTotal = 0.0;
    for(size_t i = 0; i < 8; i++)
    {
        theStats.stat[i] = TIMEZERO;
    }
    theStats.bSet = false;
    theStats.m_c = std::make_pair(0.0,0.0);
    theStats.lstValues.clear();
    theStats.lstTimesLinReg.clear();
    theStats.lstValuesLinReg.clear();
}

size_t PtpV2Clock::GetStatsSize() const
{
    return m_offset.lstValues.size();
}
