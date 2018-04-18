#include "../headers/System.h"
#define ENDTIME Time(0, 0)
#define INPUTFILENAME "../input.xml"
#define ATCFILENAME "../ATC.txt"

int main() {
    // Read input
    Input input(INPUTFILENAME);

    // Output file for atc
    ofstream atc(ATCFILENAME);

    // Initialize system
    System system(atc, ENDTIME);

    // Import data
    system.import(input);

    // Run the simulation
    system.run();

    // Log information to output file
    system.log();

    // Close file
    atc.close();

    return 0;
}