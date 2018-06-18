//============================================================================
// Name        : Airplane.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/Airplane.h"

using namespace std;

Airplane::Airplane() {
    fPassengers = -1;
    fFuel = -1;
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
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane::complete");
    return !(fEngine == kDefaultEngine or fType == kDefaultType or fFuel == -1
             or fSize == kDefaultSize or fPassengers == -1 or
            fModel.empty() or fCallsign.empty() or fNumber.empty());
}

void Airplane::decreaseAltitude(int difference) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling decreaseAltitude");
    REQUIRE(difference > 0, "Difference can't be negative");
    REQUIRE(fAltitude - difference >= 0, "New altitude can't be less than 0!");
    int oldAltitude = Airplane::fAltitude;
    Airplane::fAltitude -= difference;
    ENSURE(fAltitude == oldAltitude - difference, "Altitude hasn't been decreased correctly.");
}

void Airplane::increaseAltitude(int difference) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling increaseAltitude");
    REQUIRE(difference > 0, "Difference can't be negative");
    int oldAltitude = Airplane::fAltitude;
    Airplane::fAltitude += difference;
    ENSURE(fAltitude == oldAltitude + difference, "Altitude hasn't been increased correctly.");
}

// Getters and setters

EPlaneSize Airplane::getSize() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fSize;
}

void Airplane::setSize(EPlaneSize size) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    fSize = size;
}

EPlaneType Airplane::getType() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fType;
}

void Airplane::setType(EPlaneType type) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    fType = type;
}

EPlaneEngine Airplane::getEngine() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fEngine;
}

void Airplane::setEngine(EPlaneEngine engine) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    fEngine = engine;
}

int Airplane::getAltitude() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fAltitude;
}

void Airplane::setAltitude(int altitude) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    REQUIRE(altitude >= 0, "Altitude can't be negative");
    fAltitude = altitude;
}

int Airplane::getPassengers() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fPassengers;
}

void Airplane::setPassengers(int passengers) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    REQUIRE(passengers >= 0, "Passenger amount can't be negative");
    Airplane::fPassengers = passengers;
}

const string &Airplane::getNumber() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fNumber;
}

void Airplane::setNumber(const string &fNumber) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    Airplane::fNumber = fNumber;
}

const string &Airplane::getCallsign() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fCallsign;
}

void Airplane::setCallsign(const string &fCallsign) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    Airplane::fCallsign = fCallsign;
}

const string &Airplane::getModel() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fModel;
}

void Airplane::setModel(const string &fModel) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    Airplane::fModel = fModel;
}

EPlaneStatus Airplane::getStatus() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fStatus;
}

void Airplane::setStatus(EPlaneStatus fStatus) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    Airplane::fStatus = fStatus;
}

int Airplane::getGateID() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return Airplane::fGateID;
}

void Airplane::setGateID(int id) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    REQUIRE(id >= -1, "Gate id can't be less than -1");
    Airplane::fGateID = id;
}

int Airplane::getTimeRemaining() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fTimeRemaining;
}

void Airplane::setTimeRemaining(int time) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    REQUIRE(time >= 0, "Time remaining can't be negative");
    fTimeRemaining = time;
}

void Airplane::decreaseTimeRemaining() {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling decreaseTimeRemaining");
    if (fTimeRemaining > 0) {
        fTimeRemaining--;
    }
}

const string& Airplane::getPosition() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fPosition;
}

void Airplane::setPosition(const string &position) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    fPosition = position;
}

EPlaneRequest Airplane::getRequest() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fRequest;
}

void Airplane::setRequest(EPlaneRequest request) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    fRequest = request;
}

Runway* Airplane::getRunway() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fRunway;
}

void Airplane::setRunway(Runway* runway) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    fRunway = runway;
}

int Airplane::getFuel() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fFuel;
}

void Airplane::setFuel(int fuel) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    REQUIRE(fuel > 0, "Fuel can't be less than 1");
    fFuel = fuel;
}

