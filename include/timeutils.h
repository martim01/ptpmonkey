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

namespace ptpmonkey
{
    enum class Rate : std::int8_t { PER_SEC_128 = -7, PER_SEC_64 = -6, PER_SEC_32 = -5, PER_SEC_16 = -4, PER_SEC_8 = -3, PER_SEC_4 = -2, PER_SEC_2 = -1,
                                EVERY_1_SEC = 0, EVERY_2_SEC = 1, EVERY_4_SEC = 2, EVERY_8_SEC = 3, EVERY_16_SEC = 4};

    static constexpr std::chrono::nanoseconds TIMEZERO = std::chrono::nanoseconds(0);



};
