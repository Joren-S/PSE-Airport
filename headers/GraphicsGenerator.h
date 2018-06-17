

#ifndef PROJECTVLIEGVELD_GRAPHICSGENERATOR_H
#define PROJECTVLIEGVELD_GRAPHICSGENERATOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <ostringstream>
#include "Airport.h"
#include "TestUtils.h"

class GraphicsGenerator {
public:

    GraphicsGenerator(Airport* airport);

    void addElement(const Airplane* airplane);

    void addElement(const Runway* runway);

    std::string generateINI(double x, double y, double z) const;

private:

    void parseCoordinates(const std::string& parameters, double& x, double& y, double& z);

    std::vector<std::string> fFigures;

    Airport* fAirport;

};

#endif //PROJECTVLIEGVELD_GRAPHICSGENERATOR_H
