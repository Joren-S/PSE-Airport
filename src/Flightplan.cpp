//
// Created by Max Van Houcke on 12-4-2018.
//

#include "../headers/Flightplan.h"

Flightplan::Flightplan(): fDeparture(-1), fArrival(-1), fInterval(-1) {}

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
