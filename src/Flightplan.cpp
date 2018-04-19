//
// Created by Max Van Houcke on 12-4-2018.
//

#include "../headers/Flightplan.h"

Flightplan::Flightplan(): fAirplane(NULL), fDeparture(-1), fArrival(-1), fInterval(-1) {}

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
    Time startingTime(12, fDeparture);

    for (int i=0; i<12; i++) {
        Time newTime = Time(startingTime.getHour() + fInterval * i, fDeparture);
        if (newTime == time) {
            return kTakeoff;
        }
        newTime = Time(startingTime.getHour() + fInterval * i, fArrival);
        if (newTime == time) {
            return kLand;
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