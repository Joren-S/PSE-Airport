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
    }

    System system;

};

TEST_F(domainTestSystem, approach) {
    Airplane* airplane = new Airplane;
    airplane->setStatus(kApproaching);

    ofstream out("../test/testOutput/log.txt");

    system.approach(airplane, out);

}