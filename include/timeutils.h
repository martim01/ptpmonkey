#pragma once
#include <string>
#include <chrono>
#include "ptpdll.h"

static const std::chrono::seconds LEAP_SECONDS(37);


extern PTP_IMPEXPORT std::chrono::nanoseconds Now();
extern PTP_IMPEXPORT std::string TimeToString(const std::chrono::nanoseconds& ts);
extern PTP_IMPEXPORT std::string TimeToIsoString(const std::chrono::nanoseconds& ts);

extern PTP_IMPEXPORT long long int TimeToNano(const std::chrono::nanoseconds& ts);
extern PTP_IMPEXPORT std::chrono::nanoseconds NanoToTime(long long int nNano);

extern PTP_IMPEXPORT double TimeToDouble(const std::chrono::nanoseconds& ts);
extern PTP_IMPEXPORT std::chrono::nanoseconds DoubleToTime(double dSeconds);

extern PTP_IMPEXPORT std::chrono::nanoseconds operator*(const std::chrono::nanoseconds& t1, const std::chrono::nanoseconds& t2);
extern PTP_IMPEXPORT std::pair<std::chrono::seconds, std::chrono::nanoseconds> Split(const std::chrono::nanoseconds& t1);

namespace pml::ptpmonkey
{
    static constexpr std::chrono::nanoseconds TIMEZERO = std::chrono::nanoseconds(0);
}
