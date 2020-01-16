#pragma once
#include "timeutils.h"
#include <memory>
#include <map>
#include <list>

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
        *   @param sLocalIpAddress the ip address of the network interface you want to use to receive/send PTP messages
        *   @param nDomain the PTP domain to join
        **/
        PtpMonkey(const std::string& sLocalIpAddress, unsigned char nDomain);

        ~PtpMonkey(){}

        /** @brief Add an object to handle PTP events
        *   @param pHandler a shared_ptr to an object of a class derived from PtpEventHandler
        **/
        void AddEventHandler(std::shared_ptr<PtpEventHandler> pHandler);

        /** @brief Run PtpMonkey. This will create a separate thread to run all the sockets in
        **/
        void Run();

        /** @brief Get the time of the master clock for this domain. If there is no master clock returns the system clock time
        *   @return <i>time_s_ns</i> the time in seconds and nanoseconds
        **/
        time_s_ns GetPtpTime() const;

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

    protected:
        PtpMonkey(){}
        std::unique_ptr<PtpMonkeyImplementation, PtpMonkeyImplDeleter> m_pImpl;
};
