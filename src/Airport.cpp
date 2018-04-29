//
// Created by uauser on 3/1/18.
//

#include <vector>
#include "../headers/Airport.h"


Airport::Airport() {
    fGates = -1;
}

size_t Airport::amountOfRunways() const {
    return fRunways.size();
}

Runway* Airport::getFreeRunway(Airplane *plane) const {
    REQUIRE(plane != NULL, "Plane object does not exist.");
    vector<Runway*>::const_iterator itr;
    for (itr = fRunways.begin(); itr != fRunways.end(); ++itr) {
        if ((*itr)->isFree() and (*itr)->validForAirplane(plane)) {
            return *itr;
        }
    }
    return NULL;
}

void Airport::initStack() {
    string error = "Can't initialize gate stack, already in use";
    REQUIRE(fGateStack.empty(), error.c_str());

    // Initialize the stack for our gates (with 1 being the last value to get pushed).
    for (int i = fGates; i > 0; --i) {
        fGateStack.push(i);
    }
}

int Airport::getFreeGate() {
    string error = "Can't get free gate, no available";
    REQUIRE(!fGateStack.empty(), error.c_str());

    // Get top of stack
    int temp = Airport::fGateStack.top();

    // Pop it
    Airport::fGateStack.pop();

    // Return value
    return temp;
}

void Airport::restoreGate(int id) {
    // Check if gate exists
    string error = "Gate does not exist";
    REQUIRE(id <= fGates && id > 0, error.c_str());

    // Check if gate is already available
    stack<int> copy = fGateStack;
    bool elemInStack = false;
    while (!copy.empty()) {
        int elem = copy.top();
        copy.pop();
        if (elem == id) {
            elemInStack = true;
            break;
        }
    }
    REQUIRE(!elemInStack, "Gate is already available");

    // Push a gate ID back on the stack when it's free again.
    Airport::fGateStack.push(id);

    // Succesfully added
    error = "Gate was not succesfully added to stack";
    REQUIRE(fGateStack.top() == id, error.c_str());
}

bool Airport::complete() const {
    return !(fName.empty() or fCallsign.empty() or
             fIata.empty() or fGates == -1);
}

// Getters en setters

const string &Airport::getName() const {
    return fName;
}

void Airport::setName(const string &fName) {
    Airport::fName = fName;
}

const string &Airport::getIata() const {
    return fIata;
}

void Airport::setIata(const string &fIata) {
    Airport::fIata = fIata;
}

const string &Airport::getCallsign() const {
    return fCallsign;
}

void Airport::setCallsign(const string &fCallsign) {
    Airport::fCallsign = fCallsign;
}

int Airport::getGates() const {
    return fGates;
}

void Airport::setGates(int fGates) {
    REQUIRE(fGates >= 0, "Number of gates cannot be negative!");
    Airport::fGates = fGates;
}

void Airport::addRunway(Runway *runway) {
    REQUIRE(runway != NULL, "Runway cannot be NULL.");
    vector<Runway*>::const_iterator itr;
    bool present = false;
    for (itr = fRunways.begin(); itr != fRunways.end(); ++itr) {
        if ((*itr)->getName() == runway->getName() or
                (*itr)->getTaxiPoint() == runway->getTaxiPoint()) {
            present = true;
            break;
        }
    }
    REQUIRE(!present, "Runway is already in system.");
    fRunways.push_back(runway);
    ENSURE(fRunways.back() == runway, "Runway was not added to the system.");
}

vector<Runway*> Airport::getRunways() const {
    return fRunways;
}

Runway* Airport::getRunway(const string &taxipoint) const {
    vector<Runway*>::const_iterator itr;
    vector<Runway*> runways = getRunways();

    for (itr = runways.begin(); itr != runways.end(); ++itr) {
        if ((*itr)->getTaxiPoint() == taxipoint) {
            return *itr;
        }
    }

    return NULL;
}

Runway* Airport::getNextRunway(Airplane *airplane) const {
    vector<Runway*>::const_iterator itr;
    vector<Runway*> runways = getRunways();

    for (unsigned int i=0; i<runways.size(); i++) {
        if (runways[i]->getTaxiPoint() == airplane->getPosition()) {
            if (airplane->getStatus() == kTaxiArrival or airplane->getStatus() == kCrossing) {
                return (i == 0? NULL: runways[i-1]);
            }
            if (airplane->getStatus() == kTaxiDeparture) {
                return i+1 < runways.size()? runways[i+1]: NULL;
            }
        }
    }

    return NULL;
}