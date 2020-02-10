#include "timeutils.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>



time_s_ns TimeNow()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> tp(std::chrono::high_resolution_clock::now());
   // tp += LEAP_SECONDS;

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
    std::stringstream ss;
    std::time_t t = ts.first.count();
    if(t >= 0)
    {
        std::tm timetm = *std::localtime(&t);
        ss << std::put_time(&timetm, "%Y-%m-%dT%H:%M:%S") << "." << std::setw(9) << std::setfill('0') << ts.second.count();
    }
    return ss.str();
}

unsigned long long int TimeToNano(const time_s_ns& ts)
{

    return (std::chrono::duration_cast<std::chrono::nanoseconds>(ts.first)+ts.second).count();
}

time_s_ns NanoToTime(unsigned long long int nNano)
{
    return make_pair( std::chrono::duration_cast<std::chrono::seconds>(std::chrono::nanoseconds(nNano)), (std::chrono::nanoseconds(nNano%1000000000)));
}


time_s_ns operator+(const time_s_ns& t1, const time_s_ns& t2)
{
    time_s_ns result;
    std::chrono::nanoseconds tn = t1.second+t2.second;
    std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(tn);
    result.first = t1.first+t2.first + sec;
    result.second = tn - std::chrono::duration_cast<std::chrono::nanoseconds>(sec);
    return result;
}

time_s_ns operator-(const time_s_ns& t1, const time_s_ns& t2)
{

    time_s_ns result;
    long long int nn = t1.second.count()-t2.second.count();

    result.first = t1.first-t2.first;
    if(nn < 0)
    {
        result.first -= std::chrono::seconds(1);
        result.second = std::chrono::nanoseconds(1000000000 + (nn%1000000000));
    }
    else
    {
        result.second = std::chrono::nanoseconds(nn%1000000000);
    }

    return result;
}

time_s_ns operator/(const time_s_ns& t1, unsigned int nDivisor)
{
    time_s_ns result;
    result.first = std::chrono::seconds(t1.first.count()/nDivisor);
    std::chrono::nanoseconds remainder = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(t1.first.count()%nDivisor))+t1.second;
    result.second = std::chrono::nanoseconds(remainder.count()/nDivisor);
    return result;
}

bool operator==(const time_s_ns& t1, const time_s_ns& t2)
{
    return (t1.first == t2.first && t1.second == t2.second);
}
