#include "ptpeventloghandler.h"
#include <iostream>
#include <iomanip>
#include "ptpclock.h"
using namespace ptpmonkey;

void PtpEventLogHandler::ClockAdded(std::shared_ptr<PtpV2Clock> pClock)
{
    std::cout << "---------NEW CLOCK---------" << std::endl;
    std::cout << "Id:\t\t" << pClock->GetId()  << std::endl;
    std::cout << "Domain:\t\t" << std::dec << (int)pClock->GetDomain() << std::endl;
    std::cout << "IP Address:\t\t" << pClock->GetIpAddress() << std::endl;
    std::cout << "Offset:\t\t" << (int)pClock->GetUtcOffset()  << std::endl;
    std::cout << "Priority1:\t" << (int)pClock->GetPriority1()  << std::endl;
    std::cout << "Class:\t\t" << (int)pClock->GetClass()  << std::endl;
    std::cout << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy()  << std::endl;
    std::cout << "Variance:\t" << std::dec << (int)pClock->GetVariance()  << std::endl;
    std::cout << "Priority2:\t" << (int)pClock->GetPriority2()  << std::endl;
    std::cout << "Steps:\t\t" << (int)pClock->GetStepsRemoved()  << std::endl;
    std::cout << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource()  << std::endl;
    std::cout << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags()  << std::endl;
    std::cout << (pClock->IsMaster() ? "Master" : "Slave") << std::endl;
    std::cout << "---------------------------" << std::endl;
}

void PtpEventLogHandler::ClockUpdated(std::shared_ptr<PtpV2Clock> pClock)
{
    std::cout << "-----UPDATED CLOCK---------" << std::endl;
    std::cout << "Id:\t\t" << pClock->GetId()  << std::endl;
    std::cout << "Domain:\t\t" << std::dec << (int)pClock->GetDomain() << std::endl;
    std::cout << "IP Address:\t\t" << pClock->GetIpAddress() << std::endl;
    std::cout << "Offset:\t\t" << (int)pClock->GetUtcOffset()  << std::endl;
    std::cout << "Priority1:\t" << (int)pClock->GetPriority1()  << std::endl;
    std::cout << "Class:\t\t" << (int)pClock->GetClass()  << std::endl;
    std::cout << "Accuracy:\t" << std::hex << "0x" << (int)pClock->GetAccuracy()  << std::endl;
    std::cout << "Variance:\t" << std::dec << (int)pClock->GetVariance()  << std::endl;
    std::cout << "Priority2:\t" << (int)pClock->GetPriority2()  << std::endl;
    std::cout << "Steps:\t\t" << (int)pClock->GetStepsRemoved()  << std::endl;
    std::cout << "Source:\t\t" << std::hex << "0x" << (int)pClock->GetTimeSource()  << std::endl;
    std::cout << "Flags:\t\t" << std::hex << "0x" << (int)pClock->GetFlags()  << std::endl;
    std::cout << (pClock->IsMaster() ? "Master" : "Slave") << std::endl;
    std::cout << "---------------------------" << std::endl;
}

void PtpEventLogHandler::ClockBecomeMaster(std::shared_ptr<PtpV2Clock> pClock)
{
    std::cout << "------MASTER CLOCK---------" << std::endl;
    std::cout << "Id:\t" << pClock->GetId()  << std::endl;
    std::cout << "IP Address:\t\t" << pClock->GetIpAddress() << std::endl;
    std::cout << "---------------------------" << std::endl;
}

void PtpEventLogHandler::ClockBecomeSlave(std::shared_ptr<PtpV2Clock> pClock)
{
    std::cout << "-------SLAVE CLOCK---------" << std::endl;
    std::cout << "Id:\t" << pClock->GetId()  << std::endl;
    std::cout << "IP Address:\t\t" << pClock->GetIpAddress() << std::endl;
    std::cout << "---------------------------" << std::endl;
}

void PtpEventLogHandler::ClockRemoved(std::shared_ptr<PtpV2Clock> pClock)
{

}

void PtpEventLogHandler::ClockTimeCalculated(std::shared_ptr<PtpV2Clock> pClock)
{
    std::cout << TimeToIsoString(pClock->GetPtpTime()) << std::endl;
    std::cout << "Requests: " << pClock->GetCount(ptpV2Header::DELAY_REQ) << " interval: " << std::hex << (int)pClock->GetInterval(ptpV2Header::DELAY_REQ) << std::dec << std::endl;
    std::cout << "Responses: " << pClock->GetCount(ptpV2Header::DELAY_RESP) << " interval: " << std::hex << (int)pClock->GetInterval(ptpV2Header::DELAY_RESP) << std::dec << std::endl;
    std::cout << "Offset:" << TimeToString((pClock->GetOffset(PtpV2Clock::MAX)-pClock->GetOffset(PtpV2Clock::MIN))) << std::endl;

}
