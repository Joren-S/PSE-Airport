//============================================================================
// Name        : Airplane.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================
#include "../headers/Airplane.h"

using namespace std;

Airplane::Airplane() {
    fPassengers = fGateID;
    fAltitude = 0;
    fTimeRemaining = 0;
    fRunway = NULL;
    fRequest = kIdle;
    fGateID = -1;
    fStatus = kAway;
    fEngine = kDefaultEngine;
    fType = kDefaultType;
    fSize = kDefaultSize;
}

bool Airplane::complete() const {
    return !(fEngine == kDefaultEngine or fType == kDefaultType
             or fSize == kDefaultSize or fPassengers == -1 or
            fModel.empty() or fCallsign.empty() or fNumber.empty());
}

void Airplane::decreaseAltitude(int difference) {
    string errmsg = "New altitude can't be less than 0!";
    REQUIRE(fAltitude - difference >= 0, errmsg.c_str());
    int oldAltitude = Airplane::fAltitude;
    Airplane::fAltitude -= difference;
    errmsg = "Altitude hasn't been decreased correctly.";
    ENSURE(fAltitude == oldAltitude - difference, errmsg.c_str());
}

void Airplane::increaseAltitude(int difference) {
    int oldAltitude = Airplane::fAltitude;
    Airplane::fAltitude += difference;
    string errmsg = "Altitude hasn't been increased correctly.";
    ENSURE(fAltitude == oldAltitude + difference, errmsg.c_str());
}

// Getters and setters

EPlaneSize Airplane::getSize() const {
    return fSize;
}

void Airplane::setSize(EPlaneSize size) {
    fSize = size;
}

EPlaneType Airplane::getType() const {
    return fType;
}

void Airplane::setType(EPlaneType type) {
    fType = type;
}

EPlaneEngine Airplane::getEngine() const {
    return fEngine;
}

void Airplane::setEngine(EPlaneEngine engine) {
    fEngine = engine;
}

int Airplane::getAltitude() const {
    return fAltitude;
}

void Airplane::setAltitude(int altitude) {
    fAltitude = altitude;
}

int Airplane::getPassengers() const {
    return fPassengers;
}

void Airplane::setPassengers(int fPassengers) {
    Airplane::fPassengers = fPassengers;
}

const string &Airplane::getNumber() const {
    return fNumber;
}

void Airplane::setNumber(const string &fNumber) {
    Airplane::fNumber = fNumber;
}

const string &Airplane::getCallsign() const {
    return fCallsign;
}

void Airplane::setCallsign(const string &fCallsign) {
    Airplane::fCallsign = fCallsign;
}

const string &Airplane::getModel() const {
    return fModel;
}

void Airplane::setModel(const string &fModel) {
    Airplane::fModel = fModel;
}

EPlaneStatus Airplane::getStatus() const {
    return fStatus;
}

void Airplane::setStatus(EPlaneStatus fStatus) {
    Airplane::fStatus = fStatus;
}

int Airplane::getGateID() const {
    return Airplane::fGateID;
}

void Airplane::setGateID(int id) {
    Airplane::fGateID = id;
}

int Airplane::getTimeRemaining() const {
    return fTimeRemaining;
}

void Airplane::setTimeRemaining(int time) {
    fTimeRemaining = time;
}

void Airplane::decreaseTimeRemaining() {
    if (fTimeRemaining == 0) {
        return;
    }
    fTimeRemaining--;
}

const string& Airplane::getPosition() const {
    return fPosition;
}

void Airplane::setPosition(const string &position) {
    fPosition = position;
}

EPlaneRequest Airplane::getRequest() const {
    return fRequest;
}

void Airplane::setRequest(EPlaneRequest request) {
    fRequest = request;
}

Runway* Airplane::getRunway() const {
    return fRunway;
}

void Airplane::setRunway(Runway* runway) {
    fRunway = runway;
}