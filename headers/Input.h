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
     * Finds an airport with a specific IATA
     */
    Airport* findAirportByIATA(const string& iata) const;

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
     * Constructor, reads all the elements from a given file
     * @param filename: name of the file with input
     */
    Input(const string& filename);

    /**
     * Default constructor, for manually adding elements
     */
    Input();

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Adds an airport to the simulation
     * ENSURE: airport is added to the vector of airports
     */
    void addAirport(Airport* airport);

    /**
     * Adds a runway to the simulation with the given specifications
     * ENSURE: runway is added to the vector of runways
     */
    void addRunway(Runway* runway);

    /**
     * Adds a flightplan to the simulation with the given specifications
     * ENSURE: flightplan is added to the vector of flightplans
     */
    void addFlightplan(Flightplan* flightplan);

    /**
     * Getter for the airports in the simulation
     * @return vec of all airports
     */
    vector<Airport*> getAirports() const;

    /**
     * Getter for the flightplans in the simulation
     * @return vec of all flightplans
     */
    vector<Flightplan*> getFlightplans() const;

};