void Airplane::setSquawk(int squawk) {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    REQUIRE(squawk >= 0, "Squawk code can't be negative");
    fSquawk = squawk;
}

int Airplane::getSquawk() const {
    REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
    return fSquawk;
}


#include "../headers/ATC.h"

void Airplane::approach(ostream& log, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling approach");

    // Request has been accepted by fATC
    if (getRequest() == kAccepted) {
        // Convert int to string
        stringstream stream;
        stream << getSquawk();

        // Send message to fATC
        string message = "Descend and maintain five thousand feet, squawking " + stream.str() + ", " + getCallsign() + ".";
        fATC->sendMessage(ATC::formatMessage(fATC->getTime(), getCallsign(), message));

        // Set the altitude of the plane
        setAltitude(10);

        // Set status to descending
        setStatus(kDescending);

        // Set time remaining for first descend
        setTimeRemaining(getEngine() == kJet? 1:2);

        // Log
        log << "[" << fATC->getTime().formatted() << "] " << getCallsign() << " is approaching " << fATC->getAirport()->getName() << " at " << getAltitude() << ".000ft." << endl;

        // Set request to idle
        setRequest(kIdle);
    }

        // Send a request if not yet done
    else if (getRequest() == kIdle) {
        // Send message to fATC
        string message = fATC->getAirport()->getCallsign() + ", " + getCallsign() + ", arriving at " + fATC->getAirport()->getName() + ".";
        fATC->sendMessage(ATC::formatMessage(fATC->getTime(), getCallsign(), message));

        // Send the request
        fATC->sendRequest(fATC->getTime(), this);

        // Set request to pending
        setRequest(kPending);
    }
}

void Airplane::descend(ostream& log, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling descend");
    // Plane has landed
    if (getAltitude() == 0) {
        // Log event
        log << "[" << fATC->getTime().formatted() << "] " << getCallsign() << " has landed at " << fATC->getAirport()->getName() << " on runway " << getRunway()->getName() << endl;

        // Set runway to available
        getRunway()->setFree(true);

        // Change status
        setStatus(kTaxiArrival);

        // Set request to idle
        setRequest(kIdle);

        return;
    }

    // Plane is descending
    if ((getAltitude() != 5 and getAltitude() != 3) or getRequest() == kConfirmed) {
        // Decrease the altitude
        decreaseAltitude();

        // If plane is at one of the key altitudes, send a request to atc
        if (getAltitude() == 5 or getAltitude() == 3) {
            fATC->sendRequest(fATC->getTime(), this);
        }

        // If altitude is 0, the plane has started landing
        if (getAltitude() == 0) {
            log << "[" << fATC->getTime().formatted() << "] " << getCallsign() << " is landing at " << fATC->getAirport()->getName() << " on runway " << getRunway()->getName() << endl;
            setTimeRemaining(2);
        }

            // Normal descend
        else {
            log << "[" << fATC->getTime().formatted() << "] " << getCallsign() << " descended to " << getAltitude() << ".000ft." << endl;
            setTimeRemaining(getEngine() == kJet ? 1 : 2);
        }

        return;
    }

    // Request has been accepted by ATC
    if (getRequest() == kAccepted) {
        // Pick the right message for the altitude
        string message;
        if (getAltitude() == 5) message = "Descend and maintain three thousand feet, " + getCallsign() + ".";
        else message = "Cleared ILS approach runway " + getRunway()->getName() + ", " + getCallsign() + ".";

        // Send message to fATC
        fATC->sendMessage(ATC::formatMessage(fATC->getTime(), getCallsign(), message));

        // Set time remaining for descend
        setTimeRemaining(getEngine() == kJet? 1:2);

        // Set status to confirmed
        setRequest(kConfirmed);
    }

    // Request has been denied by ATC
    if (getRequest() == kDenied) {
        // Send message to ATC
        string message = "Holding south on the one eighty radial, " + getCallsign();
        fATC->sendMessage(ATC::formatMessage(fATC->getTime(), getCallsign(), message));

        // Set time remaining
        setTimeRemaining(3);

        // Set status to circling
        setStatus(kCircling);
    }
}

