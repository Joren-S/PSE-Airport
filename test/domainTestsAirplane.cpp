//============================================================================
// Name        : domainTestsAirplane.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"

#include "fstream"

class domainTestAirplane: public ::testing::Test {
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
        airplane->setATC(system.getATC());

        // Get runway
        runway = system.getAirport()->getRunways()[0];
    }

    std::ofstream out;
    Airplane* airplane;
    Airplane* airplaneInc;
    Runway* runway;
    System system;
};


TEST_F(domainTestAirplane, DefaultConstructor) {
    airplaneInc = new Airplane();
    EXPECT_TRUE(airplaneInc->properlyInitialized());
    EXPECT_EQ(airplaneInc->getPassengers(), -1);
    EXPECT_EQ(airplaneInc->getFuel(), -1);
    EXPECT_EQ(airplaneInc->getAltitude(), 0);
    EXPECT_EQ(airplaneInc->getTimeRemaining(), 0);
    EXPECT_TRUE(airplaneInc->getRunway() == NULL);
    EXPECT_EQ(airplaneInc->getRequest(), kIdle);
    EXPECT_EQ(airplaneInc->getGateID(), -1);
    EXPECT_EQ(airplaneInc->getStatus(), kAway);
    EXPECT_EQ(airplaneInc->getEngine(), kDefaultEngine);
    EXPECT_EQ(airplaneInc->getType(), kDefaultType);
    EXPECT_EQ(airplaneInc->getSize(), kDefaultSize);
}

TEST_F(domainTestAirplane, happyDay) {
    airplane->increaseAltitude(3000);
    EXPECT_EQ(airplane->getAltitude(), 3000);
    airplane->increaseAltitude(2000);
    EXPECT_EQ(airplane->getAltitude(), 5000);
    airplane->decreaseAltitude(5000);
    EXPECT_EQ(airplane->getAltitude(), 0);

    airplane->decreaseTimeRemaining();
    EXPECT_EQ(airplane->getTimeRemaining(), 0);

    airplane->setTimeRemaining(1);
    airplane->decreaseTimeRemaining();
    EXPECT_EQ(airplane->getTimeRemaining(), 0);
    airplane->decreaseTimeRemaining();
    EXPECT_EQ(airplane->getTimeRemaining(), 0);
}

TEST_F(domainTestAirplane, complete) {
    airplaneInc = new Airplane();
    EXPECT_FALSE(airplaneInc->complete());
    airplaneInc->setEngine(kJet);
    EXPECT_FALSE(airplaneInc->complete());
    airplaneInc->setType(kAirline);
    EXPECT_FALSE(airplaneInc->complete());
    airplaneInc->setFuel(1);
    EXPECT_FALSE(airplaneInc->complete());
    airplaneInc->setSize(kSmall);
    EXPECT_FALSE(airplaneInc->complete());
    airplaneInc->setPassengers(1);
    EXPECT_FALSE(airplaneInc->complete());
    airplaneInc->setModel("test");
    EXPECT_FALSE(airplaneInc->complete());
    airplaneInc->setCallsign("test");
    EXPECT_FALSE(airplaneInc->complete());
    airplaneInc->setNumber("test");

    EXPECT_TRUE(airplaneInc->complete());
}


TEST_F(domainTestAirplane, getFuelCost) {

    //small, propeller: 10
    airplane->setSize(kSmall);
    airplane->setEngine(kPropeller);
    EXPECT_EQ(airplane->getFuelCost(), 10);

    //small, jet: 25
    airplane->setSize(kSmall);
    airplane->setEngine(kJet);
    EXPECT_EQ(airplane->getFuelCost(), 25);

    //medium, propellor: 50
    airplane->setSize(kMedium);
    airplane->setEngine(kPropeller);
    EXPECT_EQ(airplane->getFuelCost(), 50);

    //medium, jet: 175
    airplane->setSize(kMedium);
    airplane->setEngine(kJet);
    EXPECT_EQ(airplane->getFuelCost(), 175);

    //large, propellor: 100
    airplane->setSize(kLarge);
    airplane->setEngine(kPropeller);
    EXPECT_EQ(airplane->getFuelCost(), 100);

    //large, jet: 250
    airplane->setSize(kLarge);
    airplane->setEngine(kJet);
    EXPECT_EQ(airplane->getFuelCost(), 250);

}

