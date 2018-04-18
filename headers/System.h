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
     * Vector of pointers to all the airplanes
     */
    vector<Airport*> fAirports;

    /**
     * Vector of pointers to all the runways
     */
    vector<Runway*> fRunways;

    /**
     * Vector of pointers to all the flightplans
     */
    vector<Flightplan*> fFlightplans;

    /**
     * Return how many runways an airport has
     */
    int runwaysInAirport(Airport *ap) const;

    /**
     * Get a free runway from a given airport if available.
     * Return a nullptr otherwise
     */
    Runway* getFreeRunway(Airport* ap) const;

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
    void land(Airplane*, Airport*, ostream& = cout) const;

    /**
     * Performs duties at the gate for an airplane in an airport.
     * Logs the info to an output stream, defaulted to "cout".
     * REQUIRE: plane status is kLanded &&
     * at least one airport in the system
     * ENSURE: plane status is kGate
     */
    void gate(Airplane*, Airport*, ostream& = cout) const;

    /**
     * Performs a takeoff of a plane on an airport
     * Logs the info to an output stream, defaulted to "cout".
     * REQUIRE: plane status is kGate &&
     * a runway at target airport is available &&
     * at least one airport in the system
     * ENSURE: plane status is kFinished
     */
    void takeoff(Airplane*, Airport*, ostream& = cout) const;

public:

    /**
     * Default constructor
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

    /**sys
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
    void log(const string& filename = "../log.txt");

    /**
     * Runs the complete simulation
     * REQUIRE: at least one airport in the system and
     * simulation not ended yet.
     * ENSURE: simulation ended
     */
    void run();

    /**
     * Getter for the airports in the simulation
     * @return vec of all airports
     */
    vector<Airport*> getAirports() const;

    /**
     * Getter for the runways in the simulation
     * @return vec of all runways
     */
    vector<Runway*> getRunways() const;

    /**
     * Getter for the flightplans in the simulation
     * @return
     */
    vector<Flightplan*> getFlightplans() const;

};

#endif //PROJECTVLIEGVELD_SYSTEM_H
