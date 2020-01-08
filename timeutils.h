#pragma once
#include <string>
#include <chrono>


static const std::chrono::seconds LEAP_SECONDS(37);

using time_s_ns = std::pair<std::chrono::seconds, std::chrono::nanoseconds>;

extern time_s_ns GetCurrentTaiTime();
extern std::string TimeToString(const time_s_ns& ts);
extern std::string TimeToIsoString(const time_s_ns& ts);
extern time_s_ns Minus(const time_s_ns& first, const time_s_ns& second);
extern time_s_ns Add(const time_s_ns& first, const time_s_ns& second);
extern time_s_ns Divide(const time_s_ns& first, int nDivisor);
