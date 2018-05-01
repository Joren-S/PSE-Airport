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


class Time {
private:

    /**
     * Pointer to itself
     */
    Time *fInitCheck;

    int fMinute;
    int fHour;

public:

    /**
     * Constructor, sets the time
     * Defaults to 12:00, which is the starting point of
     * the simulation
     * REQUIRE: 0 <= hour < 24 &&
     * 0 <= minute < 60
     */
    Time(int hour = 12, int minute = 0);

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Return the time in a formatted style like such: "13:45"
     */
    std::string formatted() const;

    /**
     * Advances the time by an amount of minutes
     * REQUIRE: minute >= 0
     */
    void advance(int minutes = 1);

    /**
     * Setter for the minute.
     * REQUIRE: 0 <= minute < 60
     */
    void setMinute(int minute);

    /**
     * Getter for the minute
     */
    int getMinute() const;

    /**
     * Setter for the hour.
     * REQUIRE: 0 <= hour < 24
     */
    void setHour(int hour);

    /**
     * Getter for the hour
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
