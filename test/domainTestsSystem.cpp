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
    airplane->setPassengers(7);
    system.taxiArrival(airplane, out);
    EXPECT_TRUE(airplane->getPosition().empty());
    EXPECT_EQ(airplane->getStatus(), kDeboarding);
    EXPECT_EQ(airplane->getTimeRemaining(), 4);

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
    airplane->setRunway(runway);
    system.taxiArrival(airplane, out);
    EXPECT_EQ(airplane->getStatus(), kTaxiArrival);
    EXPECT_EQ(airplane->getPosition(), airplane->getRunway()->getTaxiPoint());
    EXPECT_EQ(airplane->getRequest(), kConfirmed);
    EXPECT_EQ(airplane->getTimeRemaining(), 5);

    // Request has been accepted, plane crosses a runway
    airplane->setRequest(kAccepted);
    airplane->setPosition(system.getAirport()->getRunways()[1]->getTaxiPoint());
    system.taxiArrival(airplane, out);
    EXPECT_EQ(airplane->getStatus(), kCrossingArrival);
    EXPECT_EQ(airplane->getRequest(), kConfirmed);
}

TEST_F(domainTestSystem, crossArrival) {
    airplane->setPosition(system.getAirport()->getRunways()[1]->getTaxiPoint());
    airplane->setStatus(kTaxiArrival);
    system.crossArrival(airplane, out);
    EXPECT_EQ(airplane->getStatus(), kTaxiArrival);
    EXPECT_TRUE(runway->isFree());
    EXPECT_EQ(airplane->getPosition(), runway->getTaxiPoint());
    EXPECT_EQ(airplane->getTimeRemaining(), 5);
}

TEST_F(domainTestSystem, deboard) {
    airplane->setSize(kSmall);
    system.deboard(airplane, out);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    airplane->setSize(kMedium);
    system.deboard(airplane, out);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);

    airplane->setSize(kLarge);
    system.deboard(airplane, out);
    EXPECT_EQ(airplane->getTimeRemaining(), 3);

    EXPECT_EQ(airplane->getStatus(), kTechnicalCheck);
    EXPECT_EQ(airplane->getRequest(), kIdle);
}

TEST_F(domainTestSystem, technicalCheck) {
    // Very simple function so very simple test
    system.technicalCheck(airplane, out);
    EXPECT_EQ(airplane->getStatus(), kParked);
    EXPECT_EQ(airplane->getRequest(), kIdle);
}

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

TEST_F(domainTestSystem, ascend) {
    airplane->setStatus(kAscending);
    airplane->setEngine(kPropeller);

    // ascend 1000ft each time ascend is called and altitude < 5000ft
    EXPECT_EQ(airplane->getAltitude(), 0);
    system.ascend(airplane, out);
    EXPECT_EQ(airplane->getPosition(), "");
    EXPECT_EQ(airplane->getAltitude(), 1);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);
    system.ascend(airplane, out);
    EXPECT_EQ(airplane->getAltitude(), 2);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);
    system.ascend(airplane, out);
    EXPECT_EQ(airplane->getAltitude(), 3);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);
    system.ascend(airplane, out);
    EXPECT_EQ(airplane->getAltitude(), 4);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);
    system.ascend(airplane, out);
    EXPECT_EQ(airplane->getAltitude(), 5);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);
    system.ascend(airplane, out);
    EXPECT_EQ(airplane->getStatus(), kAway);
}

TEST_F(domainTestSystem, onRunway) {
    // Request permission to take off.
    airplane->setStatus(kDeparture);
    airplane->setRequest(kIdle);
    airplane->setRunway(runway);

    system.onRunway(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kDeparture);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC hasn't responded yet
    system.onRunway(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kDeparture);

    // ATC gave permission to take off
    airplane->setRequest(kAccepted);
    system.onRunway(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kAscending);

}

TEST_F(domainTestSystem, atRunway) {
    // Request permission to go on runway
    airplane->setStatus(kWaitingForDeparture);
    airplane->setRequest(kIdle);
    airplane->setRunway(runway);

    system.atRunway(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kWaitingForDeparture);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC hasn't responded yet
    system.atRunway(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kWaitingForDeparture);

    // ATC gave permission to line up on runway
    airplane->setRequest(kAccepted);
    system.atRunway(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kDeparture);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC gave permission to line up and take off.
    airplane->setRequest(kAcceptedImmediate);
    system.atRunway(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kAscending);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);
}

