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

constexpr short multicast_port = 319;


int main(int argc, char* argv[])
{

    PtpMonkey ptp("10.10.11.203", 0);
    ptp.AddEventHandler(std::make_shared<PtpEventLogHandler>());
    ptp.Run();

    do
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //std::cout << ptp.GetMasterClockId() << "\t\t" << TimeToString(ptp.GetPtpOffset()) << std::endl;
    }while(true);


    return 0;
}