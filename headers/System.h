//============================================================================
// Name        : System.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

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
    void initializeATC(ostream& log, bool test = false);

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
    void run(ostream& log, const string& impressionName = "../output/impressions/impression");

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
     * Performs the technical check of the plane
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling technicalCheck");
     */
    void technicalCheck(Airplane*, ostream& log);

    /**
     * Lands an airplane on the airport of the simulation.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was't initialized when calling land");
     */
    void land(Airplane*, ostream& log);

    /**
     * Ascends an airplane to 5000ft.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling ascend.");
     */
    void ascend(Airplane*, ostream& fLog);

    /**
     * Lets an airplane wait ON a runway before taking off.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling onRunway.");
     */
    void onRunway(Airplane*, ostream& fLog);

    /**
     * Lets an airplane wait AT a runway until instructions are given.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling atRunway");
     */
    void atRunway(Airplane*, ostream& fLog);

    /**
     * Performs a cross.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling taxiDepartureCross");
     */
    void taxiDepartureCross(Airplane*, ostream& fLog);

    /**
     * Performs the next taxi-step.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling taxiDepartureStep");
     */
    void taxiDepartureStep(Airplane*, ostream& fLog);

    /**
     * Lets an airplane wait before taxiing.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling taxiDepartureStart");
     */
    void taxiDepartureStart(Airplane*, ostream& fLog);

    /**
     * Pushes an airplane back from the gate.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling pushback");
     */
    void pushback(Airplane*, ostream& fLog);

    /**
     * Refuels the airplane and lets passengers board.
     * Events are logged to the given ostream&.
     * REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling prepare");
     */
    void prepare(Airplane*, ostream& fLog);

    /**
     * Performs a takeoff of a plane on an airport
     * Logs the info to an output stream, defaulted to "cout".
     * REQUIRE: plane status is kGate &&
     * a runway at target airport is available &&
     * at least one airport in the system
     * ENSURE: plane status is kFinished
     */
    void takeoff(Airplane*, ostream& log);

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
