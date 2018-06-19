//============================================================================
// Name        : Graphics.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_GRAPHICSGENERATOR_H
#define PROJECTVLIEGVELD_GRAPHICSGENERATOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "Airport.h"


/**
 * Struct containing info about a runway
 */
struct RunwayInfo {

    /**
     * The runway
     */
    Runway* runway;

    /**
     * Arriving planes at taxipoint of runway
     */
    int arrivingPlanes;

    /**
     * Departing planes at taxipoint of runway
     */
    int departingPlanes;

};


/**
 * Class that generates a string in valid .ini format.
 * \n This string can be used with the Graphics Engine to generate an image
 * \n of all the added elements and thus the airport.
 */
class Graphics {
public:

    /**
     * Constructor. Adds the figures for the gates.
     * \n ENSURE(properlyInitialized(), "Graphics object was not properly constructed");
     * @param airport: airport in the simulation
     */
    Graphics(Airport* airport);

    /**
     * Adds the figures for an airplane.
     * \n The position is calculated with the status and position of the airplane.
     * \n REQUIRE(properlyInitialized(), "Graphics was not properly initialized when calling addElement(airplane)");
     * \n REQUIRE(runway != NULL, "Element can't be NULL when calling addElement");
     * @param airplane: the airplane to be added
     */
    void addElement(Airplane* airplane);

    /**
     * Adds the figures for a runway.
     * \n The position is calculated by the number of runways already added
     * \n REQUIRE(properlyInitialized(), "Graphics was not properly initialized when calling addElement(runway)");
     * \n REQUIRE(airplane != NULL, "Element can't be NULL when calling addElement");
     * @param runway: the runway to be added
     */
    void addElement(Runway* runway);

    /**
     * Generates the ini file of all the elements in the figures vector.
     * \n REQUIRE(properlyInitialized(), "Graphics was not properly initialized when calling generateINI");
     * \n REQUIRE(size > 0, "Size can't be negative");
     * @param x: eye point x coordinate
     * @param y: eye point y coordinate
     * @param z: eye point z coordinate
     * @return string in valid ini format
     */
    std::string generateINI(double x, double y, double z, int size = 3000) const;

    /**
     * Checks if the object is properly initialized
     * @return: Boolean indicating if properly initialized or not.
     */
    bool properlyInitialized() const;

private:

    /**
     * Parses a string of coordinates in the form of "double, double, double"
     * \n and saves each number in the right coordinate.
     * \n The assumption is made that the coordinates string is in a correct format.
     * @param coordinates: string of coordinates, separated with a comma
     * @param x: x coordinate that will be set
     * @param y: y coordinate that will be set
     * @param z: z coordinate that will be set
     */
    void parseCoordinates(const std::string& coordinates, double& x, double& y, double& z) const;

    /**
     * All the figures for the ini file
     */
    std::vector<std::string> fFigures;

    /**
     * Runways added to the drawing
     */
    std::vector<RunwayInfo> fRunways;

    /**
     * Airport used in the simulation
     */
    Airport* fAirport;

    /**
     * Maximum x value
     */
    int fMaximumX;

    /**
     * Maximum y value
     */
    int fMaximumY;

    /**
     * Pointer to itself
     */
    Graphics* fInitCheck;

};

#endif //PROJECTVLIEGVELD_GRAPHICSGENERATOR_H
