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


/**
 * Class that represents the time
 */
class Time {
public:

    /**
     * Constructor, sets the time
     * \n Defaults to 12:00, which is the starting point of the simulation
     * \n REQUIRE(minute < 60 && minute >= 0, "Minute has to be between 0 and 60");
     * \n REQUIRE(hour < 24 && hour >= 0, "Hour has to be between 0 and 24");
     * \n ENSURE(properlyInitialized(), "Time wasn't properly initialized after constructing.");
     * @param hour: the hour
     * @param minute: the minute
     */
    Time(int hour = 12, int minute = 0);

    /**
     * Copy constructor
     * \n ENSURE(properlyInitialized(), "Time wasn't properly initialized after constructing.");
     * @param time: object to be copied
     */
    Time(const Time& time);

    /**
     * Return the time in a formatted style like such: "13:45"
     * \n REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling formatted");
     * @return string of time
     */
    std::string formatted() const;

    /**
     * Advances the time by an amount of minutes
     * \n REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling advance");
     * \n REQUIRE(minutes >= 0, "Advancing by a negative amount of minutes is not possible");
     */
    void advance(int minutes = 1);

    /**
     * Setter for the minute.
     * \n REQUIRE(minute < 60 && minute >= 0, "Minute has to be between 0 and 60");
     * \n REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling setMinute");
     */
    void setMinute(int minute);

    /**
     * Getter for the minute
     * \n REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling getMinute");
     */
    int getMinute() const;

    /**
     * Setter for the hour.
     * \n REQUIRE(hour < 24 && hour >= 0, "Hour has to be between 0 and 24");
     * \n REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling setHour");
     */
    void setHour(int hour);

    /**
     * Getter for the hour
     * \n REQUIRE(properlyInitialized(), "Time wasn't properlyInitialized when calling getHour");
     */
    int getHour() const;

    /**
     * Checks if the object is properly initialized
     * @return: Boolean indicating if properly initialized or not.
     */
    bool properlyInitialized() const;

    /**
     * Operator== overloaded
     */
    bool operator==(const Time&) const;

    /**
     * Operator< overloaded, returns false if comparing with 00:00
     */
    bool operator<(const Time&) const;

    /**
     * Assignment operator
     * @param time: rhs of operator
     * @return reference to this
     */
    Time& operator=(const Time& time);

private:

    /**
     * Member indicating the minute
     */
    int fMinute;

    /**
     * Member indicating the hour
     */
    int fHour;

    /**
     * Pointer to itself
     */
    Time* fInitCheck;

};


#endif //PROJECTVLIEGVELD_TIME_H
