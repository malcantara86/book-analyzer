#ifndef TIME_UTIL_HPP
#define TIME_UTIL_HPP

#include <chrono>
#include <iostream>
#include <ctime>

std::chrono::system_clock::duration duration_since_midnight()
{
    auto now = std::chrono::system_clock::now();

    time_t tnow = std::chrono::system_clock::to_time_t(now);
    tm *date = std::localtime(&tnow);
    date->tm_hour = 0;
    date->tm_min = 0;
    date->tm_sec = 0;
    auto midnight = std::chrono::system_clock::from_time_t(std::mktime(date));

    return now - midnight;
}

std::chrono::microseconds getMicrosecondsFromMidnight()
{
    auto since_midnight = duration_since_midnight();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(since_midnight);

    return microseconds;
}

#endif //TIME_UTIL_HPP