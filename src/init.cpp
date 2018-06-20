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
#define INFOFILENAME    "../output/info.txt"


int main() {

    // Read input
    Input input;
    input.read(INPUTFILENAME);

    // Output file for atc, logging and info
    std::ofstream atc(ATCFILENAME);
    std::ofstream log(LOGFILENAME);
    std::ofstream info(INFOFILENAME);

    // Initialize system
    System system(input, atc,  ENDTIME);

    // Run the simulation
    system.run(log);

    // Log general info to output file
    system.info(info);

    // Close files
    atc.close();
    log.close();
    info.close();

    // Generate wanted images
    system.generateImages(Time(12, 40), Time(12, 45));

    return 0;
}
