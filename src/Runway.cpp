//============================================================================
// Name        : Runway.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/Runway.h"


Runway::Runway(): fFree(true), fAirport(NULL), fType(kDefaultRunType), fLength(-1) {
    fInitCheck = this;
    ENSURE(properlyInitialized(), "Runway wasn't properly initialized.");
}

bool Runway::complete() const {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    return !(fType == kDefaultRunType or fAirport == NULL or
             fLength == -1 or fName.empty() or fTaxiPoint.empty());
}

ERunwayType Runway::getType() const {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    return fType;
}

void Runway::setType(ERunwayType type) {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    fType = type;
    ENSURE(fType == type, "Field wasn't set properly");
}

int Runway::getLength() const {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    return fLength;
}

void Runway::setLength(int length) {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    fLength = length;
    ENSURE(fLength == length, "Field wasn't set properly");
}

const string &Runway::getName() const {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    return fName;
}

void Runway::setName(const string &name) {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    fName = name;
    ENSURE(fName == name, "Field wasn't set properly");
}

Airport *Runway::getAirport() const {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    return fAirport;
}

void Runway::setAirport(Airport *airport) {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    fAirport = airport;
    ENSURE(fAirport == airport, "Field wasn't set properly");
}

bool Runway::isFree() const {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    return Runway::fFree;
}

void Runway::setFree(bool free) {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    fFree = free;
    ENSURE(fFree == free, "Field wasn't set properly");
}

string Runway::getTaxiPoint() const {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    return fTaxiPoint;
}

void Runway::setTaxiPoint(const string &name) {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    fTaxiPoint = name;
    ENSURE(fTaxiPoint == name, "Field wasn't set properly");
}


bool Runway::validForAirplane(Airplane *plane) const {
    REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
    REQUIRE(plane != NULL, "Plane object does not exist.");

    // Get the required info about the plane.
    EPlaneSize size = plane->getSize();
    EPlaneEngine engine = plane->getEngine();
    ERunwayType runwayType = getType();
    int length = getLength();

    // Check all possible valid combinations
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

    // Anything else gets rejected.
    return false;
}

bool Runway::properlyInitialized() const {
    return fInitCheck == this;
}