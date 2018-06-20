//============================================================================
// Name        : FlightPlan.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/FlightPlan.h"

using namespace std;

FlightPlan::FlightPlan(): fAirplane(NULL), fDeparture(-1), fArrival(-1), fInterval(-1) {
    fInitCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

bool FlightPlan::complete() const {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling complete");
    return !(fDeparture == -1 or fArrival == -1 or
            fInterval == -1 or fDestination.empty());
}

const string &FlightPlan::getDestination() const {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling getter/setter");
    return fDestination;
}

void FlightPlan::setDestination(const string &fDestination) {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling getter/setter");
    FlightPlan::fDestination = fDestination;
}

int FlightPlan::getDeparture() const {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling getter/setter");
    return fDeparture;
}

void FlightPlan::setDeparture(int departure) {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling setDeparture");
    REQUIRE(departure >= 0 && departure < 60, "Departure has to be between 0 and 60");
    FlightPlan::fDeparture = departure;
}

int FlightPlan::getArrival() const {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling getter/setter");
    return fArrival;
}

void FlightPlan::setArrival(int arrival) {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling setArrival");
    REQUIRE(arrival >= 0 && arrival < 60, "Arrival has to be between 0 and 60");
    FlightPlan::fArrival = arrival;
}

int FlightPlan::getInterval() const {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling getter/setter");
    return fInterval;
}

void FlightPlan::setInterval(int interval) {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling setInterval");
    REQUIRE(interval > 0, "Interval has to be at least 1");
    FlightPlan::fInterval = interval;
}

EEvent FlightPlan::getEvent(Time time) {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling getEvent");
    // Start at 12:00
    Time currentTime(12);

    // While not midnight
    while (true) {
        // Set to departure time
        currentTime = Time(currentTime.getHour(), fDeparture);

        // If times match up, return takeoff
        if (currentTime == time) {
            return kTakeoff;
        }

        // Set to arrival time
        currentTime = Time(currentTime.getHour(), fArrival);

        // If times match up, return land
        if (currentTime == time) {
            return kLand;
        }

        // Set advance hour by interval
        currentTime.advance(60 * fInterval);

        if (currentTime.getHour() < 12) {
            break;
        }
    }

    return kNothing;
}

Airplane* FlightPlan::getAirplane() const {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling getter/setter");
    return fAirplane;
}

void FlightPlan::setAirplane(Airplane *airplane) {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling getter/setter");
    fAirplane = airplane;
}

FlightPlan::~FlightPlan() {
    REQUIRE(this->properlyInitialized(), "FlightPlan was't initialized when calling getter/setter");
    delete fAirplane;
}

bool FlightPlan::properlyInitialized() const {
    return fInitCheck == this;
}