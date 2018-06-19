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


int main() {

    // Read input
    Input input;
    input.read(INPUTFILENAME);

    // Output file for atc
    std::ofstream atc(ATCFILENAME);

    // Output file for logging
    std::ofstream log(LOGFILENAME);

    // Initialize system
    System system(atc,  ENDTIME);

    // Import data
    system.import(input);

    // Run the simulation
    system.run(log);

    // Close files
    atc.close();
    log.close();

    // Log information to output file
    system.info();

    // Generate wanted images
    system.generateImages(Time(12, 40), Time(12, 45));

    return 0;
}