void Airplane::circle(ostream& log, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling circle");
    // Log event
    log << "[" << fATC->getTime().formatted() << "] " << getCallsign() << " has circled at " << getAltitude() << ".000ft." << endl;

    // Make the altitude available again for circling
    if (getAltitude() == 3) {
        fATC->set3occupied(false);
    }
    else {
        fATC->set3occupied(false);
    }

    // Send a new request to descend
    fATC->sendRequest(fATC->getTime(), this);

    // Set status to descending
    setStatus(kDescending);
}


void Airplane::taxiArrival(ostream& log, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling taxiArrival");
    // Arrived at gate
    if (getPosition() == fATC->getAirport()->getRunways()[0]->getTaxiPoint() and getRequest() == kConfirmed) {
        // Log event
        log << "[" << fATC->getTime().formatted() << "] " << getCallsign() << " is standing at Gate " << getGateID() << endl;

        setPosition("");

        // Set time remaining for deboarding
        setTimeRemaining(int(ceil(getPassengers() / 2.0)));

        // Change status to at airport
        setStatus(kDeboarding);
    }

        // Arrived at taxipoint
    else if (getRequest() == kConfirmed) {
        // Get runway
        Runway* runway = fATC->getAirport()->getNextRunway(this);

        // Log event
        log << "[" << fATC->getTime().formatted() << "] " << getCallsign() << " has taxied to holding point at " << runway->getName() << endl;

        // Set request to idle
        setRequest(kIdle);

        return;
    }

    // Send a request
    if (getRequest() == kIdle) {
        // Declare message to be sent to ATC
        string message;

        // Define it according to the position of the plane
        if (getPosition().empty()) {
            message = fATC->getAirport()->getCallsign() + ", " + getCallsign() + ", runway " + getRunway()->getName() + " vacated.";
        }
        else {
            Runway* runway = fATC->getAirport()->getNextRunway(this);
            message = fATC->getAirport()->getCallsign() + ", " + getCallsign() + ", holding short at " + runway->getName() + ".";
        }

        // Send the request and message
        fATC->sendRequest(fATC->getTime(), this);
        fATC->sendMessage(ATC::formatMessage(fATC->getTime(), getCallsign(), message));

        // Set request to pending
        setRequest(kPending);

        return;
    }

    // Request has been accepted by ATC
    if (getRequest() == kAccepted) {

        // Plane just arrived
        if (getPosition().empty()) {

            // Change position
            setPosition(getRunway()->getTaxiPoint());

            // Declare message to be sent to ATC
            string message;

            // Define it according to the position of the plane
            if (getPosition() == fATC->getAirport()->getRunways()[0]->getTaxiPoint()) {
                // Convert int to string with stream
                stringstream stream;
                stream << getGateID();
                message = "Taxi to gate " + stream.str() + " via " + getPosition() + ", " + getCallsign() + ".";
            }
            else {
                Runway* next = fATC->getAirport()->getNextRunway(this);
                message = "Taxi to holding point " + next->getName() + " via " + getPosition() + ", " + getCallsign() + ".";
            }

            // Send the message
            fATC->sendMessage(ATC::formatMessage(fATC->getTime(), getCallsign(), message));

            // Change status to taxi arrival
            setStatus(kTaxiArrival);

            // Set fATC->getTime() remaining
            setTimeRemaining(5);
        }

            // Plane is about to cross last runway
        else if (getPosition() == fATC->getAirport()->getRunways()[1]->getTaxiPoint()) {
            // Get next runway
            Runway* next = fATC->getAirport()->getNextRunway(this);

            // Convert int to string
            stringstream stream;
            stream << getGateID();

            // Send message to fATC
            string message = "Cleared to cross " + next->getName() + ", taxi to gate " + stream.str() + " via " + next->getTaxiPoint() + ", " + getCallsign() + ".";
            fATC->sendMessage(ATC::formatMessage(fATC->getTime(), getCallsign(), message));

            // Set plane status to crossing
            setStatus(kCrossingArrival);
        }

            // Plane crosses a runway
        else {
            // Get next runway
            Runway* next = fATC->getAirport()->getNextRunway(this);

            // Temporarily set airplane position to next taxipoint to retrieve the runway after the next one
            string original = getPosition();
            setPosition(next->getTaxiPoint());
            Runway* afterNext = fATC->getAirport()->getNextRunway(this);
            setPosition(original);

            // Send message to fATC
            string message = "Cleared to cross " + next->getName() + ", taxi to holding point " + afterNext->getName() + " via " + next->getTaxiPoint() + ", " + getCallsign() + ".";
            fATC->sendMessage(ATC::formatMessage(fATC->getTime(), getCallsign(), message));

            // Set plane status to crossing
            setStatus(kCrossingArrival);
        }

        // Set request to confirmed
        setRequest(kConfirmed);
    }
}

