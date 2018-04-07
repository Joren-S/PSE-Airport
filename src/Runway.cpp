//
// Created by uauser on 3/1/18.
//

#include "../headers/Runway.h"

Runway::Runway(bool free, const string& name, Airport* airport, ERunwayType type, int length):    fFree(free),
                                                                                                  fName(name),
                                                                                                  fAirport(airport),
                                                                                                  fType(type),
                                                                                                  fLength(length) {}

Runway::Runway() {
    fAirport = NULL;
}

ERunwayType Runway::getType() const {
    return fType;
}

void Runway::setType(ERunwayType type) {
    fType = type;
}

int Runway::getLength() const {
    return fLength;
}

void Runway::setLength(int length) {
    fLength = length;
}

const string &Runway::getFName() const {
    return fName;
}

void Runway::setFName(const string &fName) {
    Runway::fName = fName;
}

Airport *Runway::getFAirport() const {
    return fAirport;
}

void Runway::setFAirport(Airport *fAirport) {
    Runway::fAirport = fAirport;
}

bool Runway::isFree() const {
    return Runway::fFree;
}

void Runway::setFree(bool free) {
    Runway::fFree = free;
}