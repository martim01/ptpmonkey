#include "ptpeventloghandler.h"
#include <iostream>
#include <iomanip>
#include "ptpclock.h"
#include "log.h"
using namespace ptpmonkey;

void PtpEventLogHandler::ClockAdded(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::Log::Get() << "---------NEW CLOCK---------" << std::endl;
    pml::Log::Get() << "Id:\t\t" << pClock->GetId()  << std::endl;
    pml::Log::Get() << "Domain:\t\t" << std::dec << (int)pClock->GetDomain() << std::endl;
    pml::Log::Get() << "IP Address:\t\t" << pClock->GetIpAddress() << std::endl;
    pml::Log::Get() << "Offset:\t\t" << (int)pClock->GetUtcOffset()  << std::endl;
    pml::Log::Get() << "Priority1:\t" << (int)pClock->GetPriority1()  << std::endl;
    pml::Log::Get() << "Class:\t\t" << (int)pClock->GetClass()  << std::endl;
    pml::Log::Get() << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy()  << std::endl;
    pml::Log::Get() << "Variance:\t" << std::dec << (int)pClock->GetVariance()  << std::endl;
    pml::Log::Get() << "Priority2:\t" << (int)pClock->GetPriority2()  << std::endl;
    pml::Log::Get() << "Steps:\t\t" << (int)pClock->GetStepsRemoved()  << std::endl;
    pml::Log::Get() << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource()  << std::endl;
    pml::Log::Get() << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags(ptpV2Header::ANNOUNCE)  << std::endl;
    pml::Log::Get() << (pClock->IsMaster() ? "Master" : "Slave") << std::endl;
    pml::Log::Get() << "---------------------------" << std::endl;
}

void PtpEventLogHandler::ClockUpdated(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::Log::Get() << "-----UPDATED CLOCK---------" << std::endl;
    pml::Log::Get() << "Id:\t\t" << pClock->GetId()  << std::endl;
    pml::Log::Get() << "Domain:\t\t" << std::dec << (int)pClock->GetDomain() << std::endl;
    pml::Log::Get() << "IP Address:\t\t" << pClock->GetIpAddress() << std::endl;
    pml::Log::Get() << "Offset:\t\t" << (int)pClock->GetUtcOffset()  << std::endl;
    pml::Log::Get() << "Priority1:\t" << (int)pClock->GetPriority1()  << std::endl;
    pml::Log::Get() << "Class:\t\t" << (int)pClock->GetClass()  << std::endl;
    pml::Log::Get() << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy()  << std::endl;
    pml::Log::Get() << "Variance:\t" << std::dec << (int)pClock->GetVariance()  << std::endl;
    pml::Log::Get() << "Priority2:\t" << (int)pClock->GetPriority2()  << std::endl;
    pml::Log::Get() << "Steps:\t\t" << (int)pClock->GetStepsRemoved()  << std::endl;
    pml::Log::Get() << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource()  << std::endl;
    pml::Log::Get() << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags(ptpV2Header::ANNOUNCE)  << std::endl;
    pml::Log::Get() << (pClock->IsMaster() ? "Master" : "Slave") << std::endl;
    pml::Log::Get() << "---------------------------" << std::endl;
}

void PtpEventLogHandler::ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::Log::Get() << "------MASTER CLOCK---------" << std::endl;
    pml::Log::Get() << "Id:\t" << pClock->GetId()  << std::endl;
    pml::Log::Get() << "IP Address:\t\t" << pClock->GetIpAddress() << std::endl;
    pml::Log::Get() << "---------------------------" << std::endl;
}

void PtpEventLogHandler::ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::Log::Get() << "-------SLAVE CLOCK---------" << std::endl;
    pml::Log::Get() << "Id:\t" << pClock->GetId()  << std::endl;
    pml::Log::Get() << "IP Address:\t\t" << pClock->GetIpAddress() << std::endl;
    pml::Log::Get() << "---------------------------" << std::endl;
}

