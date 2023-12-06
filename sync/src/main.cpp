#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include "ptpsync.h"
#include "log.h"

int main(int argc, char* argv[])
{
    pml::LogStream::AddOutput(std::make_unique<pml::LogOutput>());
    pml::LogStream::SetOutputLevel(pml::LOG_DEBUG);
    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Start" << std::endl;

    pml::ptpmonkey::Sync sync;
    sync.Run(IpAddress("172.23.101.193"), 0, pml::ptpmonkey::Mode::MULTICAST);

    getchar();

    return 0;
}
