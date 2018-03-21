//
// Created by uauser on 3/1/18.
//

#include "../headers/airport.h"
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
    for (int i = fGates; i > 0; --i) {
        fGateStack.push(i);
    }
}

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

int airport::getFreeGate() {
    if (airport::fGateStack.empty()) {
        return -1;
    }
    int temp = airport::fGateStack.top();
    airport::fGateStack.pop();
    return temp;
}

void airport::restoreGate(int id) {
    airport::fGateStack.push(id);
}