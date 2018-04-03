//
// Created by uauser on 3/1/18.
//

#include "../headers/Runway.h"

// Getters en setters

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