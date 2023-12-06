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

constexpr short multicast_port = 319;

using namespace pml;

int main(int argc, char* argv[])
{
    pml::LogStream::AddOutput(std::unique_ptr<LogOutput>(new LogOutput()));
    pml::LogStream::SetOutputLevel(pml::LOG_TRACE);
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Start" << std::endl;



    ptpmonkey::PtpMonkey ptp(IpAddress("172.31.183.27"), 0, 10, ptpmonkey::Mode::MULTICAST, ptpmonkey::Rate::PER_SEC_2);
    //ptpmonkey::PtpMonkey ptp(IpAddress("192.168.0.75"), 127, 10, ptpmonkey::Mode::MULTICAST, ptpmonkey::Rate::PER_SEC_2);
    ptp.AddEventHandler(std::make_shared<ptpmonkey::PtpEventLogHandler>(false));
    ptp.Run();
    getchar();

//    do
//    {
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//        //std::cout << ptp.GetMasterClockId() << "\t\t" << TimeToString(ptp.GetPtpOffset()) << std::endl;
//    }while(true);


    return 0;
}
