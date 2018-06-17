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

GraphicsGenerator::GraphicsGenerator(int gates): nrRunways(0), maximumLength(0), gates(gates) {
    ostringstream stream;

    string cube = "type = \"Cube\"\n"
                  "rotateX = 0\n"
                  "rotateY = 0\n"
                  "rotateZ = 0\n";

    int offset = gates / 2 + 2;

    int xMax = 0;
    int xMin = 0;

    for (int i = 0; i < gates * 2; i++) {
        stream << "[Figure" << fFigures.size() << "]" << endl;
        stream << cube;
        stream << "center = (" << i * 6 - offset * 6 << ", 0, 2)" << endl;
        stream << "color = (0.7, 0.7, 0.7)" << endl;
        stream << "scale = 3" << endl << endl;
        fFigures.push_back(stream.str());

        stream.str(string());

        int x = i * 6 - offset * 6;

        if (xMin > x) {
            xMin = x;
        }
        if (xMax < x) {
            xMax = x;
        }

        if (i % 2 == 0) {
            stream << "[Figure" << fFigures.size() << "]" << endl;
            stream << cube;
            stream << "center = (" << i * 6 - offset * 6<< ", -4, 0)" << endl;
            stream << "color = (0.5, 0.5, 0.5)" << endl;
            stream << "scale = 1" << endl << endl;
            fFigures.push_back(stream.str());
            stream.str(string());
            stream << "[Figure" << fFigures.size() << "]" << endl;
            stream << cube;
            stream << "center = (" << i * 6 - offset * 6<< ", -4, 2)" << endl;
            stream << "color = (0.5, 0.5, 0.5)" << endl;
            stream << "scale = 1" << endl << endl;
            fFigures.push_back(stream.str());
            stream.str(string());
            stream << "[Figure" << fFigures.size() << "]" << endl;
            stream << cube;
            stream << "center = (" << i * 6 - offset * 6<< ", -6, 0)" << endl;
            stream << "color = (0.5, 0.5, 0.5)" << endl;
            stream << "scale = 1" << endl << endl;
            fFigures.push_back(stream.str());
            stream.str(string());
            stream << "[Figure" << fFigures.size() << "]" << endl;
            stream << cube;
            stream << "center = (" << i * 6 - offset * 6<< ", -6, 2)" << endl;
            stream << "color = (0.5, 0.5, 0.5)" << endl;
            stream << "scale = 1" << endl << endl;
            fFigures.push_back(stream.str());
            stream.str(string());
        }
    }

    stream << "[Figure" << fFigures.size() << "]" << endl;
    stream << "type = \"Face\"\n"
              "nrPoints = 4\n"
              "scale = 1\n"
              "rotateX = 0\n"
              "rotateY = 0\n"
              "rotateZ = 0\n"
              "center = (0, 0, 0)\n"
              "color = (0.3, 0.3, 0.3)\n";
    stream << "point0 = (" << xMax + 7 << ", 2, -0.5)" << endl;
    stream << "point1 = (" << xMax + 7 << ", -25, -0.5)" << endl;
    stream << "point2 = (" << xMin - 7 << ", -25, -0.5)" << endl;
    stream << "point3 = (" << xMin - 7 << ", 2, -0.5)" << endl;
    fFigures.push_back(stream.str());
}


std::string GraphicsGenerator::generateINI(double x, double y, double z) const {
    ostringstream ini;

    // Add general section
    ifstream generalTemplate("../IniTemplateGeneral.txt");
    string line;
    while (getline(generalTemplate, line)) {
        ini << line << endl;
    }
    ini << "nrFigures = " << fFigures.size() + 2 << endl;
    ini << "eye = (" << x << ", " << y << ", " << z << ")\n" << endl;
    generalTemplate.close();

    // Add figures
    vector<string>::const_iterator itr;
    for (itr = fFigures.begin(); itr != fFigures.end(); ++itr) {
        ini << *itr;
    }

    // Make a rectangle that contains the whole airport

    ini << "[Figure" << fFigures.size() << "]" << endl;
    ini << "type = \"Face\"\n"
              "nrPoints = 4\n"
              "scale = 1\n"
              "rotateX = 0\n"
              "rotateY = 0\n"
              "rotateZ = 0\n"
              "center = (0, 0, 0)\n"
              "color = (0.27, 0.34, 0.15)\n";

    int xLength = nrRunways * 20 + 5;

    ini << "point0 = (" << maximumLength / 2 + 10 << ", " << - 2 * xLength << ", -1)" << endl;
    ini << "point1 = (" << maximumLength / 2 + 10 << ", " << 10 << ", -1)" << endl;
    ini << "point2 = (" << - maximumLength / 2 - 10 << ", " << 10 << ", -1)" << endl;
    ini << "point3 = (" << - maximumLength / 2 - 10 << ", " << - 2 * xLength << ", -1)" << endl << endl;


    ini << "[Figure" << fFigures.size() + 1 << "]" << endl;
    ini << "type = \"Face\"\n"
              "nrPoints = 4\n"
              "scale = 1\n"
              "rotateX = 0\n"
              "rotateY = 0\n"
              "rotateZ = 0\n"
              "center = (0, 0, 0)\n"
              "color = (0.3, 0.3, 0.3)\n";

    ini << "point0 = (-7" << ", " << -5 << ", -0.5)" << endl;
    ini << "point1 = (7" << ", " << 0 << ", -0.5)" << endl;
    ini << "point2 = (7" << ", " << - 2 * xLength + 15 << ", -0.5)" << endl;
    ini << "point3 = (-7" << ", " << - 2 * xLength + 15 << ", -0.5)" << endl << endl;


//    ini << "[Figure" << fFigures.size() << "]\n";
//    ini << "type = \"Face\"\n"
//              "nrPoints = 4\n"
//              "scale = 1\n"
//              "rotateX = 0\n"
//              "rotateY = 0\n"
//              "rotateZ = 0\n";
//
//
//    // Set right points according to length
//    ini << "point0 = ("  << runwayLength / 2 << ", -7, 0)" << endl;
//    ini << "point1 = ("  << runwayLength / 2 << ", 7, 0)"  << endl;
//    ini << "point2 = (-" << runwayLength / 2 << ", 7, 0)"  << endl;
//    ini << "point3 = (-" << runwayLength / 2 << ", -7, 0)" << endl;
//    ini << "center";
//    ini << "color";

    return ini.str();
}


