//
// Created by Max Van Houcke on 12-4-2018.
//

#ifndef PROJECTVLIEGVELD_FLIGHTPLAN_H
#define PROJECTVLIEGVELD_FLIGHTPLAN_H

#include <string>
#include "DesignByContract.h"

using namespace std;

class Flightplan {
private:

    /**
     * IATA of destination airport.
     */
    string fDestination;

    /**
     * Minute of departure
     */
    int fDeparture;

    /**
     * Minute of arrival
     */
    int fArrival;

    /**
     * Hours between flights
     */
    int fInterval;

public:

    /**
     * Default constructor
     */
    Flightplan();

    /**
     * Setter for departure time
     * REQUIRE: 0 <= departure < 60
     */
    void setDeparture(int fDeparture);

    /**
     * Setter for arrival time
     * REQUIRE: 0 <= arrival < 60
     */
    void setArrival(int fArrival);

    /**
     * Setter for interval
     * REQUIRE: interval > 0
     */
    void setInterval(int fInterval);

    // Remaining Getters and Setters
    const std::string &getDestination() const;
    void setDestination(const std::string &fDestination);
    int getDeparture() const;
    int getArrival() const;
    int getInterval() const;

};


#endif //PROJECTVLIEGVELD_FLIGHTPLAN_H
