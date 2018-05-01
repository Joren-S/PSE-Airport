//============================================================================
// Name        : Time.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_TIME_H
#define PROJECTVLIEGVELD_TIME_H

#include "DesignByContract.h"
#include <string>
#include <sstream>

using namespace std;


class Time {
private:

    /**
     * Pointer to itself
     */
    Time *fInitCheck;

    /**
     * Member indicating the minute
     */
    int fMinute;

    /**
     * Member indicating the hour
     */
    int fHour;

public:

    /**
     * Constructor, sets the time
     * Defaults to 12:00, which is the starting point of
     * the simulation
     * REQUIRE(minute < 60 && minute >= 0, "Minute has to be between 0 and 60");
     * REQUIRE(hour < 24 && hour >= 0, "Hour has to be between 0 and 24");
     * ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    Time(int hour = 12, int minute = 0);

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Return the time in a formatted style like such: "13:45"
     * REQUIRE(this->properlyInitialized(), "Time was't initialized when calling formatted");
     */
    string formatted() const;

    /**
     * Advances the time by an amount of minutes
     * REQUIRE(this->properlyInitialized(), "Time was't initialized when calling advance");
     * REQUIRE(minutes >= 0, "Advancing by a negative amount of minutes is not possible");
     */
    void advance(int minutes = 1);

    /**
     * Setter for the minute.
     * REQUIRE(this->properlyInitialized(), "Time was't initialized when calling setMinute");
     * REQUIRE(minute < 60 && minute >= 0, "Minute has to be between 0 and 60");
     */
    void setMinute(int minute);

    /**
     * Getter for the minute
     * REQUIRE(this->properlyInitialized(), "Time was't initialized when calling getMinute");
     */
    int getMinute() const;

    /**
     * Setter for the hour.
     * REQUIRE(this->properlyInitialized(), "Time was't initialized when calling setHour");
     * REQUIRE(hour < 24 && hour >= 0, "Hour has to be between 0 and 24");
     */
    void setHour(int hour);

    /**
     * Getter for the hour
     * REQUIRE(this->properlyInitialized(), "Time was't initialized when calling getHour");
     */
    int getHour() const;

    /**
     * Operator== overloaded
     */
    bool operator==(const Time&) const;

    /**
     * Operator< overloaded
     */
    bool operator<(const Time&) const;

};


#endif //PROJECTVLIEGVELD_TIME_H
