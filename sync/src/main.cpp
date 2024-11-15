#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include "ptpsync.h"
#include "log.h"
#include "inimanager.h"
#include "logtofile.h"

void InitLogging(const iniManager& ini)
{
    if(ini.Get("logging", "console", 0L) > -1 )
    {
        auto nLogToConsole = pml::LogStream::AddOutput(std::make_unique<pml::LogOutput>());
        pml::LogStream::SetOutputLevel(nLogToConsole, pml::enumLevel(ini.Get("logging", "console", (long)pml::LOG_TRACE)));
    }
    
    if(ini.Get("logging", "file", (long)pml::LOG_INFO) > -1)
    {
        std::filesystem::path pathLog = ini.Get("path","log",".");
        pathLog /= "ptpsync";
        auto nLogToFile = pml::LogStream::AddOutput(std::make_unique<pml::LogToFile>(pathLog));
        pml::LogStream::SetOutputLevel(nLogToFile, pml::enumLevel(ini.Get("logging", "file", (long)pml::LOG_INFO)));
    }

}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage: ptpsync [config file full path]" << std::endl;
        return -1;
    }

    iniManager ini;
    if(ini.Read(argv[1]) == false)
    {
        std::cout << "Critical: Failed to read config file " << argv[1] << std::endl;
        return -1;
    }
   
    InitLogging(ini);

    pmlLog(pml::LOG_INFO, "pml::ptpmonkey") << "Start" << std::endl;

    pml::ptpmonkey::Sync sync;
    
    if(auto sInterface = ini.Get("ptp", "interface", ""); sInterface.empty() == false)
    {   
//        sync.Run(IpInterface(sInterface), static_cast<unsigned char>(ini.Get("ptp", "domain", 0L)), static_cast<pml::ptpmonkey::Mode>(ini.Get("ptp", "mode", 0L))), ini.Get("ptp", "hwc", -1L));
    }
    else
    {
        sync.Run(IpAddress(ini.Get("ptp", "address", "0.0.0.0")), static_cast<unsigned char>(ini.Get("ptp", "domain", 0L)), static_cast<pml::ptpmonkey::Mode>(ini.Get("ptp", "mode", 0L)), ini.Get("ptp", "hwc", -1L));
    }

    getchar();

    return 0;
}
