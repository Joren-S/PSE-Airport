//============================================================================
// Name        : Time.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================


#include "../headers/Time.h"


Time::Time(int hour, int minute) {
    fInitCheck = this;
    setHour(hour);
    setMinute(minute);
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

string Time::formatted() const {
    REQUIRE(this->properlyInitialized(), "Time was't initialized when calling formatted");
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
    REQUIRE(this->properlyInitialized(), "Time was't initialized when calling advance");
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
    REQUIRE(this->properlyInitialized(), "Time was't initialized when calling setMinute");
    REQUIRE(minute < 60 && minute >= 0, "Minute has to be between 0 and 60");
    fMinute = minute;
}

void Time::setHour(int hour) {
    REQUIRE(this->properlyInitialized(), "Time was't initialized when calling setHour");
    REQUIRE(hour < 24 && hour >= 0, "Hour has to be between 0 and 24");
    fHour = hour;
}

int Time::getHour() const {
    REQUIRE(this->properlyInitialized(), "Time was't initialized when calling getHour");
    return fHour;
}

int Time::getMinute() const {
    REQUIRE(this->properlyInitialized(), "Time was't initialized when calling getMinute");
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

bool Time::properlyInitialized() const {
    return true;
}