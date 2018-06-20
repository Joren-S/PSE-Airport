//============================================================================
// Name        : Graphics.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================


#include "../headers/Graphics.h"

using namespace std;

Graphics::Graphics(Airport* airport): fAirport(airport), fMaximumY(0) {
    fInitCheck = this;

    // Stream used to save all the figures
    ostringstream stream;

    // Preset ini format
    string bigCube =    "type = \"Cube\"\n"
                        "rotateX = 0\n"
                        "rotateY = 0\n"
                        "rotateZ = 0\n"
                        "scale = 3\n"
                        "color = (0.6, 0.6, 0.6)\n";

    string smallCube =  "type = \"Cube\"\n"
                        "rotateX = 0\n"
                        "rotateY = 0\n"
                        "rotateZ = 0\n"
                        "scale = 1\n"
                        "color = (0.8, 0.3, 0)\n";

    // Maximum and minimum values of the gates
    int xMax = 0;
    int xMin = 0;

    // Loop over gates * 2, because we generate two big cubes for every gate
    for (int i = 0; i < fAirport->getGates() * 2; i++) {
        int x = i * 6 - fAirport->getGates() * 6;

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

    ENSURE(properlyInitialized(), "Graphics object wasn't properly constructed");
}


std::string Graphics::generateINI(double x, double y, double z, int size) const {
    REQUIRE(properlyInitialized(), "Graphics wasn't properly initialized when calling generateINI");
    REQUIRE(size > 0, "Size can't be negative");
    ostringstream ini;

    // Add figures
    vector<string>::const_iterator itr;
    for (itr = fFigures.begin(); itr != fFigures.end(); ++itr) {
        ini << *itr;
    }

    int nrFigures = int(fFigures.size());

    // Make a rectangle that contains the whole airport
    ini << "[Figure" << nrFigures << "]" << endl;
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
    nrFigures++;

    // Make a road for taxiing
    ini << "[Figure" << nrFigures << "]" << endl;
    ini << "type = \"Face\"\n"
              "nrPoints = 4\n"
              "scale = 1\n"
              "rotateX = 0\n"
              "rotateY = 0\n"
              "rotateZ = 0\n"
              "center = (0, 0, 0)\n"
              "color = (0.3, 0.3, 0.3)\n";
    ini << "point0 = (-12" << ", " << - 5 << ", -0.5)" << endl;
    ini << "point1 = (12" << ", " << 0 << ", -0.5)" << endl;
    ini << "point2 = (12" << ", " << - fMaximumY  << ", -0.5)" << endl;
    ini << "point3 = (-12" << ", " << - fMaximumY << ", -0.5)" << endl << endl;
    nrFigures++;

    // Add counter for planes at a certain taxipoint
    for (unsigned int i=0; i<fRunways.size(); i++) {
        for (int j=0; j<fRunways[i].arrivingPlanes; j++) {
            ini << "[Figure" << nrFigures << "]" << endl;
            ini << "type = \"Face\"\n"
                   "nrPoints = 4\n"
                   "scale = 1\n"
                   "rotateX = 0\n"
                   "rotateY = 0\n"
                   "rotateZ = 0\n"
                   "center = (0, 0, 0)\n"
                   "color = (0.8, 0.8, 0.8)\n";
            ini << "point0 = (14" << ", " << - i * 30 - 35 + j * 2 << ", -0.5)" << endl;
            ini << "point1 = (13" << ", " << - i * 30 - 35 + j * 2 << ", -0.5)" << endl;
            ini << "point2 = (13" << ", " << - i * 30 - 35 + j * 2 + 1  << ", -0.5)" << endl;
            ini << "point3 = (14" << ", " << - i * 30 - 35 + j * 2 + 1 << ", -0.5)" << endl << endl;
            nrFigures++;
        }
        for (int j=0; j<fRunways[i].departingPlanes; j++) {
            ini << "[Figure" << nrFigures << "]" << endl;
            ini << "type = \"Face\"\n"
                   "nrPoints = 4\n"
                   "scale = 1\n"
                   "rotateX = 0\n"
                   "rotateY = 0\n"
                   "rotateZ = 0\n"
                   "center = (0, 0, 0)\n"
                   "color = (0.8, 0.8, 0.8)\n";
            ini << "point0 = (-14" << ", " << - i * 30 - 35 + j * 2 << ", -0.5)" << endl;
            ini << "point1 = (-13" << ", " << - i * 30 - 35 + j * 2 << ", -0.5)" << endl;
            ini << "point2 = (-13" << ", " << - i * 30 - 35 + j * 2 + 1  << ", -0.5)" << endl;
            ini << "point3 = (-14" << ", " << - i * 30 - 35 + j * 2 + 1 << ", -0.5)" << endl << endl;
            nrFigures++;
        }
    }

    // Add general section
    ini << "[General]\n"
           "backgroundcolor = (0.52, 0.8, 0.98)\n"
           "type = \"ZBuffering\"";

    ini << "nrFigures = " << nrFigures << endl;
    ini << "size = " << size << endl;
    ini << "eye = (" << x << ", " << y << ", " << z << ")\n" << endl;

    return ini.str();
}


void Graphics::parseCoordinates(const std::string &coordinates, double &x, double &y, double &z) const {
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


void Graphics::addElement(Runway *runway) {
    REQUIRE(properlyInitialized(), "Graphics wasn't properly initialized when calling addElement(runway)");
    REQUIRE(runway != NULL, "Element can't be NULL when calling addElement");

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
//        figure << "color = (0.27, 0.34, 0.15)\n" << endl;
        figure << "color = (0.3, 0.4, 0.2)\n" << endl;
    }

    RunwayInfo info;
    info.runway = runway;
    info.arrivingPlanes = info.departingPlanes = 0;

    fRunways.push_back(info);
    fFigures.push_back(figure.str());
}


void Graphics::addElement(Airplane *airplane) {
    REQUIRE(properlyInitialized(), "Graphics wasn't properly initialized when calling addElement(airplane)");
    REQUIRE(airplane != NULL, "Element can't be NULL when calling addElement");
    ostringstream figure;

    const char* airplaneTemplate = "../graphics/airplaneTemplate.txt";
    const char* airplaneArrivalTemplate = "../graphics/airplaneArrivalTemplate.txt";
    const char* airplaneDepartureTemplate = "../graphics/airplaneDepartureTemplate.txt";

    if (airplane->getAltitude() > 0) {
        return;
    }

    ifstream iniTemplate;
    double x = 0, y = 0, z = 0;
    EPlaneStatus status = airplane->getStatus();

    // At gate
    if (status == kDeboarding or status == kTechnicalCheck or status == kParked or
        status == kGate or status == kAirport or status == kPushback)
    {
        iniTemplate.open(airplaneArrivalTemplate);
        x = ((airplane->getGateID() - 1) * 12 - 6 * fAirport->getGates()) + 5;
        y = -11;
    }
    
    else {
        for (unsigned int i=0; i<fRunways.size(); i++) {
            Runway* runway = fRunways[i].runway;
            string taxipoint = runway->getTaxiPoint();

            // Plane is landing or taking off
            if ((airplane->getPosition().empty() and airplane->getAltitude() == 0 and airplane->getStatus() == kDescending and airplane->getRunway() == runway) or
                    (airplane->getStatus() == kAscending and airplane->getRunway()->getTaxiPoint() == taxipoint and airplane->getAltitude() == 0))
            {
                iniTemplate.open(airplaneTemplate);
                x = -6;
                y = -45 - double(i * 30);
            }

            // Plane is crossing when taxiing at arrival
            else if (airplane->getStatus() == kCrossingArrival and fAirport->getNextRunway(airplane) == runway) {
                iniTemplate.open(airplaneArrivalTemplate);
                x = 6;
                y = -47 - double(i * 30);
            }

            // Plane just landed and at taxipoint
            else if ((airplane->getPosition().empty() and airplane->getStatus() == kTaxiArrival and airplane->getRunway() == runway) or
                    (airplane->getStatus() == kTaxiArrival and airplane->getPosition() == taxipoint))
            {
                if (fRunways[i].arrivingPlanes == 0) {
                    iniTemplate.open(airplaneArrivalTemplate);
                    x = 6;
                    y = -33 - double(i * 30);
                }
                fRunways[i].arrivingPlanes++;
            }


            // Departure and at taxipoint
            else if ((airplane->getStatus() == kTaxiDeparture and airplane->getPosition() == taxipoint) or
                    (airplane->getStatus() == kWaitingForDeparture and airplane->getPosition() == taxipoint))
            {
                if (fRunways[i].departingPlanes == 0) {
                    iniTemplate.open(airplaneDepartureTemplate);
                    x = - 6;
                    y = - 28 - double(i * 30);
                }
                fRunways[i].departingPlanes++;
            }

            // Plane is waiting on runway before takeoff
            else if (airplane->getStatus() == kDeparture and airplane->getRunway()->getTaxiPoint() == taxipoint) {
                iniTemplate.open(airplaneTemplate);
                x = - double(runway->getLength()) / 14 + 10;
                y = -45 - double(i * 30);
            }

            // Plane is crossing runway at departure
            else if (airplane->getStatus() == kCrossingDeparture and airplane->getPosition() == taxipoint) {
                iniTemplate.open("../airplaneDepartureTemplate.txt");
                x = -6;
                y = -42 - double(i * 30);
            }
        }
    }

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

bool Graphics::properlyInitialized() const {
    return fInitCheck == this;
}