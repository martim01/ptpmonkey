#pragma once
#include "timeutils.h"
#include <memory>
#include <map>
#include <list>
#include "namedtype.h"

namespace ptpmonkey
{

    class PtpMonkeyImplementation;
    class PtpEventHandler;
    class PtpV2Clock;

    struct PtpMonkeyImplDeleter
    {
        void operator()(PtpMonkeyImplementation* p);
    };




    /** @class Main class for interfacing with the PTP code
    **/
    class PtpMonkey
    {
        public:


            /** @brief Constructor
            *   @param ipAddress the ip address of the network interface you want to use to receive/send PTP messages
            *   @param nDomain the PTP domain to join
            *   @param nSampleSize the number of delay req/resp to receive before assuming offset is calculated
            *   @param nDelayRequestPerSec the number of delayy request messages to send to the master clock each second
            **/
            PtpMonkey(const IpAddress& ipAddress, unsigned char nDomain, unsigned short nSampleSize, Rate enumDelayRequest=Rate::PER_SEC_4);

            /** @brief Constructor
            *   @param IpInterface the name of the network interface to use to send/receivee PTP messages
            *   @param nDomain the PTP domain to join
            *   @param nDelayRequestPerSec the number of delayy request messages to send to the master clock each second
            **/
            PtpMonkey(const IpInterface& IpInterface, unsigned char nDomain,unsigned short nSampleSize, Rate enumDelayRequest=Rate::PER_SEC_4);

            ~PtpMonkey(){}

            /** @brief Add an object to handle PTP events
            *   @param pHandler a shared_ptr to an object of a class derived from PtpEventHandler
            **/
            void AddEventHandler(std::shared_ptr<PtpEventHandler> pHandler);

            /** @brief Run PtpMonkey. This will create a separate thread to run all the sockets in
            **/
            bool Run();

            ///< @brief Stop PtpMonkey. This will stop the thread and remove all the clocks
            void Stop();

            ///< @brief Restart PtpMonkey. This will restart the PtpMonkey thread if it has been stopped.
            void Restart();

            ///< @brief Returns tru is the PtpMonkey thread has been run and is no longer running
            bool IsStopped();

            /** @brief Get the time of the master clock for this domain. If there is no master clock returns the system clock time
            *   @return <i>time_s_ns</i> the time in seconds and nanoseconds
            **/
            time_s_ns GetPtpTime() const;

            /** @brief Checks whether the clock has received enough decent messages to have a calculated offset and thus be synced to the master
            *   @return <i>bool</i> true if more than the sample size delay resp messages have been received and if the variance of the offsets is within tolerance
            **/
            bool IsSyncedToMaster() const;

            /** @brief Instructs the local clock to recalculate the offset
            **/
            void ResyncToMaster();

            /** @brief Gets the average offset between the system clock and the master clock for this domain. If there is no master clock returns <0,0>
            *   @return <i>time_s_ns</i> the time in seconds and nanoseconds
            **/
            time_s_ns GetPtpOffset() const;

            /** @brief Gets the average network between the system clock and the master clock for this domain. If there is no master clock returns <0,0>
            *   @return <i>time_s_ns</i> the time in seconds and nanoseconds
            **/
            time_s_ns GetPtpDelay() const;

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

            /** @brief Gets a const pointer to the clock with the given id. May return nullptr if there is no clock.
            *   @return <i>std::shared_ptr<const PtpV2Clock></i>
            **/
            std::shared_ptr<const PtpV2Clock> GetClock(const std::string& sClockId) const;

            /** @brief Gets the clock id of the local clock.
            *   @return <i>string<i>
            **/
            std::string GetLocalClockId() const;

            /** @brief Gets a const pointer to the local clock. May return nullptr if there is no current local clock.
            *   @return <i>std::shared_ptr<const PtpV2Clock></i>
            **/
            std::shared_ptr<const PtpV2Clock> GetLocalClock() const;

        protected:
            PtpMonkey(){}
            std::unique_ptr<PtpMonkeyImplementation, PtpMonkeyImplDeleter> m_pImpl;
    };
};
