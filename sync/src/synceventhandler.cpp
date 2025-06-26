#include "synceventhandler.h"
#include "ptpsync.h"
#include "ptpclock.h"
#include "log.h"


using namespace pml::ptpmonkey;


void SyncEventHandler::ClockAdded(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "---------CLOCK ADDED---------";
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Id:\t\t" << pClock->GetId() ;
}

void SyncEventHandler::ClockUpdated(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "---------CLOCK UPDATED---------";
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Id:\t\t" << pClock->GetId() ;
}

void SyncEventHandler::ClockRemoved(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "---------CLOCK REMOVED---------";
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Id:\t\t" << pClock->GetId() ;

    if(m_pSync)
    {
        m_pSync->ClockRemoved(pClock);
    }
}

void SyncEventHandler::ClockTimeCalculated(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{
    if(m_pSync)
    {
        m_pSync->ClockTimeCalculated(pClock);
    }
}

void SyncEventHandler::ClockBecomeMaster(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "---------CLOCK BECAME MASTER---------";
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Id:\t\t" << pClock->GetId() ;
    if(m_pSync)
    {
        m_pSync->ClockBecomeMaster(pClock);
    }
}

void SyncEventHandler::ClockBecomeSlave(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "---------CLOCK BECAME SLAVE---------";
    pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "Id:\t\t" << pClock->GetId() ;

    if(m_pSync)
    {
        m_pSync->ClockBecomeSlave(pClock);
    }
}

void SyncEventHandler::AnnounceSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{

}

void SyncEventHandler::SyncSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{

}

void SyncEventHandler::FollowUpSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{

}

void SyncEventHandler::DelayRequestSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{

}

void SyncEventHandler::DelayResponseSent(std::shared_ptr<ptpmonkey::PtpV2Clock> pClock)
{

}