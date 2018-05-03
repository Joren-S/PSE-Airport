//============================================================================
// Name        : domainTestsatc->cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"

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

    ofstream out;
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
    airplane->setAltitude(5);
    atc->processDescend(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);

    // Has to circle because of circling at 3.000ft
    airplane->setAltitude(5);
    atc->set3occupied(true);
    atc->processDescend(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kDenied);
    EXPECT_TRUE(atc->get5occupied());

    // Can descend, but 5000ft is occupied
    airplane->setAltitude(5);
    atc->set3occupied(false);
    atc->set5occupied(true);
    atc->processDescend(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
    EXPECT_TRUE(atc->get5occupied());
    EXPECT_FALSE(atc->get3occupied());

    // Can descend, but only because circling is going at 5000ft, 3000ft is also occupied
    atc->set5occupied(true);
    atc->set3occupied(true);
    airplane->setAltitude(5);
    atc->processDescend(airplane, Time());
    EXPECT_EQ(airplane->getRequest(), kAccepted);
    EXPECT_TRUE(atc->get5occupied());
    EXPECT_TRUE(atc->get3occupied());

    // Can land, all clear
    airplane->setAltitude(3);
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