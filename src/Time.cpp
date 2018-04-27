//
// Created by Max Van Houcke on 12-4-2018.
//

#include "../headers/Time.h"

using namespace std;

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
    REQUIRE(minutes >= 0, "Invalid value for minutes.");

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
    REQUIRE(minute < 60 && minute >= 0, "Invalid value for minute.");
    fMinute = minute;
}

void Time::setHour(int hour) {
    REQUIRE(hour < 24 && hour >= 0, "Invalid value for hour.");
    fHour = hour;
}

int Time::getHour() const {
    return fHour;
}

int Time::getMinute() const {
    return fMinute;
}


bool Time::operator==(const Time& time) const {
    return time.getHour() == this->getHour() and time.getMinute() == this->getMinute();
}


bool Time::operator<(const Time& time) const {
    if (this->getHour() < time.getHour()) {
        return true;
    }
    if (this->getHour() > time.getHour()) {
        return false;
    }
    return this->getMinute() < time.getMinute();
}