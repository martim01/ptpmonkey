//
// receiver.cpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include "asio.hpp"
#include "ptpparser.h"
#include "receiver.h"
#include "ptploghandler.h"
#include "sender.h"
#include "mac.h"
#include "ptpmonkey.h"
#include <thread>
#include <chrono>
#include "ptpeventloghandler.h"
#include "log.h"

#include "argparse.hpp"

constexpr short multicast_port = 319;

using namespace pml;

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("in");
    program.add_argument("-i", "--interface").help("Interface to use for the PTP").default_value("eth0");
    program.add_argument("-d", "--domain").help("PTP domain").default_value("0");

    pml::log::Stream::AddOutput(std::make_unique<pml::log::Output>());
    pml::log::Stream::SetOutputLevel(pml::LOG_TRACE);
    pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "Start" << std::endl;

    ptpmonkey::PtpMonkey ptp(IpInterface(program.get<std::string>("--interface")), std::stoi(program.get<std::string>("--domain")), 10, ptpmonkey::Mode::MULTICAST, ptpmonkey::Rate::NEVER);
//    ptp.AddEventHandler(std::make_shared<ptpmonkey::PtpEventLogHandler>(false));
    
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
