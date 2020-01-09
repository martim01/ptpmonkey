#pragma once
#include "ptpstructs.h"
#include <memory>
#include <map>
#include <list>

class PtpClock
{

};

class PtpV2Clock : public PtpClock
{
    public:
        PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce);
        void Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
        void DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload);
        void UpdateAnnounce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce);
        void AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp);

        enum enumCalc {MIN, MEAN, MAX, WEIGHTED};

        time_s_ns GetPtpTime(enumCalc eCalc = WEIGHTED);

        time_s_ns GetOffset(enumCalc eCalc = WEIGHTED);
        time_s_ns GetDelay(enumCalc eCalc = WEIGHTED);


    protected:
        unsigned char m_nDomain;
        unsigned short m_nFlags;    //?
        unsigned short m_nUtcOffset;
        unsigned char m_nGrandmasterPriority1;
        unsigned char m_nGrandmasterClass;
        unsigned char m_nGrandmasterAccuracy;
        unsigned short m_nGrandmasterVariance;
        unsigned char m_nGrandmasterPriority2;
        std::string m_sClockId;
        unsigned short m_nStepsRemoved;
        unsigned char m_nTimeSource;
        bool m_bMaster;


        std::map<unsigned short, time_s_ns> m_mDelayRequest;

        unsigned long long int  m_nt1s;
        unsigned long long int  m_nt1r;

        struct stats
        {
            stats() :nTotalNano(0), nStat{0,0,0,0}{}

            unsigned long long int nTotalNano;
            unsigned long long int nStat[4];
            std::list<unsigned long int> lstValues;
        };
        void DoStats(unsigned long long int nCurrent, stats& theStats);

        stats m_delay;
        stats m_offset;

};
