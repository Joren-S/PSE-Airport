//============================================================================
// Name        : Flightplan.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/Flightplan.h"

Flightplan::Flightplan(): fAirplane(NULL), fDeparture(-1), fArrival(-1), fInterval(-1) {
    fInitCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

bool Flightplan::complete() const {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling complete");
    return !(fDeparture == -1 or fArrival == -1 or
            fAirplane == NULL or fInterval == -1 or
            fDestination.empty());
}

const string &Flightplan::getDestination() const {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling getter/setter");
    return fDestination;
}

void Flightplan::setDestination(const string &fDestination) {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling getter/setter");
    Flightplan::fDestination = fDestination;
}

int Flightplan::getDeparture() const {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling getter/setter");
    return fDeparture;
}

void Flightplan::setDeparture(int departure) {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling setDeparture");
    REQUIRE(departure >= 0 && departure < 60, "Departure has to be between 0 and 60");
    Flightplan::fDeparture = departure;
}

int Flightplan::getArrival() const {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling getter/setter");
    return fArrival;
}

void Flightplan::setArrival(int arrival) {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling setArrival");
    REQUIRE(arrival >= 0 && arrival < 60, "Arrival has to be between 0 and 60");
    Flightplan::fArrival = arrival;
}

int Flightplan::getInterval() const {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling getter/setter");
    return fInterval;
}

void Flightplan::setInterval(int interval) {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling setInterval");
    REQUIRE(interval > 0, "Interval has to be at least 1");
    Flightplan::fInterval = interval;
}

EEvent Flightplan::getEvent(Time time) {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling getEvent");
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

Airplane* Flightplan::getAirplane() const {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling getter/setter");
    return fAirplane;
}

void Flightplan::setAirplane(Airplane *airplane) {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling getter/setter");
    fAirplane = airplane;
}

Flightplan::~Flightplan() {
    REQUIRE(this->properlyInitialized(), "Flightplan was't initialized when calling getter/setter");
    delete fAirplane;
}

bool Flightplan::properlyInitialized() const {
    return fInitCheck == this;
}