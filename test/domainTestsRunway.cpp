//============================================================================
// Name        : domainTestsRunway.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"


class domainTestRunway: public ::testing::Test {
protected:

    Runway runway;
    Airplane plane;
};

TEST_F(domainTestRunway, DefaultConstructor) {
    EXPECT_TRUE(runway.properlyInitialized());
}

TEST_F(domainTestRunway, happyDay) {
    EXPECT_TRUE(runway.properlyInitialized());
    runway.setAirport(new Airport());
    runway.setName("RW1");
    runway.setTaxiPoint("Alpha");
    runway.setLength(1000);
    runway.setType(kAsphalt);
    EXPECT_TRUE(runway.complete());

    // small, prop
    plane.setSize(kSmall);
    plane.setEngine(kPropeller);
    EXPECT_TRUE(runway.validForAirplane(&plane));
    // small, jet
    plane.setEngine(kJet);
    EXPECT_TRUE(runway.validForAirplane(&plane));
    // medium, jet
    plane.setSize(kMedium);
    EXPECT_FALSE(runway.validForAirplane(&plane));
    // medium, prop
    plane.setEngine(kPropeller);
    EXPECT_TRUE(runway.validForAirplane(&plane));
    // large, prop
    plane.setSize(kLarge);
    EXPECT_FALSE(runway.validForAirplane(&plane));
    // large, jet
    plane.setEngine(kJet);
    EXPECT_FALSE(runway.validForAirplane(&plane));

    // long enough for large jet, with correct type
    runway.setLength(3000);
    EXPECT_TRUE(runway.validForAirplane(&plane));

    // not long enough for large jet, with correct type
    runway.setLength(1000);
    EXPECT_FALSE(runway.validForAirplane(&plane));

    // long enough for large jet, with incorrect type
    runway.setLength(3000);
    runway.setType(kGrass);
    EXPECT_FALSE(runway.validForAirplane(&plane));
}

TEST_F(domainTestRunway, complete) {
    EXPECT_FALSE(runway.complete());
    runway.setType(kGrass);

    EXPECT_FALSE(runway.complete());
    runway.setAirport(new Airport());

    EXPECT_FALSE(runway.complete());
    runway.setLength(1000);

    EXPECT_FALSE(runway.complete());
    runway.setName("RW1");

    EXPECT_FALSE(runway.complete());
    runway.setTaxiPoint("Alpha");

    EXPECT_TRUE(runway.complete());

    delete runway.getAirport();
}

TEST_F(domainTestRunway, fieldManipulation) {
    Airport *ap = new Airport();

    runway.setType(kGrass);
    EXPECT_EQ(runway.getType(), kGrass);

    runway.setLength(1000);
    EXPECT_EQ(runway.getLength(), 1000);

    runway.setName("RW1");
    EXPECT_EQ(runway.getName(), "RW1");

    runway.setFree(true);
    EXPECT_TRUE(runway.isFree());

    runway.setFree(false);
    EXPECT_FALSE(runway.isFree());

    runway.setTaxiPoint("Alpha");
    EXPECT_EQ(runway.getTaxiPoint(), "Alpha");

    runway.setAirport(ap);
    EXPECT_EQ(runway.getAirport(), ap);

    delete ap;
}

TEST_F(domainTestRunway, ContractViolations) {
    EXPECT_DEATH(runway.validForAirplane(NULL), "Plane object does not exist.");
}