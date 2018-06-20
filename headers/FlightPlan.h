//============================================================================
// Name        : Flightplan.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_FLIGHTPLAN_H
#define PROJECTVLIEGVELD_FLIGHTPLAN_H

#include <string>
#include "DesignByContract.h"
#include "Time.h"
#include "Airplane.h"

/**
 * enum that indicates an event for the plane
 */
enum EEvent { kLand, kTakeoff, kNothing };



/**
 * \brief: Class that represents a flight plan in the simulation
 */
class FlightPlan {
private:

    /**
     * Pointer to itself
     */
    FlightPlan *fInitCheck;

    /**
     * Airplane associated with the flightplan
     */
    Airplane* fAirplane;

    /**
     * IATA of destination airport.
     */
    std::string fDestination;

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
     * \n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    FlightPlan();

    /**
     * Destructor
     */
    ~FlightPlan();

    /**
     * Checks if the object is properly initialized
     * @return: boolean indicating if properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Setter for departure time
     * \n REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling setDeparture");
     * \n REQUIRE(departure >= 0 && departure < 60, "Departure has to be between 0 and 60");
     * @param departure: departure time to set
     */
    void setDeparture(int departure);

    /**
     * Setter for arrival time
     * \n REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling setArrival");
     * \n REQUIRE(arrival >= 0 && arrival < 60, "Arrival has to be between 0 and 60");
     * @param arrival: arrival time to set
     */
    void setArrival(int arrival);

    /**
     * Setter for interval
     * \n REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling setInterval");
     * \n REQUIRE(interval > 0, "Interval has to be at least 1");
     * @param interval: interval to set
     */
    void setInterval(int interval);

    /**
     * Returns the event at the given time
     * \n REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling getEvent");
     * @param time: time to get event at
     * @return event at this time
     */
    EEvent getEvent(Time time);

    /**
     * Checks if all the data members were initialized
     * \n REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling complete");
     * @return: boolean indicating if all members are initialized.
     */
    bool complete() const;

    ////////////////
    ///  Getters and setters for the fields of the class.
    ///  For all:
    ///  REQUIRE(properlyInitialized(), "Flightplan wasn't properly initialized when calling getter/setter.");
    ///////////////

    const std::string &getDestination() const;
    void setDestination(const std::string &fDestination);
    int getDeparture() const;
    int getArrival() const;
    int getInterval() const;
    void setAirplane(Airplane*);
    Airplane* getAirplane() const;

};


#endif //PROJECTVLIEGVELD_FLIGHTPLAN_H
