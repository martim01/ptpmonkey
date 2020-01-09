#pragma once
#include <string>
#include <chrono>


static const std::chrono::seconds LEAP_SECONDS(37);

using time_s_ns = std::pair<std::chrono::seconds, std::chrono::nanoseconds>;

extern time_s_ns GetCurrentTaiTime();
extern std::string TimeToString(const time_s_ns& ts);
extern std::string TimeToIsoString(const time_s_ns& ts);

extern unsigned long long int TimeToNano(const time_s_ns& ts);
extern time_s_ns NanoToTime(unsigned long long int nNano);