TEST_F(domainTestAirplane, checkFuel) {

    // fuelcost: 10
    airplane->setFuel(1000);
    airplane->setSize(kSmall);
    airplane->setEngine(kPropeller);
    airplane->setSquawk(1337);

    // first call: only decrease fuel
    airplane->setCurFuel(20);
    airplane->setAltitude(4500);
    airplane->checkFuel(out);
    EXPECT_EQ(airplane->getCurFuel(), 10);

    // second call: fuel runs out, emergency at altitude >= 3000
    airplane->setAltitude(3500);
    airplane->checkFuel(out);
    EXPECT_EQ(airplane->getCurFuel(), 0);
    EXPECT_EQ(airplane->getSquawk(), 7700);
    EXPECT_EQ(airplane->getStatus(), kEmergencyLanding);

    // third call: plane in emergency at 3500, accepted by ATC
    airplane->setRequest(kAccepted);
    airplane->checkFuel(out);
    EXPECT_EQ(airplane->getAltitude(), 3000);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);

    // fourth call: fuel runs out, emergency at altitutde < 3000
    airplane->setRequest(kIdle);
    airplane->setAltitude(2000);
    airplane->setSquawk(1337);
    airplane->setCurFuel(10);
    airplane->checkFuel(out);
    EXPECT_EQ(airplane->getCurFuel(), 0);
    EXPECT_EQ(airplane->getSquawk(), 7700);
    EXPECT_EQ(airplane->getStatus(), kEmergencyLandingUrgent);

    // fifth call: urgent emergency landing, descending
    airplane->setAltitude(1000);
    airplane->checkFuel(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getAltitude(), 500);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);

    // sixth call: urgent emergency landing, landed
    airplane->setAltitude(500);
    airplane->checkFuel(out);
    EXPECT_EQ(airplane->getStatus(), kAway);
    EXPECT_TRUE(airplane->getSquawk() != 7700);
}


TEST_F(domainTestAirplane, approach) {
    // Plane approaches and hasn't send a request to atc
    airplane->setStatus(kApproaching);
    airplane->setRequest(kIdle);
    airplane->approach(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kApproaching);

    // Plane approaches and atc hasn't responded yet
    airplane->approach(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kApproaching);

    // Plane approaches and atc has accepted request
    airplane->setRequest(kAccepted);
    airplane->approach(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getAltitude(), 10000);
    EXPECT_EQ(airplane->getStatus(), kDescending);
}

TEST_F(domainTestAirplane, descend) {
    // Normal descend
    airplane->setStatus(kDescending);
    airplane->setAltitude(10000);
    airplane->descend(out);
    EXPECT_EQ(airplane->getAltitude(), 9000);
    EXPECT_EQ(airplane->getStatus(), kDescending);

    // Plane at 3000ft and can land
    airplane->setRequest(kConfirmed);
    airplane->setAltitude(3000);
    airplane->setRunway(runway);
    airplane->descend(out);
    EXPECT_EQ(airplane->getAltitude(), 2000);
    EXPECT_EQ(airplane->getStatus(), kDescending);

    // Plane at 3000ft and has to circle
    airplane->setRequest(kDenied);
    airplane->setAltitude(3000);
    airplane->descend(out);
    EXPECT_EQ(airplane->getStatus(), kCircling);
    EXPECT_EQ(airplane->getAltitude(), 3000);

    // Plane has landed
    airplane->setAltitude(0);
    airplane->setStatus(kDescending);
    airplane->descend(out);
    EXPECT_EQ(airplane->getStatus(), kTaxiArrival);
    EXPECT_EQ(airplane->getRequest(), kIdle);
}

