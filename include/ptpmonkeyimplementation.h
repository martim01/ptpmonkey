#pragma once
#include "timeutils.h"
#include "ptpstructs.h"
#include <memory>
#include <map>
#include <list>
#include <string>
#include "asio.hpp"
#include <mutex>
#include "namedtypes.h"
#include "ptpclock.h"
#include "sender.h"
#include <thread>
#include <atomic>

namespace pml
{
    namespace ptpmonkey
    {

        class PtpEventHandler;
        class PtpParser;

    /** @class Main class for interfacing with the PTP code
    **/
    class PtpMonkeyImplementation
    {
        public:
            PtpMonkeyImplementation(const IpAddress& ipAddress, unsigned char nDomain, unsigned short nSampleSize, Mode mode, Rate enumDelayRequest);
            PtpMonkeyImplementation(const IpInterface& IpInterface, unsigned char nDomain, unsigned short nSampleSize,Mode mode, Rate enumDelayRequest);

            ~PtpMonkeyImplementation();

            /** @brief Add an object to handle PTP events
            *   @param pHandler a shared_ptr to an object of a class derived from PtpEventHandler
            **/
            void AddEventHandler(std::shared_ptr<PtpEventHandler> pHandler);

            /** @brief Run PtpMonkey. This will create a separate thread to run all the sockets in
            **/
            bool Run(bool bThreaded);

            /** @brief Get the time of the master clock for this domain. If there is no master clock returns the system clock time
            *   @return <i>std::chrono::nanoseconds</i> the time in seconds and nanoseconds
            **/
            std::chrono::nanoseconds GetPtpTime() const;

            bool IsSyncedToMaster() const;

            void ResyncToMaster() const;

            /** @brief Gets the set offset between the system clock and the master clock for this domain. If there is no master clock returns <0,0>
            *   @return <i>std::chrono::nanoseconds</i> the time in seconds and nanoseconds
            **/
            std::chrono::nanoseconds GetPtpOffset(PtpV2Clock::enumCalc eCalc = PtpV2Clock::SET) const;

            /** @brief Gets the set network between the system clock and the master clock for this domain. If there is no master clock returns <0,0>
            *   @return <i>std::chrono::nanoseconds</i> the time in seconds and nanoseconds
            **/
            std::chrono::nanoseconds GetPtpDelay(PtpV2Clock::enumCalc eCalc = PtpV2Clock::SET) const;

            /** @brief Gets the clock id of the master clock for this domain. If there is no master clock returns an empty string
            *   @return <i>string<i>
            **/
            std::string GetMasterClockId() const;




            /** @brief Gets a const_iterator to the beginning of the map of clocks that exist in this domain
            *   @return <i>std::map<std::string, std::shared_ptr<PtpV2Clock> >::const_iterator</i>
            **/
            const std::map<std::string, std::shared_ptr<PtpV2Clock> >& GetClocks() const;

            /** @brief Gets a const pointer to the current sync master clock. May return nullptr if there is no current master.
            *   @return <i>std::shared_ptr<const PtpV2Clock></i>
            *   @note the returned clock may be a boundary clock or the grandmaster
            **/
            std::shared_ptr<const PtpV2Clock> GetSyncMasterClock() const;

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

            /** @brief Called by the Sender socket when it sends a delay request message and gets a tx timestamp message
            *   @note Should not be called by the user
            **/
            void DelayRequestSent(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);

            void Stop();
            void Restart();
            bool IsStopped() const;

            std::shared_ptr<const PtpV2Clock> GetClock(const std::string& sClockId) const;

            Rate GetDelayRate() const
            {   return m_delayRequest;  }

            static int GetTimestampingSupported(const IpInterface& interface);
            void ResetLocalClockStats() const;

            Mode GetMode() const;

            void SetDomain(unsigned char nDomain);
            unsigned char GetDomain() const { return m_nDomain;}

        protected:
            asio::io_context m_context;

            std::map<std::string, std::shared_ptr<PtpV2Clock> >::iterator GetOrCreateClock(std::shared_ptr<ptpV2Header> pHeader, std::shared_ptr<ptpV2Payload> pPayload);

            void ChangeSyncMaster(std::shared_ptr<PtpV2Clock> pNewMaster);
            void CheckForDeadClocks();

            bool Run();

            IpAddress m_local;
            IpInterface m_Interface;
            unsigned char m_nDomain;
            unsigned short m_nSampleSize;
            Mode m_mode;
            Rate m_delayRequest;

            std::map<std::string, std::shared_ptr<PtpV2Clock> > m_mClocks;

            std::shared_ptr<PtpV2Clock> m_pSyncMaster = nullptr;

            std::list<std::shared_ptr<PtpEventHandler>> m_lstEventHandler;

            unsigned long long int m_nLocalClockId = 0;
            std::shared_ptr<PtpV2Clock> m_pLocal = nullptr;
            mutable std::mutex m_mutex;

            std::unique_ptr<Sender> m_pSender = nullptr;
            int m_nTimestamping = 0;

            enum {TIMESTAMP_TX_HARDWARE = 1, TIMESTAMP_TX_SOFTWARE = 2, TIMESTAMP_RX_HARDWARE = 4, TIMESTAMP_RX_SOFTWARE = 8 };


            std::unique_ptr<std::thread> m_pThread = nullptr;
            std::shared_ptr<PtpParser> m_pParser = nullptr;

            static const std::string MULTICAST;
        };  
    }
}