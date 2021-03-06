#pragma once
#include "ptpstructs.h"
#include <memory>
#include <map>
#include <list>
#include "ptpdll.h"


namespace ptpmonkey
{

    class PtpClock
    {

    };

    class PTP_IMPEXPORT PtpV2Clock : public PtpClock
    {
        public:
            PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce);
            PtpV2Clock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);

            void SyncFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
            void SyncTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
            void FollowUpFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
            void FollowUpTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
            void DelayResponseFrom(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload);
            bool DelayResponseTo(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload);
            void DelayRequest(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
            bool UpdateAnnounce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pAnnounce);


            enum enumCalc {MIN=0, MEAN=1, MAX=2, WEIGHTED=3, CURRENT=4, SET=5, VARIANCE=6, SET_VARIANCE=7};

            std::chrono::nanoseconds GetPtpTime() const;

            std::chrono::nanoseconds GetOffset(enumCalc eCalc = SET) const;
            std::chrono::nanoseconds GetDelay(enumCalc eCalc = SET) const;

            std::chrono::nanoseconds GetLastCalculatedTime() const {return m_calculatedAt;}
            std::chrono::nanoseconds GetLastPtpTime() const {return m_calculatedPtp;}

            const std::string& GetClockId() const
            {   return m_sClockId;  }

            void ResyncToMaster();

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
            const std::string& GetGrandmasterClockId() const
            {   return m_sGrandmasterClockId;  }
            unsigned short GetStepsRemoved() const
            {   return m_nStepsRemoved; }
            unsigned char GetTimeSource() const
            {   return m_nTimeSource;   }
            bool IsGrandMaster() const
            {   return m_bGrandMaster;   }
            bool IsSyncMaster() const
            {   return m_bSyncMaster;   }

            unsigned short GetFlags(ptpV2Header::enumType eType) const;

            std::chrono::nanoseconds GetLastMessageTime() const
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

            double GetOffsetSlope() const;
            double GetOffsetIntersection() const;
            std::vector<std::pair<double,double> > GetOffsetData() const;


        protected:
            unsigned char m_nDomain;
            unsigned short m_nUtcOffset;
            unsigned char m_nGrandmasterPriority1;
            unsigned char m_nGrandmasterClass;
            unsigned char m_nGrandmasterAccuracy;
            unsigned short m_nGrandmasterVariance;
            unsigned char m_nGrandmasterPriority2;
            std::string m_sGrandmasterClockId;
            std::string m_sClockId;
            unsigned short m_nStepsRemoved;
            unsigned char m_nTimeSource;
            bool m_bGrandMaster;
            bool m_bSyncMaster;
            unsigned short m_nFollowUpSequence;
            unsigned short m_nSampleSize;




            std::string m_sIpAddress;
            std::map<unsigned short, std::chrono::nanoseconds> m_mDelayRequest;

            unsigned long long int  m_nt1s;
            unsigned long long int  m_nt1r;

            std::chrono::nanoseconds m_calculatedAt;
            std::chrono::nanoseconds m_calculatedFirst;
            std::chrono::nanoseconds m_calculatedPtp;

            bool m_bT1Valid;


            struct stats
            {
                stats() : total(TIMEZERO),
                          stat{TIMEZERO,TIMEZERO,TIMEZERO,TIMEZERO,TIMEZERO,TIMEZERO,TIMEZERO,TIMEZERO},
                          bSet(false),
                          m_c(0.0,0.0){}


                std::chrono::nanoseconds total;
                std::chrono::nanoseconds stat[8];
                bool bSet;
                std::pair<double, double> m_c;
                std::list<std::chrono::nanoseconds> lstValues;
                std::list<double> lstTimesLinReg;
                std::list<double> lstValuesLinReg;
            };
            bool DoStats(unsigned long long int nCurrent, std::chrono::nanoseconds calcAt, stats& theStats);

            stats m_delay;
            stats m_offset;
            std::chrono::nanoseconds m_lastMessageTime;

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
