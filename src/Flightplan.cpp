//============================================================================
// Name        : Flightplan.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/Flightplan.h"

Flightplan::Flightplan(): fAirplane(NULL), fDeparture(-1), fArrival(-1), fInterval(-1) {
    fInitCheck = this;
}

bool Flightplan::complete() const {
    return !(fDeparture == -1 or fArrival == -1 or
            fAirplane == NULL or fInterval == -1 or
            fDestination.empty());
}

const std::string &Flightplan::getDestination() const {
    return fDestination;
}

void Flightplan::setDestination(const std::string &fDestination) {
    Flightplan::fDestination = fDestination;
}

int Flightplan::getDeparture() const {
    return fDeparture;
}

void Flightplan::setDeparture(int departure) {
    REQUIRE(departure >= 0 && departure < 60, "Wrong value for departure.");
    Flightplan::fDeparture = departure;
}

int Flightplan::getArrival() const {
    return fArrival;
}

void Flightplan::setArrival(int arrival) {
    REQUIRE(arrival >= 0 && arrival < 60, "Wrong value for arrival.");
    Flightplan::fArrival = arrival;
}

int Flightplan::getInterval() const {
    return fInterval;
}

void Flightplan::setInterval(int interval) {
    REQUIRE(interval > 0, "Wrong value for interval.");
    Flightplan::fInterval = interval;
}

EEvent Flightplan::getEvent(Time time) {
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
    return fAirplane;
}

void Flightplan::setAirplane(Airplane *airplane) {
    fAirplane = airplane;
}

Flightplan::~Flightplan() {
    delete fAirplane;
}

bool Flightplan::properlyInitialized() const {
    return fInitCheck == this;
}