//
// Created by uauser on 3/1/18.
//

#include "../headers/Airport.h"
//
//Airport::Airport(const string &name, const string &iata, const string &callsign, int numGates):
//        fName(name),
//        fIata(iata),
//        fCallsign(callsign),
//        fGates(numGates) {
//    for (int i=1; i<=numGates; ++i) {
//        fGateStack.push(i);
//    }
//}

void Airport::initStack() {
    // Initialize the stack for our gates (with 1 being the last value to get pushed).
    for (int i = fGates; i > 0; --i) {
        fGateStack.push(i);
    }
}

int Airport::getFreeGate() {
    // Pops and returns the next available free gate ID from the stack.
    // Returns -1 if no gate is available.
    if (Airport::fGateStack.empty()) {
        return -1;
    }
    int temp = Airport::fGateStack.top();
    Airport::fGateStack.pop();
    return temp;
}

void Airport::restoreGate(int id) {
    // Push a gate ID back on the stack when it's free again.
    Airport::fGateStack.push(id);
}

// Getters en setters

const string &Airport::getFName() const {
    return fName;
}

void Airport::setFName(const string &fName) {
    Airport::fName = fName;
}

const string &Airport::getFIata() const {
    return fIata;
}

void Airport::setFIata(const string &fIata) {
    Airport::fIata = fIata;
}

const string &Airport::getFCallsign() const {
    return fCallsign;
}

void Airport::setFCallsign(const string &fCallsign) {
    Airport::fCallsign = fCallsign;
}

int Airport::getFGates() const {
    return fGates;
}

void Airport::setFGates(int fGates) {
    Airport::fGates = fGates;
}