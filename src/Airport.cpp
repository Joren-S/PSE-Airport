//============================================================================
// Name        : Airport.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/Airport.h"


Airport::Airport() {
    fGates = -1;
    fInitCheck = this;
    ENSURE(properlyInitialized(), "Airport wasn't properly initialized after constructing.");
}

bool Airport::complete() const {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling complete.");
    return !(fName.empty() or fCallsign.empty() or
             fIata.empty() or fGates == -1);
}

size_t Airport::amountOfRunways() const {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling amountOfRunways.");

    // Return the size of the vector containing all runways.
    return fRunways.size();
}

Runway* Airport::getFreeRunway(Airplane *plane) const {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getFreeRunway.");
    REQUIRE(plane != NULL, "Plane object does not exist.");

    // Iterate over all runways
    vector<Runway*>::const_iterator itr;
    for (itr = fRunways.begin(); itr != fRunways.end(); ++itr) {

        // If runway is free and valid for the airplane, return a pointer to it.
        if ((*itr)->isFree() and (*itr)->validForAirplane(plane)) {
            return *itr;
        }
    }

    // If no runway is found, we return a null pointer.
    return NULL;
}

void Airport::initStack() {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling initStack.");
    REQUIRE(fGateStack.empty(), "Can't initialize gate stack, already in use.");

    // Initialize the stack for our gates
    // We count down so 1 is the last value to get pushed and first to get popped.
    for (int i = fGates; i > 0; --i) {
        fGateStack.push(i);
    }
}

int Airport::getFreeGate() {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getFreeGate.");
    REQUIRE(!fGateStack.empty(), "Can't get free gate: no gate in stack.");
    REQUIRE(fGates > 0, "Airport has no gates.");

    // Get top of stack, store it, then pop it.
    int id = fGateStack.top();
    fGateStack.pop();

    ENSURE(id <= fGates && id > 0, "Gate has an invalid ID.");

    // Return the popped gate ID.
    return id;
}

void Airport::restoreGate(int id) {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling restoreGate.");
    REQUIRE(fGates > 0, "Airport has no gates.");
    REQUIRE(id <= fGates && id > 0, "Gate ID is invalid.");

    // Check if gate is already in fGateStack
    stack<int> copy = fGateStack;
    while (!copy.empty()) {
        int elem = copy.top();
        copy.pop();
        if (elem == id) {
            // If it is, we can return.
            return;
        }
    }
    // If it isn't, push the gate ID back on the stack.
    Airport::fGateStack.push(id);

    ENSURE(fGateStack.top() == id, "Gate was not properly added to stack");
}

void Airport::addRunway(Runway *runway) {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling addRunway.");
    REQUIRE(runway != NULL, "Runway cannot be NULL.");

    // Iterate over all runways
    vector<Runway*>::const_iterator itr;
    bool present = false;
    for (itr = fRunways.begin(); itr != fRunways.end(); ++itr) {
        if ((*itr)->getName() == runway->getName() or
            (*itr)->getTaxiPoint() == runway->getTaxiPoint()) {
            // If a duplicate runway already exists in the airport
            present = true;
            break;
        }
    }

    ENSURE(!present, "Runway is already in system.");
    // If valid, we can add our runway.
    fRunways.push_back(runway);
    ENSURE(fRunways.back() == runway, "Runway was not properly added to the system.");
}

Runway* Airport::getRunway(const string &taxipoint) const {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getRunway.");

    // Iterate over all runways
    vector<Runway*>::const_iterator itr;
    for (itr = fRunways.begin(); itr != fRunways.end(); ++itr) {
        if ((*itr)->getTaxiPoint() == taxipoint) {

            // If a runway with the taxipoint exist, return a pointer to it.
            return *itr;
        }
    }

    // If nothing is found, return null pointer.
    return NULL;
}

Runway* Airport::getNextRunway(Airplane *airplane) const {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getNextRunway.");
    REQUIRE(airplane != NULL, "Airplane cannot be NULL.");

    // Iterate over all runways
    vector<Runway*>::const_iterator itr;
    vector<Runway*> runways = getRunways();
    for (unsigned int i=0; i<runways.size(); i++) {

        // if the current runway is found
        if (runways[i]->getTaxiPoint() == airplane->getPosition()) {

            // Return the next runway based on the current action of the plane.
            // Return NULL if no next runway is available.
            if (airplane->getStatus() == kTaxiArrival or airplane->getStatus() == kCrossingArrival) {
                return (i == 0? NULL: runways[i-1]);
            }
            if (airplane->getStatus() == kTaxiDeparture) {
                return i+1 < runways.size()? runways[i+1]: NULL;
            }
        }
    }

    return NULL;
}

