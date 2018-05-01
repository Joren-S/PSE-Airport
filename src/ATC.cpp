//============================================================================
// Name        : ATC.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include <iostream>
#include "../headers/ATC.h"
#include "../headers/Airplane.h"
#include "../headers/Runway.h"

// Constructor and initialise-check

ATC::ATC(ostream& stream): fStream(stream) {
    fInitCheck = this;
    f3Occupied = false;
    f5Occupied = false;
    ENSURE(properlyInitialized(), "ATC was not properly initialized.");
}

bool ATC::properlyInitialized() const {
    return fInitCheck == this;
}


// Functions

void ATC::sendMessage(const string &message) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    fStream << message << endl;
}

int ATC::getQueueSize() const {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    int size = fQueue.size();
    ENSURE(size >= 0, "Queue has a negative size.");
    return size;
}

void ATC::sendRequest(Time time, Airplane *source) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    REQUIRE(source != NULL, "Source is NULL.");
    ATCRequest *rqst = new ATCRequest(time, source);

    getQueue()->push(rqst);
    ENSURE(getQueue()->back() == rqst, "Request wasn't queued properly.");
}

string ATC::formatMessage(Time time, string source, string message) {
    ostringstream result;

    // Append the time and source
    result << "[" << time.formatted() << "]";
    result << "[" << source << "]" << endl;

    // Append the message
    result << "$ " << message;

    // return as string
    return result.str();
}

ATCRequest *ATC::getNextRequest() {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");

    // If queue is empty, return NULL.
    if (getQueueSize() == 0) {
        return NULL;
    }

    // If not empty, get the message at the front of the queue and pop it.
    ATCRequest *rqst = getQueue()->front();
    getQueue()->pop();

    // Make sure our message was retrieved and popped correctly.
    ENSURE(getQueue()->front() != rqst, "Message wasn't removed from the queue.");
    ENSURE(rqst != NULL, "Request popped from queue is NULL.");

    // Return our message.
    return rqst;
}


