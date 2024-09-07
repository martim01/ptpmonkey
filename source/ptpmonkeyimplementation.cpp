#include "ptpmonkeyimplementation.h"
#include "asio.hpp"
#include "ptpparser.h"
#include "receiver.h"
#include "sender.h"
#include "ptpmonkeyhandler.h"
#include "ptploghandler.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include "ptpclock.h"
#include "ptpeventloghandler.h"
#include "mac.h"
#include "loghander.h"
#include "log.h"

#ifdef __GNU__
#include <linux/ethtool.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/net_tstamp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif // __GNU__


using namespace pml::ptpmonkey;

const std::string PtpMonkeyImplementation::MULTICAST = "224.0.1.129";

PtpMonkeyImplementation::PtpMonkeyImplementation(const IpAddress& ipAddress, unsigned char nDomain,unsigned short nSampleSize, Mode mode, Rate enumDelayRequest)  :
    m_local(ipAddress),
    m_Interface(GetInterfaceOfIpAddress(m_local)),
    m_nDomain(nDomain),
    m_nSampleSize(nSampleSize),
    m_mode(mode),
    m_delayRequest(enumDelayRequest),
    m_nLocalClockId(GenerateClockIdentity(m_local))
{
    m_delayRequestTime = std::chrono::nanoseconds(static_cast<uint64_t>(std::pow(2, static_cast<float>(enumDelayRequest))*1000000000.0));    
}

PtpMonkeyImplementation::PtpMonkeyImplementation(const IpInterface& ipInterface, unsigned char nDomain,unsigned short nSampleSize, Mode mode, Rate enumDelayRequest)  :
    m_local(GetIpAddressOfInterface(ipInterface)),
    m_Interface(ipInterface),
    m_nDomain(nDomain),
    m_nSampleSize(nSampleSize),
    m_mode(mode),
    m_delayRequest(enumDelayRequest),
    m_nLocalClockId(GenerateClockIdentity(m_local))
{
    m_delayRequestTime = std::chrono::nanoseconds(static_cast<uint64_t>(std::pow(2, static_cast<float>(enumDelayRequest))*1000000000.0));    
    }

PtpMonkeyImplementation::~PtpMonkeyImplementation()
{
    Stop();
}

void PtpMonkeyImplementation::AddEventHandler(std::shared_ptr<PtpEventHandler> pHandler)
{
    m_lstEventHandler.push_back(pHandler);
}

bool PtpMonkeyImplementation::Run(bool bThreaded)
{
    if(bThreaded)
    {
        m_pThread = std::make_unique<std::thread>([this]() {  Run(); });
        return true;
    }
    else
    {
        return Run();
    }
}

bool PtpMonkeyImplementation::Run()
{
    try
    {
    	pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Create Handler";

        std::shared_ptr<Handler> pHandler = std::make_shared<PtpMonkeyHandler>(*this);
        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Create Parser";
        m_pParser = std::make_shared<PtpParser>(pHandler,m_nDomain);

        m_pParser->AddHandler(std::make_shared<PtpLogHandler>());

        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Get Timestamping support";
        m_nTimestamping = GetTimestampingSupported(m_Interface);

        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Create R319";
        Receiver mR319(m_context, m_pParser, m_nTimestamping);

        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Create R320";
        Receiver mR320(m_context, m_pParser, m_nTimestamping);
        
        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Run R319";
        mR319.Run(asio::ip::make_address(m_local.Get()), 319,asio::ip::make_address(MULTICAST));

        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Run R320";
        mR320.Run(asio::ip::make_address(m_local.Get()), 320,asio::ip::make_address(MULTICAST));

        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Create and Run sender";
        m_pSender = std::make_unique<Sender>(*this, m_pParser, m_context, m_local, asio::ip::make_address(MULTICAST), 319, m_nDomain, m_nTimestamping, m_mode == Mode::MULTICAST);
        m_pSender->Run();

        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Run context";
        m_context.run();
    }
    catch (const std::exception& e)
    {
        pmlLog(pml::LOG_CRITICAL, "pml::ptpmonkey") << "RUN: " << e.what();
    }
       return true;
}

