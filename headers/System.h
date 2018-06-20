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
 * \brief: Main class, controls the simulation.
 */
class System {
public:

    /**
     * Constructor
     * \n REQUIRE(!input.getAirports().empty(), "There has to be an airport in the input to start the simulation");
     * \n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     * @param input: the input of the simulation
     * @param atc: the stream where atc messages will be written to
     * @param end: the ending time of the simulation
     */
    System(const Input& input, std::ostream& atc, const Time& end);

    /**
     * Destructor
     */
    ~System();

    /**
     * Runs the complete simulation
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling run");
     * \n REQUIRE(getAirport() != NULL, "No airport in the simulation.");
     * \n REQUIRE(!simulationFinished(), "Simulation is already finished");.
     * \n ENSURE(simulationFinished(), "Simulation is not finished yet, error occurred");
     * @param log: ostream where all the log messages will be written to
     * @param impressionName: basename of the files for the impressions
     * @param iniName: basename of the files for the ini files
     */
    void run(std::ostream& log,
             const std::string& impressionName = "../output/impressions/impression",
             const std::string& iniName = "../output/ini/graphics");

    /**
     * Logs information of the airports and airplanes to a text file
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling info");
     * \n REQUIRE(fAirport != NULL, "No airport in the simulation");
     * @param out: the ostream where the info will be written to
     */
    void info(std::ostream& out);

    /**
     * Generates the images from the start time until the end time, with the use of
     * \n the generated ini files and the graphics engine.
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling generateImages");
     * @param start: time of first image
     * @param end: time of last image, not included
     */
    void generateImages(Time start, Time end);

    /**
     * Checks if the simulation has ended, i.e. specified end time has been reached
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling simulationFinished");
     */
    bool simulationFinished() const;

    /**
     * Getter for the airport in the simulation
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling getAirport");
     */
    Airport* getAirport() const;

    /**
     * Getter for the air traffic control in the simulation
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling getATC");
     */
    ATC* getATC() const;

    /**
     * Getter for the flight plans in the simulation
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling getFlightPlans);
     */
    std::vector<FlightPlan*> getFlightPlans() const;

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

private:

    /**
     * The airport of the simulation
     */
    Airport* fAirport;

    /**
     * Air traffic control
     */
    ATC* fATC;

    /**
     * List of flight plans
     */
    std::vector<FlightPlan*> fFlightPlans;

    /**
     * Ending time of simulation
     */
    Time fEndTime;

    /**
     * Pointer to the system to check for proper initialization
     */
    System *fInitCheck;

};

#endif //PROJECTVLIEGVELD_SYSTEM_H
