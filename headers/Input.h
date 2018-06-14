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

// Project headers
#include "Airport.h"
#include "Airplane.h"
#include "Runway.h"
#include "Flightplan.h"


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
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Reads the given file and stores the information
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling read");
     * @param filename: name of the file with input
     * @param errorLog: output stream where errors will be written to
     */
    void read(const std::string& filename, std::ostream& errorLog = std::cerr);

    /**
     * Adds an airport to the simulation
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addAirport");
     * \n REQUIRE(airport->complete(), "Airport has to be completely initialized to add it to the simulation");
     * \n ENSURE(airports.back() == airport, "Airplane was not added to simulation.");
     * @param airport: the airport to be added
     */
    void addAirport(Airport* airport);

    /**
     * Adds a runway to the simulation with the given specifications
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addRunway");
     * \n REQUIRE(runway->complete(), "Runway has to be completely initialized to add it to the simulation");
     * \n ENSURE(runway->getAirport()->getRunways().back() == runway, "Runway was not added to the airport");
     * @param runway: the runway to be added
     */
    void addRunway(Runway* runway);

    /**
     * Adds a flightplan to the simulation with the given specifications
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addFlightplan");
     * \n REQUIRE(flightplan->complete(), "Flightplan has to be completely initialized to add it to the simulation");
     * \n ENSURE(flightplans.back() == flightplan, "Flightplan was not added to simulation.");
     * @param flightplan: the flightplan to be added
     */
    void addFlightplan(Flightplan* flightplan);

    /**
     * Finds an airport with a specific IATA.
     * \n Returns NULL if not found.
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling findAirportByIATA");
     * @param iata: the iata of the wanted airport
     */
    Airport* findAirportByIATA(const std::string& iata) const;

    /**
     * Getter for the airports in the simulation
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling getAirports");
     * @return vec of all airports
     */
    std::vector<Airport*> getAirports() const;

    /**
     * Getter for the flightplans in the simulation
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling getFlightplans");
     * @return vec of all flightplans
     */
    std::vector<Flightplan*> getFlightplans() const;

    /**
     * Checks if a given std::string is a valid unsigned int
     * \n REQUIRE(this->properlyInitialized(), "Input was't initialized when calling isNumber");
     * @param input: the std::string to be analyzed
     */
    static bool isNumber(const std::string& input);

private:

    /**
     * Pointer to itself
     */
    Input *fInitCheck;

    /**
     * Vector of pointers to all the airports
     */
    std::vector<Airport*> airports;

    /**
     * Vector of pointer to all the flightplans
     */
    std::vector<Flightplan*> flightplans;

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
     * \n REQUIRE(!airports.empty(), "No airport in simulation");
     * @param elem: the xml element
     * @param errorLog: output stream where errors will be written to
     */
    void readAirplane(TiXmlElement *elem, std::ostream& errorLog);

    /**
     * Reads a taxiroute and returns the name of the taxipoint of the runway.
     * \n If invalid, returns the empty std::string.
     * @param elem: the xml element
     * @param airport: the airport where the taxiroute is in
     * @param errorLog: output stream where errors will be written to
     */
    std::string readTaxiroute(TiXmlElement *elem, Airport* airport, std::ostream& errorLog);

    /**
     * Reads a flightplan and returns a pointer to the newly made object.
     * \n If invalid, returns NULL.
     * @param elem: the xml element
     * @param errorLog: output stream where errors will be written to
     */
    Flightplan* readFlightplan(TiXmlElement *elem, std::ostream& errorLog);

};

#endif //PROJECTVLIEGVELD_INPUT_H
