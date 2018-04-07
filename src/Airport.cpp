//
// Created by uauser on 3/1/18.
//

#include "../headers/Airport.h"

Airport::Airport(const string &name, const string &iata, const string &callsign, int gates): fName(name),
                                                                                             fIata(iata),
                                                                                             fCallsign(callsign),
                                                                                             fGates(gates) {
    initStack();
}

void Airport::initStack() {
    string error = "Can't initialize gate stack, already in use";
//    REQUIRE(fGateStack.empty(), error.c_str());

    // Initialize the stack for our gates (with 1 being the last value to get pushed).
    for (int i = fGates; i > 0; --i) {
        fGateStack.push(i);
    }
}

int Airport::getFreeGate() {
    string error = "Can't get free gate, no available";
//    REQUIRE(!fGateStack.empty(), error.c_str());

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
//    REQUIRE(id <= fGates && id > 0, error.c_str());

    // Check if gate is already available
//    error = "Gate is already available";
//    stack<int> copy = fGateStack;
//    bool elemInStack = false;
//    while (!copy.empty()) {
//        int elem = copy.top();
//        copy.pop();
//        if (elem == id) {
//            elemInStack = true;
//            break;
//        }
//    }
//    REQUIRE(!elemInStack, error);

    // Push a gate ID back on the stack when it's free again.
    Airport::fGateStack.push(id);

    // Succesfully added
    error = "Gate was not succesfully added to stack";
//    REQUIRE(fGateStack.top() == id, error.c_str());
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