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
     * \n REQUIRE(minute < 60 && minute >= 0, "Minute has to be between 0 and 60");
     * \n REQUIRE(hour < 24 && hour >= 0, "Hour has to be between 0 and 24");
     */
    Time(int hour = 12, int minute = 0);

    /**
     * Return the time in a formatted style like such: "13:45"
     */
    string formatted() const;

    /**
     * Advances the time by an amount of minutes
     * \n REQUIRE(minutes >= 0, "Advancing by a negative amount of minutes is not possible");
     */
    void advance(int minutes = 1);

    /**
     * Setter for the minute.
     * \n REQUIRE(minute < 60 && minute >= 0, "Minute has to be between 0 and 60");
     */
    void setMinute(int minute);

    /**
     * Getter for the minute
     */
    int getMinute() const;

    /**
     * Setter for the hour.
     * \n REQUIRE(hour < 24 && hour >= 0, "Hour has to be between 0 and 24");
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