void PtpMonkeyImplementation::DelayRequestSent(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    GetOrCreateClock(pHeader, pPayload);
    if(m_pLocal)
    {
        m_pLocal->DelayRequest(pHeader, pPayload);
    }
}

std::map<std::string, std::shared_ptr<PtpV2Clock> >::iterator PtpMonkeyImplementation::GetOrCreateClock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{

    auto itClock = m_mClocks.find(pHeader->source.sSourceId);
    if(itClock == m_mClocks.end())
    {
        itClock = m_mClocks.try_emplace(pHeader->source.sSourceId, std::make_shared<PtpV2Clock>(pHeader, pPayload)).first;
        if(pHeader->source.nSourceId == m_nLocalClockId)
        {
            pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Local clock";
            m_pLocal = itClock->second;
            m_pLocal->SetSampleSize(m_nSampleSize);
        }
        else
        {
            pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Clock: " << std::hex << pHeader->source.nSourceId << "\tLocal " <<  m_nLocalClockId;
        }

        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->ClockAdded(itClock->second);
        }
    }
    return itClock;
}

void PtpMonkeyImplementation::Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    //find the clock that sent the Sync Message
    
    if(auto itClock = GetOrCreateClock(pHeader, pPayload); itClock != m_mClocks.end())
    {
        itClock->second->SyncFrom(pHeader, pPayload);
        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->SyncSent(itClock->second);
        }

        if(m_pSyncMaster != itClock->second)
        {
            ChangeSyncMaster(itClock->second);
        }


    }

    //send the sync to our local clock
    if(m_pLocal)
    {
        m_pLocal->SyncTo(pHeader, pPayload);
    }
}

void PtpMonkeyImplementation::ChangeSyncMaster(std::shared_ptr<PtpV2Clock> pNewMaster)
{
    for(auto pHandler : m_lstEventHandler)
    {
        pHandler->ClockBecomeMaster(pNewMaster);
        if(m_pSyncMaster)
        {
            pHandler->ClockBecomeSlave(m_pSyncMaster);
        }
    }
    std::scoped_lock lg(m_mutex);
    m_pSyncMaster = pNewMaster;

    if(m_mode != Mode::MULTICAST)
    {
        m_pSender->ChangeEndpoint(asio::ip::make_address(m_pSyncMaster->GetIpAddress()));
    }
}

void PtpMonkeyImplementation::FollowUp(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    //find the clock....
    
    if(auto itClock = GetOrCreateClock(pHeader, pPayload); itClock != m_mClocks.end())
    {
        itClock->second->FollowUpFrom(pHeader, pPayload);
        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->FollowUpSent(itClock->second);
        }

        if(m_pSyncMaster != itClock->second)
        {
            ChangeSyncMaster(itClock->second);
        }
    }
    //send the sync to our local clock
    if(m_pLocal)
    {
        m_pLocal->FollowUpTo(pHeader, pPayload);
    }
}

void PtpMonkeyImplementation::DelayRequest(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    if(auto itClock = GetOrCreateClock(pHeader, pPayload); itClock != m_mClocks.end())
    {
        itClock->second->DelayRequest(pHeader, pPayload);
        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->DelayRequestSent(itClock->second);
        }
    }

}