void GraphicsGenerator::parseCoordinates(const std::string &coordinates, double &x, double &y, double &z) const {
    string coordinate;

    // Keeps up how many coordinates we've saved
    int counter = 0;

    // Iterate over all chars in the coordinates
    string::const_iterator itr;
    for (itr = coordinates.begin(); itr != coordinates.end(); ++itr) {
        char symbol = *itr;

        // Parameter ended
        if (symbol == ',') {

            // Save it in the right coordinate
            if (counter == 0) {
                x = atof(coordinate.c_str());
            }
            else {
                y = atof(coordinate.c_str());
            }

            // Reset and increase counter
            coordinate = "";
            counter++;
        }

        // If it's a digit, point or minus, add to coordinate
        else if (isdigit(symbol) or symbol == '.' or symbol == '-') {
            coordinate += symbol;
        }
    }

    // Add last one
    z = atof(coordinate.c_str());
}


void GraphicsGenerator::addElement(const Runway *runway) {
    REQUIRE(runway != NULL, "Runway can't be NULL when calling addElement");

    ostringstream figure;

    // Add standard information
    figure << "[Figure" << fFigures.size() << "]\n";
    figure << "type = \"Face\"\n"
              "nrPoints = 4\n"
              "scale = 1\n"
              "rotateX = 0\n"
              "rotateY = 0\n"
              "rotateZ = 0\n";

    // Drawing is scaled 1:7
    int runwayLength = runway->getLength() / 7;

    if (runwayLength > maximumLength) {
        maximumLength = runwayLength;
    }

    // Set right points according to length
    figure << "point0 = ("  << runwayLength / 2 << ", -7, 0)" << endl;
    figure << "point1 = ("  << runwayLength / 2 << ", 7, 0)"  << endl;
    figure << "point2 = (-" << runwayLength / 2 << ", 7, 0)"  << endl;
    figure << "point3 = (-" << runwayLength / 2 << ", -7, 0)" << endl;


    int offset = nrRunways * 30 + 45;

    // Center at right position
    figure << "center = (0, -" << offset << ", 0)" << endl;

    // Set right color
    if (runway->getType() == kAsphalt) {
        figure << "color = (0.15, 0.15, 0.15)\n" << endl;
    }
    else {
        figure << "color = (0.17, 0.70, 0.21)\n" << endl;
    }

    nrRunways++;
    fFigures.push_back(figure.str());
}


void GraphicsGenerator::addElement(const Airplane *airplane) {
    ostringstream figure;

//    ifstream iniTemplate("../airplaneTemplate.txt");
    ifstream iniTemplate("../airplaneArrivalTemplate.txt");
//    ifstream iniTemplate("../airplaneDepartureTemplate.txt");
    string line;

    // TODO set these
    double x=12, y=-11, z=0;
//    double rotateZ = 90;

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
            string coordinates = line.substr(line.find('(') + 1, line.find(')') - line.find('(') - 1);
            parseCoordinates(coordinates, xC, yC, zC);
            figure << "center = (" << x + xC << ", " << y + yC << ", " << z + zC << ")\n";
        }

//        else if (line.find("rotateZ") != string::npos) {
//            string angleStr = line.substr(line.find('='));
//            double angle = atof(angleStr.c_str()) + rotateZ;
//            figure << "rotateZ = " << angle << endl;
//        }

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