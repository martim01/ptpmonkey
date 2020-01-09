#include "ptpclock.h"


PtpV2Clock::PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce) :
    m_nDomain(pHeader->nDomain),
    m_nFlags(pHeader->nFlags),
    m_nUtcOffset(pAnnounce->nUtcOffset),
    m_nGrandmasterPriority1(pAnnounce->nGrandmasterPriority1),
    m_nGrandmasterClass(pAnnounce->nGrandmasterClass),
    m_nGrandmasterAccuracy(pAnnounce->nGrandmasterAccuracy),
    m_nGrandmasterVariance(pAnnounce->nGrandmasterVariance),
    m_nGrandmasterPriority2(pAnnounce->nGrandmasterPriority2),
    m_sClockId(pAnnounce->sClockId),
    m_nStepsRemoved(pAnnounce->nStepsRemoved),
    m_nTimeSource(pAnnounce->nTimeSource),
    m_bMaster(false),
    m_nt1s(0),
    m_nt1r(0)
{

}

void PtpV2Clock::AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp)
{
    m_mDelayRequest.insert(make_pair(nSequence, timestamp));
}

void PtpV2Clock::Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_bMaster = true;
    m_nt1s = TimeToNano(pPayload->originTime);
    m_nt1r = TimeToNano(pHeader->timestamp);
}

void PtpV2Clock::DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload)
{
    if(!m_bMaster)
        return;

    auto request = m_mDelayRequest.find(pHeader->nSequenceId);
    if(request != m_mDelayRequest.end())
    {
        unsigned long long int nt2s = TimeToNano(request->second);
        unsigned long long int nt2r = TimeToNano(pPayload->originTime);
        unsigned long long int nt3s = TimeToNano(pPayload->originTime);
        unsigned long long int nt3r = TimeToNano(pHeader->timestamp);

        unsigned long long int nOffsetNano = (m_nt1r-m_nt1s-nt2r+nt2s)/2;
        unsigned long long int nDelayNano = (m_nt1r-m_nt1s)-nOffsetNano;

        DoStats(nOffsetNano, m_offset);
        DoStats(nDelayNano, m_delay);

        m_mDelayRequest.erase(request);
    }
}

void PtpV2Clock::DoStats(unsigned long long int nCurrent, stats& theStats)
{
    theStats.nTotalNano += nCurrent;

    theStats.lstValues.push_back(nCurrent);
    if(theStats.lstValues.size() > 1000)
    {
        theStats.nTotalNano -= theStats.lstValues.front();
        theStats.lstValues.pop_front();
    }


    theStats.nStat[MEAN] = (theStats.nTotalNano/theStats.lstValues.size());

    unsigned long long int nMinAv = theStats.nStat[MEAN]*0.7;
    unsigned long long int nMaxAv = theStats.nStat[MEAN]/0.7;
    unsigned int nValid(0);
    theStats.nStat[WEIGHTED] = 0;
    for(auto value : theStats.lstValues)
    {
        if(theStats.nStat[MIN] == 0 || theStats.nStat[MIN] > value)
        {
            theStats.nStat[MIN] = value;
        }
        if(theStats.nStat[MAX] < value)
        {
            theStats.nStat[MAX] = value;
        }
        if(value > nMinAv && value < nMaxAv)
        {
            theStats.nStat[WEIGHTED] += value;
            ++nValid;
        }
    }
    theStats.nStat[WEIGHTED] /= nValid;
}


time_s_ns PtpV2Clock::GetPtpTime(enumCalc eCalc)
{
    unsigned long long int nLocal(TimeToNano(GetCurrentTaiTime()));
    return NanoToTime((nLocal-m_offset.nStat[eCalc]));
}

time_s_ns PtpV2Clock::GetOffset(enumCalc eCalc)
{
    return NanoToTime(m_offset.nStat[eCalc]);
}

time_s_ns PtpV2Clock::GetDelay(enumCalc eCalc)
{
    return NanoToTime(m_delay.nStat[eCalc]);
}

void PtpV2Clock::UpdateAnnounce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce)
{
    m_nDomain = pHeader->nDomain;
    m_nFlags = pHeader->nFlags;
    m_nUtcOffset = pAnnounce->nUtcOffset;
    m_nGrandmasterPriority1 = pAnnounce->nGrandmasterPriority1;
    m_nGrandmasterClass = pAnnounce->nGrandmasterClass;
    m_nGrandmasterAccuracy = pAnnounce->nGrandmasterAccuracy;
    m_nGrandmasterVariance = pAnnounce->nGrandmasterVariance;
    m_nGrandmasterPriority2 = pAnnounce->nGrandmasterPriority2;
    m_sClockId = pAnnounce->sClockId;
    m_nStepsRemoved = pAnnounce->nStepsRemoved;
    m_nTimeSource = pAnnounce->nTimeSource;
}
