//============================================================================
// Name        : Input.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_INPUT_H
#define PROJECTVLIEGVELD_INPUT_H

// STL
#include <iostream>
#include <vector>
#include <cstdlib>

// Libraries
#include "DesignByContract.h"
#include "../tinyXML/tinyxml.h"
#include "TestUtils.h"

// Project headers
#include "Airport.h"
#include "Airplane.h"
#include "Runway.h"
#include "FlightPlan.h"


/**
 * \brief: Class that reads input for the simulation
 */
class Input {
public:

    /**
     * Default constructor
     * \n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    Input();

    /**
     * Reads the given file and stores the information
     * \n REQUIRE(TiXmlDocument::LoadFile(filename.c_str()), "Couldn't open $filename.");
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling read");
     * @param filename: name of the file with input
     * @param errorLog: output stream where errors will be written to
     */
    void read(const std::string& filename, std::ostream& errorLog = std::cerr);

    /**
     * Adds an airport if all data members are initialized.
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addAirport");
     * \n REQUIRE(airport->complete(), "Airport has to be completely initialized to add it to the simulation");
     * \n ENSURE(getAirports().back() == airport, "Airplane was not added to simulation.");
     * @param airport: the airport to be added
     */
    void addAirport(Airport* airport);

    /**
     * Adds a runway if all data members are initialized.
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addRunway");
     * \n REQUIRE(runway->complete(), "Runway has to be completely initialized to add it to the simulation");
     * \n ENSURE(runway->getAirport()->getRunways().back() == runway, "Runway was not added to the airport");
     * @param runway: the runway to be added
     */
    void addRunway(Runway* runway);

    /**
     * Adds a flight plan if all data members are initialized.
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addFlightplan");
     * \n REQUIRE(flightPlan->complete(), "FlightPlan has to be completely initialized to add it to the simulation");
     * \n ENSURE(getFlightPlans().back() == flightPlan, "FlightPlan was not added to simulation.");
     * @param flightPlan: the flight plan to be added
     */
    void addFlightPlan(FlightPlan* flightPlan);

    /**
     * Finds an airport with a specific IATA.
     * \n Returns NULL if not found.
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling findAirportByIATA");
     * @param iata: the iata of the wanted airport
     * @return the airport if found
     */
    Airport* findAirportByIATA(const std::string& iata) const;

    /**
     * Getter for the airports in the simulation
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling getAirports");
     * @return vec of all airports
     */
    std::vector<Airport*> getAirports() const;

    /**
     * Getter for the flight plans in the simulation
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling getFlightPlans");
     * @return vec of all flight plans
     */
    std::vector<FlightPlan*> getFlightPlans() const;

    /**
     * Checks if a given std::string is a valid unsigned int
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling isNumber");
     * @param input: the std::string to be analyzed
     * @return boolean isNumber
     */
    static bool isNumber(const std::string& input);

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

private:

    /**
     * Reads an airport from a given xml element
     * @param elem: the xml element
     * @param errorLog: output stream where errors will be written to     *
     */
    void readAirport(TiXmlElement *elem, std::ostream& errorLog);

    /**
     * Reads a runway from a given xml element
     * @param elem: the xml element
     * @param errorLog: output stream where errors will be written to
     */
    void readRunway(TiXmlElement *elem, std::ostream& errorLog);

    /**
     * Reads an airplane from a given xml element
     * @param elem: the xml element
     * @param errorLog: output stream where errors will be written to
     */
    void readAirplane(TiXmlElement *elem, std::ostream& errorLog);

    /**
     * Reads a taxi route
     * @param elem: the xml element
     * @param airport: the airport where the taxiroute is in
     * @param errorLog: output stream where errors will be written to
     * @return the name of the taxiPoint of the runway if found, else empty
     */
    std::string readTaxiRoute(TiXmlElement *elem, Airport* airport, std::ostream& errorLog);

    /**
     * Reads a flight plan
     * @param elem: the xml element
     * @param errorLog: output stream where errors will be written to
     * @return pointer to the newly made object, if unsuccessful NULL
     */
    FlightPlan* readFlightPlan(TiXmlElement *elem, std::ostream& errorLog);

    /**
     * Vector of pointers to all the airports
     */
    std::vector<Airport*> fAirports;

    /**
     * Vector of pointer to all the flightplans
     */
    std::vector<FlightPlan*> fFlightPlans;

    /**
     * Pointer to itself
     */
    Input *fInitCheck;

};

#endif //PROJECTVLIEGVELD_INPUT_H
