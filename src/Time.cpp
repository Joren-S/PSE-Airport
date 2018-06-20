//============================================================================
// Name        : Time.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================


#include "../headers/Time.h"

using namespace std;

Time::Time(int hour, int minute) {
    fInitCheck = this;
    setHour(hour);
    setMinute(minute);
    ENSURE(properlyInitialized(), "Time wasn't properly initialized after constructing.");
}

Time::Time(const Time &time) {
    fInitCheck = this;
    setHour(time.getHour());
    setMinute(time.getMinute());
    ENSURE(properlyInitialized(), "Time wasn't properly initialized after constructing.");
}

string Time::formatted() const {
    REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling setMinute");
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
    REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling advance");
    REQUIRE(minutes >= 0, "Advancing by a negative amount of minutes is not possible");

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
    REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling setMinute");
    REQUIRE(minute < 60 && minute >= 0, "Minute has to be between 0 and 60");
    fMinute = minute;
}

void Time::setHour(int hour) {
    REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling setHour");
    REQUIRE(hour < 24 && hour >= 0, "Hour has to be between 0 and 24");
    fHour = hour;
}

int Time::getHour() const {
    REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling getHour");
    return fHour;
}

int Time::getMinute() const {
    REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling getMinute");
    return fMinute;
}


bool Time::operator==(const Time& time) const {
    return time.getHour() == this->getHour() and time.getMinute() == this->getMinute();
}


bool Time::operator<(const Time& time) const {
    // If it's midnight, return true
    if (time == Time() and !(*this == Time())) {
        return false;
    }
    if (this->getHour() < time.getHour()) {
        return true;
    }
    if (this->getHour() > time.getHour()) {
        return false;
    }
    return this->getMinute() < time.getMinute();
}

Time& Time::operator=(const Time &time) {
    fMinute = time.getMinute();
    fHour = time.getHour();
    fInitCheck = this;
    return *this;
}

bool Time::properlyInitialized() const {
    return fInitCheck == this;
}