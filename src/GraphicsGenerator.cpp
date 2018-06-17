//============================================================================
// Name        : GraphicsGenerator.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
// !
// The file contains mainly large strings with
// preset ini text for the Graphics Engine where just a
// couple of variables like position have to be set.
// !
//============================================================================


#include <cstdlib>
#include "../headers/GraphicsGenerator.h"

using namespace std;

GraphicsGenerator::GraphicsGenerator(Airport* airport): fAirport(airport) {

}


std::string GraphicsGenerator::generateINI(double x, double y, double z) const {
    string ini;

    // Convert eyepoint coordinates to string tuple
    ostringstream converter;
    converter << "eye = (" << x << ", " << y << ", " << z << ")" << endl;
    string eyeTuple = converter.str();

    // Add general section
    ifstream generalTemplate("../IniTemplateGeneral.txt");
    string line;
    while (getline(generalTemplate, line)) {
        ini += line;
    }
    ini += "nrFigures = " + ToString(int(fFigures.size())) + "\n";
    ini += eyeTuple + "\n";

    // Add figures
    for (const auto& figure: fFigures) {
        ini += figure;
    }

    return ini;
}


void GraphicsGenerator::parseCoordinates(const std::string &parameters, double &x, double &y, double &z) {
    int counter = 0;
    string parameter;
    for (char symbol: parameters) {
        if (symbol == ',') {
            if (counter == 0) {
                x = atof(parameter.c_str());
            }
            else {
                y = atof(parameter.c_str());
            }
            counter++;
        }
        else if (isdigit(symbol) or symbol == '.') {
            parameter += symbol;
        }
    }
    z = atof(parameter.c_str());
}


void GraphicsGenerator::addElement(const Runway *runway) {
    // TODO set these
    double x, y, z;

    string figure = "[Figure" + ToString(int(fFigures.size())) + "\n" +
                    "type = \"Face\"\n"
                    "nrPoints = 4\n"
                    "scale = 1\n"
                    "rotateX = 0\n"
                    "rotateY = 0\n"
                    "rotateZ = 0\n";

    // Check type of runway
    figure += "POINTS";
    figure += "CENTER";
    "color = (0.15, 0.15, 0.15)\n";

}


void GraphicsGenerator::addElement(const Airplane *airplane) {
    ostringstream figure;

    ifstream iniTemplate("../IniTemplateAirplane");
    string line;

    // TODO set these
    double x=0, y=0, z=0;
    double xC, yC, zC;
    
    while (getline(iniTemplate, line)) {
        // Comment
        if (line[0] == ';')  {
            continue;
        }

        // New figure
        if (line[0] == '[') {
            // Save this figure
            string newPart = figure.str();
            if (!newPart.empty()) {
                fFigures.push_back(newPart);
            }

            // Reset figure and add new line
            figure.str(string());
            figure << "[Figure" << fFigures.size() << "]\n";
        }

        // Parse center tuple and add values to it
        else if (line.find("center") != string::npos) {
            string parameters = line.substr(line.find('('), line.find(')') - line.find('('));
            parseCoordinates(parameters, xC, yC, zC);
            figure << "center = (" << x + xC << ", " << y + yC << ", " << z + zC << ")\n";
        }

        // Standard line, add to figure
        else {
            figure << line << endl;
        }
    }

    // Save last figure
    string newPart = figure.str();
    if (!newPart.empty()) {
        fFigures.push_back(newPart);
    }
}

