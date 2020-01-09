#include "ptpmanager.h"
#include "asio.hpp"
#include "ptpparser.h"
#include "receiver.h"
#include "sender.h"
#include "ptpmanagerhandler.h"
#include <chrono>
#include <iomanip>
#include <thread>
#include "ptpclock.h"


PtpManager::PtpManager(unsigned char nDomain)  : m_nDomain(nDomain), m_pMaster(nullptr)
{
}

void PtpManager::Run()
{
    std::thread t([this]()
    {
        try
        {
            std::stringstream ssMulticast;
            ssMulticast << "224.0.1." << (static_cast<unsigned int>(m_nDomain)+129);

            std::shared_ptr<Handler> pHandler = std::make_shared<PtpManagerHandler>(*this);

            asio::io_context io_context;
            Receiver r319(io_context, std::unique_ptr<Parser>(new PtpParser(pHandler)));
            r319.run(asio::ip::make_address("0.0.0.0"),asio::ip::make_address(ssMulticast.str()), 319);

            Receiver r320(io_context, std::unique_ptr<Parser>(new PtpParser(pHandler)));
            r320.run(asio::ip::make_address("0.0.0.0"),asio::ip::make_address(ssMulticast.str()), 320);

            Sender sDelay(*this, io_context, asio::ip::address_v4::from_string("10.10.11.218"), asio::ip::make_address(ssMulticast.str()), 319);
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
    //find the master clock and add the value to it
    if(m_pMaster)
    {
        m_pMaster->AddDelayRequest(nSequence, timestamp);
    }

}


void PtpManager::Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    //find the clock....
    auto itClock = m_mClocks.find(pHeader->source.sSourceId);
    if(itClock != m_mClocks.end())
    {
        itClock->second->Sync(pHeader, pPayload);
        if(m_pMaster != itClock->second)
        {
            m_pMaster = itClock->second;
            //@todo alert master clock has changed
        }
    }

}

void PtpManager::DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload)
{
    auto itClock = m_mClocks.find(pHeader->source.sSourceId);
    if(itClock != m_mClocks.end())
    {
        itClock->second->DelayResponse(pHeader, pPayload);
    }
}

void PtpManager::Announce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pPayload)
{
    auto itClock = m_mClocks.find(pHeader->source.sSourceId);
    if(itClock != m_mClocks.end())
    {
        itClock->second->UpdateAnnounce(pHeader, pPayload);
    }
    else
    {
        m_mClocks.insert(std::make_pair(pHeader->source.sSourceId, std::make_shared<PtpV2Clock>(pHeader, pPayload)));
    }
}


time_s_ns PtpManager::GetPtpTime() const
{
    if(m_pMaster)
    {
        return m_pMaster->GetPtpTime();
    }
    return std::make_pair(std::chrono::seconds(0),std::chrono::nanoseconds(0));
}


std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator PtpManager::GetClocksBegin() const
{
    return m_mClocks.begin();
}

std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator PtpManager::GetClocksEnd() const
{
    return m_mClocks.end();
}

std::shared_ptr<const PtpV2Clock> PtpManager::GetMasterClock() const
{
    return m_pMaster;
}
