//============================================================================
// Name        : ATC.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================


#include "../headers/ATC.h"

// Constructor and initialise-check

ATC::ATC(ostream& stream, bool test): fStream(stream) {
    fInitCheck = this;
    f3Occupied = false;
    f5Occupied = false;
    fTest = test;
    ENSURE(properlyInitialized(), "ATC was not properly initialized after constructing.");
}

bool ATC::properlyInitialized() const {
    return fInitCheck == this;
}


// Functions

void ATC::sendMessage(const string &message) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling sendMessage.");
    fStream << message << endl;
}

int ATC::getQueueSize() const {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling getQueueSize.");
    int size = fQueue.size();
    ENSURE(size >= 0, "Queue has a negative size.");
    return size;
}

void ATC::sendRequest(Time time, Airplane *source) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling sendRequest.");
    REQUIRE(source != NULL, "Source is NULL.");
    ATCRequest *rqst = new ATCRequest(time, source);

    getQueue()->push(rqst);
    ENSURE(getQueue()->back() == rqst, "Request wasn't queued properly.");
}

string ATC::formatMessage(Time time, string source, string message) {
    // Variables
    ostringstream result;
    ostringstream natoBuffer;

    // Setup nato map
    map<string, string> nato;
    nato.insert(make_pair("0", "zero"));
    nato.insert(make_pair("1", "one"));
    nato.insert(make_pair("2", "two"));
    nato.insert(make_pair("3", "three"));
    nato.insert(make_pair("4", "four"));
    nato.insert(make_pair("5", "five"));
    nato.insert(make_pair("6", "six"));
    nato.insert(make_pair("7", "seven"));
    nato.insert(make_pair("8", "eight"));
    nato.insert(make_pair("9", "nine"));

    nato.insert(make_pair("a", "alfa"));
    nato.insert(make_pair("b", "bravo"));
    nato.insert(make_pair("c", "charlie"));
    nato.insert(make_pair("d", "delta"));
    nato.insert(make_pair("e", "echo"));
    nato.insert(make_pair("f", "foxtrot"));
    nato.insert(make_pair("g", "golf"));
    nato.insert(make_pair("h", "hotel"));
    nato.insert(make_pair("i", "india"));
    nato.insert(make_pair("j", "juliett"));
    nato.insert(make_pair("k", "kilo"));
    nato.insert(make_pair("l", "lima"));
    nato.insert(make_pair("m", "mike"));
    nato.insert(make_pair("n", "november"));
    nato.insert(make_pair("o", "oscar"));
    nato.insert(make_pair("p", "papa"));
    nato.insert(make_pair("q", "quebec"));
    nato.insert(make_pair("r", "romeo"));
    nato.insert(make_pair("s", "sierra"));
    nato.insert(make_pair("t", "tango"));
    nato.insert(make_pair("u", "uniform"));
    nato.insert(make_pair("v", "victor"));
    nato.insert(make_pair("w", "whiskey"));
    nato.insert(make_pair("x", "x-ray"));
    nato.insert(make_pair("y", "yankee"));
    nato.insert(make_pair("z", "zulu"));

    // Iterate over every character in the message
    for(string::iterator it = message.begin(); it != message.end(); ++it) {
        string curChar(1, *it);

        // if current char is a number
        if (isdigit(*it)) {

            // if number has a nato representation
            if(nato.find(curChar) != nato.end()) {

                // append nato to buffer
                string numAsNato = nato.find(curChar)->second;
                natoBuffer << numAsNato;

                // if not at end of message
                if ((it + 1) != message.end()) {
                    string nextChar(1, *(it + 1));

                    // check if next character is a space or special character.
                    // if not, we add a space
                    if (nextChar != " " and nextChar != "." and nextChar != ",") {
                        natoBuffer << " ";
                    }
                }

                // we continue since we don't want to append the number itself
                continue;
            }
        }

        else if (isalpha(*it)) {
            if (it == message.begin() or isdigit(*(it - 1)) or *(it - 1) == '.' or *(it - 1) == ' ' or *(it - 1) == ',') {
                if (it == message.end() - 1 or isdigit(*(it + 1)) or *(it + 1) == '.' or *(it + 1) == ' ' or *(it + 1) == ',') {
                    string currentChar (1, tolower(*it));
                    string letterAsNato = nato.find(currentChar)->second;
                    natoBuffer << letterAsNato;
                    continue;
                }
            }
        }
        natoBuffer << curChar;
    }

    // Append the time and source
    result << "[" << time.formatted() << "]";
    result << "[" << source << "]" << endl;

    // Append the message
    result << "$ " << natoBuffer.str() << endl;

    // convert to string and return
    return result.str();
}

