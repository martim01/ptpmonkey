#include "ptpeventloghandler.h"
#include <iostream>
#include <iomanip>
#include "ptpclock.h"
#include "log.h"
using namespace ptpmonkey;

void PtpEventLogHandler::ClockAdded(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::Log() << "---------NEW CLOCK---------";
    pml::Log() << "Id:\t\t" << pClock->GetId() ;
    pml::Log() << "Domain:\t\t" << std::dec << (int)pClock->GetDomain();
    pml::Log() << "IP Address:\t\t" << pClock->GetIpAddress();
    pml::Log() << "Offset:\t\t" << (int)pClock->GetUtcOffset() ;
    pml::Log() << "Priority1:\t" << (int)pClock->GetPriority1() ;
    pml::Log() << "Class:\t\t" << (int)pClock->GetClass() ;
    pml::Log() << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy() ;
    pml::Log() << "Variance:\t" << std::dec << (int)pClock->GetVariance() ;
    pml::Log() << "Priority2:\t" << (int)pClock->GetPriority2() ;
    pml::Log() << "Steps:\t\t" << (int)pClock->GetStepsRemoved() ;
    pml::Log() << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource() ;
    pml::Log() << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags(ptpV2Header::ANNOUNCE) ;
    pml::Log() << (pClock->IsGrandMaster() ? "Master" : "Slave");
    pml::Log() << (pClock->IsSyncMaster() ? "Sync Master" : "Slave");
    pml::Log() << "---------------------------";
}

void PtpEventLogHandler::ClockUpdated(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::Log() << "-----UPDATED CLOCK---------";
    pml::Log() << "Id:\t\t" << pClock->GetId() ;
    pml::Log() << "Domain:\t\t" << std::dec << (int)pClock->GetDomain();
    pml::Log() << "IP Address:\t\t" << pClock->GetIpAddress();
    pml::Log() << "Offset:\t\t" << (int)pClock->GetUtcOffset() ;
    pml::Log() << "Priority1:\t" << (int)pClock->GetPriority1() ;
    pml::Log() << "Class:\t\t" << (int)pClock->GetClass() ;
    pml::Log() << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy() ;
    pml::Log() << "Variance:\t" << std::dec << (int)pClock->GetVariance() ;
    pml::Log() << "Priority2:\t" << (int)pClock->GetPriority2() ;
    pml::Log() << "Steps:\t\t" << (int)pClock->GetStepsRemoved() ;
    pml::Log() << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource() ;
    pml::Log() << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags(ptpV2Header::ANNOUNCE) ;
    pml::Log() << (pClock->IsGrandMaster() ? "Master" : "Slave");
    pml::Log() << (pClock->IsSyncMaster() ? "Sync Master" : "Slave");
    pml::Log() << "---------------------------";
}

void PtpEventLogHandler::ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::Log() << "------MASTER CLOCK---------";
    pml::Log() << "Id:\t" << pClock->GetId() ;
    pml::Log() << "IP Address:\t\t" << pClock->GetIpAddress();
    pml::Log() << "---------------------------";
}

void PtpEventLogHandler::ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::Log() << "-------SLAVE CLOCK---------";
    pml::Log() << "Id:\t" << pClock->GetId() ;
    pml::Log() << "IP Address:\t\t" << pClock->GetIpAddress();
    pml::Log() << "---------------------------";
}

void PtpEventLogHandler::ClockRemoved(std::shared_ptr<PtpV2Clock> pClock)
{

}

void PtpEventLogHandler::ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::Log() << "Time Calculated :\t\t" << pClock->GetId();
    pml::Log() << TimeToIsoString(pClock->GetPtpTime()) << "\tat " << TimeToIsoString(Now()) << "\t" << TimeToString(Now()-pClock->GetPtpTime());
    pml::Log() << "Requests: " << pClock->GetCount(ptpV2Header::DELAY_REQ) << " interval: " << std::hex << (int)pClock->GetInterval(ptpV2Header::DELAY_REQ) << std::dec;
    pml::Log() << "Responses: " << pClock->GetCount(ptpV2Header::DELAY_RESP) << " interval: " << std::hex << (int)pClock->GetInterval(ptpV2Header::DELAY_RESP) << std::dec;
    pml::Log() << "Delay_Set:" << TimeToString(pClock->GetDelay(PtpV2Clock::SET));
    pml::Log() << "Delay_Mean:" << TimeToString(pClock->GetDelay(PtpV2Clock::MEAN));
    pml::Log() << "DelayRange :" << TimeToString(pClock->GetDelay(PtpV2Clock::MIN)) << "-" << TimeToString(pClock->GetDelay(PtpV2Clock::MAX)) << "=" << TimeToString((pClock->GetDelay(PtpV2Clock::MAX)-pClock->GetDelay(PtpV2Clock::MIN)));
    pml::Log() << "Delay Variance :" << TimeToString(pClock->GetDelay(PtpV2Clock::SET_VARIANCE)) << " " << TimeToString(pClock->GetDelay(PtpV2Clock::VARIANCE));
    pml::Log() << "Offset_Set:" << TimeToString(pClock->GetOffset(PtpV2Clock::SET));
    pml::Log() << "Offset_Diff:" << TimeToString(pClock->GetOffset(PtpV2Clock::SET)-pClock->GetOffset(PtpV2Clock::CURRENT));
    pml::Log() << "Offset_Mean:" << TimeToString(pClock->GetOffset(PtpV2Clock::MEAN));
    pml::Log() << "OffsetRange :" << TimeToString(pClock->GetOffset(PtpV2Clock::MIN)) << "-" << TimeToString(pClock->GetOffset(PtpV2Clock::MAX)) << "=" << TimeToString((pClock->GetOffset(PtpV2Clock::MAX)-pClock->GetOffset(PtpV2Clock::MIN)));
    pml::Log() << "Offset Variance :" << TimeToString(pClock->GetOffset(PtpV2Clock::SET_VARIANCE)) << " " << TimeToString(pClock->GetOffset(PtpV2Clock::VARIANCE));
    pml::Log() << "---------------------------";
}

void PtpEventLogHandler::AnnounceSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::Log() << "Announce :\t\t" << pClock->GetId();
    }
}

void PtpEventLogHandler::SyncSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::Log() << "Sync :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::FollowUpSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::Log() << "FollowUp :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::DelayRequestSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::Log() << "DelayRequest :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::DelayResponseSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::Log() << "DelayResponse :\t\t" << pClock->GetId() ;
    }
}
