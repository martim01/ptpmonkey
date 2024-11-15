#include "timeutils.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>
#include "log.h"

using namespace pml::ptpmonkey;


std::chrono::nanoseconds Now()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> tp(std::chrono::high_resolution_clock::now());
   // tp += LEAP_SECONDS;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
}

std::pair<std::chrono::seconds, std::chrono::nanoseconds> Split(const std::chrono::nanoseconds& ts)
{
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(ts);
    auto rounded = std::chrono::duration_cast<std::chrono::nanoseconds>(seconds);
    auto nano =    std::chrono::nanoseconds(0);
    if(ts.count() > 0)
    {  
        nano = ts - rounded;
        
    }
    else
    {
        nano = rounded - ts ;
    }
    return std::make_pair(seconds, nano);
}

std::string TimeToString(const std::chrono::nanoseconds& ts)
{
    std::stringstream ss;
    auto [sec, nano] = Split(ts);

    ss << sec.count() << "." << std::setw(9) << std::setfill('0') << nano.count();
    return ss.str();
}

std::string TimeToIsoString(const std::chrono::nanoseconds& ts)
{
    std::stringstream ss;
    auto [sec, nano] = Split(ts);

    std::time_t t(sec.count());
    if(t >= 0)
    {
        std::tm timetm = *std::localtime(&t);
        ss << std::put_time(&timetm, "%Y-%m-%dT%H:%M:%S") << "." << std::setw(9) << std::setfill('0') << nano.count();
    }
    return ss.str();
}

long long int TimeToNano(const std::chrono::nanoseconds& ts)
{
    return ts.count();

}

double TimeToDouble(const std::chrono::nanoseconds& ts)
{
    double dNano = ts.count();
    return dNano / (1e9);

}
std::chrono::nanoseconds DoubleToTime(double dSeconds)
{
    return std::chrono::nanoseconds(static_cast<long long int>(dSeconds*1e9));
}

std::chrono::nanoseconds NanoToTime(long long int nNano)
{
    return std::chrono::nanoseconds(nNano);
}

std::chrono::nanoseconds operator*(const std::chrono::nanoseconds& t1, const std::chrono::nanoseconds& t2)
{
    return std::chrono::nanoseconds(t1.count()*t2.count());
}