TEST_F(domainTestAirplane, circle) {
    // Very simple function so very simple test
    airplane->circle(out);
    EXPECT_EQ(airplane->getStatus(), kDescending);
}

TEST_F(domainTestAirplane, taxiArrival) {
    // Arrived at gate
    airplane->setPosition(runway->getTaxiPoint());
    airplane->setRequest(kConfirmed);
    airplane->setPassengers(7);
    airplane->taxiArrival(out);
    EXPECT_TRUE(airplane->getPosition().empty());
    EXPECT_EQ(airplane->getStatus(), kDeboarding);
    EXPECT_EQ(airplane->getTimeRemaining(), 4);

    // Arrived at taxipoint
    airplane->setStatus(kTaxiArrival);
    airplane->setPosition(system.getAirport()->getRunways()[1]->getTaxiPoint());
    airplane->setRequest(kConfirmed);
    airplane->taxiArrival(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);

    // Hasn't send a request yet for taxi instructions
    airplane->setPosition(system.getAirport()->getRunways()[1]->getTaxiPoint());
    airplane->setRequest(kIdle);
    airplane->taxiArrival(out);
    EXPECT_EQ(airplane->getRequest(), kPending);

    // Request has been accepted, plane just landed
    airplane->setRequest(kAccepted);
    airplane->setPosition("");
    airplane->setRunway(runway);
    airplane->taxiArrival(out);
    EXPECT_EQ(airplane->getStatus(), kTaxiArrival);
    EXPECT_EQ(airplane->getPosition(), airplane->getRunway()->getTaxiPoint());
    EXPECT_EQ(airplane->getRequest(), kConfirmed);
    EXPECT_EQ(airplane->getTimeRemaining(), 5);

    // Request has been accepted, plane crosses a runway
    airplane->setRequest(kAccepted);
    airplane->setPosition(system.getAirport()->getRunways()[1]->getTaxiPoint());
    airplane->taxiArrival(out);
    EXPECT_EQ(airplane->getStatus(), kCrossingArrival);
    EXPECT_EQ(airplane->getRequest(), kConfirmed);
}

TEST_F(domainTestAirplane, crossArrival) {
    airplane->setPosition(system.getAirport()->getRunways()[1]->getTaxiPoint());
    airplane->setStatus(kTaxiArrival);
    airplane->crossArrival(out);
    EXPECT_EQ(airplane->getStatus(), kTaxiArrival);
    EXPECT_TRUE(runway->isFree());
    EXPECT_EQ(airplane->getPosition(), runway->getTaxiPoint());
    EXPECT_EQ(airplane->getTimeRemaining(), 5);
}

TEST_F(domainTestAirplane, deboard) {
    airplane->setSize(kSmall);
    airplane->deboard(out);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    airplane->setSize(kMedium);
    airplane->deboard(out);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);

    airplane->setSize(kLarge);
    airplane->deboard(out);
    EXPECT_EQ(airplane->getTimeRemaining(), 3);

    EXPECT_EQ(airplane->getStatus(), kTechnicalCheck);
    EXPECT_EQ(airplane->getRequest(), kIdle);
}


TEST_F(domainTestAirplane, technicalCheck) {
    // Very simple function so very simple test
    airplane->technicalCheck(out);
    EXPECT_EQ(airplane->getStatus(), kParked);
    EXPECT_EQ(airplane->getRequest(), kIdle);
}


TEST_F(domainTestAirplane, ascend) {
    airplane->setStatus(kAscending);
    airplane->setEngine(kPropeller);

    // ascend 1000ft each time ascend is called and altitude < 5000ft
    EXPECT_EQ(airplane->getAltitude(), 0);
    airplane->ascend(out);
    EXPECT_EQ(airplane->getPosition(), "");
    EXPECT_EQ(airplane->getAltitude(), 1000);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);
    airplane->ascend(out);
    EXPECT_EQ(airplane->getAltitude(), 2000);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);
    airplane->ascend(out);
    EXPECT_EQ(airplane->getAltitude(), 3000);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);
    airplane->ascend(out);
    EXPECT_EQ(airplane->getAltitude(), 4000);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);
    airplane->ascend(out);
    EXPECT_EQ(airplane->getAltitude(), 5000);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);
    airplane->ascend(out);
    EXPECT_EQ(airplane->getStatus(), kAway);
}

