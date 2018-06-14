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
     * \n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    System(std::ostream& atc, Time end);

    /**
     * Default constructor
     * \n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
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
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling setEndTime");
     */
    void setEndTime(Time end);

    /**
     * Initializes the air traffic control of the simulation with an std::ostream
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling initializeATC");
     */
    void initializeATC(std::ostream& log, bool test = false);

    /**
     * Imports the given input
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling import");
     */
    void import(Input&);

    /**
     * Logs information of the airports and airplanes to a text file
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling info");
     * \n REQUIRE(fAirport != NULL, "No airport in the simulation");
     * @param filename: name of the to be generated txt file, default to "../output/info.txt"
     */
    void info(const std::string &filename = "../output/info.txt");

    /**
     * Runs the complete simulation
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling run");
     * \n REQUIRE(fAirport != NULL, "No airport in the simulation.");
     * \n REQUIRE(!simulationFinished(), "Simulation is already finished");.
     * \n ENSURE(simulationFinished(), "Simulation is not finished yet, error occured");
     */
    void run(std::ostream& log, const std::string& impressionName = "../output/impressions/impression");

    /**
     * Checks if the simulation has ended, i.e. specified end time has been reached
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling simulationFinished");
     */
    bool simulationFinished() const;

    /**
     * Performs the approach of a given plane.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling approach");
     */
    void approach(Airplane*, std::ostream& log);

    /**
     * Performs the descend of a given plane.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling descend");
     */
    void descend(Airplane*, std::ostream& log);

    /**
     * Performs the circling of a given plane.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling circle");
     */
    void circle(Airplane*, std::ostream& log);

    /**
     * Performs the taxiing upon arrival of a given plane.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling taxiArrival");
     */
    void taxiArrival(Airplane*, std::ostream& log);

    /**
     * Crosses a runway
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling crossArrival");
     */
    void crossArrival(Airplane*, std::ostream& log);

    /**
     * Performs the deboarding of a given plane.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling deboard");
     */
    void deboard(Airplane*, std::ostream& log);

    /**
     * Performs the technical check of the plane
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling technicalCheck");
     */
    void technicalCheck(Airplane*, std::ostream& log);

    /**
     * Lands an airplane on the airport of the simulation.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling land");
     */
    void land(Airplane*, std::ostream& log);

    /**
     * Ascends an airplane to 5000ft.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling ascend.");
     */
    void ascend(Airplane*, std::ostream& fLog);

    /**
     * Lets an airplane wait ON a runway before taking off.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling onRunway.");
     */
    void onRunway(Airplane*, std::ostream& fLog);

    /**
     * Lets an airplane wait AT a runway until instructions are given.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling atRunway");
     */
    void atRunway(Airplane*, std::ostream& fLog);

    /**
     * Performs a cross.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling taxiDepartureCross");
     */
    void taxiDepartureCross(Airplane*, std::ostream& fLog);

    /**
     * Performs the next taxi-step.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling taxiDepartureStep");
     */
    void taxiDepartureStep(Airplane*, std::ostream& fLog);

    /**
     * Lets an airplane wait before taxiing.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling taxiDepartureStart");
     */
    void taxiDepartureStart(Airplane*, std::ostream& fLog);

    /**
     * Pushes an airplane back from the gate.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling pushback");
     */
    void pushback(Airplane*, std::ostream& fLog);

    /**
     * Refuels the airplane and lets passengers board.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling prepare");
     */
    void prepare(Airplane*, std::ostream& fLog);

    /**
     * Performs a takeoff of a plane on an airport
     * Logs the info to an output stream, defaulted to "cout".
     * \n REQUIRE: plane status is kGate &&
     * a runway at target airport is available &&
     * at least one airport in the system
     * \n ENSURE: plane status is kFinished
     */
    void takeoff(Airplane*, std::ostream& log);

    /**
     * Getter for the airport in the simulation
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling getAirport");
     */
    Airport* getAirport() const;

    /**
     * Getter for the flightplans in the simulation
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling getFlightplans);
     */
    std::vector<Flightplan*> getFlightplans() const;

    /**
     * Getter for the air traffic control in the simulation
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling getATC");
     */
    ATC* getATC() const;

    /**
     * Getter for the current time
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling getTime");
     */
    Time getTime() const;

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
    std::vector<Flightplan*> fFlightplans;

};

#endif //PROJECTVLIEGVELD_SYSTEM_H
