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

    switch (status) {
        case kApproaching:
            REQUIRE(!f5Occupied, "Airport capacity exceeded, 50000ft already occupied");
            fStream << "";
            airplane->setRequest(kAccepted);
            break;
        case kDescending:
            fStream << "";
            if (altitude == 5) {
                airplane->setRequest(f3Occupied? kDenied: kAccepted);
            }

            if (altitude == 3) {
                Runway* runway = getAirport()->getFreeRunway(airplane);
                if (runway == NULL) {
                    airplane->setRequest(kDenied);
                }
                else {
                    airplane->setRequest(kAccepted);
                    airplane->setPosition(runway->getTaxiPoint());
                    runway->setFree(false);
                }
            }
            break;
        case kTaxiArrival:
            // get next runway towards gate
            // go to taxipoint associated with runway

            break;
        case kTaxiDeparture:
            // get next runway towards destination runway
            // go to taxipoint associated with runway
            //
            break;
        case kCrossing:
            Runway* runway = getAirport()->getRunway(position);
            if (runway->isFree()) {
                airplane->setRequest(kAccepted);
                runway->setFree(false);
            }
            break;
        case kGate: case kDeparture: case kAway: case kAirport:
            break;
    }

    //  kApproaching, kDescending, kTaxiArrival, kTaxiDeparture, kGate, kDeparture, kAway, kAirport, kCrossing };



//    // If the ATC can send a message
//    if (canSend(curTime)) {
//
//        // Get the next queued message (and dequeue)
//        ATCMessage *msg = getNextQueuedMessage();
//
//        // If this message exists
//        if (msg != NULL) {
//
//            // send and delete object.
//            fStream << msg->fMessage;
//            setLastActive(msg->fTime);
//            delete msg;
//        }
//    }
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

