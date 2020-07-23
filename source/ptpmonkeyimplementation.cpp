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


using namespace ptpmonkey;

PtpMonkeyImplementation::PtpMonkeyImplementation(const IpAddress& ipAddress, unsigned char nDomain,unsigned short nSampleSize, Rate enumDelayRequest)  :
    m_local(ipAddress),
    m_Interface(GetInterfaceOfIpAddress(m_local)),
    m_nDomain(nDomain),
    m_nSampleSize(nSampleSize),
    m_delayRequest(enumDelayRequest),
    m_pSyncMaster(nullptr),
    m_nLocalClockId(GenerateClockIdentity(m_local)),
    m_pLocal(nullptr)
{
}

PtpMonkeyImplementation::PtpMonkeyImplementation(const IpInterface& ipInterface, unsigned char nDomain,unsigned short nSampleSize, Rate enumDelayRequest)  :
    m_local(GetIpAddressOfInterface(ipInterface)),
    m_Interface(ipInterface),
    m_nDomain(nDomain),
    m_nSampleSize(nSampleSize),
    m_delayRequest(enumDelayRequest),
    m_pSyncMaster(nullptr),
    m_nLocalClockId(GenerateClockIdentity(m_local)),
    m_pLocal(nullptr)
{

}

void PtpMonkeyImplementation::AddEventHandler(std::shared_ptr<PtpEventHandler> pHandler)
{
    m_lstEventHandler.push_back(pHandler);
}

bool PtpMonkeyImplementation::Run()
{
    std::stringstream ssMulticast;
    ssMulticast << "224.0.1." << (static_cast<unsigned int>(m_nDomain)+129);


    std::thread t([this]()
    {
        try
        {
            std::stringstream ssMulticast;
            ssMulticast << "224.0.1." << (static_cast<unsigned int>(m_nDomain)+129);

            std::shared_ptr<Handler> pHandler = std::make_shared<PtpMonkeyHandler>(*this);

            std::shared_ptr<Parser> pParser = std::make_shared<PtpParser>(pHandler);

            int nTimestamping = GetTimestampingSupported(m_Interface);

            Receiver r319(m_context, pParser, nTimestamping);
            Receiver r320(m_context, pParser, nTimestamping);
            Sender sDelay(*this, m_context, m_local, asio::ip::make_address(ssMulticast.str()), 319, nTimestamping);
            r319.Run(asio::ip::make_address("0.0.0.0"),asio::ip::make_address(ssMulticast.str()), 319);
            r320.Run(asio::ip::make_address("0.0.0.0"),asio::ip::make_address(ssMulticast.str()), 320);
            sDelay.Run();

            m_context.run();
        }
        catch (const std::exception& e)
        {
            pml::Log::Get(pml::Log::LOG_CRITICAL) << "RUN: " << e.what() << "\n";
        }
    });

    t.detach();
    //}


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
        itClock = m_mClocks.insert(std::make_pair(pHeader->source.sSourceId, std::make_shared<PtpV2Clock>(pHeader, pPayload))).first;
        if(pHeader->source.nSourceId == m_nLocalClockId)
        {
            m_pLocal = itClock->second;
            m_pLocal->SetSampleSize(m_nSampleSize);
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
    auto itClock = GetOrCreateClock(pHeader, pPayload);
    if(itClock != m_mClocks.end())
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
    std::lock_guard<std::mutex> lg(m_mutex);
    m_pSyncMaster = pNewMaster;
}

void PtpMonkeyImplementation::FollowUp(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    //find the clock....
    auto itClock = GetOrCreateClock(pHeader, pPayload);
    if(itClock != m_mClocks.end())
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
    auto itClock = GetOrCreateClock(pHeader, pPayload);

    if(itClock != m_mClocks.end())
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
        bool bSync = itClock->second->DelayResponseTo(pHeader, pPayload);

        if(itClock->second == m_pLocal)
        {
            //update the delay request to match what the master clock says
            m_delayRequest = static_cast<ptpmonkey::Rate>(pHeader->nInterval);
            //if(bSync)
            {
                for(auto pHandler : m_lstEventHandler)
                {
                    pHandler->ClockTimeCalculated(itClock->second);
                }
            }
        }
    }
}

