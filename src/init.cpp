//============================================================================
// Name        : init.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/System.h"
#define ENDTIME         Time(16)
#define INPUTFILENAME   "../input.xml"
#define ATCFILENAME     "../output/ATC.txt"
#define LOGFILENAME     "../output/log.txt"

#include "../headers/GraphicsGenerator.h"

int main() {

//    // Read input
//    Input input;
//    input.read(INPUTFILENAME);
//
//    // Output file for atc
//    std::ofstream atc(ATCFILENAME);
//
//    // Output file for logging
//    std::ofstream log(LOGFILENAME);
//
//    // Initialize system
//    System system(atc,  ENDTIME);
//
//    // Import data
//    system.import(input);
//
//    // Run the simulation
//    system.run(log);
//
//    // Close files
//    atc.close();
//    log.close();
//
//    // Log information to output file
//    system.info();

    Airport* airport = new Airport;
    airport->setGates(6);
    
    Runway* asphalt = new Runway;
    asphalt->setType(kAsphalt);
    asphalt->setLength(1000);
    asphalt->setTaxiPoint("a");
    asphalt->setAirport(airport);
    asphalt->setName("asphalt");

    Runway* grass = new Runway;
    grass->setType(kGrass);
    grass->setLength(700);
    grass->setTaxiPoint("b");
    asphalt->setAirport(airport);
    asphalt->setName("grass");
    
    airport->addRunway(asphalt);
    airport->addRunway(grass);

    Airplane* plane = new Airplane;
    plane->setStatus(kAscending);
    plane->setAltitude(0);
    plane->setGateID(4);
    plane->setPosition("a");
    plane->setRunway(grass);

    GraphicsGenerator generator(airport);
    generator.addElement(asphalt);
    generator.addElement(grass);

    generator.addElement(plane);

    
    std::string ini = generator.generateINI(100, 20, 50);

    std::ofstream out("../OUTPUTINI.ini");
    out << ini;
    out.close();

    system("../engine ../OUTPUTINI.ini");

    return 0;
}
