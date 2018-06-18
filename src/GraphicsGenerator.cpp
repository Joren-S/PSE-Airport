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

GraphicsGenerator::GraphicsGenerator(int gates): fMaximumY(0) {
    // Stream used to save all the figures
    ostringstream stream;

    // Preset ini format
    string bigCube =    "type = \"Cube\"\n"
                        "rotateX = 0\n"
                        "rotateY = 0\n"
                        "rotateZ = 0\n"
                        "scale = 3\n"
                        "color = (0.7, 0.7, 0.7)\n";

    string smallCube =  "type = \"Cube\"\n"
                        "rotateX = 0\n"
                        "rotateY = 0\n"
                        "rotateZ = 0\n"
                        "scale = 1\n"
                        "color = (0.5, 0.5, 0.5)\n";

    // Off set to calculate x values
    int offset = gates;

    // Maximum and minimum values of the gates
    int xMax = 0;
    int xMin = 0;

    // Loop over gates * 2, because we generate two big cubes for every gate
    for (int i = 0; i < gates * 2; i++) {
        int x = i * 6 - offset * 6;

        // Add a big cube
        stream << "[Figure" << fFigures.size() << "]" << endl;
        stream << bigCube;
        stream << "center = (" << x << ", 0, 2)" << endl;
        fFigures.push_back(stream.str());
        stream.str(string());

        // Keep track of extrema
        if (xMin > x) {
            xMin = x;
        }
        if (xMax < x) {
            xMax = x;
        }

        // Every even time, we add a small rectangle next to the building
        if (i % 2 == 0) {
            for (int j = 0; j < 4; j++) {
                int y = j < 2? -4: -6;
                int z = j % 2 == 0? 0:  2;

                // Add a small cube
                stream << "[Figure" << fFigures.size() << "]" << endl;
                stream << smallCube;
                stream << "center = (" << x << ", " << y << ", " << z << ")" << endl;
                fFigures.push_back(stream.str());
                stream.str(string());
            }
        }
    }

    // Keep track of extrema
    fMaximumX = xMax;
    if (fMaximumX < -xMin) {
        fMaximumX = - xMin;
    }

    // Add the ground under building/gates
    stream << "[Figure" << fFigures.size() << "]" << endl;
    stream << "type = \"Face\"\n"
              "nrPoints = 4\n"
              "scale = 1\n"
              "rotateX = 0\n"
              "rotateY = 0\n"
              "rotateZ = 0\n"
              "center = (0, 0, 0)\n"
              "color = (0.3, 0.3, 0.3)\n";
    stream << "point0 = (" << xMax + 7 << ", 4, -0.5)" << endl;
    stream << "point1 = (" << xMax + 7 << ", -25, -0.5)" << endl;
    stream << "point2 = (" << xMin - 7 << ", -25, -0.5)" << endl;
    stream << "point3 = (" << xMin - 7 << ", 4, -0.5)" << endl;
    fFigures.push_back(stream.str());
}


std::string GraphicsGenerator::generateINI(double x, double y, double z) const {
    ostringstream ini;

    // Add general section
    ini << "[General]\n"
           "size = 2500\n"
           "backgroundcolor = (0.52, 0.8, 0.98)\n"
           "type = \"ZBuffering\"";

    ini << "nrFigures = " << fFigures.size() + 2 << endl;
    ini << "eye = (" << x << ", " << y << ", " << z << ")\n" << endl;

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


    ini << "point0 = (" << fMaximumX + 10 << ", " << - (fMaximumY + 15) << ", -1)" << endl;
    ini << "point1 = (" << fMaximumX  + 10 << ", " << 10 << ", -1)" << endl;
    ini << "point2 = (" << - (fMaximumX + 10) << ", " << 10 << ", -1)" << endl;
    ini << "point3 = (" << - (fMaximumX + 10) << ", " << - (fMaximumY + 15) << ", -1)" << endl << endl;


    // Make a road for taxiing
    ini << "[Figure" << fFigures.size() + 1 << "]" << endl;
    ini << "type = \"Face\"\n"
              "nrPoints = 4\n"
              "scale = 1\n"
              "rotateX = 0\n"
              "rotateY = 0\n"
              "rotateZ = 0\n"
              "center = (0, 0, 0)\n"
              "color = (0.3, 0.3, 0.3)\n";

    ini << "point0 = (-7" << ", " << - 5 << ", -0.5)" << endl;
    ini << "point1 = (7" << ", " << 0 << ", -0.5)" << endl;
    ini << "point2 = (7" << ", " << - fMaximumY  << ", -0.5)" << endl;
    ini << "point3 = (-7" << ", " << - fMaximumY << ", -0.5)" << endl << endl;

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


void GraphicsGenerator::addElement(Runway *runway) {
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

    if (runwayLength / 2 > fMaximumX) {
        fMaximumX = runwayLength / 2;
    }

    // Set right points according to length
    figure << "point0 = (" << runwayLength / 2 << ", -7, 0)" << endl;
    figure << "point1 = (" << runwayLength / 2 << ", 7, 0)"  << endl;
    figure << "point2 = (" << - runwayLength / 2 << ", 7, 0)"  << endl;
    figure << "point3 = (" << - runwayLength / 2 << ", -7, 0)" << endl;


    int offset = int(fRunways.size() * 30 + 45);

    // Center at right position
    figure << "center = (0, -" << offset << ", 0)" << endl;

    if (fMaximumY < offset) {
        fMaximumY = offset;
    }

    // Set right color
    if (runway->getType() == kAsphalt) {
        figure << "color = (0.15, 0.15, 0.15)\n" << endl;
    }
    else {
        figure << "color = (0.17, 0.70, 0.21)\n" << endl;
    }

    fRunways.push_back(runway);
    fFigures.push_back(figure.str());
}


void GraphicsGenerator::addElement(Airplane *airplane) {
    ostringstream figure;

    if (airplane->getAltitude() > 0) {
        return;
    }

    ifstream iniTemplate;




    ifstream iniTemplate("../airplaneTemplate.txt");
//    ifstream iniTemplate("../airplaneArrivalTemplate.txt");
//    ifstream iniTemplate("../airplaneDepartureTemplate.txt");

    // TODO set these
//    double x=12, y=-11, z=0;
    double x=0, y=-45, z=0;

    double xC, yC, zC;

    string line;
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
