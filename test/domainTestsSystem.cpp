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

        // Get runway
        runway = system.getAirport()->getRunways()[0];
    }

    Runway* runway;
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

TEST_F(domainTestSystem, descend) {
    // Normal descend
    airplane->setStatus(kDescending);
    airplane->setAltitude(10);
    system.descend(airplane, out);
    EXPECT_EQ(airplane->getAltitude(), 9);
    EXPECT_EQ(airplane->getStatus(), kDescending);

    // Plane at 3000ft and can land
    airplane->setRequest(kConfirmed);
    airplane->setAltitude(3);
    airplane->setRunway(runway);
    system.descend(airplane, out);
    EXPECT_EQ(airplane->getAltitude(), 2);
    EXPECT_EQ(airplane->getStatus(), kDescending);

    // Plane at 3000ft and has to circle
    airplane->setRequest(kDenied);
    airplane->setAltitude(3);
    system.descend(airplane, out);
    EXPECT_EQ(airplane->getStatus(), kCircling);
    EXPECT_EQ(airplane->getAltitude(), 3);

    // Plane has landed
    airplane->setAltitude(0);
    airplane->setStatus(kDescending);
    system.descend(airplane, out);
    EXPECT_EQ(airplane->getStatus(), kTaxiArrival);
    EXPECT_EQ(airplane->getRequest(), kIdle);
}

TEST_F(domainTestSystem, circle) {
    // Very simple function so very simple test
    system.circle(airplane, out);
    EXPECT_EQ(airplane->getStatus(), kDescending);
}

TEST_F(domainTestSystem, taxiArrival) {
    // Arrived at gate
    airplane->setPosition(runway->getTaxiPoint());
    airplane->setRequest(kConfirmed);
    system.taxiArrival(airplane, out);
    EXPECT_TRUE(airplane->getPosition().empty());
    EXPECT_EQ(airplane->getStatus(), kDeboarding);

    // Arrived at taxipoint
    airplane->setStatus(kTaxiArrival);
    airplane->setPosition(system.getAirport()->getRunways()[1]->getTaxiPoint());
    airplane->setRequest(kConfirmed);
    system.taxiArrival(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);

    // Hasn't send a request yet for taxi instructions
    airplane->setPosition(system.getAirport()->getRunways()[1]->getTaxiPoint());
    airplane->setRequest(kIdle);
    system.taxiArrival(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);

    // Request has been accepted, plane just landed
    airplane->setRequest(kAccepted);
    airplane->setPosition("");

}

TEST_F(domainTestSystem, crossArrival) {

}

TEST_F(domainTestSystem, deboard) {

}

TEST_F(domainTestSystem, technicalCheck) {

}