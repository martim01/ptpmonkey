#include "ptpeventloghandler.h"
#include <iostream>
#include <iomanip>
#include "ptpclock.h"
#include "log.h"
using namespace ptpmonkey;

void PtpEventLogHandler::ClockAdded(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog() << "---------NEW CLOCK---------";
    pmlLog() << "Id:\t\t" << pClock->GetId() ;
    pmlLog() << "Domain:\t\t" << std::dec << (int)pClock->GetDomain();
    pmlLog() << "IP Address:\t\t" << pClock->GetIpAddress();
    pmlLog() << "Offset:\t\t" << (int)pClock->GetUtcOffset() ;
    pmlLog() << "Priority1:\t" << (int)pClock->GetPriority1() ;
    pmlLog() << "Class:\t\t" << (int)pClock->GetClass() ;
    pmlLog() << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy() ;
    pmlLog() << "Variance:\t" << std::dec << (int)pClock->GetVariance() ;
    pmlLog() << "Priority2:\t" << (int)pClock->GetPriority2() ;
    pmlLog() << "Steps:\t\t" << (int)pClock->GetStepsRemoved() ;
    pmlLog() << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource() ;
    pmlLog() << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags(ptpV2Header::ANNOUNCE) ;
    pmlLog() << (pClock->IsGrandMaster() ? "Master" : "Slave");
    pmlLog() << (pClock->IsSyncMaster() ? "Sync Master" : "Slave");
    pmlLog() << "---------------------------";
}

void PtpEventLogHandler::ClockUpdated(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog() << "-----UPDATED CLOCK---------";
    pmlLog() << "Id:\t\t" << pClock->GetId() ;
    pmlLog() << "Domain:\t\t" << std::dec << (int)pClock->GetDomain();
    pmlLog() << "IP Address:\t\t" << pClock->GetIpAddress();
    pmlLog() << "Offset:\t\t" << (int)pClock->GetUtcOffset() ;
    pmlLog() << "Priority1:\t" << (int)pClock->GetPriority1() ;
    pmlLog() << "Class:\t\t" << (int)pClock->GetClass() ;
    pmlLog() << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy() ;
    pmlLog() << "Variance:\t" << std::dec << (int)pClock->GetVariance() ;
    pmlLog() << "Priority2:\t" << (int)pClock->GetPriority2() ;
    pmlLog() << "Steps:\t\t" << (int)pClock->GetStepsRemoved() ;
    pmlLog() << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource() ;
    pmlLog() << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags(ptpV2Header::ANNOUNCE) ;
    pmlLog() << (pClock->IsGrandMaster() ? "Master" : "Slave");
    pmlLog() << (pClock->IsSyncMaster() ? "Sync Master" : "Slave");
    pmlLog() << "---------------------------";
}

void PtpEventLogHandler::ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog() << "------MASTER CLOCK---------";
    pmlLog() << "Id:\t" << pClock->GetId() ;
    pmlLog() << "IP Address:\t\t" << pClock->GetIpAddress();
    pmlLog() << "---------------------------";
}

void PtpEventLogHandler::ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog() << "-------SLAVE CLOCK---------";
    pmlLog() << "Id:\t" << pClock->GetId() ;
    pmlLog() << "IP Address:\t\t" << pClock->GetIpAddress();
    pmlLog() << "---------------------------";
}

void PtpEventLogHandler::ClockRemoved(std::shared_ptr<PtpV2Clock> pClock)
{

}

void PtpEventLogHandler::ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog() << "Time Calculated :\t\t" << pClock->GetId();
    pmlLog() << TimeToIsoString(pClock->GetLastPtpTime()) << "\tat " << TimeToIsoString(pClock->GetLastCalculatedTime()) << "\t" << TimeToString(pClock->GetOffset(PtpV2Clock::CURRENT));
    pmlLog() << "Sync: " << (pClock->IsSynced() ? "YES" : "NO");
    pmlLog() << "Requests: " << pClock->GetCount(ptpV2Header::DELAY_REQ) << " interval: " << std::hex << (int)pClock->GetInterval(ptpV2Header::DELAY_REQ) << std::dec;
    pmlLog() << "Responses: " << pClock->GetCount(ptpV2Header::DELAY_RESP) << " interval: " << std::hex << (int)pClock->GetInterval(ptpV2Header::DELAY_RESP) << std::dec;
    pmlLog() << "Delay_Mean:" << TimeToString(pClock->GetDelay(PtpV2Clock::MEAN));
    pmlLog() << "DelayRange :" << TimeToString(pClock->GetDelay(PtpV2Clock::MIN)) << "-" << TimeToString(pClock->GetDelay(PtpV2Clock::MAX)) << "=" << TimeToString((pClock->GetDelay(PtpV2Clock::MAX)-pClock->GetDelay(PtpV2Clock::MIN)));
    pmlLog() << "Delay SD :" << TimeToString(pClock->GetDelay(PtpV2Clock::SD)) << " " << TimeToString(pClock->GetDelay(PtpV2Clock::SE));
    pmlLog() << "Offset_Current:" << TimeToString(pClock->GetOffset(PtpV2Clock::CURRENT));
    pmlLog() << "Offset_Mean:" << TimeToString(pClock->GetOffset(PtpV2Clock::MEAN));
    pmlLog() << "OffsetRange :" << TimeToString(pClock->GetOffset(PtpV2Clock::MIN)) << "-" << TimeToString(pClock->GetOffset(PtpV2Clock::MAX)) << "=" << TimeToString((pClock->GetOffset(PtpV2Clock::MAX)-pClock->GetOffset(PtpV2Clock::MIN)));
    pmlLog() << "Offset SD :" << TimeToString(pClock->GetOffset(PtpV2Clock::SD)) << " " << TimeToString(pClock->GetOffset(PtpV2Clock::SE));
    pmlLog() << "Offset Slope&C :" << pClock->GetOffsetSlope()*1e9 << "\t" << pClock->GetOffsetIntersection();
    pmlLog() << "---------------------------";
}


void PtpEventLogHandler::AnnounceSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pmlLog() << "Announce :\t\t" << pClock->GetId();
    }
}

void PtpEventLogHandler::SyncSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pmlLog() << "Sync :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::FollowUpSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pmlLog() << "FollowUp :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::DelayRequestSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pmlLog() << "DelayRequest :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::DelayResponseSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pmlLog() << "DelayResponse :\t\t" << pClock->GetId() ;
    }
}
