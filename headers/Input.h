//
// Created by Max on 4/7/2018.
//

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

using namespace std;

class Input {
private:

    /**
     * Vector of pointers to all the airports
     */
    vector<Airport*> airports;

    /**
     * Vector of pointers to all the airplanes
     */
    vector<Airplane*> airplanes;

    /**
     * Vector of pointers to all the runways
     */
    vector<Runway*> runways;

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

public:

    /**
     * Constructor, reads all the elements from a given file
     * @param filename: name of the file with input
     */
    Input(const string& filename);

    /**
     * Default constructor, for manually adding elements
     */
    Input() {}

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
     * Adds an airplane to the simulation with the given specifications
     * REQUIRE: airport available
     * ENSURE: airplane is added to the vector of airplanes
     */
    void addAirplane(Airplane* airplane);

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
     * Getter for the airplanes in the simulation
     * @return vec of all airplanes
     */
    vector<Airplane*> getAirplanes() const;

};