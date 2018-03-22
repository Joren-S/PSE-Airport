//
// Created by uauser on 3/1/18.
//

#include "../../PSE-Airport/headers/airport.h"
//
//airport::airport(const string &name, const string &iata, const string &callsign, int numGates):
//        fName(name),
//        fIata(iata),
//        fCallsign(callsign),
//        fGates(numGates) {
//    for (int i=1; i<=numGates; ++i) {
//        fGateStack.push(i);
//    }
//}

void airport::initStack() {
    // Initialize the stack for our gates (with 1 being the last value to get pushed).
    for (int i = fGates; i > 0; --i) {
        fGateStack.push(i);
    }
}

int airport::getFreeGate() {
    // Pops and returns the next available free gate ID from the stack.
    // Returns -1 if no gate is available.
    if (airport::fGateStack.empty()) {
        return -1;
    }
    int temp = airport::fGateStack.top();
    airport::fGateStack.pop();
    return temp;
}

void airport::restoreGate(int id) {
    // Push a gate ID back on the stack when it's free again.
    airport::fGateStack.push(id);
}

// Getters en setters

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