TEST_F(domainTestAirplane, onRunway) {
    // Request permission to take off.
    airplane->setStatus(kDeparture);
    airplane->setRequest(kIdle);
    airplane->setRunway(runway);

    airplane->onRunway(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kDeparture);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC hasn't responded yet
    airplane->onRunway(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kDeparture);

    // ATC gave permission to take off
    airplane->setRequest(kAccepted);
    airplane->onRunway(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kAscending);

}

TEST_F(domainTestAirplane, atRunway) {
    // Request permission to go on runway
    airplane->setStatus(kWaitingForDeparture);
    airplane->setRequest(kIdle);
    airplane->setRunway(runway);

    airplane->atRunway(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kWaitingForDeparture);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC hasn't responded yet
    airplane->atRunway(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kWaitingForDeparture);

    // ATC gave permission to line up on runway
    airplane->setRequest(kAccepted);
    airplane->atRunway(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kDeparture);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC gave permission to line up and take off.
    airplane->setRequest(kAcceptedImmediate);
    airplane->atRunway(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kAscending);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);
}

TEST_F(domainTestAirplane, taxiDepartureCross) {
    airplane->setStatus(kCrossingDeparture);
    airplane->setRequest(kIdle);
    airplane->setPosition(runway->getTaxiPoint());
    airplane->taxiDepartureCross(out);
    EXPECT_EQ(airplane->getStatus(), kTaxiDeparture);
    EXPECT_EQ(airplane->getTimeRemaining(), 5);
}

TEST_F(domainTestAirplane, taxiDepartureStep) {
    airplane->setSize(kSmall);
    airplane->setEngine(kPropeller);
    airplane->setStatus(kTaxiDeparture);
    airplane->setRequest(kIdle);
    airplane->setRunway(runway);
    airplane->setPosition("");

    // first ever taxi step
    airplane->taxiDepartureStep(out);
    EXPECT_EQ(airplane->getStatus(), kTaxiDeparture);
    EXPECT_EQ(airplane->getPosition(), system.getAirport()->getRunways().at(0)->getTaxiPoint());
    EXPECT_EQ(airplane->getTimeRemaining(), 5);

    // request crossing
    airplane->setRequest(kIdle);
    airplane->taxiDepartureStep(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kTaxiDeparture);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // atc hasn't responded yet
    airplane->taxiDepartureStep(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kTaxiDeparture);

    // atc has responded and has accepted the request
    airplane->setRequest(kAccepted);
    airplane->taxiDepartureStep(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kCrossingDeparture);
    EXPECT_FALSE(system.getAirport()->getRunway(airplane->getPosition())->isFree());
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // atc has responded and has confirmed the request (arrived at destination)
    airplane->setRequest(kConfirmed);
    airplane->getRunway()->setName("RW1");
    airplane->getRunway()->setTaxiPoint("Alpha");
    airplane->setPosition("Alpha");
    airplane->taxiDepartureStep(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kWaitingForDeparture);
}

TEST_F(domainTestAirplane, taxiDepartureStart) {
    // Requesting permission to start taxiing
    airplane->setStatus(kPushback);
    airplane->setRequest(kIdle);
    airplane->setCallsign("CSAP");
    airplane->taxiDepartureStart(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kPushback);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC hasn't responded yet
    airplane->taxiDepartureStart(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kPushback);

    // ATC has responded and accepted request
    airplane->setRequest(kAccepted);
    airplane->taxiDepartureStart(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kTaxiDeparture);
    EXPECT_EQ(airplane->getPosition(), "");
}

