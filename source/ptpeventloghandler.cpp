#include "ptpeventloghandler.h"
#include <iostream>
#include <iomanip>
#include "ptpclock.h"
#include "log.h"
using namespace pml::ptpmonkey;

void PtpEventLogHandler::ClockAdded(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "---------NEW CLOCK---------";
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Id:\t\t" << pClock->GetId() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Domain:\t\t" << std::dec << (int)pClock->GetDomain();
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "IP Address:\t\t" << pClock->GetIpAddress();
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Offset:\t\t" << (int)pClock->GetUtcOffset() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Priority1:\t" << (int)pClock->GetPriority1() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Class:\t\t" << (int)pClock->GetClass() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Variance:\t" << std::dec << (int)pClock->GetVariance() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Priority2:\t" << (int)pClock->GetPriority2() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Steps:\t\t" << (int)pClock->GetStepsRemoved() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags(hdr::enumType::ANNOUNCE) ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << (pClock->IsGrandMaster() ? "Master" : "Slave");
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << (pClock->IsSyncMaster() ? "Sync Master" : "Slave");
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "---------------------------";
}

void PtpEventLogHandler::ClockUpdated(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "-----UPDATED CLOCK---------";
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Id:\t\t" << pClock->GetId() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Domain:\t\t" << std::dec << (int)pClock->GetDomain();
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "IP Address:\t\t" << pClock->GetIpAddress();
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Offset:\t\t" << (int)pClock->GetUtcOffset() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Priority1:\t" << (int)pClock->GetPriority1() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Class:\t\t" << (int)pClock->GetClass() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Variance:\t" << std::dec << (int)pClock->GetVariance() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Priority2:\t" << (int)pClock->GetPriority2() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Steps:\t\t" << (int)pClock->GetStepsRemoved() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags(hdr::enumType::ANNOUNCE) ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << (pClock->IsGrandMaster() ? "Master" : "Slave");
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << (pClock->IsSyncMaster() ? "Sync Master" : "Slave");
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "---------------------------";
}

void PtpEventLogHandler::ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "------MASTER CLOCK---------";
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Id:\t" << pClock->GetId() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "IP Address:\t\t" << pClock->GetIpAddress();
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "---------------------------";
}

void PtpEventLogHandler::ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "-------SLAVE CLOCK---------";
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Id:\t" << pClock->GetId() ;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "IP Address:\t\t" << pClock->GetIpAddress();
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "---------------------------";
}

void PtpEventLogHandler::ClockRemoved(std::shared_ptr<PtpV2Clock> pClock)
{

}

void PtpEventLogHandler::ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock)
{
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Time Calculated :\t\t" << pClock->GetId();
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << TimeToIsoString(pClock->GetLastPtpTime()) << "\tat " << TimeToIsoString(pClock->GetLastCalculatedTime()) << "\t" << TimeToString(pClock->GetOffset(PtpV2Clock::CURRENT));
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Sync: " << (pClock->IsSynced() ? "YES" : "NO");
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Requests: " << pClock->GetCount(hdr::enumType::DELAY_REQ) << " interval: " << std::hex << (int)pClock->GetInterval(hdr::enumType::DELAY_REQ) << std::dec;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Responses: " << pClock->GetCount(hdr::enumType::DELAY_RESP) << " interval: " << std::hex << (int)pClock->GetInterval(hdr::enumType::DELAY_RESP) << std::dec;
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Delay_Mean:" << TimeToString(pClock->GetDelay(PtpV2Clock::MEAN));
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "DelayRange :" << TimeToString(pClock->GetDelay(PtpV2Clock::MIN)) << "-" << TimeToString(pClock->GetDelay(PtpV2Clock::MAX)) << "=" << TimeToString((pClock->GetDelay(PtpV2Clock::MAX)-pClock->GetDelay(PtpV2Clock::MIN)));
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Delay SD :" << TimeToString(pClock->GetDelay(PtpV2Clock::SD)) << " " << TimeToString(pClock->GetDelay(PtpV2Clock::SE));
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Offset_Current:" << TimeToString(pClock->GetOffset(PtpV2Clock::CURRENT));
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Offset_Mean:" << TimeToString(pClock->GetOffset(PtpV2Clock::MEAN));
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "OffsetRange :" << TimeToString(pClock->GetOffset(PtpV2Clock::MIN)) << "-" << TimeToString(pClock->GetOffset(PtpV2Clock::MAX)) << "=" << TimeToString((pClock->GetOffset(PtpV2Clock::MAX)-pClock->GetOffset(PtpV2Clock::MIN)));
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Offset SD :" << TimeToString(pClock->GetOffset(PtpV2Clock::SD)) << " " << TimeToString(pClock->GetOffset(PtpV2Clock::SE));
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Offset Slope&C :" << pClock->GetOffsetSlope()*1e9 << "\t" << pClock->GetOffsetIntersection();
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "---------------------------";
}


void PtpEventLogHandler::AnnounceSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Announce :\t\t" << pClock->GetId();
    }
}

void PtpEventLogHandler::SyncSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Sync :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::FollowUpSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "FollowUp :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::DelayRequestSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "DelayRequest :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::DelayResponseSent(std::shared_ptr<PtpV2Clock> pClock)
{
    if(m_bVerbose)
    {
        pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "DelayResponse :\t\t" << pClock->GetId() ;
    }
}

void PtpEventLogHandler::ManagementMessageReceived(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpManagement> pMessage)
{
    pMessage->OutputValues();
}