void PtpMonkeyImplementation::DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload)
{

    auto itClock = GetOrCreateClock(pHeader, pPayload);

    if(itClock != m_mClocks.end())
    {
        itClock->second->DelayResponseFrom(pHeader, pPayload);
        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->DelayResponseSent(itClock->second);
        }
        if(m_pSyncMaster != itClock->second)
        {
            ChangeSyncMaster(itClock->second);
        }
    }

    //upddate the details of the clock the delay response is for
    itClock = m_mClocks.find(pPayload->source.sSourceId);
    if(itClock != m_mClocks.end())
    {
        itClock->second->DelayResponseTo(pHeader, pPayload);
        if(itClock->second == m_pLocal)
        {
            //update the delay request to match what the master clock says
            if(pHeader->nInterval >= -7 && pHeader->nInterval <=4 )
            {
                m_delayRequest = static_cast<ptpmonkey::Rate>(pHeader->nInterval);
            }
            for(auto pHandler : m_lstEventHandler)
            {
                pHandler->ClockTimeCalculated(itClock->second);
            }
        }
    }
}

void PtpMonkeyImplementation::Announce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pPayload)
{
    if(auto itClock = m_mClocks.find(pHeader->source.sSourceId); itClock != m_mClocks.end())
    {
        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->AnnounceSent(itClock->second);
        }

        if(itClock->second->UpdateAnnounce(pHeader, pPayload))
        {
            for(auto pHandler : m_lstEventHandler)
            {
                pHandler->ClockUpdated(itClock->second);
            }
        }
    }
    else
    {
        itClock = m_mClocks.try_emplace(pHeader->source.sSourceId, std::make_shared<PtpV2Clock>(pHeader, pPayload)).first;
        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->ClockAdded(itClock->second);
            pHandler->AnnounceSent(itClock->second);
        }
    }
}


std::chrono::nanoseconds PtpMonkeyImplementation::GetPtpTime() const
{
    if(m_pLocal)
    {
        return m_pLocal->GetPtpTime();
    }
    else
    {
        return TIMEZERO;
    }
}

bool PtpMonkeyImplementation::IsSyncedToMaster() const
{
    return (m_pLocal && m_pLocal->IsSynced());

}

void PtpMonkeyImplementation::ResyncToMaster() const
{
    if(m_pLocal)
    {
        m_pLocal->ResyncToMaster();
    }
}


const std::map<std::string, std::shared_ptr<PtpV2Clock> >& PtpMonkeyImplementation::GetClocks() const
{
    return m_mClocks;
}

std::shared_ptr<const PtpV2Clock> PtpMonkeyImplementation::GetSyncMasterClock() const
{
    std::scoped_lock lg(m_mutex);

    return m_pSyncMaster;
}

std::string PtpMonkeyImplementation::GetMasterClockId() const
{
    std::scoped_lock lg(m_mutex);

    if(m_pSyncMaster)
    {
        return m_pSyncMaster->GetGrandmasterClockId();
    }
    return "";
}


std::chrono::nanoseconds PtpMonkeyImplementation::GetPtpOffset(PtpV2Clock::enumCalc eCalc) const
{
    std::scoped_lock lg(m_mutex);
    if(m_pLocal)
    {
        return m_pLocal->GetOffset(eCalc);
    }
    else
    {
        return TIMEZERO;
    }
}

std::chrono::nanoseconds PtpMonkeyImplementation::GetPtpDelay(PtpV2Clock::enumCalc eCalc) const
{
    std::scoped_lock lg(m_mutex);
    if(m_pLocal)
    {
        return m_pLocal->GetDelay(eCalc);
    }
    else
    {
        return TIMEZERO;
    }
}

void PtpMonkeyImplementation::CheckForDeadClocks()
{

    auto now = Now();
    for(auto itClock  = m_mClocks.begin(); itClock != m_mClocks.end();)
    {
        if((now.count() - (itClock->second->GetLastMessageTime()).count()) > 5000000000)
        {
            for(auto pHandler : m_lstEventHandler)
            {
                pHandler->ClockRemoved(itClock->second);
            }
            if(m_pSyncMaster == itClock->second)
            {
                m_pSyncMaster = nullptr;
            }
            if(m_pLocal == itClock->second)
            {
                m_pLocal = nullptr;
            }
            auto itDelete(itClock);
            ++itClock;
            m_mClocks.erase(itDelete);
        }
        else
        {
            ++itClock;
        }
    }
}

