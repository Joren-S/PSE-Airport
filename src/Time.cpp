//
// Created by Max Van Houcke on 12-4-2018.
//

#include "../headers/Time.h"

Time::Time(int hour, int minute) {
    setHour(hour);
    setMinute(minute);
}

string Time::formatted() const {
    // Set up an output string stream
    ostringstream stream;

    // Append a 0 if the hour is not two digits
    if (fHour < 10) {
        stream << 0;
    }

    // Set the hour
    stream << fHour;

    // Add the ':'
    stream << ':';

    // Append a 0 if the minute is not two digits
    if (fMinute < 10) {
        stream << 0;
    }

    // Set the minute
    stream << fMinute;

    // Return the string
    return stream.str();
}

void Time::advance(int minutes) {
    REQUIRE(minutes >= 0, "Invalid value for minute.");

    // While there are minutes remaining
    while (minutes) {
        // Increase minute
        fMinute = fMinute == 59? 0: fMinute + 1;

        if (fMinute == 0) {
            // Increase hour
            fHour = fHour == 23? 0: fHour + 1;
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