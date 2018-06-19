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
     * \n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     * @param atc:
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
     * Sets the end time of the simulation
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling setEndTime");
     * @param sets the
     */
    void setEndTime(Time end);

    /**
     * Initializes the air traffic control of the simulation with an std::ostream
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling initializeATC");
     * @param log: ostream where all the ATC messages will be written to
     * @param test: if true, it will set all the squawk codes to zero, for testing purposes
     */
    void initializeATC(std::ostream& log, bool test = false);

    /**
     * Imports the given input
     * \n REQUIRE(this->properlyInitialized(), "System was't initialized when calling import");
     * @param input: the input used in the simulation
     */
    void import(Input& input);

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
     * @param log: ostream where all the log messages will be written to
     * @param impressionName: basename of the files for the impressions
     * @param iniName: basename of the files for the ini files
     */
    void run(std::ostream& log,
             const std::string& impressionName = "../output/impressions/impression",
             const std::string& iniName = "../output/ini/graphics");

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
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;


private:

    /**
     * Pointer to the system to check for proper initialization
     */
    System *fInitCheck;

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