TEST_F(domainTestSystem, taxiDepartureCross) {
    airplane->setStatus(kCrossingDeparture);
    airplane->setRequest(kIdle);
    airplane->setPosition(runway->getTaxiPoint());
    system.taxiDepartureCross(airplane, out);
    EXPECT_EQ(airplane->getStatus(), kTaxiDeparture);
    EXPECT_EQ(airplane->getTimeRemaining(), 5);
}

TEST_F(domainTestSystem, taxiDepartureStep) {
    airplane->setSize(kSmall);
    airplane->setEngine(kPropeller);
    airplane->setStatus(kTaxiDeparture);
    airplane->setRequest(kIdle);
    airplane->setRunway(runway);
    airplane->setPosition("");

    // first ever taxi step
    system.taxiDepartureStep(airplane, out);
    EXPECT_EQ(airplane->getStatus(), kTaxiDeparture);
    EXPECT_EQ(airplane->getPosition(), system.getAirport()->getRunways().at(0)->getTaxiPoint());
    EXPECT_EQ(airplane->getTimeRemaining(), 5);

    // request crossing
    airplane->setRequest(kIdle);
    system.taxiDepartureStep(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kTaxiDeparture);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // atc hasn't responded yet
    system.taxiDepartureStep(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kTaxiDeparture);

    // atc has responded and has accepted the request
    airplane->setRequest(kAccepted);
    system.taxiDepartureStep(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kCrossingDeparture);
    EXPECT_FALSE(system.getAirport()->getRunway(airplane->getPosition())->isFree());
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // atc has responded and has confirmed the request (arrived at destination)
    airplane->setRequest(kConfirmed);
    airplane->getRunway()->setName("RW1");
    airplane->getRunway()->setTaxiPoint("Alpha");
    airplane->setPosition("Alpha");
    system.taxiDepartureStep(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kWaitingForDeparture);
}

TEST_F(domainTestSystem, taxiDepartureStart) {
    // Requesting permission to start taxiing
    airplane->setStatus(kPushback);
    airplane->setRequest(kIdle);
    airplane->setCallsign("CSAP");
    system.taxiDepartureStart(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kPushback);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC hasn't responded yet
    system.taxiDepartureStart(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kPushback);

    // ATC has responded and accepted request
    airplane->setRequest(kAccepted);
    system.taxiDepartureStart(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kTaxiDeparture);
    EXPECT_EQ(airplane->getPosition(), "");
}

TEST_F(domainTestSystem, pushback) {
    // Requesting permission to pushback
    airplane->setStatus(kGate);
    airplane->setRequest(kIdle);
    system.pushback(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kGate);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC hasn't responded yet
    system.pushback(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kGate);

    // ATC has responded and accepted request
    // different plane sizes:
    airplane->setRequest(kAccepted);
    airplane->setSize(kSmall);
    system.pushback(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kPushback);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    airplane->setRequest(kAccepted);
    airplane->setSize(kMedium);
    system.pushback(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kPushback);
    EXPECT_EQ(airplane->getTimeRemaining(), 2);

    airplane->setRequest(kAccepted);
    airplane->setSize(kLarge);
    system.pushback(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kPushback);
    EXPECT_EQ(airplane->getTimeRemaining(), 3);
}

TEST_F(domainTestSystem, prepare) {
    // Requesting IFR clearancy
    airplane->setStatus(kAirport);
    airplane->setRequest(kIdle);
    runway->setName("RW1");
    airplane->setRunway(runway);
    system.prepare(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kAirport);
    EXPECT_EQ(airplane->getTimeRemaining(), 1);

    // ATC hasn't responded yet
    system.prepare(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kPending);
    EXPECT_EQ(airplane->getStatus(), kAirport);

    // ATC has responded and accepted request
    airplane->setRequest(kAccepted);
    system.prepare(airplane, out);
    EXPECT_EQ(airplane->getRequest(), kIdle);
    EXPECT_EQ(airplane->getStatus(), kGate);
}

TEST_F(domainTestSystem, fieldManipulation) {
    Time time = Time(12, 0);
    system.setEndTime(time);
    EXPECT_EQ(system.getTime().getHour(), 12);
    EXPECT_EQ(system.getTime().getMinute(), 0);
}