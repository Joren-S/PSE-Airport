//
// Created by Joren Servotte on 21.04.18.
//

#include "../headers/ATC.h"

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

void ATC::sendMessage(Time time, string source, string message) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");
    Time lastActive = getLastActive();
    REQUIRE(time < lastActive, "Message can't be send at a time that has already passed.");

    string msgFormatted = formatMessage(time, source, message);
    ATCMessage *msg = new ATCMessage(time, msgFormatted);

    // first, check to see if there are messages waiting in the queue already:
    if (getQueueSize() > 0) {
        // if so: the current message gets queued in the back
        getQueue().push(msg);
        ENSURE(getQueue().back() == msg, "Message wasn't queued properly.");
    }
    else {
        // if not, we can try and send the message right away.
        // first, we check and see if we can send our message.
        if (canSend(time)) {
            // if so, we can send it right away and we change LastActive
            delete msg;
            fStream << msgFormatted;
            setLastActive(time);
        }
        else {
            // if not, we queue our message.
            getQueue().push(msg);
            ENSURE(getQueue().back() == msg, "Message wasn't queued properly.");
        }
    }
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

ATCMessage *ATC::getNextQueuedMessage() const {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");

    // If queue is empty, return NULL.
    if (getQueueSize() == 0) {
        return NULL;
    }

    // If not empty:
    // Get the message at the front of the queue and pop it.
    ATCMessage *msg = getQueue().front();
    getQueue().pop();

    // Make sure our message was retrieved and dequeued correctly.
    ENSURE(getQueue().front() != msg, "Message wasn't removed from the queue.");
    ENSURE(msg != NULL, "Message popped from queue is NULL.");

    // Return our message.
    return msg;
}

bool ATC::canSend(Time time) const {
    return getLastActive() < time;
}

void ATC::doHeartbeat(Time curTime) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized.");

    // If the ATC can send a message
    if (canSend(curTime)) {

        // Get the next queued message (and dequeue)
        ATCMessage *msg = getNextQueuedMessage();

        // If this message exists
        if (msg != NULL) {

            // send and delete object.
            fStream << msg->fMessage;
            delete msg;
        }
    }
}


// Getters and setters
queue<ATCMessage *> ATC::getQueue() const {
    return fQueue;
}

void ATC::setLastActive(Time time) {
    fLastActive = time;
}

Time ATC::getLastActive() const {
    return fLastActive;
}