void Airplane::crossArrival(ostream& log, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling crossArrival");
    // Get the runway the plane just crossed
    Runway* runway = fATC->getAirport()->getNextRunway(this);

    // Log event
    log << "[" << fATC->getTime().formatted() << "] " << getCallsign() << " crossed " << runway->getName() << "." << endl;

    // Make runway available again
    runway->setFree(true);

    // Set time remaining
    setTimeRemaining(5);

    // Change position
    setPosition(runway->getTaxiPoint());

    // Chane status back to taxiing
    setStatus(kTaxiArrival);
}

void Airplane::deboard(ostream& log, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling deboard");

    // Log event
    log << "[" << fATC->getTime().formatted() << "] " << getPassengers() << " passengers exited " <<
        getCallsign() << " at gate " << getGateID() << " of " << fATC->getAirport()->getName() << endl;


    if (getSize() == kSmall) {
        setTimeRemaining(1);
    }
    else if (getSize() == kMedium) {
        setTimeRemaining(2);
    }
    else if (getSize() == kLarge) {
        setTimeRemaining(3);
    }

    // Change status
    setStatus(kTechnicalCheck);

    // Set request status to idle
    setRequest(kIdle);
}

void Airplane::technicalCheck(ostream &log, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling technicalCheck");

    // Log event
    log << "[" << fATC->getTime().formatted() << "] " << getCallsign() << " has been checked for technical malfunctions" << endl;

    // Change status
    setStatus(kParked);

    // Set request status to idle
    setRequest(kIdle);
}

void Airplane::land(ostream& log, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling land");

    // If time remaining is not 0, plane is still busy
    if (getTimeRemaining() != 0) {
        return;
    }

    // Plane is approaching
    if (getStatus() == kApproaching) {
        approach(log, fATC);
    }

        // Plane is descending
    else if (getStatus() == kDescending) {
        descend(log, fATC);
    }

        // Plane is circling
    else if (getStatus() == kCircling) {
        circle(log, fATC);
    }

        // Plane is taxiing
    else if (getStatus() == kTaxiArrival) {
        taxiArrival(log, fATC);
    }

        // Plane is crossing a runway
    else if (getStatus() == kCrossingArrival) {
        crossArrival(log, fATC);
    }

        // Plane is deboarding
    else if (getStatus() == kDeboarding) {
        deboard(log, fATC);
    }

        // Plane is having a technical check
    else if (getStatus() == kTechnicalCheck) {
        technicalCheck(log, fATC);
    }
}



// ---- TAKE-OFF ----

