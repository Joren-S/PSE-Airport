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

        system.initializeATC(out);

        // Import info
        system.import(input);

        // Get airplane
        airplane = system.getFlightplans()[0]->getAirplane();
    }

    ofstream out;
    Airplane* airplane;
    System system;

};

TEST_F(domainTestSystem, approach) {
    // Plane approaches and hasn't send a request to atc
    airplane->setStatus(kApproaching);
    airplane->setRequest(kIdle);
    system.approach(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kApproaching);

    // Plane approaches and atc hasn't responded yet
    system.approach(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kApproaching);

    // Plane approaches and atc has accepted request
    airplane->setRequest(kAccepted);
    system.approach(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getAltitude(), 10);
    EXPECT_EQ(airplane->getStatus(), kDescending);
}