ATCRequest *ATC::getNextRequest() {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling getNextRequest.");

    // If queue is empty, return NULL.
    if (getQueueSize() == 0) {
        return NULL;
    }

    // If not empty, get the message at the front of the queue and pop it.
    ATCRequest *rqst = getQueue()->front();
    getQueue()->pop();

    // Make sure our message was retrieved correctly.
    ENSURE(rqst != NULL, "Request popped from queue is NULL.");

    // Return our message.
    return rqst;
}

void ATC::doHeartbeat(Time curTime) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling doHeartbeat.");

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

    // Plane has requested an approach
    if (status == kApproaching) {
        processApproach(airplane, curTime);
    }

    // Plane has requested a descend/landing
    else if (status == kDescending) {
        processDescend(airplane, curTime);
    }

    // Plane has requested instructions for taxiing at arrival
    else if (status == kTaxiArrival) {
        processTaxiArrival(airplane, curTime);
    }



    // ---- TAKE-OFF ----

    // First stage : Not refueled and not boarded.
    else if (status == kAirport) {
        processIFRClearance(airplane, curTime);
    }

    // Plane was refueled and boared.
    else if (status == kGate) {
        processPushback(airplane, curTime);
    }

    // Plane was pushed back.
    else if (status == kPushback) {
        processTaxiInitialise(airplane, curTime);
    }

    // Plane started taxiing.
    else if (status == kTaxiDeparture) {
        processTaxiInstruction(airplane, curTime);
    }

    // Plane is waiting AT runway
    else if (status == kWaitingForDeparture) {
        processTakeOff(airplane, curTime);
    }

    // Plane is waiting ON runway
    else if (status == kDeparture) {
        processTakeOffRunway(airplane, curTime);
    }

    // Delete request
    delete request;

    // Let the plane wait a minute, message has to be sent
    airplane->setTimeRemaining(1);
}


int ATC::getSquawk(Airplane *airplane) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling getSquawk.");
    srand(time(NULL));

    if (fTest) {
        return 0;
    }

    // Keep going until a code is found
    while (true) {
        // Set squawk to -1
        int squawk = -1;

        // Get info
        EPlaneType type = airplane->getType();
        EPlaneEngine engine = airplane->getEngine();
        EPlaneSize size = airplane->getSize();

        if (type == kPrivate and size == kSmall) {
            squawk = (rand() % 777) + 1;
        } else if (type == kPrivate and size == kMedium and engine == kJet) {
            squawk = (rand() % 778) + 1000;
        } else if (type == kAirline and size == kMedium and engine == kPropeller) {
            squawk = (rand() % 778) + 2000;
        } else if (type == kAirline and size == kMedium and engine == kJet) {
            squawk = (rand() % 778) + 3000;
        } else if (type == kAirline and size == kLarge and engine == kJet) {
            squawk = (rand() % 778) + 4000;
        } else if (type == kMilitary and
                   ((size == kSmall and engine == kJet) or (size == kLarge and engine == kPropeller))) {
            squawk = (rand() % 778) + 5000;
        } else if (type == kEmergency and size == kSmall and engine == kPropeller) {
            squawk = (rand() % 778) + 6000;
        }

        // If the code is not yet in use, return it
        if (!fUsedCodes.count(squawk)) {
            // If the code is -1, there aren't any squawk codes available for the type of plane
            if (squawk != -1) {
                // Add it to the set of usedCodes
                fUsedCodes.insert(squawk);
            }
            return squawk;
        }
    }
}

