//
// Created by Max Van Houcke on 12-4-2018.
//

#ifndef PROJECTVLIEGVELD_FLIGHTPLAN_H
#define PROJECTVLIEGVELD_FLIGHTPLAN_H

#include <string>
#include "DesignByContract.h"
#include "Time.h"
#include "Airplane.h"

enum EEvent { kLand, kTakeoff, kNothing };

using namespace std;

class Flightplan {
private:
    /**
     * Pointer to itself
     */
    Flightplan *fInitCheck;

    /**
     * Airplane associated with the flightplan
     */
    Airplane* fAirplane;

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
     * Destructor
     */
    ~Flightplan();

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

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

    /**
     * Returns the event at the given time
     * @param time
     */
    EEvent getEvent(Time time);

    /**
     * Checks if all the data members were initialized
     */
    bool complete() const;

    // Remaining Getters and Setters
    const std::string &getDestination() const;
    void setDestination(const std::string &fDestination);
    int getDeparture() const;
    int getArrival() const;
    int getInterval() const;
    void setAirplane(Airplane*);
    Airplane* getAirplane() const;

};


#endif //PROJECTVLIEGVELD_FLIGHTPLAN_H
