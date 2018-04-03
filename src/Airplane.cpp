//
// Created by uauser on 3/1/18.
//

#include "../headers/Airplane.h"


// Getters and setters

int Airplane::getFPassengers() const {
    return fPassengers;
}

void Airplane::setFPassengers(int fPassengers) {
    Airplane::fPassengers = fPassengers;
}

const string &Airplane::getFNumber() const {
    return fNumber;
}

void Airplane::setFNumber(const string &fNumber) {
    Airplane::fNumber = fNumber;
}

const string &Airplane::getFCallsign() const {
    return fCallsign;
}

void Airplane::setFCallsign(const string &fCallsign) {
    Airplane::fCallsign = fCallsign;
}

const string &Airplane::getFModel() const {
    return fModel;
}

void Airplane::setFModel(const string &fModel) {
    Airplane::fModel = fModel;
}

EPlaneStatus Airplane::getFStatus() const {
    return fStatus;
}

void Airplane::setFStatus(EPlaneStatus fStatus) {
    Airplane::fStatus = fStatus;
}

int Airplane::getFGateID() const {
    return Airplane::fGateID;
}

void Airplane::setFGateID(int id) {
    Airplane::fGateID = id;
}