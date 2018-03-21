//
// Created by uauser on 3/1/18.
//

#include "../headers/runway.h"

const string &runway::getFName() const {
    return fName;
}

void runway::setFName(const string &fName) {
    runway::fName = fName;
}

airport *runway::getFAirport() const {
    return fAirport;
}

void runway::setFAirport(airport *fAirport) {
    runway::fAirport = fAirport;
}

bool runway::isFree() const {
    return runway::fFree;
}

void runway::setFree(bool free) {
    runway::fFree = free;
}