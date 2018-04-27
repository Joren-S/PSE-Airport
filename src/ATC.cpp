//
// Created by Joren Servotte on 21.04.18.
//

#include <iostream>
#include "../headers/ATC.h"
#include "../headers/Airplane.h"
#include "../headers/Runway.h"

// Constructor and initialise-check

ATC::ATC(ostream& stream): fStream(stream) {
    fInitCheck = this;
    f3Occupied = false;
    f5Occupied = false;
}

bool ATC::properlyInitialized() const {
    return fInitCheck == this;
}

void ATC::setAirport(Airport *airport) {
    fAirport = airport;
}

void ATC::sendMessage(const string &message) {
    fStream << message << endl;
}

// Functions

int ATC::getQueueSize() const {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    int size = fQueue.size();
    ENSURE(size >= 0, "Queue has a negative size.");
    return size;
}

void ATC::sendRequest(Time time, Airplane *source) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
//    Time lastActive = getLastActive();
//    REQUIRE(time < lastActive, "Message can't be send at a time that has already passed.");

    ATCRequest *msg = new ATCRequest(time, source);

    getQueue()->push(msg);
    ENSURE(getQueue()->back() == msg, "Message wasn't queued properly.");

//    // first, check to see if there are messages waiting in the queue already:
//    if (getQueueSize() > 0) {
//        // if so: the current message gets queued in the back
//        getQueue()->push(msg);
//        ENSURE(getQueue()->back() == msg, "Message wasn't queued properly.");
//    }
//    else {
//        // if not, we can try and send the message right away.
//        // first, we check and see if we can send our message.
//        if (canSend(time)) {
//            // if so, we can send it right away and we change LastActive
//            fStream << msg->fMessage;
//            setLastActive(msg->fTime);
//            delete msg;
//        }
//        else {
//            // if not, we queue our message.
//            getQueue()->push(msg);
//            ENSURE(getQueue()->back() == msg, "Message wasn't queued properly.");
//        }
//    }
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

    // If not empty:
    // Get the message at the front of the queue and pop it.
    ATCRequest *msg = getQueue()->front();
    getQueue()->pop();

    // Make sure our message was retrieved and dequeued correctly.
    ENSURE(getQueue()->front() != msg, "Message wasn't removed from the queue.");
    ENSURE(msg != NULL, "Message popped from queue is NULL.");

    // Return our message.
    return msg;
}


void ATC::doHeartbeat(Time curTime) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");

    // Fetch the next request in the queue
    ATCRequest* request = getNextRequest();

    // If there are none, return
    if (request == NULL) {
        return;
    }

    // Get the airplane and status of airplane
    Airplane* airplane = request->fAirplane;
    EPlaneStatus status = airplane->getStatus();

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
            airplane->setPosition(airplane->getRunway()->getTaxiPoint());

            // Runway closest to the gates
            if (airplane->getPosition() == fAirport->getRunways()[0]->getTaxiPoint()) {

                // Get free gate
                int gate = fAirport->getFreeGate();
                ostringstream stream;
                stream << gate;

                // Set up message
                string message = airplane->getCallsign() + ", taxi tot gate " + stream.str()
                                 + " via " + airplane->getPosition() + ".";

                // Send message to plane
                fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;

            }

            else {
                // Get the next runway
                Runway* runway = fAirport->getNextRunway(airplane);

                // Set up message
                string message = airplane->getCallsign() + ", taxi to holding point " +
                                 runway->getName() + " via " + airplane->getPosition() + ".";

                // Send message to plane
                fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;
            }
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

                    message = airplane->getCallsign() + ", cleared to cross " +
                              runway->getName() + " taxi to gate " + stream.str()
                              + " via " + runway->getTaxiPoint() + ", " + airplane->getCallsign() + ".";
                }

                else {
                    message = airplane->getCallsign() + ", cleared to cross " +
                              runway->getName() + " taxi to holding point " + runway->getName()
                              + " via " + airplane->getPosition() + ", " + airplane->getCallsign();
                }

                // Send message to plane
                fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;
            }

            // Runway is not free, set request to denied
            else {
                airplane->setRequest(kDenied);
            }
        }
    }

    // Let the plane wait a minute, message has to be sent
    airplane->setTimeRemaining(1);
}


// Getters and setters
queue<ATCRequest *> *ATC::getQueue() {
    return &fQueue;
}

void ATC::setLastActive(Time time) {
    fLastActive = time;
}

Time ATC::getLastActive() const {
    return fLastActive;
}

Airport* ATC::getAirport() const {
    return fAirport;
}

