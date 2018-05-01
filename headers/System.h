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


class System {
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

    /**
     * Checks if the simulation has ended.
     * i.e. the status of all planes is kFinished
     */
    bool simulationFinished() const;

    /**
     * Performs the approach of a given plane.
     * Events are logged to the given ostream&.
     */
    void approach(Airplane*, ostream& log);

    /**
     * Performs the descend of a given plane.
     * Events are logged to the given ostream&.
     */
    void descend(Airplane*, ostream& log);

    /**
     * Performs the circling of a given plane.
     * Events are logged to the given ostream&.
     */
    void circle(Airplane*, ostream& log);

    /**
     * Performs the taxiing upon arrival of a given plane.
     * Events are logged to the given ostream&.
     */
    void taxiArrival(Airplane*, ostream& log);

    /**
     * Crosses a runway
     * Events are logged to the given ostream&.
     */
    void crossArrival(Airplane*, ostream& log);

    /**
     * Performs the deboarding of a given plane.
     * Events are logged to the given ostream&.
     */
    void deboard(Airplane*, ostream& log);

    /**
     * Lands an airplane on the airport of the simulation.
     * Events are logged to the given ostream&.
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
