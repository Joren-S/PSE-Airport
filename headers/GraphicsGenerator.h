

#ifndef PROJECTVLIEGVELD_GRAPHICSGENERATOR_H
#define PROJECTVLIEGVELD_GRAPHICSGENERATOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Airport.h"
#include "TestUtils.h"

class GraphicsGenerator {
public:

    GraphicsGenerator(int gates);

    void addElement(const Airplane* airplane);

    void addElement(const Runway* runway);

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

    std::vector<std::string> fFigures;

    int nrRunways;

    int maximumLength;

    int gates;

};

#endif //PROJECTVLIEGVELD_GRAPHICSGENERATOR_H