TEST_F(domainTestAirplane, pushback) {
    // Requesting permission to pushback
    airplane->setStatus(kGate);
    airplane->setRequest(kIdle);
    airplane->pushback(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kGate);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC hasn't responded yet
    airplane->pushback(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kGate);

    // ATC has responded and accepted request
    // different plane sizes:
    airplane->setRequest(kAccepted);
    airplane->setSize(kSmall);
    airplane->pushback(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kPushback);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    airplane->setRequest(kAccepted);
    airplane->setSize(kMedium);
    airplane->pushback(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kPushback);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);

    airplane->setRequest(kAccepted);
    airplane->setSize(kLarge);
    airplane->pushback(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kPushback);
    EXPECT_EQ(airplane->getTimeRemaining(), 3);
}

TEST_F(domainTestAirplane, prepare) {
    // Requesting IFR clearancy
    airplane->setStatus(kAirport);
    airplane->setRequest(kIdle);
    runway->setName("RW1");
    airplane->setRunway(runway);
    airplane->prepare(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kAirport);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC hasn't responded yet
    airplane->prepare(out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kAirport);

    // ATC has responded and accepted request
    airplane->setRequest(kAccepted);
    airplane->prepare(out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kGate);
}


TEST_F(domainTestAirplane, fieldManipulation) {
    airplane->setPosition("Alpha");
    EXPECT_EQ(airplane->getPosition(), "Alpha");

    airplane->setType(kMilitary);
    EXPECT_EQ(airplane->getType(), kMilitary);

    airplane->setSquawk(12345);
    EXPECT_EQ(airplane->getSquawk(), 12345);

    airplane->setRequest(kAcceptedImmediate);
    EXPECT_EQ(airplane->getRequest(), kAcceptedImmediate);

    airplane->setSize(kSmall);
    EXPECT_EQ(airplane->getSize(), kSmall);

    airplane->setEngine(kPropeller);
    EXPECT_EQ(airplane->getEngine(), kPropeller);

    airplane->setStatus(kAway);
    EXPECT_EQ(airplane->getStatus(), kAway);

    airplane->setCallsign("PlaneCS");
    EXPECT_EQ(airplane->getCallsign(), "PlaneCS");

    airplane->setTimeRemaining(50);
    EXPECT_EQ(airplane->getTimeRemaining(), 50);

    airplane->setAltitude(100000);
    EXPECT_EQ(airplane->getAltitude(), 100000);

    airplane->setFuel(50000);
    EXPECT_EQ(airplane->getFuel(), 50000);

    airplane->setGateID(5);
    EXPECT_EQ(airplane->getGateID(), 5);

    airplane->setModel("F16");
    EXPECT_EQ(airplane->getModel(), "F16");

    airplane->setNumber("16");
    EXPECT_EQ(airplane->getNumber(), "16");

    airplane->setPassengers(2);
    EXPECT_EQ(airplane->getPassengers(), 2);

    Runway *rw = new Runway();
    airplane->setRunway(rw);
    EXPECT_TRUE(airplane->getRunway() == rw);
    delete rw;
}

TEST_F(domainTestAirplane, ContractViolations) {
    EXPECT_DEATH(airplane->decreaseAltitude(-3000), "Difference can't be negative");
    EXPECT_DEATH(airplane->increaseAltitude(-2000), "Difference can't be negative");

    // Set altitude to 1 and try to decrease by 2
    airplane->setAltitude(1000);
    EXPECT_DEATH(airplane->decreaseAltitude(2000), "New altitude can't be less than 0!");

    EXPECT_DEATH(airplane->setAltitude(-3), "Altitude can't be negative");
    EXPECT_DEATH(airplane->setTimeRemaining(-5), "Time remaining can't be negative");
    EXPECT_DEATH(airplane->setPassengers(-4), "Passenger amount can't be negative");
    EXPECT_DEATH(airplane->setGateID(-3), "Gate id can't be less than -1");
    EXPECT_DEATH(airplane->setFuel(0), "Fuel can't be less than 1");
    EXPECT_DEATH(airplane->setFuel(-3), "Fuel can't be less than 1");
    EXPECT_DEATH(airplane->setSquawk(-4), "Squawk code can't be negative");
}