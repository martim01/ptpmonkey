#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include "log.h"

#include "asio.hpp"
#include "mac.h"
#include "ptpeventloghandler.h"
#include "ptploghandler.h"
#include "ptpmonkey.h"
#include "ptpparser.h"
#include "receiver.h"
#include "sender.h"

#include "argparse.hpp"

constexpr short multicast_port = 319;

using namespace pml;

const std::map<std::string, ptpmonkey::Rate> kRateMap = {
    {"NEVER", ptpmonkey::Rate::kNever},
    {"PER_SEC_128", ptpmonkey::Rate::kPerSec128},
    {"PER_SEC_64", ptpmonkey::Rate::kPerSec64},
    {"PER_SEC_32", ptpmonkey::Rate::kPerSec32},
    {"PER_SEC_16", ptpmonkey::Rate::kPerSec16},
    {"PER_SEC_8", ptpmonkey::Rate::kPerSec8},
    {"PER_SEC_4", ptpmonkey::Rate::kPerSec4},
    {"PER_SEC_2", ptpmonkey::Rate::kPerSec2},
    {"EVERY_1_SEC", ptpmonkey::Rate::kEvery1Sec},
    {"EVERY_2_SEC", ptpmonkey::Rate::kEvery2Sec},
    {"EVERY_4_SEC", ptpmonkey::Rate::kEvery4Sec},
    {"EVERY_8_SEC", ptpmonkey::Rate::kEvery8Sec},
    {"EVERY_16_SEC", ptpmonkey::Rate::kEvery16Sec}
};

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("in");
    
    program.add_argument("-i", "--interface").help("Interface to use for the PTP").default_value("eth0");
    program.add_argument("-d", "--domain").help("PTP domain").default_value("0");
    program.add_argument("-m", "--mode").help("PTP mode (MULTICAST, UNICAST, HYBRID)").default_value("MULTICAST");
    program.add_argument("-r", "--rate").help("Delay request rate (NEVER, PER_SEC_128, PER_SEC_64, PER_SEC_32, PER_SEC_16, PER_SEC_8, PER_SEC_4, PER_SEC_2, EVERY_1_SEC, EVERY_2_SEC, EVERY_4_SEC, EVERY_8_SEC, EVERY_16_SEC)").default_value("NEVER");


    try
    {
        program.parse_args(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cout << "CAUGHT " << e.what() << std::endl;
        std::cout << program << std::endl;
        std::exit(1);
    }


    pml::log::Stream::AddOutput(std::make_unique<pml::log::Output>());
    pml::log::Stream::SetOutputLevel(pml::LOG_TRACE);
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Start" << std::endl;

    auto mode = ptpmonkey::Mode::kMulticast;

    auto sMode = program.get<std::string>("--mode");
    if(sMode == "UNICAST")
    {
        mode = ptpmonkey::Mode::kUnicast;
    }
    else if(sMode == "HYBRID")
    {
        mode = ptpmonkey::Mode::kHybrid;
    }

    auto rate = ptpmonkey::Rate::kNever;
    if(auto it = kRateMap.find(program.get<std::string>("--rate")); it != kRateMap.end())
    {
        rate = it->second;
    }

    ptpmonkey::PtpMonkey ptp(IpInterface(program.get<std::string>("--interface")), std::stoi(program.get<std::string>("--domain")), 10, mode, rate);
    ptp.AddEventHandler(std::make_shared<ptpmonkey::PtpEventLogHandler>(false));
    
    ptp.Run();
    getchar();
    
    
    //ptp.Manage().UsePtp4l(true);
//    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::PRIORITY1);
//    getchar();


    /*
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::PORT_STATS_NP);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::CLOCK_DESCRIPTION);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::USER_DESCRIPTION);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::DEFAULT_DATA_SET);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::CURRENT_DATA_SET);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::PORT_DATA_SET);
    getchar();
    ptp.Get(ptpmonkey::mngmnt::enumGet::PARENT_DATA_SET);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::TIME_PROPERTIES_DATA_SET);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::PRIORITY1);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::PRIORITY2);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::DOMAIN);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::SLAVE_ONLY);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::MASTER_ONLY);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::SYNCHRONIZATION_UNCERTAIN_NP);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::LOG_ANNOUNCE_INTERVAL);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::LOG_MIN_PDELAY_REQ_INTERVAL);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::DELAY_MECHANISM);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::LOG_SYNC_INTERVAL);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::ANNOUNCE_RECEIPT_TIMEOUT);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::ALTERNATE_TIME_OFFSET_ENABLE);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::CLOCK_ACCURACY);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::TRACEABILITY_PROPERTIES);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::TIMESCALE_PROPERTIES);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::VERSION_NUMBER);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::ALTERNATE_TIME_OFFSET_NAME);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::ALTERNATE_TIME_OFFSET_PROPERTIES);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::TIME_STATUS_NP);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::GRANDMASTER_SETTINGS_NP);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::PORT_PROPERTIES_NP);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::PORT_STATS_NP);
    getchar();
    ptp.Manage().Get(ptpmonkey::mngmnt::enumGet::PORT_SERVICE_STATS_NP);
    getchar();

    */
    return 0;
}
