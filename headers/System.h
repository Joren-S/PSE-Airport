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
     * The airport of the simulation
     */
    Airport* fAirport;

    ATC* fATC;

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

public:

    /**
     * Constructor
     * ENSURE: properlyInitialized
     */
    System(ostream& atc, Time end);

    /**
     * Destructor
     */
    ~System();

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;


    void setEndTime(Time end);

    void initializeATC(ostream& log);

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
    void run(ostream& log);

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