/*
void GraphicsGenerator::addAirplane(const Airplane *airplane) {
    // TODO: set these
    double x = 0, y = 0, z = 0;

    // These will be custom for each figure
    double xC, yC, zC;
    ostringstream centerTuple;

    ///
    /// Next come all the figures in the airplane, with added x, y and z coordinates
    ///

    xC = 0 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    string figure = "[Figure" + ToString(fFigures.size()) + "]\n"
                  "type = \"Cylinder\"\n"
                  "height = 5\n"
                  "n = 36\n"
                  "scale = 1\n"
                  "rotateX = 0\n"
                  "rotateY = 90\n"
                  "rotateZ = 0\n"
                  "color = (0.7, 0.7, 0.7)\n" +
                  centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 5 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Sphere\"\n"
            "n = 3\n"
            "scale = 1\n"
            "rotateX = 0\n"
            "rotateY = 0\n"
            "rotateZ = 0\n"
            "color = (0.7, 0.7, 0.7)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 0 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Sphere\"\n"
            "n = 3\n"
            "scale = 1\n"
            "rotateX = 0\n"
            "rotateY = 0\n"
            "rotateZ = 0\n"
            "color = (0.7, 0.7, 0.7)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 5.1 + x;
    yC = 0 + y;
    zC = 1.1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Torus\"\n"
            "r = 1\n"
            "R = 1\n"
            "m = 36\n"
            "n = 36\n"
            "scale = 0.48\n"
            "rotateX = 0\n"
            "rotateY = 0\n"
            "rotateZ = 0\n"
            "color = (0.15, 0.15, 0.15)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 0 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Cone\"\n"
            "height = 2.5\n"
            "n = 36\n"
            "scale = 1\n"
            "rotateX = 0\n"
            "rotateY = -68\n"
            "rotateZ = 0\n"
            "center = (0, 0, 1.07)\n"
            "color = (0.7, 0.7, 0.7)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 0 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Face\"\n"
            "nrPoints = 4\n"
            "point0 = (-0.5, 0, 2)\n"
            "point1 = (-2, 0, 2)\n"
            "point2 = (-2, 0, 3.5)\n"
            "point3 = (-1.5, 0, 3.5)\n"
            "scale = 1\n"
            "rotateX = 0\n"
            "rotateY = 0\n"
            "rotateZ = 0\n"
            "center = (0, 0, 0)\n"
            "color = (0.6, 0.6, 0.6)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 0 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Face\"\n"
            "nrPoints = 4\n"
            "point0 = (3.5, 1, 1)\n"
            "point1 = (1.5, 1, 1)\n"
            "point2 = (1.5, 5, 1)\n"
            "point3 = (1.7, 5, 1)\n"
            "scale = 1\n"
            "rotateX = 0\n"
            "rotateY = 0\n"
            "rotateZ = 0\n"
            "center = (0, 0, 0)\n"
            "color = (0.80, 0.80, 0.80)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 0 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Face\"\n"
            "nrPoints = 4\n"
            "point0 = (3.5, -1, 1)\n"
            "point1 = (1.5, -1, 1)\n"
            "point2 = (1.5, -5, 1)\n"
            "point3 = (1.7, -5, 1)\n"
            "scale = 1\n"
            "rotateX = 0\n"
            "rotateY = 0\n"
            "rotateZ = 0\n"
            "center = (0, 0, 0)\n"
            "color = (0.80, 0.80, 0.80)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 0 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Cylinder\"\n"
            "height = 2.2\n"
            "n = 36\n"
            "scale = 0.48\n"
            "rotateX = 0\n"
            "rotateY = 90\n"
            "rotateZ = 0\n"
            "center = (2, 2.5, 0.49)\n"
            "color = (0.3, 0.3, 0.3)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 0 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Cylinder\"\n"
            "height = 2.2\n"
            "n = 36\n"
            "scale = 0.48\n"
            "rotateX = 0\n"
            "rotateY = 90\n"
            "rotateZ = 0\n"
            "center = (2, -2.5, 0.49)\n"
            "color = (0.3, 0.3, 0.3)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 0 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Face\"\n"
            "nrPoints = 4\n"
            "point0 = (-1.3, 0, 1.8)\n"
            "point1 = (-2, 0, 1.8)\n"
            "point2 = (-2, 2, 1.8)\n"
            "point3 = (-1.8, 2, 1.8)\n"
            "scale = 1\n"
            "rotateX = 0\n"
            "rotateY = 0\n"
            "rotateZ = 0\n"
            "center = (0, 0, 0)\n"
            "color = (0.6, 0.6, 0.6)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;

    xC = 0 + x;
    yC = 0 + y;
    zC = 1 + z;
    centerTuple.str(string());
    centerTuple << "(" << xC << ", " << yC << ", " << zC << ")" << endl;
    figure += "[Figure" + ToString(fFigures.size()) +
            "type = \"Face\"\n"
            "nrPoints = 4\n"
            "point0 = (-1.3, 0, 1.8)\n"
            "point1 = (-2, 0, 1.8)\n"
            "point2 = (-2, -2, 1.8)\n"
            "point3 = (-1.8, -2, 1.8)\n"
            "scale = 1\n"
            "rotateX = 0\n"
            "rotateY = 0\n"
            "rotateZ = 0\n"
            "center = (0, 0, 0)\n"
            "color = (0.6, 0.6, 0.6)\n" +
            centerTuple.str() + "\n\n";
    fFigures.size()++;
} */