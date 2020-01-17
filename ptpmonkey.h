#pragma once
#include "timeutils.h"
#include "ptpstructs.h"
#include <memory>
#include <map>
#include <list>
#include <asio.hpp>

class PtpV2Clock;
class PtpEventHandler;

class PtpMonkey
{
    public:
        PtpMonkey(const std::string& sLocalIpAddress, unsigned char nDomain);
        ~PtpMonkey(){}
        void AddEventHandler(std::shared_ptr<PtpEventHandler> pHandler);
        void Run();
        void Stop();
        void Restart();
        void AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp);
        bool IsStopped();

        void Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
        void DelayRequest(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
        void DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload);
        void Announce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pPayload);

        time_s_ns GetPtpTime() const;
        time_s_ns GetPtpOffset() const;
        time_s_ns GetPtpDelay() const;
        std::string GetMasterClockId() const;

        std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator GetClocksBegin() const;
        std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator GetClocksEnd() const;
        std::shared_ptr<const PtpV2Clock> GetMasterClock() const;

    protected:
        asio::io_context io_context;
        std::map<std::string, std::shared_ptr<PtpV2Clock> >::iterator GetOrCreateClock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);


        std::string m_sLocalIpAddress;
        unsigned char m_nDomain;
        std::map<std::string, std::shared_ptr<PtpV2Clock> > m_mClocks;

        std::shared_ptr<PtpV2Clock> m_pMaster;

        std::list<std::shared_ptr<PtpEventHandler>> m_lstEventHandler;
};
