//============================================================================
// Name        : domainTestsAirport.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"
#include "../headers/TestUtils.h"


class domainTestAirport: public ::testing::Test {
protected:

    Airport airport;
    Runway runway;
    Airplane plane;
};

TEST_F(domainTestAirport, DefaultConstructor) {
    EXPECT_TRUE(airport.properlyInitialized());
    EXPECT_EQ(airport.getGates(), -1);
}

TEST_F(domainTestAirport, happyDay) {
    // init airport
    EXPECT_FALSE(airport.complete());
    airport.setName("Antwerp Airport");
    airport.setCallsign("Antwerp Tower");
    airport.setIata("ANR");
    airport.setGates(10);
    EXPECT_TRUE(airport.complete());

    // plane
    plane.setPosition("Alpha");
    plane.setEngine(kJet);
    plane.setSize(kLarge);

    // 1st runway
    Runway* rw2 = new Runway();
    rw2->setName("RW1");
    rw2->setTaxiPoint("Alpha");
    rw2->setType(kGrass);
    rw2->setLength(20000);

    // 2nd runway
    Runway *rw = new Runway();
    rw->setName("RW2");
    rw->setTaxiPoint("Bravo");
    rw->setType(kGrass);
    rw->setLength(20000);

    // test for occupying/restoring gates.
    airport.initStack();
    for (int i = 1; i <= 10; i++) {
        EXPECT_EQ(airport.getFreeGate(), i);
    }
    for (int i = 10; i > 0; i--) {
        airport.restoreGate(i);
    }
    EXPECT_EQ(airport.getFreeGate(), 1);
    EXPECT_EQ(airport.getFreeGate(), 2);
    airport.restoreGate(1);
    EXPECT_EQ(airport.getFreeGate(), 1);


    EXPECT_TRUE(airport.amountOfRunways() == 0);

    // 1 runway in airport
    airport.addRunway(rw2);
    EXPECT_TRUE(airport.amountOfRunways() == 1);

    // test for finding runways by taxipoint
    EXPECT_EQ(airport.getRunway("Alpha"), rw2);
    EXPECT_TRUE(airport.getRunway("Gibberish") == NULL);

    // plane is not taxiing
    EXPECT_TRUE(airport.getNextRunway(&plane) == NULL);

    // plane is taxiing, but no next runway found
    plane.setStatus(kTaxiDeparture);
    EXPECT_TRUE(airport.getNextRunway(&plane) == NULL);

    // 2 runways in system
    airport.addRunway(rw);
    EXPECT_TRUE(airport.amountOfRunways() == 2);

    // plane is taxiing and there is a next runway.
    EXPECT_EQ(airport.getNextRunway(&plane), rw);

    // get free runway, but no valid runway for large jets
    EXPECT_TRUE(airport.getFreeRunway(&plane) == NULL);

    // alter rw so it's a valid runway.
    rw->setType(kAsphalt);
    EXPECT_EQ(airport.getFreeRunway(&plane), rw);
}

TEST_F(domainTestAirport, complete) {
    EXPECT_FALSE(airport.complete());
    airport.setName("Antwerp Airport");

    EXPECT_FALSE(airport.complete());
    airport.setCallsign("Antwerp Tower");

    EXPECT_FALSE(airport.complete());
    airport.setIata("ANR");

    EXPECT_FALSE(airport.complete());
    airport.setGates(10);

    EXPECT_TRUE(airport.complete());
}

TEST_F(domainTestAirport, fieldManipulation) {
    airport.setName("Antwerp Airport");
    EXPECT_EQ(airport.getName(), "Antwerp Airport");

    airport.setCallsign("Antwerp Tower");
    EXPECT_EQ(airport.getCallsign(), "Antwerp Tower");

    airport.setIata("ANR");
    EXPECT_EQ(airport.getIata(), "ANR");

    airport.setGates(10);
    EXPECT_EQ(airport.getGates(), 10);

    Runway* runway1 = new Runway();

    runway1->setName("RW1");
    runway1->setTaxiPoint("Alpha");
    airport.addRunway(runway1);
    std::vector<Runway*> rws = airport.getRunways();
    EXPECT_TRUE(rws.size() == 1);
    EXPECT_EQ(rws.at(0), runway1);
}


TEST_F(domainTestAirport, ContractViolations) {
    // getFreeGate
    airport.setGates(10);
    EXPECT_DEATH(airport.getFreeGate(), "Can't get free gate: no gate in stack.");
    airport.initStack();

    // initStack
    EXPECT_DEATH(airport.initStack(), "Can't initialize gate stack, already in use.");

    // getFreeGate continued
    airport.setGates(0);
    EXPECT_DEATH(airport.getFreeGate(), "Airport has no gates.");
    airport.setGates(1);
    airport.getFreeGate();
    EXPECT_DEATH(airport.getFreeGate(), "Gate has an invalid ID.");


    // restoreGate
    airport.setGates(0);
    EXPECT_DEATH(airport.restoreGate(11), "Airport has no gates.");
    airport.setGates(10);
    EXPECT_DEATH(airport.restoreGate(11), "Gate ID is invalid.");
    EXPECT_DEATH(airport.restoreGate(0), "Gate ID is invalid.");
    EXPECT_DEATH(airport.restoreGate(-1), "Gate ID is invalid.");

    // addRunway
    runway.setName("RW1");
    runway.setTaxiPoint("Alpha");
    EXPECT_DEATH(airport.addRunway(NULL), "Runway cannot be NULL.");
    Runway* runway1 = new Runway();
    airport.addRunway(runway1);
    EXPECT_DEATH(airport.addRunway(runway1), "Runway is already in system.");

    // getNextRunway
    EXPECT_DEATH(airport.getNextRunway(NULL), "Airplane cannot be NULL.");

    // setGates
    EXPECT_DEATH(airport.setGates(-1), "Number of gates cannot be negative!");

    // getFreeRunway
    EXPECT_DEATH(airport.getFreeRunway(NULL), "Plane object does not exist.");
}



TEST_F(domainTestAirport, impressions) {
    Runway* rw2 = new Runway();
    rw2->setName("RW1");
    rw2->setTaxiPoint("Alpha");
    rw2->setType(kGrass);
    rw2->setLength(20000);

    // 2nd runway
    Runway *rw = new Runway();
    rw->setName("RW2");
    rw->setTaxiPoint("Bravo");
    rw->setType(kGrass);
    rw->setLength(20000);

    airport.addRunway(rw);
    airport.addRunway(rw2);

    std::vector<Flightplan*> plans;

    Airplane* airplane = new Airplane;
    airplane->setStatus(kDeboarding);
    airplane->setGateID(1);
    Flightplan* plan = new Flightplan;
    plan->setAirplane(airplane);
    plans.push_back(plan);

    std::string tempname = "../test/testOutput/temp.txt";

    std::ofstream out(tempname.c_str());
    out << airport.drawImpression(Time(), plans);
    out.close();

    EXPECT_TRUE(FileCompare("../test/testOutput/expectedImpression1.txt", tempname));

    airplane->setStatus(kTaxiArrival);
    airplane->setPosition("Alpha");

    out.open(tempname.c_str());
    out << airport.drawImpression(Time(), plans);
    out.close();

    EXPECT_TRUE(FileCompare("../test/testOutput/expectedImpression2.txt", "../test/testOutput/temp.txt"));

    airplane->setStatus(kCrossingDeparture);

    out.open(tempname.c_str());
    out << airport.drawImpression(Time(), plans);
    out.close();

    EXPECT_TRUE(FileCompare("../test/testOutput/expectedImpression3.txt", "../test/testOutput/temp.txt"));
}
