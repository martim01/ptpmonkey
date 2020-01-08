#include "timeutils.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>

time_s_ns GetCurrentTaiTime()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> tp(std::chrono::high_resolution_clock::now());
    tp += LEAP_SECONDS;

    std::chrono::seconds first = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
    std::chrono::nanoseconds second = std::chrono::nanoseconds(std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count()%1000000000);
    return std::make_pair(first,second);
}

std::string TimeToString(const time_s_ns& ts)
{
    std::stringstream ss;
    ss << ts.first.count() << "." << std::setw(9) << std::setfill('0') << ts.second.count();
    return ss.str();
}

std::string TimeToIsoString(const time_s_ns& ts)
{
    std::time_t t = ts.first.count();
    std::tm timetm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&timetm, "%Y-%m-%dT%H:%M:%S") << "." << std::setw(9) << std::setfill('0') << ts.second.count();
    return ss.str();
}


time_s_ns Minus(const time_s_ns& firstTime, const time_s_ns& secondTime)
{
    std::chrono::nanoseconds totalFirst(std::chrono::duration_cast<std::chrono::nanoseconds>(firstTime.first) + firstTime.second);
    std::chrono::nanoseconds totalSecond(std::chrono::duration_cast<std::chrono::nanoseconds>(secondTime.first) + secondTime.second);
    std::chrono::nanoseconds result = totalFirst-totalSecond;
    return std::make_pair(std::chrono::duration_cast<std::chrono::seconds>(result), std::chrono::nanoseconds(result.count()%1000000000));
}

time_s_ns Add(const time_s_ns& firstTime, const time_s_ns& secondTime)
{
    std::chrono::nanoseconds totalFirst(std::chrono::duration_cast<std::chrono::nanoseconds>(firstTime.first) + firstTime.second);
    std::chrono::nanoseconds totalSecond(std::chrono::duration_cast<std::chrono::nanoseconds>(secondTime.first) + secondTime.second);
    std::chrono::nanoseconds result = totalFirst+totalSecond;
    return std::make_pair(std::chrono::duration_cast<std::chrono::seconds>(result), std::chrono::nanoseconds(result.count()%1000000000));
}

time_s_ns Divide(const time_s_ns& firstTime, int nDivisor)
{
    std::chrono::nanoseconds totalFirst(std::chrono::duration_cast<std::chrono::nanoseconds>(firstTime.first) + firstTime.second);

    unsigned long long int nResult = (totalFirst.count())/nDivisor;
    std::chrono::nanoseconds result(nResult);
    return std::make_pair(std::chrono::duration_cast<std::chrono::seconds>(result), std::chrono::nanoseconds(result.count()%1000000000));
}
