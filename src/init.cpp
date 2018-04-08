#include "../headers/System.h"

int main() {
    // Read input
    Input input("../input2.xml");

    // Initialize system with input
    System system(input);

    // Run the simulation
    system.run();

    // Log information to output file
    system.log();

    return 0;
}