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
    airplane.increaseAltitude(3000);
    EXPECT_EQ(airplane.getAltitude(), 3000);
    airplane.increaseAltitude(2000);
    EXPECT_EQ(airplane.getAltitude(), 5000);
    airplane.decreaseAltitude(5000);
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

TEST_F(domainTestAirplane, fieldManipulation) {
    airplane.setPosition("Alpha");
    EXPECT_EQ(airplane.getPosition(), "Alpha");

    airplane.setType(kMilitary);
    EXPECT_EQ(airplane.getType(), kMilitary);

    airplane.setSquawk(12345);
    EXPECT_EQ(airplane.getSquawk(), 12345);

    airplane.setRequest(kAcceptedImmediate);
    EXPECT_EQ(airplane.getRequest(), kAcceptedImmediate);

    airplane.setSize(kSmall);
    EXPECT_EQ(airplane.getSize(), kSmall);

    airplane.setEngine(kPropeller);
    EXPECT_EQ(airplane.getEngine(), kPropeller);

    airplane.setStatus(kAway);
    EXPECT_EQ(airplane.getStatus(), kAway);

    airplane.setCallsign("PlaneCS");
    EXPECT_EQ(airplane.getCallsign(), "PlaneCS");

    airplane.setTimeRemaining(50);
    EXPECT_EQ(airplane.getTimeRemaining(), 50);

    airplane.setAltitude(100000);
    EXPECT_EQ(airplane.getAltitude(), 100000);

    airplane.setFuel(50000);
    EXPECT_EQ(airplane.getFuel(), 50000);

    airplane.setGateID(5);
    EXPECT_EQ(airplane.getGateID(), 5);

    airplane.setModel("F16");
    EXPECT_EQ(airplane.getModel(), "F16");

    airplane.setNumber("16");
    EXPECT_EQ(airplane.getNumber(), "16");

    airplane.setPassengers(2);
    EXPECT_EQ(airplane.getPassengers(), 2);

    Runway *rw = new Runway();
    airplane.setRunway(rw);
    EXPECT_TRUE(airplane.getRunway() == rw);
    delete rw;
}

TEST_F(domainTestAirplane, ContractViolations) {
    EXPECT_DEATH(airplane.decreaseAltitude(-3000), "Difference can't be negative");
    EXPECT_DEATH(airplane.increaseAltitude(-2000), "Difference can't be negative");

    // Set altitude to 1 and try to decrease by 2
    airplane.setAltitude(1000);
    EXPECT_DEATH(airplane.decreaseAltitude(2000), "New altitude can't be less than 0!");

    EXPECT_DEATH(airplane.setAltitude(-3), "Altitude can't be negative");
    EXPECT_DEATH(airplane.setTimeRemaining(-5), "Time remaining can't be negative");
    EXPECT_DEATH(airplane.setPassengers(-4), "Passenger amount can't be negative");
    EXPECT_DEATH(airplane.setGateID(-3), "Gate id can't be less than -1");
    EXPECT_DEATH(airplane.setFuel(0), "Fuel can't be less than 1");
    EXPECT_DEATH(airplane.setFuel(-3), "Fuel can't be less than 1");
    EXPECT_DEATH(airplane.setSquawk(-4), "Squawk code can't be negative");
}