//============================================================================
// Name        : domainTestsFlightplan.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <gtest/gtest.h>
#include "../headers/System.h"


class domainTestFlightplan: public ::testing::Test {
protected:

    Flightplan flightplan;

};


TEST_F(domainTestFlightplan, defaultConstructor) {
    EXPECT_TRUE(flightplan.properlyInitialized());
    EXPECT_EQ(flightplan.getDeparture(), -1);
    EXPECT_EQ(flightplan.getArrival(), -1);
    EXPECT_EQ(flightplan.getInterval(), -1);
    EXPECT_TRUE(flightplan.getAirplane() == NULL);
}

TEST_F(domainTestFlightplan, happyDay) {
    flightplan.setInterval(1);
    EXPECT_EQ(flightplan.getInterval(), 1);
    flightplan.setArrival(5);
    EXPECT_EQ(flightplan.getArrival(), 5);
    flightplan.setDeparture(12);
    EXPECT_EQ(flightplan.getDeparture(), 12);

    EEvent event;
    event = flightplan.getEvent(Time(12, 12));
    EXPECT_EQ(event, kTakeoff);
    event = flightplan.getEvent(Time(13, 12));
    EXPECT_EQ(event, kTakeoff);

    event = flightplan.getEvent(Time(13, 5));
    EXPECT_EQ(event, kLand);
    event = flightplan.getEvent(Time(12, 5));
    EXPECT_EQ(event, kLand);

    event = flightplan.getEvent(Time(14, 55));
    EXPECT_EQ(event, kNothing);
}

TEST_F(domainTestFlightplan, contractViolations) {
    EXPECT_DEATH(flightplan.setDeparture(-2), "Departure has to be between 0 and 60");
    EXPECT_DEATH(flightplan.setDeparture(65), "Departure has to be between 0 and 60");
    EXPECT_DEATH(flightplan.setArrival(-6), "Arrival has to be between 0 and 60");
    EXPECT_DEATH(flightplan.setArrival(200), "Arrival has to be between 0 and 60");
    EXPECT_DEATH(flightplan.setInterval(0), "Interval has to be at least 1");
}