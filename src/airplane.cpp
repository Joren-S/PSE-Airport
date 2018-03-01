//
// Created by uauser on 3/1/18.
//

#include "../headers/airplane.h"

const string &airplane::getFNumber() const {
    return fNumber;
}

void airplane::setFNumber(const string &fNumber) {
    airplane::fNumber = fNumber;
}

const string &airplane::getFCallsign() const {
    return fCallsign;
}

void airplane::setFCallsign(const string &fCallsign) {
    airplane::fCallsign = fCallsign;
}

const string &airplane::getFModel() const {
    return fModel;
}

void airplane::setFModel(const string &fModel) {
    airplane::fModel = fModel;
}

EPlaneStatus airplane::getFStatus() const {
    return fStatus;
}

void airplane::setFStatus(EPlaneStatus fStatus) {
    airplane::fStatus = fStatus;
}
