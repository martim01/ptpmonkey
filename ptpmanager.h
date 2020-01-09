#pragma once
#include "timeutils.h"
#include "ptpstructs.h"
#include <memory>
#include <map>
#include <list>

class PtpV2Clock;

class PtpManager
{
    public:
        PtpManager(unsigned char nDomain);

        void Run();

        void AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp);

        void Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
        void DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload);
        void Announce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pPayload);

        time_s_ns GetPtpTime() const;

        std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator GetClocksBegin() const;
        std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator GetClocksEnd() const;
        std::shared_ptr<const PtpV2Clock> GetMasterClock() const;

    protected:
        unsigned char m_nDomain;
        std::map<std::string, std::shared_ptr<PtpV2Clock> > m_mClocks;

        std::shared_ptr<PtpV2Clock> m_pMaster;
};
