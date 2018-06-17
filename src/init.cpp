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
    GraphicsGenerator generator(airport->getGates());
    Airplane* airplane = new Airplane;
    generator.addElement(airplane);
    Runway* runway = new Runway;
    runway->setType(kAsphalt);
    runway->setLength(1000);
//    generator.addElement(runway);
    Runway* runway1 = new Runway;
    runway1->setLength(700);
//    generator.addElement(runway1);
    std::string ini = generator.generateINI(50, 50, 50);
    std::ofstream out("../OUTPUTINI.ini");
    out << ini;
    out.close();

    system("../engine ../OUTPUTINI.ini");

    return 0;
}
