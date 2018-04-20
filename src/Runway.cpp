//
// Created by uauser on 3/1/18.
//

#include "../headers/Runway.h"


Runway::Runway(): fFree(true), fAirport(NULL), fType(kDefaultRunType), fLength(-1) {}

bool Runway::complete() const {
    return !(fType == kDefaultRunType or fAirport == NULL or
             fLength == -1 or fName.empty() or fTaxiPoint.empty());
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

string Runway::getTaxiPoint() const {
    return fTaxiPoint;
}

void Runway::setTaxiPoint(const string &name) {
    fTaxiPoint = name;
}

bool Runway::validForAirplane(Airplane *plane) const {
    REQUIRE(plane != NULL, "Plane object does not exist.");

    EPlaneSize size = plane->getSize();
    EPlaneEngine engine = plane->getEngine();
    ERunwayType runwayType = getType();
    int length = getLength();


    if (size == kSmall) {
        if (engine == kPropeller) {
            if (length >= 500) {
                // Small, propeller, grass/no grass, l >= 500
                return true;
            }
        }
        if (engine == kJet) {
            if (runwayType != kGrass) {
                if (length >= 1000) {
                    // Small, jet, no grass, l >= 1000
                    return true;
                }
            }
        }
    }

    if (size == kMedium) {
        if (engine == kPropeller) {
            if (runwayType != kGrass) {
                if (length >= 1000) {
                    // Medium, propeller, no grass, l >= 1000
                    return true;
                }
            }
        }
        if (engine == kJet) {
            if (runwayType != kGrass) {
                if (length >= 2000) {
                    // Medium, jet, no grass, l >= 2000
                    return true;
                }
            }
        }
    }

    if (size == kLarge) {
        if (engine == kPropeller) {
            if (runwayType != kGrass) {
                if (length >= 1500) {
                    // Large, propeller, no grass, l >= 1500
                    return true;
                }
            }
        }
        if (engine == kJet) {
            if (runwayType != kGrass) {
                if (length >= 3000) {
                    // Large, jet, no grass, l >= 3000
                    return true;
                }
            }
        }
    }
    return false;
}