// Getters and setters
queue<ATCRequest *> *ATC::getQueue() {
    REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
    return &fQueue;
}

void ATC::setLastActive(Time time) {
    REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
    fLastActive = time;
    ENSURE(fLastActive == time, "Field wasn't set properly");
}

Time ATC::getLastActive() const {
    REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
    return fLastActive;
}

Airport* ATC::getAirport() const {
    REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
    return fAirport;
}

void ATC::setAirport(Airport *airport) {
    REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
    fAirport = airport;
    ENSURE(fAirport == airport, "Field wasn't set properly");
}

bool ATC::get3occupied() const {
    REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
    return f3Occupied;
}

void ATC::set3occupied(bool occupied) {
    REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
    f3Occupied = occupied;
    ENSURE(f3Occupied == occupied, "Field wasn't set properly");
}

bool ATC::get5occupied() const {
    REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
    return f5Occupied;
}

void ATC::set5occupied(bool occupied) {
    REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
    f5Occupied = occupied;
    ENSURE(f5Occupied == occupied, "Field wasn't set properly");
}

void ATC::processApproach(Airplane *airplane, Time time) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processApproach.");
    // Change request status
    airplane->setRequest(kAccepted);

    // Squawk code for plane
    int squawk = getSquawk(airplane);
    airplane->setSquawk(squawk);

    // Convert to string
    stringstream stream;
    stream << squawk;

    // Send message to plane
    string message = airplane->getCallsign() + ", radar contact, descend and maintain five thousand feet, squawk " + stream.str() + ".";
    fStream << formatMessage(time, fAirport->getCallsign(), message) << endl;
}

void ATC::processDescend(Airplane *airplane, Time curTime) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processDescend.");
    if (airplane->getAltitude() == 5) {

        // A plane is already circling at 3000ft, 5000ft is clear
        if (f3Occupied and !f5Occupied) {

            // Change request status
            airplane->setRequest(kDenied);

            // Set this level occupied
            f5Occupied = true;

            // Get clearance time
            Time clearance = curTime;
            clearance.advance(4);

            // Send message to plane
            string message = airplane->getCallsign() + ", hold south on the one eighty radial, expect further clearance at " + clearance.formatted() + ".";
            fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;
        }

        // Accept request
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

void ATC::processTaxiArrival(Airplane *airplane, Time curTime) {
    REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processTaxiArrival.");
    // Plane position is not set yet, so it just arrived
    if (airplane->getPosition().empty()) {

        // Runway closest to the gates
        if (airplane->getRunway() == fAirport->getRunways()[0]) {

            // Get free gate
            int gate = fAirport->getFreeGate();
            ostringstream stream;
            stream << gate;

            // Set gate in airplane
            airplane->setGateID(gate);

            // Set up message
            string message = airplane->getCallsign() + ", taxi to gate " + stream.str()
                             + " via " + airplane->getRunway()->getTaxiPoint() + ".";

            // Send message to plane
            fStream << formatMessage(curTime, fAirport->getCallsign(), message) << endl;

        }

        else {
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

        // Accept request
        airplane->setRequest(kAccepted);
    }

    else {
        // Get runway to cross
        Runway* runway = fAirport->getNextRunway(airplane);

        // If it's free, accept request
        if (runway->isFree()) {
            // Declare message
            string message;

            // Airplane crosses last runway before gates
            if (airplane->getPosition() == fAirport->getRunways()[1]->getTaxiPoint()) {

                // Get free gate
                int gate = fAirport->getFreeGate();
                ostringstream stream;
                stream << gate;

                // Set gate in airplane
                airplane->setGateID(gate);

                message = airplane->getCallsign() + ", cleared to cross " +
                          runway->getName() + ", taxi to gate " + stream.str()
                          + " via " + runway->getTaxiPoint() + ", " + airplane->getCallsign() + ".";
            }

            // Runway is not last
            else {
                // Temporarily set plane position to get runway after the one it crosses
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

            // Accept request
            airplane->setRequest(kAccepted);
        }

        // Runway is not free, set request to denied
        else {
            airplane->setRequest(kDenied);
        }
    }
}

void ATC::processIFRClearance(Airplane* airplane, Time time) {
    // Requesting IFR clearance.
    Runway *dest = getAirport()->getFreeRunway(airplane);
    if (dest == NULL) {
        // IFR clearance denied
        airplane->setRunway(NULL);
        airplane->setRequest(kDenied);
        sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", no free runway, hold position."));
    } else {

        // IFR clearance granted.
        airplane->setRunway(dest);
        airplane->setRequest(kAccepted);
        airplane->setSquawk(getSquawk(airplane));
        stringstream sqwk;
        sqwk << airplane->getSquawk();
        sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", " + getAirport()->getCallsign() + ", cleared to " + dest->getName() +
                                                                        ", maintain five thousand, expect flight level one zero zero - ten minutes after departure, squawk " + sqwk.str() + "."));
    }
}

