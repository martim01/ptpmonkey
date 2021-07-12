#include "ptpclock.h"
#include <iostream>
#include "log.h"
#include "linearregression.h"

using namespace ptpmonkey;

PtpV2Clock::PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce) :
    m_nDomain(pHeader->nDomain),
    m_nUtcOffset(pAnnounce->nUtcOffset),
    m_nGrandmasterPriority1(pAnnounce->nGrandmasterPriority1),
    m_nGrandmasterClass(pAnnounce->nGrandmasterClass),
    m_nGrandmasterAccuracy(pAnnounce->nGrandmasterAccuracy),
    m_nGrandmasterVariance(pAnnounce->nGrandmasterVariance),
    m_nGrandmasterPriority2(pAnnounce->nGrandmasterPriority2),
    m_sClockId(pHeader->source.sSourceId),
    m_nStepsRemoved(pAnnounce->nStepsRemoved),
    m_nTimeSource(pAnnounce->nTimeSource),
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
    m_mFlags[ptpV2Header::ANNOUNCE] = pHeader->nFlags;
    m_mInterval[ptpV2Header::ANNOUNCE] = pHeader->nInterval;
    m_mCount[ptpV2Header::ANNOUNCE].value++;

}

PtpV2Clock::PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload) :
    m_nDomain(pHeader->nDomain),
    m_nUtcOffset(0),
    m_nGrandmasterPriority1(0),
    m_nGrandmasterClass(0),
    m_nGrandmasterAccuracy(0),
    m_nGrandmasterVariance(0),
    m_nGrandmasterPriority2(0),
    m_sClockId(pHeader->source.sSourceId),
    m_nStepsRemoved(0),
    m_nTimeSource(0),
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
    m_mFlags[pHeader->nType] = pHeader->nFlags;
}

void PtpV2Clock::SyncFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_lastMessageTime = pHeader->timestamp;

    m_mInterval[ptpV2Header::SYNC] = pHeader->nInterval;
    m_mCount[ptpV2Header::SYNC].value++;

    m_mFlags[pHeader->nType] = pHeader->nFlags;
    m_bSyncMaster = true;

}

void PtpV2Clock::SyncTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_bSyncMaster = false;
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


void PtpV2Clock::FollowUpFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_lastMessageTime = pHeader->timestamp;
    m_mInterval[ptpV2Header::FOLLOW_UP] = pHeader->nInterval;
    m_mCount[ptpV2Header::FOLLOW_UP].value++;
    m_mFlags[pHeader->nType] = pHeader->nFlags;

    m_bSyncMaster = true;


}

void PtpV2Clock::FollowUpTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_bSyncMaster = false;
    m_nt1s = TimeToNano(pPayload->originTime);

    //check the follow up sequence is correct
    m_bT1Valid = (pHeader->nSequenceId == m_nFollowUpSequence);


}


void PtpV2Clock::DelayRequest(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{

    m_lastMessageTime = pHeader->timestamp;
    m_mInterval[ptpV2Header::DELAY_REQ] = pHeader->nInterval;
    m_mCount[ptpV2Header::DELAY_REQ].value++;
    m_mFlags[pHeader->nType] = pHeader->nFlags;


    // if we've already had a tx timestamp set for this message workout the os delay
    auto itRequest = m_mDelayRequest.find(pHeader->nSequenceId);
    if(itRequest != m_mDelayRequest.end())
    {
        pmlLog(pml::LOG_WARN) << "PtpMonkey\tDelayRequest: Sequence Id repeated: " << std::dec << pHeader->nSequenceId;
    }
    else
    {
        m_mDelayRequest.insert(make_pair(pHeader->nSequenceId, pHeader->timestamp));
        pmlLog(pml::LOG_TRACE) << "PtpMonkey\tDelayRequest: " << std::dec << pHeader->nSequenceId << " timed at: " << TimeToIsoString(pHeader->timestamp);
    }
}

void PtpV2Clock::DelayResponseFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload)
{
    m_lastMessageTime = pHeader->timestamp;
    m_mInterval[ptpV2Header::DELAY_RESP] = pHeader->nInterval;
    m_mCount[ptpV2Header::DELAY_RESP].value++;
    m_mFlags[pHeader->nType] = pHeader->nFlags;
    m_bSyncMaster = true;

}

