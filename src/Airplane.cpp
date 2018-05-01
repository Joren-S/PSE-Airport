//============================================================================
// Name        : Airplane.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/Airplane.h"

using namespace std;

Airplane::Airplane() {
    fPassengers = -1;
    fAltitude = 0;
    fTimeRemaining = 0;
    fRunway = NULL;
    fRequest = kIdle;
    fGateID = -1;
    fStatus = kAway;
    fEngine = kDefaultEngine;
    fType = kDefaultType;
    fSize = kDefaultSize;

    fInitCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

bool Airplane::properlyInitialized() const {
    return this == fInitCheck;
}

bool Airplane::complete() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane::complete");
    return !(fEngine == kDefaultEngine or fType == kDefaultType
             or fSize == kDefaultSize or fPassengers == -1 or
            fModel.empty() or fCallsign.empty() or fNumber.empty());
}

void Airplane::decreaseAltitude(int difference) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling decreaseAltitude");
    REQUIRE(difference > 0, "Difference can't be negative");
    REQUIRE(fAltitude - difference >= 0, "New altitude can't be less than 0!");
    int oldAltitude = Airplane::fAltitude;
    Airplane::fAltitude -= difference;
    ENSURE(fAltitude == oldAltitude - difference, "Altitude hasn't been decreased correctly.");
}

void Airplane::increaseAltitude(int difference) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling increaseAltitude");
    REQUIRE(difference > 0, "Difference can't be negative");
    int oldAltitude = Airplane::fAltitude;
    Airplane::fAltitude += difference;
    ENSURE(fAltitude == oldAltitude + difference, "Altitude hasn't been increased correctly.");
}

// Getters and setters

EPlaneSize Airplane::getSize() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fSize;
}

void Airplane::setSize(EPlaneSize size) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    fSize = size;
}

EPlaneType Airplane::getType() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fType;
}

void Airplane::setType(EPlaneType type) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    fType = type;
}

EPlaneEngine Airplane::getEngine() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fEngine;
}

void Airplane::setEngine(EPlaneEngine engine) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    fEngine = engine;
}

int Airplane::getAltitude() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fAltitude;
}

void Airplane::setAltitude(int altitude) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    fAltitude = altitude;
}

int Airplane::getPassengers() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fPassengers;
}

void Airplane::setPassengers(int fPassengers) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    Airplane::fPassengers = fPassengers;
}

const string &Airplane::getNumber() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fNumber;
}

void Airplane::setNumber(const string &fNumber) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    Airplane::fNumber = fNumber;
}

const string &Airplane::getCallsign() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fCallsign;
}

void Airplane::setCallsign(const string &fCallsign) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    Airplane::fCallsign = fCallsign;
}

const string &Airplane::getModel() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fModel;
}

void Airplane::setModel(const string &fModel) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    Airplane::fModel = fModel;
}

EPlaneStatus Airplane::getStatus() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fStatus;
}

void Airplane::setStatus(EPlaneStatus fStatus) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    Airplane::fStatus = fStatus;
}

int Airplane::getGateID() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return Airplane::fGateID;
}

void Airplane::setGateID(int id) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    Airplane::fGateID = id;
}

int Airplane::getTimeRemaining() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fTimeRemaining;
}

void Airplane::setTimeRemaining(int time) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    fTimeRemaining = time;
}

void Airplane::decreaseTimeRemaining() {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling decreaseTimeRemaining");
    if (fTimeRemaining > 0) {
        fTimeRemaining--;
    }
}

const string& Airplane::getPosition() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fPosition;
}

void Airplane::setPosition(const string &position) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    fPosition = position;
}

EPlaneRequest Airplane::getRequest() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fRequest;
}

void Airplane::setRequest(EPlaneRequest request) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    fRequest = request;
}

Runway* Airplane::getRunway() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    return fRunway;
}

void Airplane::setRunway(Runway* runway) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling Airplane getter/setter");
    fRunway = runway;
}