void PtpEventLogHandler::ClockRemoved(std::shared_ptr<PtpV2Clock> pClock)
{

}

void PtpEventLogHandler::ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::Log::Get() << "Time Calculated :\t\t" << pClock->GetId() << std::endl;
    pml::Log::Get() << TimeToIsoString(pClock->GetPtpTime()) << "\tat " << TimeToIsoString(TimeNow()) << "\t" << TimeToString(TimeNow()-pClock->GetPtpTime()) << std::endl;
    pml::Log::Get() << "Requests: " << pClock->GetCount(ptpV2Header::DELAY_REQ) << " interval: " << std::hex << (int)pClock->GetInterval(ptpV2Header::DELAY_REQ) << std::dec << std::endl;
    pml::Log::Get() << "Responses: " << pClock->GetCount(ptpV2Header::DELAY_RESP) << " interval: " << std::hex << (int)pClock->GetInterval(ptpV2Header::DELAY_RESP) << std::dec << std::endl;
    pml::Log::Get() << "Delay_Set:" << TimeToString(pClock->GetDelay(PtpV2Clock::SET)) << std::endl;
    pml::Log::Get() << "Delay_Mean:" << TimeToString(pClock->GetDelay(PtpV2Clock::MEAN)) << std::endl;
    pml::Log::Get() << "DelayRange :" << TimeToString(pClock->GetDelay(PtpV2Clock::MIN)) << "-" << TimeToString(pClock->GetDelay(PtpV2Clock::MAX)) << "=" << TimeToString((pClock->GetDelay(PtpV2Clock::MAX)-pClock->GetDelay(PtpV2Clock::MIN))) << std::endl;
    pml::Log::Get() << "Delay Variance :" << TimeToString(pClock->GetDelay(PtpV2Clock::SET_VARIANCE)) << " " << TimeToString(pClock->GetDelay(PtpV2Clock::VARIANCE)) << std::endl;
    pml::Log::Get() << "Offset_Set:" << TimeToString(pClock->GetOffset(PtpV2Clock::SET)) << std::endl;
    pml::Log::Get() << "Offset_Diff:" << TimeToString(pClock->GetOffset(PtpV2Clock::SET)-pClock->GetOffset(PtpV2Clock::CURRENT)) << std::endl;
    pml::Log::Get() << "Offset_Mean:" << TimeToString(pClock->GetOffset(PtpV2Clock::MEAN)) << std::endl;
    pml::Log::Get() << "OffsetRange :" << TimeToString(pClock->GetOffset(PtpV2Clock::MIN)) << "-" << TimeToString(pClock->GetOffset(PtpV2Clock::MAX)) << "=" << TimeToString((pClock->GetOffset(PtpV2Clock::MAX)-pClock->GetOffset(PtpV2Clock::MIN))) << std::endl;
    pml::Log::Get() << "Offset Variance :" << TimeToString(pClock->GetOffset(PtpV2Clock::SET_VARIANCE)) << " " << TimeToString(pClock->GetOffset(PtpV2Clock::VARIANCE)) << std::endl;
    pml::Log::Get() << "---------------------------" << std::endl;
}

void PtpEventLogHandler::AnnounceSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::Log::Get() << "Announce :\t\t" << pClock->GetId() << std::endl;
    }
}

void PtpEventLogHandler::SyncSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::Log::Get() << "Sync :\t\t" << pClock->GetId()  << std::endl;
    }
}

void PtpEventLogHandler::FollowUpSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::Log::Get() << "FollowUp :\t\t" << pClock->GetId()  << std::endl;
    }
}

void PtpEventLogHandler::DelayRequestSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::Log::Get() << "DelayRequest :\t\t" << pClock->GetId()  << std::endl;
    }
}

void PtpEventLogHandler::DelayResponseSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::Log::Get() << "DelayResponse :\t\t" << pClock->GetId()  << std::endl;
    }
}
