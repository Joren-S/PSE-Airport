//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_SYSTEM_H
#define PROJECTVLIEGVELD_SYSTEM_H

// STL
#include <vector>
#include <iostream>
#include <fstream>

// Libraries
#include "gtest/gtest.h"
#include "DesignByContract.h"

// Project headers
#include "Airplane.h"
#include "Airport.h"
#include "Runway.h"
#include "Input.h"
#include "Time.h"


class System {
private:

    /**
     * Pointer to the system to check for proper initialization
     */
    System *fInitCheck;

    /**
     * Object that keeps track of time
     */
    Time fTime;

    /**
     * Ending time of simulation
     */
    Time fEndTime;

    /**
     * Output stream for ATC
     */
    ostream& fATC;

    /**
     * Output stream for all logging
     */
    ostream& fLog;

    /**
     * The airport of the simulation
     */
    Airport* fAirport;

    bool f3Free;
    bool f5Free;

    /**
     * Vector containing all the flightplans
     */
    vector<Flightplan*> fFlightplans;

    /**
     * Checks if the simulation has ended.
     * i.e. the status of all planes is kFinished
     */
    bool simulationFinished() const;

    /**
     * Lands an airplane on an airport.
     * Logs the info to an output stream, defaulted to "cout".
     * REQUIRE: plane status is approaching &&
     * a gate at target airport is available &&
     * a runway at target airport is available &&
     * at least one airport in the system
     * ENSURE: plane status is kLanded
     */
    void land(Airplane*) const;

    /**
     * Performs duties at the gate for an airplane in an airport.
     * Logs the info to an output stream, defaulted to "cout".
     * REQUIRE: plane status is kLanded &&
     * at least one airport in the system
     * ENSURE: plane status is kGate
     */
    void gate(Airplane*) const;

    /**
     * Performs a takeoff of a plane on an airport
     * Logs the info to an output stream, defaulted to "cout".
     * REQUIRE: plane status is kGate &&
     * a runway at target airport is available &&
     * at least one airport in the system
     * ENSURE: plane status is kFinished
     */
    void takeoff(Airplane*) const;

public:

    /**
     * Constructor
     * ENSURE: properlyInitialized
     */
    System(ostream& atc, ostream& log, Time end);

    /**
     * Destructor
     */
    ~System();

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Imports the given input
     * REQUIRE: properlyInitialized
     */
    void import(Input&);

    /**
     * Logs information of the airports and airplanes to a
     * text file.
     * REQUIRE: at least one airport in the system
     * @param filename: name of the to be generated txt file
     */
    void info(const string &filename = "../output/info.txt");

    /**
     * Runs the complete simulation
     * REQUIRE: at least one airport in the system and
     * simulation not ended yet.
     * ENSURE: simulation ended
     */
    void run();

    /**
     * Getter for the airport in the simulation
     */
    Airport* getAirport() const;

    /**
     * Getter for the flightplans in the simulation
     */
    vector<Flightplan*> getFlightplans() const;

};

#endif //PROJECTVLIEGVELD_SYSTEM_H
