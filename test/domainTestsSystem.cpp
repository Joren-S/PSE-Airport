//============================================================================
// Name        : domainTestsSystem.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"


class domainTestSystem: public ::testing::Test {
protected:

    virtual void SetUp() {

        // Read input
        Input input;
        input.read("../test/testInput/happyDay.xml");

        // Output file for atc
        ofstream atc("../test/testOutput/atc.txt");

        // Initialize system
        system.initializeATC(atc);

        // Import info
        system.import(input);

        // Get airplane
        airplane = system.getFlightplans()[0]->getAirplane();

        // Open a file for logging
        out.open("../test/testOutput/log.txt");
    }

    virtual void TearDown() {
        // Close file
        out.close();
    }

    ofstream out;
    Airplane* airplane;
    System system;

};

TEST_F(domainTestSystem, approach) {
    airplane->setStatus(kApproaching);
    system.approach(airplane, out);

}