void Airplane::prepare(ostream& fLog, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling prepare.");
    EPlaneRequest request = getRequest();
    Runway *dest = getRunway();
    string planeCS = getCallsign();

    // Check if plane already has IFR clearancy
    if (request == kAccepted) {
        // response
        stringstream stream;
        stream << getSquawk();
        fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, "Cleared to " + dest->getName() + ", initial altitude five thousand, expecting one zero zero in ten, squawking " + stream.str() + ", " + planeCS + "."));

        // If so, we refuel and let passengers board
        setTimeRemaining(int(ceil(getPassengers() / 2.0)) + int(ceil(getFuel() / 10000.0)));

        // log
        fLog << "[" << fATC->getTime().formatted() << "] " << getModel() << " is being refueled" << endl;
        if (getPassengers() == 1) {
            fLog << "[" << fATC->getTime().formatted() << "] " << getPassengers() << " passenger is boarding "
                 << getModel() << " at Gate " << getGateID() << " of " << fATC->getAirport()->getName() << endl;
        }
        else if (getPassengers() > 1) {
            fLog << "[" << fATC->getTime().formatted() << "] " << getPassengers() << " passengers are boarding "
                 << getModel() << " at Gate " << getGateID() << " of " << fATC->getAirport()->getName() << endl;
        }
        fLog << "[" << fATC->getTime().formatted() << "] " << getModel() << " is standing at Gate " << getGateID() << endl;

        // set status to kGate (= fully ready to start takeoff sequence) and return
        setStatus(kGate);

        // set request status to idle, since current request is finished.
        setRequest(kIdle);
        return;
    }

        // If plane hasn't sent a request yet, or the previous request was denied
    else if (request == kIdle or request == kDenied) {

        // Send new request to ATC, which takes 1 minute, and we change the status of the request
        fATC->sendRequest(fATC->getTime(), this);
        fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, fATC->getAirport()->getCallsign() + ", " + planeCS + ", requesting IFR clearancy."));

        // Advance
        setTimeRemaining(1);
        setRequest(kPending);
        return;
    }

        // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
    else return;
}

void Airplane::pushback(ostream &fLog, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling pushback.");
    EPlaneRequest request = getRequest();
    EPlaneSize size = getSize();
    string planeCS = getCallsign();

    // Check if plane already has permission to start pushback
    if (request == kAccepted) {
        // response
        fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, "Pushback approved, " + planeCS + "."));

        // log
        fLog << "[" << fATC->getTime().formatted() << "] " << getModel() << " is being pushed back" << endl;

        // If so, we start the pushback and "forget" the previous request (kIdle)
        setStatus(kPushback);
        setRequest(kIdle);

        // Depending on the size, pushing back takes longer or shorter
        if (size == kSmall) {
            setTimeRemaining(1);
        }
        if (size == kMedium) {
            setTimeRemaining(2);
        }
        if (size == kLarge) {
            setTimeRemaining(3);
        }
        return;
    }

        // If plane hasn't sent a request yet, or the previous request was denied
    else if (request == kIdle or request == kDenied) {

        // Send new request to ATC, which takes 1 minute, and we change the status of the request
        fATC->sendRequest(fATC->getTime(), this);

        stringstream ss;
        ss << planeCS << ", " << planeCS << " at gate " << getGateID() << ", requesting pushback.";

        fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, ss.str()));
        setTimeRemaining(1);
        setRequest(kPending);
        return;
    }

        // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
    else return;
}

void Airplane::taxiDepartureStart(ostream &fLog, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling taxiDepartureStart.");
    REQUIRE(fATC->getAirport()->amountOfRunways() >= 1, "No runways in airport.");
    EPlaneRequest request = getRequest();
    string planeCS = getCallsign();

    // Check if plane already has permission to start taxiing
    if (request == kAccepted) {

        Runway *firstRW = fATC->getAirport()->getRunways().at(0);
        fLog << "[" << fATC->getTime().formatted() << "] " << getModel() << " has started taxiing to runway " << firstRW->getName() << endl;

        setPosition("");
        setStatus(kTaxiDeparture);
        setRequest(kIdle);
    }

        // If plane hasn't sent a request yet, or the previous request was denied
    else if (request == kIdle or request == kDenied) {

        // Send new request to ATC, which takes 1 minute, and we change the status of the request
        fATC->sendRequest(fATC->getTime(), this);
        fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, planeCS + " is ready to taxi."));

        setTimeRemaining(1);
        setRequest(kPending);
        return;
    }

        // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
    else return;
}

