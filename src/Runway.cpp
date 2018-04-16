//
// Created by uauser on 3/1/18.
//

#include "../headers/Runway.h"


Runway::Runway() {
    fAirport = NULL;
    fType = kAsphalt;
    fLength = -1;
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

const string &Runway::getName() const {
    return fName;
}

void Runway::setName(const string &fName) {
    Runway::fName = fName;
}

Airport *Runway::getAirport() const {
    return fAirport;
}

void Runway::setAirport(Airport *fAirport) {
    Runway::fAirport = fAirport;
}

bool Runway::isFree() const {
    return Runway::fFree;
}

void Runway::setFree(bool free) {
    Runway::fFree = free;
}