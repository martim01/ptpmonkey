#include "ptpeventloghandler.h"
#include <iostream>
#include <iomanip>
#include "ptpclock.h"
#include "log.h"
using namespace pml::ptpmonkey;

void PtpEventLogHandler::ClockAdded(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "---------NEW CLOCK---------";
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Id:\t\t" << pClock->GetId() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Domain:\t\t" << std::dec << (int)pClock->GetDomain();
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "IP Address:\t\t" << pClock->GetIpAddress();
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Offset:\t\t" << (int)pClock->GetUtcOffset() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Priority1:\t" << (int)pClock->GetPriority1() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Class:\t\t" << (int)pClock->GetClass() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Variance:\t" << std::dec << (int)pClock->GetVariance() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Priority2:\t" << (int)pClock->GetPriority2() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Steps:\t\t" << (int)pClock->GetStepsRemoved() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags(ptpV2Header::ANNOUNCE) ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << (pClock->IsGrandMaster() ? "Master" : "Slave");
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << (pClock->IsSyncMaster() ? "Sync Master" : "Slave");
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "---------------------------";
}

void PtpEventLogHandler::ClockUpdated(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "-----UPDATED CLOCK---------";
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Id:\t\t" << pClock->GetId() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Domain:\t\t" << std::dec << (int)pClock->GetDomain();
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "IP Address:\t\t" << pClock->GetIpAddress();
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Offset:\t\t" << (int)pClock->GetUtcOffset() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Priority1:\t" << (int)pClock->GetPriority1() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Class:\t\t" << (int)pClock->GetClass() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Variance:\t" << std::dec << (int)pClock->GetVariance() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Priority2:\t" << (int)pClock->GetPriority2() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Steps:\t\t" << (int)pClock->GetStepsRemoved() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags(ptpV2Header::ANNOUNCE) ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << (pClock->IsGrandMaster() ? "Master" : "Slave");
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << (pClock->IsSyncMaster() ? "Sync Master" : "Slave");
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "---------------------------";
}

void PtpEventLogHandler::ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "------MASTER CLOCK---------";
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Id:\t" << pClock->GetId() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "IP Address:\t\t" << pClock->GetIpAddress();
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "---------------------------";
}

void PtpEventLogHandler::ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "-------SLAVE CLOCK---------";
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Id:\t" << pClock->GetId() ;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "IP Address:\t\t" << pClock->GetIpAddress();
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "---------------------------";
}

void PtpEventLogHandler::ClockRemoved(std::shared_ptr<PtpV2Clock> pClock)
{

}

void PtpEventLogHandler::ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock)
{
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Time Calculated :\t\t" << pClock->GetId();
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << TimeToIsoString(pClock->GetLastPtpTime()) << "\tat " << TimeToIsoString(pClock->GetLastCalculatedTime()) << "\t" << TimeToString(pClock->GetOffset(PtpV2Clock::CURRENT));
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Sync: " << (pClock->IsSynced() ? "YES" : "NO");
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Requests: " << pClock->GetCount(ptpV2Header::DELAY_REQ) << " interval: " << std::hex << (int)pClock->GetInterval(ptpV2Header::DELAY_REQ) << std::dec;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Responses: " << pClock->GetCount(ptpV2Header::DELAY_RESP) << " interval: " << std::hex << (int)pClock->GetInterval(ptpV2Header::DELAY_RESP) << std::dec;
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Delay_Mean:" << TimeToString(pClock->GetDelay(PtpV2Clock::MEAN));
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "DelayRange :" << TimeToString(pClock->GetDelay(PtpV2Clock::MIN)) << "-" << TimeToString(pClock->GetDelay(PtpV2Clock::MAX)) << "=" << TimeToString((pClock->GetDelay(PtpV2Clock::MAX)-pClock->GetDelay(PtpV2Clock::MIN)));
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Delay SD :" << TimeToString(pClock->GetDelay(PtpV2Clock::SD)) << " " << TimeToString(pClock->GetDelay(PtpV2Clock::SE));
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Offset_Current:" << TimeToString(pClock->GetOffset(PtpV2Clock::CURRENT));
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Offset_Mean:" << TimeToString(pClock->GetOffset(PtpV2Clock::MEAN));
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "OffsetRange :" << TimeToString(pClock->GetOffset(PtpV2Clock::MIN)) << "-" << TimeToString(pClock->GetOffset(PtpV2Clock::MAX)) << "=" << TimeToString((pClock->GetOffset(PtpV2Clock::MAX)-pClock->GetOffset(PtpV2Clock::MIN)));
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Offset SD :" << TimeToString(pClock->GetOffset(PtpV2Clock::SD)) << " " << TimeToString(pClock->GetOffset(PtpV2Clock::SE));
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Offset Slope&C :" << pClock->GetOffsetSlope()*1e9 << "\t" << pClock->GetOffsetIntersection();
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "---------------------------";
}


void PtpEventLogHandler::AnnounceSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Announce :\t\t" << pClock->GetId();
    }
}

void PtpEventLogHandler::SyncSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Sync :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::FollowUpSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "FollowUp :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::DelayRequestSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "DelayRequest :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::DelayResponseSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "DelayResponse :\t\t" << pClock->GetId() ;
    }
}
