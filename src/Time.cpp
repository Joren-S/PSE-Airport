//
// Created by Max Van Houcke on 12-4-2018.
//

#include "../headers/Time.h"

Time::Time(int hour, int minute) {
    setHour(hour);
    setMinute(minute);
}

void Time::advance(int minutes) {
    // Check for valid value for minutes
    string error = "Invalid value for minute.";
//    REQUIRE(minutes >= 0, error.c_str());

    // While there are minutes remaining
    while (minutes) {
        // Special case for last minute of the hour
        if (fMinute == 59) {
            // Special case for last hour of the day
            if (fHour == 23) {
                fHour = 0;
            }
            else {
                ++fHour;
            }
            fMinute = 0;
        }
        else {
            ++fMinute;
        }

        // Decrease minutes
        --minutes;
    }
}

void Time::setMinute(int minute) {
    string error = "Invalid value for minute.";
//    REQUIRE(minute < 60 && minute >= 0, error.c_str());
    fMinute = minute;
}

void Time::setHour(int hour) {
    string error = "Invalid value for hour.";
//    REQUIRE(hour < 24 && hour >= 0, error.c_str());
    fHour = hour;
}

int Time::getHour() const {
    return fHour;
}

int Time::getMinute() const {
    return fMinute;
}