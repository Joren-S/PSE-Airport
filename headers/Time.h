//
// Created by Max Van Houcke on 12-4-2018.
//

#ifndef PROJECTVLIEGVELD_TIME_H
#define PROJECTVLIEGVELD_TIME_H

#include "DesignByContract.h"
#include <string>
#include <sstream>

using namespace std;

class Time {
private:

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
     * Return the time in a formatted style like such: "13:45"
     */
    string formatted() const;

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

};


#endif //PROJECTVLIEGVELD_TIME_H
