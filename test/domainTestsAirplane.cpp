//============================================================================
// Name        : domainTestsAirplane.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"


class domainTestAirplane: public ::testing::Test {
protected:

    Airplane airplane;

};


TEST_F(domainTestAirplane, DefaultConstructor) {
    EXPECT_TRUE(airplane.properlyInitialized());
    EXPECT_EQ(airplane.getPassengers(), -1);
    EXPECT_EQ(airplane.getFuel(), -1);
    EXPECT_EQ(airplane.getAltitude(), 0);
    EXPECT_EQ(airplane.getTimeRemaining(), 0);
    EXPECT_TRUE(airplane.getRunway() == NULL);
    EXPECT_EQ(airplane.getRequest(), kIdle);
    EXPECT_EQ(airplane.getGateID(), -1);
    EXPECT_EQ(airplane.getStatus(), kAway);
    EXPECT_EQ(airplane.getEngine(), kDefaultEngine);
    EXPECT_EQ(airplane.getType(), kDefaultType);
    EXPECT_EQ(airplane.getSize(), kDefaultSize);
}

TEST_F(domainTestAirplane, happyDay) {
    airplane.increaseAltitude(3);
    EXPECT_EQ(airplane.getAltitude(), 3);
    airplane.increaseAltitude(2);
    EXPECT_EQ(airplane.getAltitude(), 5);
    airplane.decreaseAltitude(5);
    EXPECT_EQ(airplane.getAltitude(), 0);

    airplane.decreaseTimeRemaining();
    EXPECT_EQ(airplane.getTimeRemaining(), 0);

    airplane.setTimeRemaining(1);
    airplane.decreaseTimeRemaining();
    EXPECT_EQ(airplane.getTimeRemaining(), 0);
    airplane.decreaseTimeRemaining();
    EXPECT_EQ(airplane.getTimeRemaining(), 0);
}

TEST_F(domainTestAirplane, complete) {
    EXPECT_FALSE(airplane.complete());
    airplane.setEngine(kJet);
    EXPECT_FALSE(airplane.complete());
    airplane.setType(kAirline);
    EXPECT_FALSE(airplane.complete());
    airplane.setFuel(1);
    EXPECT_FALSE(airplane.complete());
    airplane.setSize(kSmall);
    EXPECT_FALSE(airplane.complete());
    airplane.setPassengers(1);
    EXPECT_FALSE(airplane.complete());
    airplane.setModel("test");
    EXPECT_FALSE(airplane.complete());
    airplane.setCallsign("test");
    EXPECT_FALSE(airplane.complete());
    airplane.setNumber("test");

    EXPECT_TRUE(airplane.complete());
}

TEST_F(domainTestAirplane, ContractViolations) {
    EXPECT_DEATH(airplane.decreaseAltitude(-3), "Difference can't be negative");
    EXPECT_DEATH(airplane.increaseAltitude(-2), "Difference can't be negative");

    // Set altitude to 1 and try to decrease by 2
    airplane.setAltitude(1);
    EXPECT_DEATH(airplane.decreaseAltitude(2), "New altitude can't be less than 0!");

    EXPECT_DEATH(airplane.setAltitude(-3), "Altitude can't be negative");
    EXPECT_DEATH(airplane.setTimeRemaining(-5), "Time remaining can't be negative");
    EXPECT_DEATH(airplane.setPassengers(-4), "Passenger amount can't be negative");
    EXPECT_DEATH(airplane.setGateID(-3), "Gate id can't be less than -1");
    EXPECT_DEATH(airplane.setFuel(0), "Fuel can't be less than 1");
    EXPECT_DEATH(airplane.setFuel(-3), "Fuel can't be less than 1");
    EXPECT_DEATH(airplane.setSquawk(-4), "Squawk code can't be negative");
}