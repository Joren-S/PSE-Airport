//
// Created by uauser on 3/1/18.
//

#include "../headers/Airplane.h"

Airplane::Airplane(const string &number, const string &callsign, const string &model, EPlaneStatus status,
                   int passengers): fNumber(number),
                                    fCallsign(callsign),
                                    fModel(model),
                                    fStatus(status),
                                    fPassengers(passengers) {
    // Set altitude
    if (status == kGate or status == kLanded) {
        fAltitude = 0;
    }
    else {
        fAltitude = 10;
    }

    // Set to no gate, is changed when added to the system
    fGateID = -1;
}

Airplane::Airplane() {
    fPassengers = fGateID = fAltitude = -1;
    fStatus = kApproaching;
}

void Airplane::decreaseAltitude(int difference) {
    string errmsg = "New altitude can't be less than 0!";
    //REQUIRE(fAltitude - difference >= 0, errmsg.c_str());
    //int oldAltitude = Airplane::fAltitude;
    Airplane::fAltitude -= difference;
    errmsg = "Altitude hasn't been decreased correctly.";
    //ENSURE(fAltitude == oldAltitude - difference, errmsg.c_str());
}

void Airplane::increaseAltitude(int difference) {
    //int oldAltitude = Airplane::fAltitude;
    Airplane::fAltitude += difference;
    string errmsg = "Altitude hasn't been increased correctly.";
    //ENSURE(fAltitude == oldAltitude + difference, errmsg.c_str());
}

// Getters and setters

int Airplane::getAltitude() const {
    return fAltitude;
}

void Airplane::setAltitude(int altitude) {
    fAltitude = altitude;
}

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