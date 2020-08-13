#pragma once
#include <string>
#include <chrono>
#include "ptpdll.h"


static const std::chrono::seconds LEAP_SECONDS(37);

using time_s_ns = std::pair<std::chrono::seconds, std::chrono::nanoseconds>;

extern PTP_IMPEXPORT time_s_ns Now();
extern PTP_IMPEXPORT std::string TimeToString(const time_s_ns& ts);
extern PTP_IMPEXPORT std::string TimeToIsoString(const time_s_ns& ts);

extern PTP_IMPEXPORT long long int TimeToNano(const time_s_ns& ts);
extern PTP_IMPEXPORT time_s_ns NanoToTime(long long int nNano);

extern PTP_IMPEXPORT double TimeToDouble(const time_s_ns& ts);
extern PTP_IMPEXPORT time_s_ns DoubleToTime(double dNano);

extern time_s_ns operator+(const time_s_ns& t1, const time_s_ns& t2);
extern time_s_ns operator-(const time_s_ns& t1, const time_s_ns& t2);
extern time_s_ns operator/(const time_s_ns& t1, unsigned int nDivisor);
extern bool operator==(const time_s_ns& t1, const time_s_ns& t2);


namespace ptpmonkey
{
    enum class Rate : std::int8_t { PER_SEC_128 = -7, PER_SEC_64 = -6, PER_SEC_32 = -5, PER_SEC_16 = -4, PER_SEC_8 = -3, PER_SEC_4 = -2, PER_SEC_2 = -1,
                                EVERY_1_SEC = 0, EVERY_2_SEC = 1, EVERY_4_SEC = 2, EVERY_8_SEC = 3, EVERY_16_SEC = 4};

    static constexpr time_s_ns TIMEZERO = std::make_pair(std::chrono::seconds(0), std::chrono::nanoseconds(0));



};