void Airplane::taxiDepartureStep(ostream &fLog, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling taxiDepartureStep.");
    REQUIRE(fATC->getAirport()->getRunways().size() > 0, "There are no runways in the airport.");
    EPlaneRequest request = getRequest();
    string tp = getPosition();
    Runway *cur_rw = fATC->getAirport()->getRunway(tp);
    Runway *next_rw = fATC->getAirport()->getNextRunway(this);
    string planeCS = getCallsign();
    string portCS = fATC->getAirport()->getCallsign();

    // If at start of taxi-procedure / gate after pushback
    if (tp.empty()) {

        // go to first taxipoint
        Runway *firstRW = fATC->getAirport()->getRunways().at(0);
        setPosition(firstRW->getTaxiPoint());
        setTimeRemaining(5);
        return;
    }

        // If taxiing
    else {

        // Check if plane already has permission to start crossing
        if (request == kAccepted) {
            // response
            fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, "Cleared to cross " + cur_rw->getName() + ", taxi to holding point " + next_rw->getName() + " via " + next_rw->getTaxiPoint() + ", " + planeCS + "."));
            setStatus(kCrossingDeparture);
            setRequest(kIdle);
            setTimeRemaining(1); // crossing
            cur_rw->setFree(false);
            return;
        }

        // If at destination
        if (request == kConfirmed) {

            fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, "Holding position, " + planeCS + "."));

            setStatus(kWaitingForDeparture);
            setRequest(kIdle);

            return;
        }

        // not allowed to cross
        if (request == kDenied) {
            fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, "Holding position, " + planeCS + "."));
            setTimeRemaining(1); // wait
            setRequest(kIdle);
            return;
        }

            // If plane hasn't sent a request yet, or the previous request was denied and plane waited.
        else if (request == kIdle) {

            // Send new request to ATC, which takes 1 minute, and we change the status of the request
            fATC->sendRequest(fATC->getTime(), this);
            fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, portCS + ", " + planeCS + ", holding short at " + cur_rw->getName() + "."));
            setTimeRemaining(1);
            setRequest(kPending);
            return;
        }

            // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
        else return;
    }
}

void Airplane::taxiDepartureCross(ostream &fLog, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling taxiDepartureCross");

    Runway *next_rw = fATC->getAirport()->getNextRunway(this);
    Runway *cur_rw = fATC->getAirport()->getRunway(getPosition());

    // done crossing, set it to free
    cur_rw->setFree(true);

    // fLog
    fLog << "[" << fATC->getTime().formatted() << "] " << getModel() << " crossed " << cur_rw->getName() << "." << endl;

    // Taxi to next holding point
    setTimeRemaining(5);
    Time tTime(fATC->getTime().getHour(), fATC->getTime().getMinute());
    tTime.advance(5);
    fLog << "[" << tTime.formatted() << "] " << getModel() << " has taxied to holding point at " << next_rw->getTaxiPoint() << "." << endl;

    // set fields
    setPosition(next_rw->getTaxiPoint());
    setStatus(kTaxiDeparture);
    return;
}

void Airplane::atRunway(ostream &fLog, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling atRunway");
    EPlaneRequest request = getRequest();
    Runway *dest = getRunway();
    string planeCS = getCallsign();
    string portCS = fATC->getAirport()->getCallsign();

    // Check if plane already has permission to go on runway and start taking off
    if (request == kAcceptedImmediate) {
        setStatus(kAscending);
        setRequest(kIdle);
        setTimeRemaining(1); // line up
        fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, "Runway " + dest->getName() + " cleared for take-off, " + planeCS + "."));
        return;
    }

    // Check if plane already has permission to go on runway and wait
    if (request == kAccepted) {
        setStatus(kDeparture);
        setRequest(kIdle);
        setTimeRemaining(1); // line up
        fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, "Lining up runway " + dest->getName() + " and wait, " + planeCS + "."));
        return;
    }

    // plane has to wait
    if (request == kDenied) {
        setTimeRemaining(1);
        setRequest(kIdle);
        fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, "Holding position, " + planeCS + "."));
        return;
    }

        // If plane hasn't sent a request yet, or the previous request was denied and plane has waited
    else if (request == kIdle) {

        // Send new request to ATC, which takes 1 minute, and we change the status of the request
        fATC->sendRequest(fATC->getTime(), this);
        fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, portCS + ", " + planeCS + ", holding short at " + dest->getName() + "."));
        setTimeRemaining(1);
        setRequest(kPending);
        return;
    }

        // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
    else return;
}