void ATC::processPushback(Airplane* airplane, Time time) {

    // Requesting pushback
    airplane->setRequest(kAccepted);
    sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", " + getAirport()->getCallsign() + ", pushback approved."));
}

void ATC::processTaxiInitialise(Airplane* airplane, Time time) {

    // Requesting permission to taxi
    Runway *firstRW = getAirport()->getRunways().at(0);
    sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", taxi to holding point " + firstRW->getName() + " via " + firstRW->getTaxiPoint() + "."));
    airplane->setRequest(kAccepted);
}

void ATC::processTaxiInstruction(Airplane* airplane, Time time) {
    // Requesting taxi instructions
    Runway *curRw = getAirport()->getRunway(airplane->getPosition());
    Runway *dest = airplane->getRunway();

    // if at destination -> go to runway
    if (dest->getTaxiPoint() == curRw->getTaxiPoint()) {
        sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", hold position."));
        airplane->setRequest(kConfirmed);
    }

        // if not at destination
    else {
        // if runway is free, plane can cross
        if (curRw->isFree()) {
            sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", cleared to cross " + curRw->getName() + "."));
            airplane->setRequest(kAccepted);
        }

            //if not, plane has to wait
        else {
            sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", hold position."));
            airplane->setRequest(kDenied);
        }
    }
}

void ATC::processTakeOff(Airplane* airplane, Time time) {
    // Requesting permission to take-off.
    Runway *curRW = airplane->getRunway();
    if (curRW->isFree()) {
        if (f3Occupied == false) {

            // Permission to line-up and take-off
            sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", runway " + curRW->getName() + " cleared for take-off."));
            airplane->setRequest(kAcceptedImmediate);
        } else {

            // Permission to line-up
            sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", line-up runway " + curRW->getName() + " and wait."));
            airplane->setRequest(kAccepted);
        }
    } else {

        // Permission denied, keep waiting.
        sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", hold position."));
        airplane->setRequest(kDenied);
    }
};

void ATC::processTakeOffRunway(Airplane* airplane, Time time) {
    // Requesting permission to start taking off
    if (f3Occupied == false) {

        // Permission granted, start take-off
        airplane->setRequest(kAccepted);
        string runwayName = getAirport()->getRunway(airplane->getPosition())->getName();
        sendMessage(formatMessage(time, getAirport()->getCallsign(),
                                  airplane->getCallsign() + ", runway " + runwayName + " cleared for take-off."));
    }
    else {

        // Permission denied.
        sendMessage(formatMessage(time, getAirport()->getCallsign(), airplane->getCallsign() + ", hold position."));
        airplane->setRequest(kDenied);
    }
}

ATC::~ATC() {
    while (!fQueue.empty()) {
        ATCRequest* request = fQueue.front();
        delete request;
        fQueue.pop();
    }
}