void Airport::drawImpression(Time time, ostream &stream, vector<Flightplan*> plans) {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling drawImpression.");

    stringstream impression;

    // We begin with a timestamp
    impression << "[" << time.formatted() << "]" << endl;

    // Iterate over all runways
    vector<Runway*>::const_reverse_iterator i;
    for (i = fRunways.rbegin(); i != fRunways.rend(); ++i) {
        Runway *curRW = *i;
        string curTP = curRW->getTaxiPoint();

        // begin of line
        impression << curRW->getName() << " | =====";

        // Iterate over flightplans
        bool planeFound = false;
        int planesAtTaxiPoint = 0;
        vector<Flightplan*>::const_iterator j;
        for (j = plans.begin(); j != plans.end(); ++j) {
            Airplane *curPlane = (*j)->getAirplane();

            // Plane is landing
            if (curPlane->getPosition().empty() and curPlane->getAltitude() == 0 and curPlane->getStatus() == kDescending and curPlane->getRunway() == curRW) {
                planeFound = true;
            }

            // Plane is crossing when taxiing at arrival
            else if (curPlane->getStatus() == kCrossingArrival and this->getNextRunway(curPlane) == curRW) {
                planeFound = true;
            }

            // Plane just landed
            else if (curPlane->getPosition().empty() and curPlane->getStatus() == kTaxiArrival and curPlane->getRunway() == curRW) {
                planesAtTaxiPoint++;
            }

            // At taxipoint
            else if (curPlane->getStatus() == kTaxiArrival and curPlane->getPosition() == curTP) {
                planesAtTaxiPoint++;
            }
//
//            if (curPlane->getPosition() == curTP) {
//                EPlaneStatus status = curPlane->getStatus();
//
//                // if plane is on runway
//                if (status == kCrossingArrival or status == kCrossingDeparture or status == kDeparture or (status == kDescending and curPlane->getAltitude() == 0)) {
//                    impression << "V====" << endl;
//                    planeFound = true;
//                }
//                else {
//                    planesAtTaxiPoint++;
//                }
//            }
        }
        if (!planeFound) {
            impression << "=====" << endl;
        }
        else {
            impression << "V====" << endl;
        }

        string planesattaxi (planesAtTaxiPoint, 'V');
        impression << "TP" << curTP.at(0) << " | " << planesattaxi << endl;
    }

    // Count the planes at the gates
    int planesAtGate = 0;
    vector<Flightplan*>::const_iterator j;
    for (j = plans.begin(); j != plans.end(); ++j) {
        Airplane *curPlane = (*j)->getAirplane();
        EPlaneStatus status = curPlane->getStatus();

        // Landing
        if (status == kDeboarding or status == kTechnicalCheck or status == kParked) {
            planesAtGate++;
        }

        if (status == kGate or status == kAirport or status == kPushback) {
            planesAtGate++;
        }
    }

    // Write result to impression
    string planesatgate (planesAtGate, 'V');
    impression << "Gates [ " << planesatgate << " ]" << endl;

    // Write the result to our stream
    stream << impression.str();
}

// Getters en setters

const string &Airport::getName() const {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
    return fName;
}

void Airport::setName(const string &name) {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
    fName = name;
    ENSURE(fName == name, "Field wasn't set properly");
}

const string &Airport::getIata() const {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
    return fIata;
}

void Airport::setIata(const string &iata) {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
    fIata = iata;
    ENSURE(fIata == iata, "Field wasn't set properly");
}

const string &Airport::getCallsign() const {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
    return fCallsign;
}

void Airport::setCallsign(const string &callsign) {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
    fCallsign = callsign;
    ENSURE(fCallsign == callsign, "Field wasn't set properly");
}

int Airport::getGates() const {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
    return fGates;
}

void Airport::setGates(int gates) {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
    REQUIRE(gates >= 0, "Number of gates cannot be negative!");
    fGates = gates;
    ENSURE(fGates == gates, "Field wasn't set properly");
}

vector<Runway*> Airport::getRunways() const {
    REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
    return fRunways;
}

// Initialisation check

bool Airport::properlyInitialized() const {
    return fInitCheck == this;
}