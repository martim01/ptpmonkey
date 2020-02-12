#pragma once
#include "timeutils.h"
#include "ptpstructs.h"
#include <memory>
#include <map>
#include <list>
#include <string>
#include "asio.hpp"
#include <mutex>
#include "namedtype.h"
#include "ptpclock.h"

namespace ptpmonkey
{

class PtpEventHandler;

/** @class Main class for interfacing with the PTP code
**/
class PtpMonkeyImplementation
{
    public:
        PtpMonkeyImplementation(const IpAddress& ipAddress, unsigned char nDomain, unsigned short nSampleSize,Rate enumDelayRequest);
        PtpMonkeyImplementation(const IpInterface& IpInterface, unsigned char nDomain, unsigned short nSampleSize,Rate enumDelayRequest);

        ~PtpMonkeyImplementation(){}

        /** @brief Add an object to handle PTP events
        *   @param pHandler a shared_ptr to an object of a class derived from PtpEventHandler
        **/
        void AddEventHandler(std::shared_ptr<PtpEventHandler> pHandler);

        /** @brief Run PtpMonkey. This will create a separate thread to run all the sockets in
        **/
        bool Run();

        /** @brief Get the time of the master clock for this domain. If there is no master clock returns the system clock time
        *   @return <i>time_s_ns</i> the time in seconds and nanoseconds
        **/
        time_s_ns GetPtpTime() const;

        bool IsSyncedToMaster() const;

        void ResyncToMaster();

        /** @brief Gets the set offset between the system clock and the master clock for this domain. If there is no master clock returns <0,0>
        *   @return <i>time_s_ns</i> the time in seconds and nanoseconds
        **/
        time_s_ns GetPtpOffset(PtpV2Clock::enumCalc eCalc = PtpV2Clock::SET) const;

        /** @brief Gets the set network between the system clock and the master clock for this domain. If there is no master clock returns <0,0>
        *   @return <i>time_s_ns</i> the time in seconds and nanoseconds
        **/
        time_s_ns GetPtpDelay(PtpV2Clock::enumCalc eCalc = PtpV2Clock::SET) const;

        /** @brief Gets the clock id of the master clock for this domain. If there is no master clock returns an empty string
        *   @return <i>string<i>
        **/
        std::string GetMasterClockId() const;




        /** @brief Gets a const_iterator to the beginning of the map of clocks that exist in this domain
        *   @return <i>std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator</i>
        **/
        std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator GetClocksBegin() const;

        /** @brief Gets a const_iterator to the end of the map of clocks that exist in this domain
        *   @return <i>std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator</i>
        **/
        std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator GetClocksEnd() const;

        /** @brief Gets a const pointer to the current master clock. May return nullptr if there is no current master.
        *   @return <i>std::shared_ptr<const PtpV2Clock></i>
        **/
        std::shared_ptr<const PtpV2Clock> GetMasterClock() const;

        /** @brief Gets the clock id of the local clock.
        *   @return <i>string<i>
        **/
        std::string GetLocalClockId() const;

        /** @brief Gets a const pointer to the local clock. May return nullptr if there is no current local clock.
        *   @return <i>std::shared_ptr<const PtpV2Clock></i>
        **/
        std::shared_ptr<const PtpV2Clock> GetLocalClock() const;


        /** @brief Called by PtpMonkeyHandler when a sync message is received.
        *   @note Should not be called by the user
        **/
        void Sync(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);
        void FollowUp(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);

        /** @brief Called by PtpMonkeyHandler when a delay request message is received.
        *   @note Should not be called by the user
        **/
        void DelayRequest(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);

        /** @brief Called by PtpMonkeyHandler when a delay response message is received.
        *   @note Should not be called by the user
        **/
        void DelayResponse(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpDelayResponse> pPayload);

        /** @brief Called by PtpMonkeyHandler when an announce is received.
        *   @note Should not be called by the user
        **/
        void Announce(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpAnnounce> pPayload);

        /** @brief Called by the Sender socket when it sends a delay request message
        *   @note Should not be called by the user
        **/
        void AddDelayRequest(unsigned short nSequence, const time_s_ns& timestamp);

        void Stop();
        void Restart();
        bool IsStopped();

        std::shared_ptr<const PtpV2Clock> GetClock(const std::string& sClockId) const;

        Rate GetDelayRate() const
        {   return m_delayRequest;  }

    protected:
        asio::io_context m_context;

        std::map<std::string, std::shared_ptr<PtpV2Clock> >::iterator GetOrCreateClock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);

        void ChangeMaster(std::shared_ptr<PtpV2Clock> pNewMaster);
        void CheckForDeadClocks();

        IpAddress m_local;
        unsigned char m_nDomain;
        unsigned short m_nSampleSize;
        Rate m_delayRequest;

        std::map<std::string, std::shared_ptr<PtpV2Clock> > m_mClocks;

        std::shared_ptr<PtpV2Clock> m_pMaster;

        std::list<std::shared_ptr<PtpEventHandler>> m_lstEventHandler;

        bool m_bRunning;

        unsigned long long int m_nLocalClockId;
        std::shared_ptr<PtpV2Clock> m_pLocal;
        mutable std::mutex m_mutex;

};


};
