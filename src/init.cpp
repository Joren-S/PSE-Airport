#include "../headers/System.h"
#define ENDTIME Time(0, 0)
#define INPUTFILENAME "../input.xml"
#define ATCFILENAME "../output/ATC.txt"
#define LOGFILENAME "../outpu/log.txt"

int main() {
    // Read input
    Input input(INPUTFILENAME);

    // Output file for atc
    ofstream atc(ATCFILENAME);

    // Output file for logging
    ofstream log(LOGFILENAME);

    // Initialize system
    System system(atc, log, ENDTIME);

    // Import data
    system.import(input);

    // Run the simulation
    system.run();

    // Close files
    atc.close();
    log.close();

    // Log information to output file
    system.info();

    return 0;
}
