#include "../headers/System.h"

int main() {
    // Read input
    Input input("../input.xml");

    // Initialize system
    System system;

    // Import data
    system.import(input);

    // Run the simulation
    system.run();

    // Log information to output file
    system.log();

    return 0;
}