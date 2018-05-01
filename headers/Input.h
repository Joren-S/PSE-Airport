//============================================================================
// Name        : Input.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_INPUT_H
#define PROJECTVLIEGVELD_INPUT_H

#endif //PROJECTVLIEGVELD_INPUT_H

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

using namespace std;

/**
 * \brief: Class that reads input for the simulation
 */
class Input {
private:

    /**
     * Pointer to itself
     */
    Input *fInitCheck;

    /**
     * Vector of pointers to all the airports
     */
    vector<Airport*> airports;

    /**
     * Vector of pointer to all the flightplans
     */
     vector<Flightplan*> flightplans;

    /**
     * Reads an airport from a given xml element
     */
    void readAirport(TiXmlElement *elem);

    /**
     * Reads a runway from a given xml element
     */
    void readRunway(TiXmlElement *elem);

    /**
     * Reads an airplane from a given xml element
     */
    void readAirplane(TiXmlElement *elem);

    /**
     * Reads a taxiroute and return the name of the
     * taxipoint of the runway.
     * When returning the empty string, taxiroute
     * was not valid.
     */
    string readTaxiroute(TiXmlElement *elem, Airport*);

    /**
     * Reads a flightplan from a given xml element
     * Returns a pointer to the flightplan, so when there is an error in
     * reading it or the associated airplane, we can do cleanup
     */
    Flightplan* readFlightplan(TiXmlElement *elem);

public:

    /**
     * Default constructor
     * ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    Input();

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Reads the given file and stores the information
     * REQUIRE(this->properlyInitialized(), "Input was't initialized when calling read");
     * @param filename: name of the file with input
     */
    void read(const string& filename);

    /**
     * Adds an airport to the simulation
     * REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addAirport");
     * REQUIRE(airport->complete(), "Airport has to be completely initialized to add it to the simulation");
     * ENSURE(airports.back() == airport, "Airplane was not added to simulation.");
     */
    void addAirport(Airport* airport);

    /**
     * Adds a runway to the simulation with the given specifications
     * REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addRunway");
     * REQUIRE(runway->complete(), "Runway has to be completely initialized to add it to the simulation");
     * ENSURE(runway->getAirport()->getRunways().back() == runway, "Runway was not added to the airport");
     */
    void addRunway(Runway* runway);

    /**
     * Adds a flightplan to the simulation with the given specifications
     * REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addFlightplan");
     * REQUIRE(flightplan->complete(), "Flightplan has to be completely initialized to add it to the simulation");
     * ENSURE(flightplans.back() == flightplan, "Flightplan was not added to simulation.");
     */
    void addFlightplan(Flightplan* flightplan);

    /**
     * Finds an airport with a specific IATA
     * REQUIRE(this->properlyInitialized(), "Input was't initialized when calling findAirportByIATA");
     */
    Airport* findAirportByIATA(const string& iata) const;

    /**
     * Getter for the airports in the simulation
     * REQUIRE(this->properlyInitialized(), "Input was't initialized when calling getAirports");
     * @return vec of all airports
     */
    vector<Airport*> getAirports() const;

    /**
     * Getter for the flightplans in the simulation
     * REQUIRE(this->properlyInitialized(), "Input was't initialized when calling getFlightplans");
     * @return vec of all flightplans
     */
    vector<Flightplan*> getFlightplans() const;

};