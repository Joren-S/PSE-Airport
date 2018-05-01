//============================================================================
// Name        : init.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/System.h"
#define ENDTIME         Time(18)
#define INPUTFILENAME   "../input.xml"
#define ATCFILENAME     "../output/ATC.txt"
#define LOGFILENAME     "../output/log.txt"

int main() {
    // Read input
    Input input(INPUTFILENAME);

    // Output file for atc
    ofstream atc(ATCFILENAME);

    // Output file for logging
    ofstream log(LOGFILENAME);

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

    return 0;
}
