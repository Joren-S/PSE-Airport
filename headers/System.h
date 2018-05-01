//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_SYSTEM_H
#define PROJECTVLIEGVELD_SYSTEM_H

// STL
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

// Libraries
#include "gtest/gtest.h"
#include "DesignByContract.h"

// Project headers
#include "Airplane.h"
#include "Airport.h"
#include "Runway.h"
#include "Input.h"
#include "Time.h"
#include "ATC.h"


/**
 * \brief: Main class, controls the simulation
 */
class System {
public:

    /**
     * Constructor
     * ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    System(ostream& atc, Time end);

    /**
     * Default constructor
     * ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    System();

    /**
     * Destructor
     */
    ~System();

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Sets the end time of the simulation
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling setEndTime");
     */
    void setEndTime(Time end);

    /**
     * Initializes the air traffic control of the simulation with an ostream
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling initializeATC");
     */
    void initializeATC(ostream& log);

    /**
     * Imports the given input
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling import");
     */
    void import(Input&);

    /**
     * Logs information of the airports and airplanes to a text file
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling info");
     * REQUIRE(fAirport != NULL, "No airport in the simulation");
     * @param filename: name of the to be generated txt file, default to "../output/info.txt"
     */
    void info(const string &filename = "../output/info.txt");

    /**
     * Runs the complete simulation
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling run");
     * REQUIRE(fAirport != NULL, "No airport in the simulation.");
     * REQUIRE(!simulationFinished(), "Simulation is already finished");.
     * ENSURE(simulationFinished(), "Simulation is not finished yet, error occured");
     */
    void run(ostream& log);

    /**
     * Checks if the simulation has ended, i.e. specified end time has been reached
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling simulationFinished");
     */
    bool simulationFinished() const;

    /**
     * Performs the approach of a given plane.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling approach");
     */
    void approach(Airplane*, ostream& log);

    /**
     * Performs the descend of a given plane.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling descend");
     */
    void descend(Airplane*, ostream& log);

    /**
     * Performs the circling of a given plane.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling circle");
     */
    void circle(Airplane*, ostream& log);

    /**
     * Performs the taxiing upon arrival of a given plane.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling taxiArrival");
     */
    void taxiArrival(Airplane*, ostream& log);

    /**
     * Crosses a runway
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling crossArrival");
     */
    void crossArrival(Airplane*, ostream& log);

    /**
     * Performs the deboarding of a given plane.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling deboard");
     */
    void deboard(Airplane*, ostream& log);

    /**
     * Lands an airplane on the airport of the simulation.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling land");
     */
    void land(Airplane*, ostream& log);

    /**
     * Performs a takeoff of a plane on an airport
     * Logs the info to an output stream, defaulted to "cout".
     * REQUIRE: plane status is kGate &&
     * a runway at target airport is available &&
     * at least one airport in the system
     * ENSURE: plane status is kFinished
     */
    void takeoff(Airplane*, ostream& log) const;

    /**
     * Getter for the airport in the simulation
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling getAirport");
     */
    Airport* getAirport() const;

    /**
     * Getter for the flightplans in the simulation
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling getFlightplans);
     */
    vector<Flightplan*> getFlightplans() const;

private:

    /**
     * Pointer to the system to check for proper initialization
     */
    System *fInitCheck;

    /**
     * Object that keeps track of current time
     */
    Time fTime;

    /**
     * Ending time of simulation
     */
    Time fEndTime;

    /**
     * The airport of the simulation
     */
    Airport* fAirport;

    /**
     * Air traffic control
     */
    ATC* fATC;

    /**
     * Vector containing all the flightplans
     */
    vector<Flightplan*> fFlightplans;

};

#endif //PROJECTVLIEGVELD_SYSTEM_H
