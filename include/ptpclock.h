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
        PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);

        void Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
        void DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload);
        bool UpdateAnnounce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce);
        void AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp);

        enum enumCalc {MIN=0, MEAN=1, MAX=2, WEIGHTED=3, CURRENT=4};

        time_s_ns GetPtpTime(enumCalc eCalc = MEAN);

        time_s_ns GetOffset(enumCalc eCalc = MEAN);
        time_s_ns GetDelay(enumCalc eCalc = MEAN);

        const std::string& GetClockId() const
        {   return m_sClockId;  }


        unsigned char GetDomain() const
        {   return m_nDomain;   }
        unsigned short GetUtcOffset() const
        {   return m_nUtcOffset;    }
        unsigned char GetPriority1() const
        {   return m_nGrandmasterPriority1; }
        unsigned char GetClass() const
        {   return m_nGrandmasterClass; }
        unsigned char GetAccuracy() const
        {   return m_nGrandmasterAccuracy;  }
        unsigned short GetVariance() const
        {   return m_nGrandmasterVariance;  }
        unsigned char GetPriorty2() const
        {   return m_nGrandmasterPriority2; }
        const std::string& GetId() const
        {   return m_sClockId;  }
        unsigned short GetStepsRemoved() const
        {   return m_nStepsRemoved; }
        unsigned char GetTimeSource() const
        {   return m_nTimeSource;   }
        bool IsMaster() const
        {   return m_bMaster;   }

        time_s_ns GetLastMessageTime() const
        {
            return m_lastMessageTime;
        }

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

//        unsigned short

        std::map<unsigned short, time_s_ns> m_mDelayRequest;

        unsigned long long int  m_nt1s;
        unsigned long long int  m_nt1r;

        struct stats
        {
            stats() : total(std::make_pair(std::chrono::seconds(0), std::chrono::nanoseconds(0))),
                                     stat{std::make_pair(std::chrono::seconds(0), std::chrono::nanoseconds(0)),
                                         std::make_pair(std::chrono::seconds(0), std::chrono::nanoseconds(0)),
                                         std::make_pair(std::chrono::seconds(0), std::chrono::nanoseconds(0)),
                                         std::make_pair(std::chrono::seconds(0), std::chrono::nanoseconds(0)),
                                         std::make_pair(std::chrono::seconds(0), std::chrono::nanoseconds(0))}{}



            time_s_ns total;
            time_s_ns stat[5];
            std::list<time_s_ns> lstValues;
        };
        void DoStats(unsigned long long int nCurrent, stats& theStats);

        stats m_delay;
        stats m_offset;
        time_s_ns m_lastMessageTime;
};
