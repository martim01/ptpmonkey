#pragma once
#include "ptpstructs.h"
#include <memory>
#include <map>
#include <list>

namespace ptpmonkey
{

    class PtpClock
    {

    };

    class PtpV2Clock : public PtpClock
    {
        public:
            PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce);
            PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);

            void SyncFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
            void SyncTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
            void FollowUpFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
            void FollowUpTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
            void DelayResponseFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload);
            void DelayResponseTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload);
            void DelayRequest(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
            bool UpdateAnnounce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce);
            void AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp);

            enum enumCalc {MIN=0, MEAN=1, MAX=2, WEIGHTED=3, CURRENT=4, SET=5};

            time_s_ns GetPtpTime() const;

            time_s_ns GetOffset(enumCalc eCalc = SET) const;
            time_s_ns GetDelay(enumCalc eCalc = SET) const;

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
            unsigned char GetPriority2() const
            {   return m_nGrandmasterPriority2; }
            const std::string& GetId() const
            {   return m_sClockId;  }
            unsigned short GetStepsRemoved() const
            {   return m_nStepsRemoved; }
            unsigned char GetTimeSource() const
            {   return m_nTimeSource;   }
            bool IsMaster() const
            {   return m_bMaster;   }
            unsigned short GetFlags(ptpV2Header::enumType eType) const;

            time_s_ns GetLastMessageTime() const
            {
                return m_lastMessageTime;
            }
            const std::string GetIpAddress() const
            {   return m_sIpAddress;    }

            unsigned char GetInterval(ptpV2Header::enumType eType) const;
            unsigned long long int GetCount(ptpV2Header::enumType eType) const;

            void SetSampleSize(unsigned short nSampleSize)
            {
                m_nSampleSize = nSampleSize;
            }
            bool IsSynced() const;
        protected:
            unsigned char m_nDomain;
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
            unsigned short m_nFollowUpSequence;
            unsigned short m_nSampleSize;




            std::string m_sIpAddress;
            std::map<unsigned short, time_s_ns> m_mDelayRequest;

            unsigned long long int  m_nt1s;
            unsigned long long int  m_nt1r;

            time_s_ns m_calculatedAt;
            time_s_ns m_calculatedPtp;

            bool m_bT1Valid;


            struct stats
            {
                stats() : total(TIMEZERO),
                          stat{TIMEZERO,TIMEZERO,TIMEZERO,TIMEZERO,TIMEZERO,TIMEZERO},
                          bSet(false){}



                time_s_ns total;
                time_s_ns stat[6];
                bool bSet;
                std::list<time_s_ns> lstValues;
            };
            void DoStats(unsigned long long int nCurrent, stats& theStats);

            stats m_delay;
            stats m_offset;
            time_s_ns m_lastMessageTime;

            bool m_bTimeSet;

            std::map<unsigned char, unsigned char> m_mInterval;

            struct counter
            {
                unsigned long long value = 0;
            };
            std::map<unsigned char, counter> m_mCount;
            std::map<unsigned char, unsigned short> m_mFlags;
    };
};
