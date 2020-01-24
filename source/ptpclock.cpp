#include "ptpclock.h"
#include <iostream>


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
    m_sIpAddress(pHeader->sIpAddress),
    m_nt1s(0),
    m_nt1r(0),
    m_bT1Valid(false),
    m_lastMessageTime(pHeader->timestamp)
{
    m_mInterval[ptpV2Header::ANNOUNCE] = pHeader->nInterval;
    m_mCount[ptpV2Header::ANNOUNCE].value++;
}

PtpV2Clock::PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload) :
    m_nDomain(pHeader->nDomain),
    m_nFlags(pHeader->nFlags),
    m_nUtcOffset(0),
    m_nGrandmasterPriority1(0),
    m_nGrandmasterClass(0),
    m_nGrandmasterAccuracy(0),
    m_nGrandmasterVariance(0),
    m_nGrandmasterPriority2(0),
    m_sClockId(pHeader->source.sSourceId),
    m_nStepsRemoved(0),
    m_nTimeSource(0),
    m_bMaster(false),
    m_sIpAddress(pHeader->sIpAddress),
    m_nt1s(0),
    m_nt1r(0),
    m_bT1Valid(false),
    m_lastMessageTime(pHeader->timestamp)
{

}

void PtpV2Clock::AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp)
{
    m_mDelayRequest.insert(make_pair(nSequence, timestamp));

    //m_mInterval[ptpV2Header::DELAY_REQ] = pHeader->nInterval;
    m_mCount[ptpV2Header::DELAY_REQ].value++;
}

void PtpV2Clock::Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_lastMessageTime = pHeader->timestamp;

    m_mInterval[ptpV2Header::SYNC] = pHeader->nInterval;
    m_mCount[ptpV2Header::SYNC].value++;

    m_bMaster = true;
    if((pHeader->nFlags & ptpV2Header::TWO_STEP) != 0)   //2-step
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

void PtpV2Clock::FollowUp(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_lastMessageTime = pHeader->timestamp;
    m_mInterval[ptpV2Header::FOLLOW_UP] = pHeader->nInterval;
    m_mCount[ptpV2Header::FOLLOW_UP].value++;

    m_bMaster = true;
    m_nt1s = TimeToNano(pPayload->originTime);

    //check the follow up sequence is correct
    m_bT1Valid = (pHeader->nSequenceId == m_nFollowUpSequence);

}


void PtpV2Clock::DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload)
{
    m_lastMessageTime = pHeader->timestamp;
    m_mInterval[ptpV2Header::DELAY_RESP] = pHeader->nInterval;
    m_mCount[ptpV2Header::DELAY_RESP].value++;

    if(!m_bMaster || !m_bT1Valid)
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
    theStats.stat[CURRENT] = NanoToTime(nCurrent);

    theStats.total = theStats.total+theStats.stat[CURRENT];

    theStats.lstValues.push_back(theStats.stat[CURRENT]);
    if(theStats.lstValues.size() > 1000)
    {
        theStats.total = theStats.total-theStats.lstValues.front();
        theStats.lstValues.pop_front();
    }
    theStats.stat[MEAN] = theStats.total/theStats.lstValues.size();

    for(auto value : theStats.lstValues)
    {
        if(theStats.stat[MIN] == std::make_pair(std::chrono::seconds(0), std::chrono::nanoseconds(0)) || theStats.stat[MIN] > value)
        {
            theStats.stat[MIN] = value;
        }
        if(theStats.stat[MAX] < value)
        {
            theStats.stat[MAX] = value;
        }
    }
}


time_s_ns PtpV2Clock::GetPtpTime(enumCalc eCalc)
{
    return (GetCurrentTaiTime()-m_offset.stat[eCalc]);
}

time_s_ns PtpV2Clock::GetOffset(enumCalc eCalc)
{
    return m_offset.stat[eCalc];
}

time_s_ns PtpV2Clock::GetDelay(enumCalc eCalc)
{
    return m_delay.stat[eCalc];
}

bool PtpV2Clock::UpdateAnnounce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce)
{
    m_lastMessageTime = pHeader->timestamp;
    m_mInterval[ptpV2Header::ANNOUNCE] = pHeader->nInterval;
    m_mCount[ptpV2Header::ANNOUNCE].value++;

    bool bChanged(false);
    if(m_nDomain != pHeader->nDomain)
    {
        m_nDomain = pHeader->nDomain;
        bChanged = true;
    }
    if(m_nFlags != pHeader->nFlags)
    {
        m_nFlags = pHeader->nFlags;
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

    if(m_nGrandmasterAccuracy != pAnnounce->nGrandmasterAccuracy)
    {
        m_nGrandmasterAccuracy = pAnnounce->nGrandmasterAccuracy;
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

    if(m_sClockId != pAnnounce->sClockId)
    {
        m_sClockId = pAnnounce->sClockId;
        bChanged = true;
    }

    if(m_nStepsRemoved != pAnnounce->nStepsRemoved)
    {
        m_nStepsRemoved = pAnnounce->nStepsRemoved;
        bChanged = true;
    }

    if(m_nTimeSource != pAnnounce->nTimeSource)
    {
        m_nTimeSource = pAnnounce->nTimeSource;
        bChanged = true;
    }
    return bChanged;
}

unsigned char PtpV2Clock::GetInterval(ptpV2Header::enumType eType) const
{
    auto itInterval = m_mInterval.find(static_cast<unsigned char>(eType));
    if(itInterval != m_mInterval.end())
    {
        return itInterval->second;
    }
    return 126;
}

unsigned long long int PtpV2Clock::GetCount(ptpV2Header::enumType eType) const
{
    auto itCount = m_mCount.find(static_cast<unsigned char>(eType));
    if(itCount != m_mCount.end())
    {
        return itCount->second.value;
    }
    return 0;
}
