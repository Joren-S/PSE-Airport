//
// Created by Joren Servotte on 21.04.18.
//

#include "../headers/ATC.h"
#include "../headers/Airplane.h"
#include "../headers/Runway.h"

// Constructor and initialise-check

ATC::ATC(ostream& stream): fStream(stream) {
    fInitCheck = this;
}

bool ATC::properlyInitialized() const {
    return fInitCheck == this;
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
    Time lastActive = getLastActive();
    REQUIRE(time < lastActive, "Message can't be send at a time that has already passed.");

    ATCRequest *msg = new ATCRequest(time, source);


    getQueue()->push(msg);
    ENSURE(getQueue()->back() == msg, "Message wasn't queued properly.");

    source->setRequest(kPending);
    ENSURE(source->getRequest() == kPending, "Plane status wasn't set properly.");

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

string ATC::formatMessage(Time time, string source, string message) const {
    ostringstream result;

    // Append the time and source
    result << "[" << time.formatted() << "]";
    result << "[" << source << "]" << endl;

    // Append the message
    result << "$ " << message << endl;

    // return as string
    return result.str();
}

ATCRequest *ATC::getNextRequest() const {
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

    ATCRequest* request = getNextRequest();

    if (request == NULL) {
        return;
    }

    Airplane* airplane = request->fAirplane;

    EPlaneStatus status = airplane->getStatus();

    int altitude = airplane->getAltitude();

    string position = airplane->getPosition();


    REQUIRE(airplane->getRequest() == kPending, "Request has to be pending");
    REQUIRE(airplane->getTimeRemaining() == 0, "Plane is already busy with operation, cannot process request.");

    string planeCS = airplane->getCallsign();
    string portCS = getAirport()->getCallsign();

    switch (status) {

        // cases for takeoff

        case kAirport:
            // IFR clearancy for takeoff: accepted if runway was assigned
            Runway *dest = getAirport()->getFreeRunway(airplane);
            if (dest == NULL) {
                airplane->setDestination("");
                airplane->setRequest(kDenied);
            }
            else {
                airplane->setDestination(dest->getTaxiPoint());
                airplane->setRequest(kAccepted);
                sendMessage(formatMessage(curTime, "ATC", planeCS + ", " + portCS + ", cleared to " + dest->getName() + ", maintain five thousand, expect flight level one zero zero - ten minutes after departure, squawk " + getSquawk() + "."));
            }
            break;

        case kGate:
            // takeoff: request to push back: always accepted
            airplane->setRequest(kAccepted);
            sendMessage( formatMessage(curTime, "ATC", planeCS + ", " + portCS + ", pushback approved.") );
            break;


        case kPushback:
            // permission to start taxiing
            Runway *next_rw = getAirport()->getNextRunway(airplane);
            sendMessage( formatMessage(curTime, "ATC", planeCS + ", taxi to holding point " + next_rw->getName() + " via " + next_rw->getTaxiPoint() + "." ) );
            airplane->setRequest(kAccepted);
            break;

        case kTaxiDeparture:
            // taxi instructions
            Runway *runway = getAirport()->getNextRunway(airplane);

            // if at destination -> go to runway
            if (airplane->getPosition() == airplane->getDestination()) {
                sendMessage( formatMessage(curTime, "ATC", planeCS + ", taxi to runway " + next_rw->getName() + " via " + next_rw->getTaxiPoint() + "." ) );
            }

            // if not
            else {
                // if runway is free, plane can cross
                if (runway->isFree()) {
                    sendMessage(formatMessage(curTime, "ATC", planeCS + ", cleared to cross " + runway->getName() + "."));
                    airplane->setRequest(kAccepted);
                }
                //if not, plane has to wait
                else {
                    sendMessage(formatMessage(curTime, "ATC", planeCS + ", hold position."));
                    airplane->setRequest(kDenied);
                }
            }
            break;

        case kWaitingForDeparture:
            // waiting at runway
            // 3 options: wait, takeoff, go on runway
            Runway *curRW = getAirport()->getRunway(airplane->getDestination());
            if (curRW->isFree()) {
                if (f3Occupied == false) {
                    sendMessage(formatMessage(curTime, "ATC", planeCS + ", runway " + curRW->getName() + " cleared for take-off."));
                    airplane->setRequest(kAcceptedImmediate);
                }
                else {
                    sendMessage(formatMessage(curTime, "ATC", planeCS + ", line-up runway " + curRW->getName() + " and wait."));
                    airplane->setRequest(kAccepted);
                }
            }
            else {
                sendMessage(formatMessage(curTime, "ATC", planeCS + ", hold position."));
                airplane->setRequest(kDenied);
            }
            break;

        case kDeparture:
            // waiting on runway, needing permission to start taking off
            airplane->setRequest(kAccepted);
            string runwayName = getAirport()->getRunway(airplane->getPosition())->getName();
            sendMessage( formatMessage(curTime, "ATC", planeCS + ", runway " + runwayName + " cleared for take-off.") );
            break;
    }

    // kAscending kDeparture kWaitingForDeparture kCrossing kTaxiDeparture kPushback

}


// Getters and setters
queue<ATCRequest *> *ATC::getQueue() const {
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