void Airplane::onRunway(ostream &fLog, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling onRunway.");
    EPlaneRequest request = getRequest();
    Runway *dest = getRunway();
    string planeCS = getCallsign();

    // Check if plane already has permission to take off
    if (request == kAccepted) {
        setStatus(kAscending);
        setRequest(kIdle);
        fATC->sendMessage(fATC->formatMessage(fATC->getTime(), planeCS, "Runway " + dest->getName() + " cleared for take-off, " + planeCS + "."));

        // log
        fLog << "[" << fATC->getTime().formatted() << "] " << getModel() << " is taking off at " << fATC->getAirport()->getName() << " on runway " << dest->getName() << endl;
    }

        // If plane hasn't sent a request yet, or the previous request was denied
    else if (request == kIdle or request == kDenied) {

        // Send new request to ATC, which takes 1 minute, and we change the status of the request
        // no message is needed since in this scenario (waiting on runway) the ATC will contact first.
        fATC->sendRequest(fATC->getTime(), this);
        setTimeRemaining(1);
        setRequest(kPending);
        return;
    }

        // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
    else return;
}

void Airplane::ascend(ostream &fLog, ATC *fATC) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized when calling ascend.");
    setPosition("");
    // If plane is at a height < 5000 ft
    if (getAltitude() < 5) {

        // Ascend 1000 ft
        increaseAltitude(1);
        setTimeRemaining(getEngine() == kPropeller? 2 : 1);
        fLog << "[" << fATC->getTime().formatted() << "] " << getModel() << " ascended to " << getAltitude() << ".000 ft." << endl;
    }

        // plane is at a 5000 ft or higher
    else {

        // plane is finished, left airport
        fLog << "[" << fATC->getTime().formatted() << "] " << getModel() << " has left " << fATC->getAirport()->getName() << endl;
        setStatus(kAway);
    }
    return;
}

void Airplane::takeoff(ostream& fLog, ATC *fATC) {
    REQUIRE(fATC->properlyInitialized(), "ATC was not properly initialized.");
    REQUIRE(fATC->getAirport() != NULL, "No airport in the simulation.");
    REQUIRE(fATC->getAirport()->properlyInitialized(), "ATC was not properly initialized.");
    REQUIRE(properlyInitialized(), "Plane was not properly initialized.");

    // if plane is still busy, we do nothing
    if (getTimeRemaining() == 0) {

        EPlaneStatus status = getStatus();

        // if plane is at airport, but not yet refueled and boarded
        if (status == kAirport) {
            prepare(fLog, fATC);
        }

        // plane is done refueling and boarding
        if (status == kGate) {
            pushback(fLog, fATC);
        }

        // plane is done pushing back
        if (status == kPushback) {
            taxiDepartureStart(fLog, fATC);
        }

        // plane is at a taxipoint
        if (status == kTaxiDeparture) {
            taxiDepartureStep(fLog, fATC);
        }

        // plane is done crossing runway
        if (status == kCrossingDeparture) {
            taxiDepartureCross(fLog, fATC);
        }

        // Plane is done taxiing and is waiting at runway
        if (status == kWaitingForDeparture) {
            atRunway(fLog, fATC);
        }

        // Plane is waiting on runway
        if (status == kDeparture) {
            onRunway(fLog, fATC);
        }

        // Plane is taking off
        if (status == kAscending) {
            ascend(fLog, fATC);
        }
    }
}
