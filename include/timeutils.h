#pragma once
#include <string>
#include <chrono>


static const std::chrono::seconds LEAP_SECONDS(37);

using time_s_ns = std::pair<std::chrono::seconds, std::chrono::nanoseconds>;

extern time_s_ns TimeNow();
extern std::string TimeToString(const time_s_ns& ts);
extern std::string TimeToIsoString(const time_s_ns& ts);

extern unsigned long long int TimeToNano(const time_s_ns& ts);
extern time_s_ns NanoToTime(unsigned long long int nNano);

extern time_s_ns operator+(const time_s_ns& t1, const time_s_ns& t2);
extern time_s_ns operator-(const time_s_ns& t1, const time_s_ns& t2);
extern time_s_ns operator/(const time_s_ns& t1, unsigned int nDivisor);

enum class Rate : std::int8_t { PER_SEC_128 = -7, PER_SEC_64 = -6, PER_SEC_32 = -5, PER_SEC_16 = -4, PER_SEC_8 = -3, PER_SEC_4 = -2, PER_SEC_2 = -1,
                                EVERY_1_SEC = 0, EVERY_2_SEC = 1, EVERY_4_SEC = 2, EVERY_8_SEC = 3, EVERY_16_SEC = 4};
