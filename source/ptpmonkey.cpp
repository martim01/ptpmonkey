#include "ptpmonkey.h"
#include "ptpmonkeyimplementation.h"
#include <iostream>
#include <cstring>




using namespace pml::ptpmonkey;
void PtpMonkeyImplDeleter::operator()(PtpMonkeyImplementation* p)
{
    delete p;
}


PtpMonkey::PtpMonkey(const IpAddress& ipAddress, unsigned char nDomain, unsigned short nSampleSize,Mode mode, Rate enumDelayRequest) :
 m_pImpl(new PtpMonkeyImplementation(ipAddress, nDomain, nSampleSize, mode, enumDelayRequest))
{
}

PtpMonkey::PtpMonkey(const IpInterface& ipInterface, unsigned char nDomain, unsigned short nSampleSize, Mode mode, Rate enumDelayRequest) :
 m_pImpl(new PtpMonkeyImplementation(ipInterface, nDomain, nSampleSize, mode, enumDelayRequest))
{
}

void PtpMonkey::AddEventHandler(std::shared_ptr<PtpEventHandler> pHandler)
{
    m_pImpl->AddEventHandler(pHandler);

}

bool PtpMonkey::Run(bool bThreaded)
{
    return m_pImpl->Run(bThreaded);
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


const std::map<std::string, std::shared_ptr<PtpV2Clock> >& PtpMonkey::GetClocks() const
{
    return m_pImpl->GetClocks();
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

bool PtpMonkey::IsStopped() const
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

Mode PtpMonkey::GetMode() const
{
    return m_pImpl->GetMode();
}

void PtpMonkey::SetDomain(unsigned char nDomain)
{
    m_pImpl->SetDomain(nDomain);
}

unsigned char PtpMonkey::GetDomain() const
{
    return m_pImpl->GetDomain();
}
