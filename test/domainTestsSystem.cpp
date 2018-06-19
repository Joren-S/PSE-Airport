//============================================================================
// Name        : domainTestsSystem.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"

using namespace std;

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

    std::ofstream out;
    Airplane* airplane;
    System system;

};

TEST_F(domainTestSystem, simulationFinished) {
    Time end(13, 34);
    system.setEndTime(end);
    EXPECT_FALSE(system.simulationFinished());

    Time end2 (11);
    system.setEndTime(end2);
    EXPECT_TRUE(system.simulationFinished());
}

TEST_F(domainTestSystem, import) {
    // setup
    Input input;
    input.read("../test/testInput/happyDay.xml");
    system.initializeATC(out);

    system.import(input);
    EXPECT_EQ(system.getFlightplans(), input.getFlightplans());
    EXPECT_EQ(system.getAirport(), input.getAirports()[0]);
    EXPECT_EQ(system.getATC()->getAirport(), input.getAirports()[0]);
}

TEST_F(domainTestSystem, fieldManipulation) {
    Time time = Time(12, 0);
    system.setEndTime(time);
    EXPECT_EQ(system.getATC()->getTime().getHour(), 12);
    EXPECT_EQ(system.getATC()->getTime().getMinute(), 0);
}