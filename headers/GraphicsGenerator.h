

#ifndef PROJECTVLIEGVELD_GRAPHICSGENERATOR_H
#define PROJECTVLIEGVELD_GRAPHICSGENERATOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Airport.h"
#include "TestUtils.h"

/**
 * Class that generates a string in valid .ini format.
 * \n This string can be used with the Graphics Engine to generate an image
 * \n of all the added elements and thus the airport.
 */
class GraphicsGenerator {
public:

    /**
     * Constructor. Adds the figures for the gates.
     * @param gates: amount of gates in the airport
     */
    GraphicsGenerator(int gates);

    /**
     * Adds the figures for an airplane.
     * \n The position is calculated with the status and position of the airplane.
     * @param airplane: the airplane to be added
     */
    void addElement(Airplane* airplane);

    /**
     * Adds the figures for a runway.
     * \n The position is calculated by the number of runways already added
     * @param runway: the runway to be added
     */
    void addElement(Runway* runway);

    /**
     * Generates the ini file of all the elements in the figures vector.
     * @param x: eye point x coordinate
     * @param y: eye point y coordinate
     * @param z: eye point z coordinate
     * @return string in valid ini format
     */
    std::string generateINI(double x, double y, double z) const;

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
    std::vector<Runway*> fRunways;

    /**
     * Maximum x value
     */
    int fMaximumX;

    /**
     * Maximum y value
     */
    int fMaximumY;

};

#endif //PROJECTVLIEGVELD_GRAPHICSGENERATOR_H