void ATC::doHeartbeat(Time curTime) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");

    // Fetch the next request in the queue
    ATCRequest* request = getNextRequest();

    // If there are none, return
    if (request == NULL) {
        return;
    }

    // Get the airplane and status of airplane that made the request
    Airplane* airplane = request->fAirplane;
    EPlaneStatus status = airplane->getStatus();

    // ---- LANDING ----

    // If plane is approaching, always accept request
    if (status == kApproaching) {

        // Change request status
        airplane->setRequest(kAccepted);

        // Send message to plane
        string message = airplane->getCallsign() + ", radar contact, descend and maintain five thousand feet.";
        fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;

    }

    // Plane is descending, process request
    else if (status == kDescending) {
        if (airplane->getAltitude() == 5) {

            // A plane is already circling at 3000ft
            if (f3Occupied) {

                // Change request status
                airplane->setRequest(kDenied);

                REQUIRE(!f5Occupied, "Exceeded capacity");

                f5Occupied = true;

                // Get clearance time
                Time clearance = curTime;
                clearance.advance(4);

                // Send message to plane
                string message = airplane->getCallsign() + ", hold south on the one eighty radial, expect further clearance at " + clearance.formatted() + ".";
                fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;
            }

            else {
                // Change request status
                airplane->setRequest(kAccepted);

                // Send message to plane
                string message = airplane->getCallsign() + ", descend and maintain three thousand feet.";
                fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;
            }
        }

        else if (airplane->getAltitude() == 3) {

            // Get a free runway
            Runway* runway = getAirport()->getFreeRunway(airplane);

            // If no available
            if (runway == NULL) {

                // Change request status
                airplane->setRequest(kDenied);

                REQUIRE(!f3Occupied, "Exceeded capacity");

                f3Occupied = true;

                // Get clearance time
                Time clearance = curTime;
                clearance.advance(4);

                // Send message to plane
                string message = airplane->getCallsign() + ", hold south on the one eighty radial, expect further clearance at " + clearance.formatted() + ".";
                fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;
            }

            // Runway available
            else {

                // Change request status
                airplane->setRequest(kAccepted);

                // Set airplane runway
                airplane->setRunway(runway);

                // Reset position in airport
                airplane->setPosition("");

                // Change status of runway
                runway->setFree(false);

                // Send message to plane
                string message = airplane->getCallsign() + ", cleared ILS approach runway " + runway->getName() + ".";
                fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;
            }
        }
    }

    // Plane has landed and is taxiing
    else if (status == kTaxiArrival) {

        // Plane position is not set yet, so it just arrived
        if (airplane->getPosition().empty()) {

            // Change the position of the plane to the taxipoint of the runway
            // airplane->setPosition(airplane->getRunway()->getTaxiPoint());

            // Runway closest to the gates
            if (airplane->getRunway() == fAirport->getRunways()[0]) {

                // Get free gate
                int gate = fAirport->getFreeGate();
                ostringstream stream;
                stream << gate;

                airplane->setGateID(gate);

                // Set up message
                string message = airplane->getCallsign() + ", taxi to gate " + stream.str()
                                 + " via " + airplane->getRunway()->getTaxiPoint() + ".";

                // Send message to plane
                fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;

            }

            else {
                // Runway* runway = NULL;
                // if ()
                // Get the next runway by temporarily setting the position of the plane
                airplane->setPosition(airplane->getRunway()->getTaxiPoint());
                Runway* runway = fAirport->getNextRunway(airplane);
                airplane->setPosition("");

                // Set up message
                string message = airplane->getCallsign() + ", taxi to holding point " +
                                 runway->getName() + " via " + airplane->getRunway()->getTaxiPoint() + ".";

                // Send message to plane
                fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;
            }

            airplane->setRequest(kAccepted);
        }

        else {
            // Get runway to cross
            Runway* runway = fAirport->getNextRunway(airplane);

            // If it's free, accept request
            if (runway->isFree()) {
                string message;


                // Set up message
                if (airplane->getPosition() == fAirport->getRunways()[1]->getTaxiPoint()) {

                    // Get free gate
                    int gate = fAirport->getFreeGate();
                    ostringstream stream;
                    stream << gate;

                    airplane->setGateID(gate);

                    message = airplane->getCallsign() + ", cleared to cross " +
                              runway->getName() + " taxi to gate " + stream.str()
                              + " via " + runway->getTaxiPoint() + ", " + airplane->getCallsign() + ".";
                }

                else {
                    string position = airplane->getPosition();
                    airplane->setPosition(runway->getTaxiPoint());
                    Runway* nextRunway = fAirport->getNextRunway(airplane);
                    airplane->setPosition(position);


                    message = airplane->getCallsign() + ", cleared to cross " +
                              runway->getName() + ", taxi to holding point " + nextRunway->getName()
                              + " via " + runway->getTaxiPoint() + ", " + airplane->getCallsign();
                }

                // Send message to plane
                fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;

                airplane->setRequest(kAccepted);
            }

            // Runway is not free, set request to denied
            else {
                airplane->setRequest(kDenied);
            }
        }
    }



    // ---- TAKE-OFF ----

    // First stage : Not refueled and not boarded.
    else if (status == kAirport) {

        // Requesting IFR clearance.
        Runway *dest = getAirport()->getFreeRunway(airplane);
        if (dest == NULL) {

            // IFR clearance denied
            airplane->setRunway(NULL);
            airplane->setRequest(kDenied);
        } else {

            // IFR clearance granted.
            airplane->setRunway(dest);
            airplane->setRequest(kAccepted);
            sendMessage(formatMessage(curTime, getAirport()->getCallsign(), airplane->getCallsign() + ", " + getAirport()->getCallsign() + ", cleared to " + dest->getName() +
                                                      ", maintain five thousand, expect flight level one zero zero - ten minutes after departure, squawk " + airplane->getSquawk() + "."));
        }
    }

    // Plane was refueled and boared.
    else if (status == kGate) {

        // Requesting pushback
        airplane->setRequest(kAccepted);
        sendMessage(formatMessage(curTime, getAirport()->getCallsign(), airplane->getCallsign() + ", " + getAirport()->getCallsign() + ", pushback approved."));
    }

    // Plane was pushed back.
    else if (status == kPushback) {

        // Requesting permission to taxi
        REQUIRE(getAirport()->getRunways().size() >= 1, "No runways in airport.");
        Runway *firstRW = getAirport()->getRunways().at(0);
        sendMessage(formatMessage(curTime, getAirport()->getCallsign(), airplane->getCallsign() + ", taxi to holding point " + firstRW->getName() + " via " + firstRW->getTaxiPoint() + "."));
        airplane->setRequest(kAccepted);
    }

    // Plane started taxiing.
    else if (status == kTaxiDeparture) {

        // Requesting taxi instructions
        Runway *rw = airplane->getRunway();

        // if at destination -> go to runway
        if (airplane->getPosition() == rw->getTaxiPoint()) {
            sendMessage(formatMessage(curTime, getAirport()->getCallsign(), airplane->getCallsign() + ", taxi to runway " + rw->getName() + " via " + rw->getTaxiPoint() + "."));
            airplane->setRequest(kDenied);
        }

        // if not at destination
        else {

            // if runway is free, plane can cross
            if (rw->isFree()) {
                sendMessage(formatMessage(curTime, getAirport()->getCallsign(), airplane->getCallsign() + ", cleared to cross " + rw->getName() + "."));
                airplane->setRequest(kAccepted);
            }

            //if not, plane has to wait
            else {
                sendMessage(formatMessage(curTime, getAirport()->getCallsign(), airplane->getCallsign() + ", hold position."));
                airplane->setRequest(kDenied);
            }
        }
    }

    // Plane is waiting AT runway
    else if (status == kWaitingForDeparture) {

        // Requesting permission to take-off.
        Runway *curRW = airplane->getRunway();
        if (curRW->isFree()) {
            if (f3Occupied == false) {

                // Permission to line-up and take-off
                sendMessage(formatMessage(curTime, getAirport()->getCallsign(), airplane->getCallsign() + ", runway " + curRW->getName() + " cleared for take-off."));
                airplane->setRequest(kAcceptedImmediate);
            } else {

                // Permission to line-up
                sendMessage(formatMessage(curTime, getAirport()->getCallsign(), airplane->getCallsign() + ", line-up runway " + curRW->getName() + " and wait."));
                airplane->setRequest(kAccepted);
            }
        } else {

            // Permission denied, keep waiting.
            sendMessage(formatMessage(curTime, getAirport()->getCallsign(), airplane->getCallsign() + ", hold position."));
            airplane->setRequest(kDenied);
        }
    }

    // Plane is waiting ON runway
    else if (status == kDeparture) {

        // Requesting permission to start taking off
        if (f3Occupied == false) {

            // Permission granted, start take-off
            airplane->setRequest(kAccepted);
            string runwayName = getAirport()->getRunway(airplane->getPosition())->getName();
            sendMessage(formatMessage(curTime, getAirport()->getCallsign(),
                                      airplane->getCallsign() + ", runway " + runwayName + " cleared for take-off."));
        }
        else {

            // Permission denied.
            sendMessage(formatMessage(curTime, getAirport()->getCallsign(), airplane->getCallsign() + ", hold position."));
            airplane->setRequest(kDenied);
        }
    }

    // Let the plane wait a minute, message has to be sent
    airplane->setTimeRemaining(1);
}


// Getters and setters
queue<ATCRequest *> *ATC::getQueue() {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    return &fQueue;
}

void ATC::setLastActive(Time time) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    fLastActive = time;
    ENSURE(fLastActive == time, "Field wasn't set properly");
}

Time ATC::getLastActive() const {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    return fLastActive;
}

Airport* ATC::getAirport() const {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    return fAirport;
}

void ATC::setAirport(Airport *airport) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    fAirport = airport;
    ENSURE(fAirport == airport, "Field wasn't set properly");
}

bool ATC::get3occupied() const {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    return f3Occupied;
}

void ATC::set3occupied(bool occupied) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    f3Occupied = occupied;
    ENSURE(f3Occupied == occupied, "Field wasn't set properly");
}

bool ATC::get5occupied() const {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    return f5Occupied;
}

void ATC::set5occupied(bool occupied) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    f5Occupied = occupied;
    ENSURE(f5Occupied == occupied, "Field wasn't set properly");
}