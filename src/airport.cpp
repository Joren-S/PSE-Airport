//
// Created by uauser on 3/1/18.
//

#include "../headers/airport.h"

const string &airport::getFName() const {
    return fName;
}

void airport::setFName(const string &fName) {
    airport::fName = fName;
}

const string &airport::getFIata() const {
    return fIata;
}

void airport::setFIata(const string &fIata) {
    airport::fIata = fIata;
}

const string &airport::getFCallsign() const {
    return fCallsign;
}

void airport::setFCallsign(const string &fCallsign) {
    airport::fCallsign = fCallsign;
}

int airport::getFGates() const {
    return fGates;
}

void airport::setFGates(int fGates) {
    airport::fGates = fGates;
}