void PtpMonkeyImplementation::Stop()
{
    m_context.stop();
    m_pSyncMaster = nullptr;
    m_mClocks.clear();

    if(m_pThread)
    {   
        m_pThread->join();
        m_pThread = nullptr;
    }
}

void PtpMonkeyImplementation::Restart()
{
    if(m_context.stopped())
    {
        auto bThreaded = false;
        if(m_pThread)
        {
            bThreaded = true;
            m_pThread->join();
            m_pThread = nullptr;
        }

        m_context.restart();
        Run(bThreaded);
    }
}

bool PtpMonkeyImplementation::IsStopped() const
{
    return m_context.stopped();
}


std::shared_ptr<const PtpV2Clock> PtpMonkeyImplementation::GetClock(const std::string& sClockId) const
{
    if(auto itClock = m_mClocks.find(sClockId); itClock != m_mClocks.end())
    {
        return itClock->second;
    }
    return nullptr;
}


std::string PtpMonkeyImplementation::GetLocalClockId() const
{
    if(m_pLocal)
    {
        return m_pLocal->GetClockId();
    }
    return "";
}

std::shared_ptr<const PtpV2Clock> PtpMonkeyImplementation::GetLocalClock() const
{
    return m_pLocal;
}


int PtpMonkeyImplementation::GetTimestampingSupported(const IpInterface& interface)
{
    int nSupports(0);
    #ifdef __GNU__
    ethtool_ts_info tsi = {.cmd = ETHTOOL_GET_TS_INFO};
    ifreq ifr;
    strcpy(ifr.ifr_name, interface.Get().c_str());
    ifr.ifr_data = (char*)&tsi;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    ioctl(fd, SIOCETHTOOL, & ifr);


    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "" << tsi.so_timestamping;
    if(tsi.so_timestamping & SOF_TIMESTAMPING_TX_HARDWARE)
    {
        nSupports |= port::enumTimestamping::TIMESTAMP_TX_HARDWARE;
        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << ""<< ifr.ifr_name << " supports harware tx";
    }
    if(tsi.so_timestamping & SOF_TIMESTAMPING_TX_SOFTWARE)
    {
        nSupports |= port::enumTimestamping::TIMESTAMP_TX_SOFTWARE;
        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << ""<< ifr.ifr_name << " supports software tx";
    }

    if(tsi.so_timestamping & SOF_TIMESTAMPING_RX_HARDWARE)
    {
        nSupports |= port::enumTimestamping::TIMESTAMP_RX_HARDWARE;
        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << ""<< ifr.ifr_name << " supports harware rx";
    }
    if(tsi.so_timestamping & SOF_TIMESTAMPING_RX_SOFTWARE)
    {
        nSupports |= port::enumTimestamping::TIMESTAMP_RX_SOFTWARE;
        pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << ""<< ifr.ifr_name << " supports software rx";
    }
    #endif // __GNU__
    return nSupports;
}

void PtpMonkeyImplementation::ResetLocalClockStats() const
{
    m_pLocal->ClearStats();
}


Mode PtpMonkeyImplementation::GetMode() const
{
    return m_mode;
}


void PtpMonkeyImplementation::SetDomain(unsigned char nDomain)
{
    if(m_nDomain != nDomain)
    {
        std::scoped_lock lg(m_mutex);
        m_mClocks.clear();
        m_pLocal = nullptr;
        m_pSyncMaster = nullptr;
        m_nLocalClockId = 0;
        m_pParser->SetDomain(nDomain);
        m_pSender->SetDomain(nDomain);
    }
}

void PtpMonkeyImplementation::Get(mngmnt::enumGet id, uint8_t nHops, const std::string& sTargetPortId, uint16_t nTargetPortNumber)
{
    if(m_pSender)
    {
        m_pSender->Get(id, nHops, sTargetPortId, nTargetPortNumber);
    }
}