bool PtpV2Clock::DelayResponseTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload)
{
    m_mInterval[ptpV2Header::DELAY_RESP] = pHeader->nInterval;
    m_mCount[ptpV2Header::DELAY_RESP].value++;
    m_mFlags[pHeader->nType] = pHeader->nFlags;

    bool bSyncChange(false);
    auto request = m_mDelayRequest.find(pHeader->nSequenceId);
    if(request != m_mDelayRequest.end())
    {
        if(m_bT1Valid)
        {
            long long int nt2s = TimeToNano(request->second);
            long long int nt2r = TimeToNano(pPayload->originTime);
            long long int nt3s = TimeToNano(pPayload->originTime);
            long long int nt3r = TimeToNano(pHeader->timestamp);

            //unsigned long long int nOffsetNano = (m_nt1r-m_nt1s-nt2r+nt2s)/2;

            long long int nDelayNano = (m_nt1r-m_nt1s+nt2r-nt2s)/2;
            long long int nOffsetNano = nt3r-(nt3s+nDelayNano);
            long long int nCheck = (m_nt1r-m_nt1s)-nOffsetNano;

            m_calculatedAt = pHeader->timestamp;
            if(m_calculatedAt != TIMEZERO)
            {

                m_calculatedPtp = NanoToTime(nDelayNano)+pPayload->originTime;
                if(m_calculatedFirst == TIMEZERO)
                {
                    m_calculatedFirst = m_calculatedAt;
                }

                 DoStats(nDelayNano, m_calculatedAt, m_delay);

                 bSyncChange = DoStats(nOffsetNano, m_calculatedAt, m_offset);

            }
        }
        m_mDelayRequest.erase(request);
    }
    return bSyncChange;
}

bool PtpV2Clock::DoStats(unsigned long long int nCurrent, std::chrono::nanoseconds calcAt, stats& theStats)
{
    theStats.stat[CURRENT] = NanoToTime(nCurrent);

    theStats.total = theStats.total+theStats.stat[CURRENT];

    theStats.lstValues.push_back(theStats.stat[CURRENT]);
    theStats.lstTimesLinReg.push_back(TimeToDouble(calcAt-m_calculatedFirst));
    theStats.lstValuesLinReg.push_back(TimeToDouble(theStats.stat[CURRENT]));


    if(theStats.lstValues.size() > m_nSampleSize*20)
    {
        theStats.total = theStats.total-theStats.lstValues.front();
        theStats.lstValues.pop_front();
        theStats.lstTimesLinReg.pop_front();
        theStats.lstValuesLinReg.pop_front();
    }
    theStats.stat[MEAN] = theStats.total/theStats.lstValues.size();

    if(theStats.lstValues.size() >= m_nSampleSize)   //got enough to create an offset
    {
        theStats.stat[MIN] = TIMEZERO;
        theStats.stat[MAX] = TIMEZERO;
        theStats.stat[VARIANCE] = TIMEZERO;
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
            double dVar = TimeToDouble((value-theStats.stat[MEAN]));

            dVariance += (dVar*dVar);

        }
        theStats.stat[VARIANCE] = DoubleToTime(dVariance/static_cast<double>(theStats.lstValues.size()-1));

        theStats.m_c = GetSlopeAndIntercept(theStats.lstTimesLinReg, theStats.lstValuesLinReg);

        //pmlLog() << "Predicted: " << (theStats.lstTimesLinReg.back()*theStats.m_c.first + theStats.m_c.second);


        if(theStats.bSet == false)
        {
            if(TimeToNano(theStats.stat[VARIANCE]) < 12)
            {
                theStats.stat[SET] = theStats.stat[MEAN];
                theStats.stat[SET_VARIANCE] = theStats.stat[VARIANCE];
                theStats.bSet = true;
                return true;
            }
            return false;
        }
        else
        {
             //@todo calculate whether we should change the offset or not
             if(theStats.stat[VARIANCE] < theStats.stat[SET_VARIANCE])
             {
                 theStats.stat[SET_VARIANCE] = theStats.stat[VARIANCE];
                 return true;
             }
             else
             {
                 return false;
            }
        }
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
    if(eCalc == CURRENT)
        return (m_calculatedPtp-m_calculatedAt);
    return m_offset.stat[eCalc];
}

std::chrono::nanoseconds PtpV2Clock::GetDelay(enumCalc eCalc) const
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

    if(m_mFlags[ptpV2Header::ANNOUNCE] != pHeader->nFlags)
    {
        m_mFlags[ptpV2Header::ANNOUNCE] = pHeader->nFlags;
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

    if(m_sGrandmasterClockId != pAnnounce->sGrandmasterClockId)
    {
        m_sGrandmasterClockId = pAnnounce->sGrandmasterClockId;
        m_bGrandMaster = (m_sGrandmasterClockId == m_sClockId);
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


unsigned short PtpV2Clock::GetFlags(ptpV2Header::enumType eType) const
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
