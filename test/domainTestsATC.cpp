//============================================================================
// Name        : domainTestsatc->cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"

using namespace std;

class domainTestATC: public ::testing::Test {
protected:
    
    virtual void SetUp() {
        // Read input
        Input input;
        input.read("../test/testInput/happyDay.xml");
        
        // Initialize atc
        out.open("../test/testOutput/atc.txt");
        system.initializeATC(out);

        // Import info
        system.import(input);

        // Get airplane
        airplane = system.getFlightplans()[0]->getAirplane();

        // Get runway
        runway = system.getAirport()->getRunways()[0];

        // Get ATC
        atc = system.getATC();
    }

    std::ofstream out;
    System system;
    Runway* runway;
    Airplane* airplane;
    ATC* atc;

};



TEST_F(domainTestATC, DefaultConstructor) {
    EXPECT_TRUE(atc->properlyInitialized());
    EXPECT_FALSE(atc->get3occupied());
    EXPECT_FALSE(atc->get5occupied());
}

TEST_F(domainTestATC, processApproach) {
    // Very simple function so very simple test
    atc->processApproach(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
}

TEST_F(domainTestATC, processDescend) {
    // All clear, can descend
    airplane->setAltitude(5000);
    atc->processDescend(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);

    // Has to circle because of circling at 3.000ft
    airplane->setAltitude(5000);
    atc->set3occupied(true);
    atc->processDescend(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kDenied);
    EXPECT_TRUE(atc->get5occupied());

    // Can descend, but 5000ft is occupied
    airplane->setAltitude(5000);
    atc->set3occupied(false);
    atc->set5occupied(true);
    atc->processDescend(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
    EXPECT_TRUE(atc->get5occupied());
    EXPECT_FALSE(atc->get3occupied());

    // Can descend, but only because circling is going at 5000ft, 3000ft is also occupied
    atc->set5occupied(true);
    atc->set3occupied(true);
    airplane->setAltitude(5000);
    atc->processDescend(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
    EXPECT_TRUE(atc->get5occupied());
    EXPECT_TRUE(atc->get3occupied());

    // Can land, all clear
    airplane->setAltitude(3000);
    atc->processDescend(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
    EXPECT_EQ(airplane->getRunway(), runway);
    EXPECT_FALSE(runway->isFree());

    // Can't land, runway is not free
    atc->set3occupied(false);
    atc->set5occupied(false);
    atc->processDescend(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kDenied);

    // Can't land, runway is not free and a plane is already circling
    atc->set3occupied(true);
    EXPECT_DEATH(atc->processDescend(airplane, Time()), "Exceeded capacity");
}

TEST_F(domainTestATC, processTaxiArrival) {
    // Just landed on first runway
    airplane->setPosition("");
    airplane->setRunway(runway);
    atc->processTaxiArrival(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
    EXPECT_TRUE(airplane->getGateID() != -1);
    EXPECT_TRUE(airplane->getPosition().empty());

    // Landed on another runway
    airplane->setStatus(kTaxiArrival);
    airplane->setRunway(system.getAirport()->getRunways()[1]);
    airplane->setGateID(-1);
    atc->processTaxiArrival(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
    EXPECT_EQ(airplane->getGateID(), -1);

    // Airplane can cross runway, it's free
    airplane->setRunway(system.getAirport()->getRunways()[1]);
    airplane->setPosition(airplane->getRunway()->getTaxiPoint());
    runway->setFree(true);
    atc->processTaxiArrival(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);


    // Airplane can't cross runway, it's in use
    runway->setFree(false);
    atc->processTaxiArrival(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kDenied);
}

TEST_F(domainTestATC, squawk) {
    int squawk;
    airplane->setType(kPrivate);
    airplane->setSize(kSmall);
    squawk = atc->getSquawk(airplane);
    EXPECT_TRUE(squawk < 778 and squawk > 0);

    airplane->setSize(kMedium);
    airplane->setEngine(kJet);
    squawk = atc->getSquawk(airplane);
    EXPECT_TRUE(squawk < 1778 and squawk > 999);

    airplane->setType(kAirline);
    airplane->setEngine(kPropeller);
    squawk = atc->getSquawk(airplane);
    EXPECT_TRUE(squawk < 2778 and squawk > 1999);

    airplane->setEngine(kJet);
    squawk = atc->getSquawk(airplane);
    EXPECT_TRUE(squawk < 3778 and squawk > 2999);

    airplane->setSize(kLarge);
    squawk = atc->getSquawk(airplane);
    EXPECT_TRUE(squawk < 4778 and squawk > 3999);

    airplane->setType(kMilitary);
    airplane->setSize(kSmall);
    squawk = atc->getSquawk(airplane);
    EXPECT_TRUE(squawk < 5778 and squawk > 4999);

    airplane->setSize(kLarge);
    airplane->setEngine(kPropeller);
    squawk = atc->getSquawk(airplane);
    EXPECT_TRUE(squawk < 5778 and squawk > 4999);

    airplane->setType(kEmergency);
    airplane->setSize(kSmall);
    airplane->setEngine(kPropeller);
    squawk = atc->getSquawk(airplane);
    EXPECT_TRUE(squawk < 6778 and squawk > 5999);
}

TEST_F(domainTestATC, processIFRClearance) {
    airplane->setStatus(kAirport);

    // large jet
    airplane->setType(kAirline);
    airplane->setEngine(kJet);
    airplane->setSize(kLarge);
    airplane->setSquawk(0);

    // no runway available
    airplane->setRequest(kPending);
    runway->setType(kGrass);
    runway->setLength(100);
    atc->processIFRClearance(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kDenied);
    EXPECT_TRUE(airplane->getRunway() == NULL);

    // runway availble
    airplane->setRequest(kPending);
    runway->setType(kAsphalt);
    runway->setLength(10000);
    atc->processIFRClearance(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
    EXPECT_TRUE(airplane->getRunway() != NULL);
}

TEST_F(domainTestATC, processPushback) {
    // simple function.
    airplane->setStatus(kGate);
    airplane->setRequest(kPending);
    atc->processPushback(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
}

TEST_F(domainTestATC, processTaxiInitialise) {
    // simple function.
    airplane->setStatus(kPushback);
    airplane->setRequest(kPending);
    atc->processTaxiInitialise(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
}

TEST_F(domainTestATC, processTaxiInstruction) {
    airplane->setStatus(kTaxiDeparture);
    airplane->setRequest(kPending);
    airplane->setPosition("Alpha");
    airplane->setType(kAirline);

    // create a valid runway (all planes can land on asphalt with l=10k)
    runway->setType(kAsphalt);
    runway->setLength(10000);
    runway->setTaxiPoint("Alpha");

    // create a second runway
    Runway *rw = new Runway;
    rw->setType(kAsphalt);
    rw->setLength(1000);
    airplane->setRunway(rw);

    // at destination
    rw->setTaxiPoint("Alpha");
    atc->processTaxiInstruction(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kConfirmed);

    // not at destination, can cross
    rw->setTaxiPoint("Bravo");
    runway->setFree(true);
    atc->processTaxiInstruction(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);

    // not at destination, can't cross
    rw->setTaxiPoint("Bravo");
    runway->setFree(false);
    atc->processTaxiInstruction(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kDenied);

    delete rw;
}

TEST_F(domainTestATC, processTakeOff) {
    airplane->setStatus(kWaitingForDeparture);
    airplane->setRequest(kPending);
    airplane->setType(kAirline);

    // create a valid runway (all planes can land on asphalt with l=10k)
    runway->setType(kAsphalt);
    runway->setLength(10000);
    runway->setTaxiPoint("Alpha");
    airplane->setRunway(runway);

    // runway is free and 3000ft is free: line up and take off
    runway->setFree(true);
    atc->set3occupied(false);
    atc->processTakeOff(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAcceptedImmediate);

    // runway is free and 3000ft is not free: line up and wait
    runway->setFree(true);
    atc->set3occupied(true);
    atc->processTakeOff(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);

    // runway is not free and 3000ft is not free: wait
    runway->setFree(false);
    atc->set3occupied(false);
    atc->processTakeOff(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kDenied);

    // runway is not free and 3000ft is free: wait
    runway->setFree(false);
    atc->set3occupied(true);
    atc->processTakeOff(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kDenied);
}

TEST_F(domainTestATC, processTakeOffRunway) {
    airplane->setStatus(kDeparture);
    airplane->setRequest(kPending);
    airplane->setType(kAirline);

    runway->setType(kAsphalt);
    runway->setLength(10000);
    runway->setTaxiPoint("Alpha");
    airplane->setRunway(runway);
    airplane->setPosition("Alpha");

    // 3000 ft is free: take off
    atc->set3occupied(false);
    atc->processTakeOffRunway(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);

    // 3000 ft is not free: wait
    atc->set3occupied(true);
    atc->processTakeOffRunway(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kDenied);
}

TEST_F(domainTestATC, processEmergency) {

    // setup airplane in emergency that contacted atc
    airplane->setStatus(kEmergencyLanding);
    airplane->setRequest(kIdle);

    // no free runway
    runway->setFree(false);
    atc->processEmergency(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kDenied);

    // free runway
    runway->setFree(true);
    atc->processEmergency(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
    EXPECT_EQ(airplane->getRunway(), runway);
    EXPECT_EQ(airplane->getPosition(), "");
    EXPECT_EQ(runway->isFree(), false);
}

TEST_F(domainTestATC, processUrgentEmergency) {

    // setup airplane in urgent emergency that contacted atc
    airplane->setStatus(kEmergencyLandingUrgent);
    airplane->setRequest(kIdle);

    atc->processUrgentEmergency(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
}

TEST_F(domainTestATC, fieldManipulation) {
    atc->set3occupied(true);
    EXPECT_EQ(atc->get3occupied(), true);

    atc->set5occupied(true);
    EXPECT_EQ(atc->get5occupied(), true);

    Time time = Time(12, 0);
    atc->setLastActive(time);
    EXPECT_EQ(atc->getLastActive().getHour(), 12);
    EXPECT_EQ(atc->getLastActive().getMinute(), 0);

    atc->setAirport(NULL);
    EXPECT_TRUE(atc->getAirport() == NULL);
}