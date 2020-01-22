#include "ptpmonkey.h"
#include "ptpmonkeyimplementation.h"

void PtpMonkeyImplDeleter::operator()(PtpMonkeyImplementation* p)
{
    delete p;
}


PtpMonkey::PtpMonkey(const IpAddress& ipAddress, unsigned char nDomain, Rate enumDelayRequest) :
 m_pImpl(new PtpMonkeyImplementation(ipAddress, nDomain, enumDelayRequest))
{
}

PtpMonkey::PtpMonkey(const IpInterface& ipInterface, unsigned char nDomain, Rate enumDelayRequest) :
 m_pImpl(new PtpMonkeyImplementation(ipInterface, nDomain, enumDelayRequest))
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


time_s_ns PtpMonkey::GetPtpTime() const
{
    return m_pImpl->GetPtpTime();
}


std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator PtpMonkey::GetClocksBegin() const
{
    return m_pImpl->GetClocksBegin();
}

std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator PtpMonkey::GetClocksEnd() const
{
    return m_pImpl->GetClocksEnd();
}

std::shared_ptr<const PtpV2Clock> PtpMonkey::GetMasterClock() const
{
    return m_pImpl->GetMasterClock();
}

std::string PtpMonkey::GetMasterClockId() const
{
    return m_pImpl->GetMasterClockId();
}


time_s_ns PtpMonkey::GetPtpOffset() const
{
    return m_pImpl->GetPtpOffset();
}

time_s_ns PtpMonkey::GetPtpDelay() const
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
