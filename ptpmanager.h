#pragma once
#include "timeutils.h"
#include "ptpstructs.h"
#include <memory>
#include <map>
#include <list>


class PtpManager
{
    public:
        PtpManager();

        void Run();

        void AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp);

        void Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
        void DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload);
        void Announce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pPayload);

    protected:
        std::map<unsigned short, time_s_ns> m_mDelayRequest;
        bool m_bSync;

        time_s_ns m_t1s;
        time_s_ns m_t1r;
        time_s_ns m_t2s;
        time_s_ns m_t2r;
        time_s_ns m_t3s;
        time_s_ns m_t3r;

        time_s_ns m_offset;
        time_s_ns m_delay;

        unsigned long long int m_nMax;
        unsigned long long int m_nMin;

        std::list<unsigned long int> m_lstDelay;
};
