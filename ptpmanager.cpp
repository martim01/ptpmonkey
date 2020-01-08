#include "ptpmanager.h"
#include "asio.hpp"
#include "ptpparser.h"
#include "receiver.h"
#include "sender.h"
#include "ptpmanagerhandler.h"
#include <chrono>
#include <iomanip>
#include <thread>

PtpManager::PtpManager() : m_bSync(false), m_nMax(0), m_nMin(0), m_nTotal(0)
{
}

void PtpManager::Run()
{
    std::thread t([this]()
    {
        try
        {
            std::shared_ptr<Handler> pHandler = std::make_shared<PtpManagerHandler>(*this);

            asio::io_context io_context;
            Receiver r319(io_context, std::unique_ptr<Parser>(new PtpParser(pHandler)));
            r319.run(asio::ip::make_address("0.0.0.0"),asio::ip::make_address("224.0.1.129"), 319);

            Receiver r320(io_context, std::unique_ptr<Parser>(new PtpParser(pHandler)));
            r320.run(asio::ip::make_address("0.0.0.0"),asio::ip::make_address("224.0.1.129"), 320);

            Sender sDelay(*this, io_context, asio::ip::make_address("224.0.1.129"), 319);
            io_context.run();
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }
    });
    t.detach();
}


void PtpManager::AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp)
{

    m_mDelayRequest.insert(make_pair(nSequence, timestamp));
}


void PtpManager::Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    m_bSync = true;
    m_t1s = pPayload->originTime;
    m_t1r = pHeader->timestamp;
}

void PtpManager::DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload)
{
    if(!m_bSync)
        return;

  // std::cout << "DelayResponse" << std::endl;
    auto request = m_mDelayRequest.find(pHeader->nSequenceId);
    if(request != m_mDelayRequest.end())
    {
        m_t2s = request->second;
        m_t2r = pPayload->originTime;
        m_t3s = pPayload->originTime;
        m_t3r = pHeader->timestamp;

//        std::cout << "T1\t" << TimeToString(m_t1s) << std::endl;
//        std::cout << "T1'\t" << TimeToString(m_t1r) << std::endl;
//        std::cout << "T2\t" << TimeToString(m_t2s) << std::endl;
//        std::cout << "T2'\t" << TimeToString(m_t2r) << std::endl;
//        std::cout << "T3\t" << TimeToString(m_t3s) << std::endl;
//        std::cout << "T3'\t " << TimeToString(m_t3r) << std::endl;
//        std::cout << "T1'-T1\t" << TimeToString(Minus(m_t1r,m_t1s)) << std::endl;
//        std::cout << "T2'-T1\t" << TimeToString(Minus(m_t2r,m_t2s)) << std::endl;


        m_offset = Divide(Add(Minus(Minus(m_t1r, m_t1s), m_t2r),m_t2s),2);
        m_delay = Minus(Minus(m_t1r, m_t1s),m_offset);

        m_nTotal += m_delay.second.count();


        m_lstDelay.push_back(m_delay.second.count());   //@todo take count of seconds as well
        if(m_lstDelay.size() > 1000)
        {
            m_nTotal -= m_lstDelay.front();
            m_lstDelay.pop_front();
        }


        unsigned long long int nAverage(m_nTotal/m_lstDelay.size());
        unsigned long long int nWeighted(0);
        unsigned long long int nMinAv = nAverage*0.7;
        unsigned long long int nMaxAv = nAverage/0.7;
        unsigned int nValid(0);
        for(auto delay : m_lstDelay)
        {
            if(m_nMin == 0 || m_nMin > delay)
            {
                m_nMin = delay;
            }
            if(m_nMax < delay)
            {
                m_nMax = delay;
            }
            if(delay > nMinAv && delay < nMaxAv)
            {
                nWeighted += delay;
                ++nValid;
            }
        }
        nWeighted /= nValid;

//        std::cout << "Offset:\t" << TimeToString(m_offset) << std::endl;
//        std::cout << "Delay:\t" << TimeToString(m_delay) << std::endl;
//        std::cout << "Average:\t" << std::dec <<  "0." << std::setw(9) << std::setfill('0') << nAverage
//                  << "\tWeighted\t" << "0." << std::setw(9) << std::setfill('0') << nWeighted
//                  << "\tMin\t" << "0." << std::setw(9) << std::setfill('0') << m_nMin
//                  << "\tMax\t" << "0."<< std::setw(9) << std::setfill('0') << m_nMax
//                  << "\tRange\t" << "0." << std::setw(9) << std::setfill('0') <<(m_nMax-m_nMin) << std::endl;
//        std::cout << "PTP Time\t" << TimeToIsoString(GetPtpTime()) << std::endl;
        m_mDelayRequest.erase(request);
    }
}

void PtpManager::Announce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pPayload)
{

}


time_s_ns PtpManager::GetPtpTime() const
{
    time_s_ns local(GetCurrentTaiTime());
    return Minus(local, m_offset);
}