void PtpMonkeyImplementation::Announce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pPayload)
{
    auto itClock = m_mClocks.find(pHeader->source.sSourceId);
    if(itClock != m_mClocks.end())
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
        itClock = m_mClocks.insert(std::make_pair(pHeader->source.sSourceId, std::make_shared<PtpV2Clock>(pHeader, pPayload))).first;
        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->ClockAdded(itClock->second);
            pHandler->AnnounceSent(itClock->second);
        }
    }
}


time_s_ns PtpMonkeyImplementation::GetPtpTime() const
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

void PtpMonkeyImplementation::ResyncToMaster()
{
    if(m_pLocal)
    {
        m_pLocal->ResyncToMaster();
    }
}


std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator PtpMonkeyImplementation::GetClocksBegin() const
{
    return m_mClocks.begin();
}

std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator PtpMonkeyImplementation::GetClocksEnd() const
{
    return m_mClocks.end();
}

std::shared_ptr<const PtpV2Clock> PtpMonkeyImplementation::GetSyncMasterClock() const
{
    std::lock_guard<std::mutex> lg(m_mutex);

    return m_pSyncMaster;
}

std::string PtpMonkeyImplementation::GetMasterClockId() const
{
    std::lock_guard<std::mutex> lg(m_mutex);

    if(m_pSyncMaster)
    {
        return m_pSyncMaster->GetGrandmasterClockId();
    }
    return "";
}


time_s_ns PtpMonkeyImplementation::GetPtpOffset(PtpV2Clock::enumCalc eCalc) const
{
    std::lock_guard<std::mutex> lg(m_mutex);
    if(m_pLocal)
    {
        return m_pLocal->GetOffset(eCalc);
    }
    else
    {
        return TIMEZERO;
    }
}

time_s_ns PtpMonkeyImplementation::GetPtpDelay(PtpV2Clock::enumCalc eCalc) const
{
    std::lock_guard<std::mutex> lg(m_mutex);
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

    auto now = TimeNow();
    for(auto itClock  = m_mClocks.begin(); itClock != m_mClocks.end();)
    {
        if((TimeToNano(now) - TimeToNano(itClock->second->GetLastMessageTime())) > 5*1e9)
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
}

void PtpMonkeyImplementation::Restart()
{
    if(m_context.stopped())
    {
        m_context.restart();
        Run();
    }
}

bool PtpMonkeyImplementation::IsStopped()
{
    return m_context.stopped();
}


std::shared_ptr<const PtpV2Clock> PtpMonkeyImplementation::GetClock(const std::string& sClockId) const
{
    auto itClock = m_mClocks.find(sClockId);
    if(itClock != m_mClocks.end())
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


    pml::Log::Get(pml::Log::LOG_DEBUG) << tsi.so_timestamping << std::endl;
    if(tsi.so_timestamping & SOF_TIMESTAMPING_TX_HARDWARE)
    {
        nSupports |= TIMESTAMP_TX_HARDWARE;
        pml::Log::Get(pml::Log::LOG_DEBUG) << ifr.ifr_name << " supports harware tx" << std::endl;
    }
    if(tsi.so_timestamping & SOF_TIMESTAMPING_TX_SOFTWARE)
    {
        nSupports |= TIMESTAMP_TX_SOFTWARE;
        pml::Log::Get(pml::Log::LOG_DEBUG) << ifr.ifr_name << " supports software tx" << std::endl;
    }

    if(tsi.so_timestamping & SOF_TIMESTAMPING_RX_HARDWARE)
    {
        nSupports |= TIMESTAMP_RX_HARDWARE;
        pml::Log::Get(pml::Log::LOG_DEBUG) << ifr.ifr_name << " supports harware rx" << std::endl;
    }
    if(tsi.so_timestamping & SOF_TIMESTAMPING_RX_SOFTWARE)
    {
        nSupports |= TIMESTAMP_RX_SOFTWARE;
        pml::Log::Get(pml::Log::LOG_DEBUG) << ifr.ifr_name << " supports software rx" << std::endl;
    }
    #endif // __GNU__
    return nSupports;
}
