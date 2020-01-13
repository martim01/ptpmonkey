#include "ptpmonkey.h"
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

PtpMonkey::PtpMonkey(const std::string& sLocalIpAddress, unsigned char nDomain)  :
    m_sLocalIpAddress(sLocalIpAddress),
    m_nDomain(nDomain),
    m_pMaster(nullptr)
{
}

void PtpMonkey::AddEventHandler(std::shared_ptr<PtpEventHandler> pHandler)
{
    m_lstEventHandler.push_back(pHandler);
}

void PtpMonkey::Run()
{
    std::thread t([this]()
    {
        try
        {
            std::stringstream ssMulticast;
            ssMulticast << "224.0.1." << (static_cast<unsigned int>(m_nDomain)+129);

            std::shared_ptr<Handler> pHandler = std::make_shared<PtpMonkeyHandler>(*this);

            std::shared_ptr<Parser> pParser = std::make_shared<PtpParser>(pHandler);
           // pParser->AddHandler(std::make_shared<PtpLogHandler>());

            asio::io_context io_context;
            Receiver r319(io_context, pParser);
            r319.run(asio::ip::make_address("0.0.0.0"),asio::ip::make_address(ssMulticast.str()), 319);

            Receiver r320(io_context, pParser);
            r320.run(asio::ip::make_address("0.0.0.0"),asio::ip::make_address(ssMulticast.str()), 320);

            Sender sDelay(*this, io_context, asio::ip::address_v4::from_string(m_sLocalIpAddress), asio::ip::make_address(ssMulticast.str()), 319);
            io_context.run();
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }
    });
    t.detach();
}


void PtpMonkey::AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp)
{
    //find the master clock and add the value to it
    if(m_pMaster)
    {
        m_pMaster->AddDelayRequest(nSequence, timestamp);
    }

}

std::map<std::string, std::shared_ptr<PtpV2Clock> >::iterator PtpMonkey::GetOrCreateClock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    auto itClock = m_mClocks.find(pHeader->source.sSourceId);
    if(itClock == m_mClocks.end())
    {
        itClock = m_mClocks.insert(std::make_pair(pHeader->source.sSourceId, std::make_shared<PtpV2Clock>(pHeader, pPayload))).first;

        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->ClockAdded(itClock->second);
        }
    }
    return itClock;
}

void PtpMonkey::Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    //find the clock....
    auto itClock = GetOrCreateClock(pHeader, pPayload);
    if(itClock != m_mClocks.end())
    {
        itClock->second->Sync(pHeader, pPayload);
        if(m_pMaster != itClock->second)
        {
            for(auto pHandler : m_lstEventHandler)
            {
                pHandler->ClockBecomeMaster(itClock->second);
                if(m_pMaster)
                {
                    pHandler->ClockBecomeSlave(m_pMaster);
                }
            }

            m_pMaster = itClock->second;
        }
    }

}

void PtpMonkey::DelayRequest(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload)
{
    auto itClock = GetOrCreateClock(pHeader, pPayload);
}

void PtpMonkey::DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload)
{
    auto itClock = GetOrCreateClock(pHeader, pPayload);

    if(itClock != m_mClocks.end())
    {
        itClock->second->DelayResponse(pHeader, pPayload);
        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->ClockTimeCalculated(itClock->second);
        }

        if(m_pMaster != itClock->second)
        {
            for(auto pHandler : m_lstEventHandler)
            {
                pHandler->ClockBecomeMaster(itClock->second);
                if(m_pMaster)
                {
                    pHandler->ClockBecomeSlave(m_pMaster);
                }
            }
            m_pMaster = itClock->second;
        }
    }
}

void PtpMonkey::Announce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pPayload)
{
    auto itClock = m_mClocks.find(pHeader->source.sSourceId);
    if(itClock != m_mClocks.end())
    {
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
        m_mClocks.insert(std::make_pair(pHeader->source.sSourceId, std::make_shared<PtpV2Clock>(pHeader, pPayload)));
        for(auto pHandler : m_lstEventHandler)
        {
            pHandler->ClockAdded(itClock->second);
        }
    }
}


time_s_ns PtpMonkey::GetPtpTime() const
{
    if(m_pMaster)
    {
        return m_pMaster->GetPtpTime();
    }
    else
    {
        return std::make_pair(std::chrono::seconds(0),std::chrono::nanoseconds(0));
    }
}


std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator PtpMonkey::GetClocksBegin() const
{
    return m_mClocks.begin();
}

std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator PtpMonkey::GetClocksEnd() const
{
    return m_mClocks.end();
}

std::shared_ptr<const PtpV2Clock> PtpMonkey::GetMasterClock() const
{
    return m_pMaster;
}

std::string PtpMonkey::GetMasterClockId() const
{
    if(m_pMaster)
    {
        return m_pMaster->GetClockId();
    }
    return "";
}


time_s_ns PtpMonkey::GetPtpOffset() const
{
    if(m_pMaster)
    {
        return m_pMaster->GetOffset();
    }
    else
    {
        return std::make_pair(std::chrono::seconds(0),std::chrono::nanoseconds(0));
    }
}

time_s_ns PtpMonkey::GetPtpDelay() const
{
    if(m_pMaster)
    {
        return m_pMaster->GetDelay();
    }
    else
    {
        return std::make_pair(std::chrono::seconds(0),std::chrono::nanoseconds(0));
    }
}