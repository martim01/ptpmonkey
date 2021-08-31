#include "ptpmonkey.h"
#include "ptpmonkeyimplementation.h"
#include <iostream>
#include <cstring>




using namespace ptpmonkey;
void PtpMonkeyImplDeleter::operator()(PtpMonkeyImplementation* p)
{
    delete p;
}


PtpMonkey::PtpMonkey(const IpAddress& ipAddress, unsigned char nDomain, unsigned short nSampleSize,Rate enumDelayRequest) :
 m_pImpl(new PtpMonkeyImplementation(ipAddress, nDomain, nSampleSize, enumDelayRequest))
{
}

PtpMonkey::PtpMonkey(const IpInterface& ipInterface, unsigned char nDomain, unsigned short nSampleSize,Rate enumDelayRequest) :
 m_pImpl(new PtpMonkeyImplementation(ipInterface, nDomain, nSampleSize, enumDelayRequest))
{
}

void PtpMonkey::AddEventHandler(std::shared_ptr<PtpEventHandler> pHandler)
{
    m_pImpl->AddEventHandler(pHandler);

}

bool PtpMonkey::Run()
{
    return m_pImpl->Run();
}


std::chrono::nanoseconds PtpMonkey::GetPtpTime() const
{
    return m_pImpl->GetPtpTime();
}

bool PtpMonkey::IsSyncedToMaster() const
{
    return m_pImpl->IsSyncedToMaster();
}

void PtpMonkey::ResyncToMaster()
{
    return m_pImpl->ResyncToMaster();
}


std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator PtpMonkey::GetClocksBegin() const
{
    return m_pImpl->GetClocksBegin();
}

std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator PtpMonkey::GetClocksEnd() const
{
    return m_pImpl->GetClocksEnd();
}

std::shared_ptr<const PtpV2Clock> PtpMonkey::GetSyncMasterClock() const
{
    return m_pImpl->GetSyncMasterClock();
}

std::string PtpMonkey::GetMasterClockId() const
{
    return m_pImpl->GetMasterClockId();
}

std::shared_ptr<const PtpV2Clock> PtpMonkey::GetLocalClock() const
{
    return m_pImpl->GetLocalClock();
}

std::string PtpMonkey::GetLocalClockId() const
{
    return m_pImpl->GetLocalClockId();
}




std::chrono::nanoseconds PtpMonkey::GetPtpOffset() const
{
    return m_pImpl->GetPtpOffset();
}

std::chrono::nanoseconds PtpMonkey::GetLastPtpOffset() const
{
    return m_pImpl->GetPtpOffset(PtpV2Clock::CURRENT);
}

std::chrono::nanoseconds PtpMonkey::GetPtpDelay() const
{
    return m_pImpl->GetPtpDelay();
}


void PtpMonkey::Stop()
{
    m_pImpl->Stop();
}

void PtpMonkey::Restart()
{
    m_pImpl->Restart();
}

bool PtpMonkey::IsStopped()
{
    return m_pImpl->IsStopped();
}

std::shared_ptr<const PtpV2Clock> PtpMonkey::GetClock(const std::string& sClockId) const
{
    return m_pImpl->GetClock(sClockId);
}



int PtpMonkey::GetTimestampingSupported(const IpInterface& interface)
{
    return PtpMonkeyImplementation::GetTimestampingSupported(interface);
}


void PtpMonkey::ResetLocalClockStats()
{
    m_pImpl->ResetLocalClockStats();
}
