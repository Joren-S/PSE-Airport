//
// Created by uauser on 3/1/18.
//

#include "../headers/System.h"

System::System(ostream& atc, ostream& log, Time end): fEndTime(end), fLog(log), fATC(ATC(atc)) {
    fTime = Time();
    fAirport = NULL;
    fInitCheck = this;
    ENSURE(this->properlyInitialized(), "System was not properly initialized.");
}

bool System::properlyInitialized() const {
    return fInitCheck == this;
}

void System::import(Input &input) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");
    fFlightplans = input.getFlightplans();
    fAirport = input.getAirports()[0];
    fATC.setAirport(fAirport);
}

void System::info(const string &filename) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");
    REQUIRE(fAirport != NULL, "No airport in the simulation");

    // Output file
    ofstream fLog(filename.c_str());

    // Log airport info
    fLog << "Airport: " << fAirport->getName() << " (" << fAirport->getIata() << ")\n";
    fLog << " -> gates: " << fAirport->getGates() << endl;
    fLog << " -> runways: " << fAirport->amountOfRunways() << endl;
    fLog << endl;

    // Set up iterator
    vector<Flightplan *>::iterator itr_air;

    // Get flightplans
    vector<Flightplan *> flightplans = getFlightplans();

    // Loop over all of them
    for (itr_air = flightplans.begin(); itr_air < flightplans.end(); ++itr_air) {
        // Get airplane
        Airplane* cur_ap = (*itr_air)->getAirplane();

        // Log information
        fLog << "Airplane: " << cur_ap->getCallsign() << " (" << cur_ap->getNumber() << ")\n";
        fLog << " -> model: " << cur_ap->getModel() << endl;
        fLog << " -> type: ";
        if (cur_ap->getType() == kPrivate) {
            fLog << "private";
        } else if (cur_ap->getType() == kAirline) {
            fLog << "airline";
        } else if (cur_ap->getType() == kMilitary) {
            fLog << "military";
        } else if (cur_ap->getType() == kEmergency) {
            fLog << "emergency";
        }
        fLog << endl << " -> engine: ";
        if (cur_ap->getEngine() == kJet) {
            fLog << "jet";
        } else if (cur_ap->getEngine() == kPropeller) {
            fLog << "propeller";
        }
        fLog << endl << " -> size: ";
        if (cur_ap->getSize() == kSmall) {
            fLog << "small";
        } else if (cur_ap->getSize() == kMedium) {
            fLog << "medium";
        } else if (cur_ap->getSize() == kLarge) {
            fLog << "large";
        }
        fLog << endl << " -> passengers: " << cur_ap->getPassengers() << endl;
        fLog << endl;
    }

    // Close file
    fLog.close();
}

void System::land(Airplane *plane) {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");

    // Get status
    EPlaneStatus status = plane->getStatus();

    // If time remaining is 0, plane is not doing anything
    if (plane->getTimeRemaining() == 0) {

        // Plane is approaching
        if (status == kApproaching) {

            // If request is idle, nothing has been sent yet
            if (plane->getRequest() == kIdle) {

                // Send message to fATC
                string message = fAirport->getCallsign() + ", " + plane->getCallsign() + ", arriving at " + fAirport->getName() + ".";
                fATC.sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

                // Send the request
                fATC.sendRequest(fTime, plane);

                // Set request to pending
                plane->setRequest(kPending);

                return;
            }

            // Request has been accepted by fATC
            if (plane->getRequest() == kAccepted) {
                // Send message to fATC
                string message = "Descend and maintain five thousand feet, squawking SQUAWKCODE, " + plane->getCallsign();
                fATC.sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

                // Set request to confirmed
                plane->setRequest(kConfirmed);

                return;
            }

            // Request is pending, so wait
            if (plane->getRequest() == kPending) {
                return;
            }

            // Plane has already sent confirmation message, descend to 9000ft
            if (plane->getRequest() == kConfirmed) {
                // Set time remaining
                plane->setTimeRemaining(plane->getEngine() == kJet? 1:2);
            }
        }

        // Plane is descending
        else if (status == kDescending) {

            // If plane is at two of the key altitudes, check with fATC
            if (plane->getAltitude() == 5 or plane->getAltitude() == 3) {

                // If request is idle, nothing has been sent yet
                if (plane->getRequest() == kIdle) {
                    // Send request to fATC
                    fATC.sendRequest(fTime, plane);
                    return;
                }

                // Request has been accepted by fATC
                if (plane->getRequest() == kAccepted) {
                    // Send message to fATC
                    string message;
                    if (plane->getAltitude() == 5) message = "Descend and maintain three thousand feet, " + plane->getCallsign() + ".";
                    else message = "Cleared ILS approach runway " + plane->getRunway()->getName() + ", " + plane->getCallsign() + ".";
                    fATC.sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

                    // Set request
                    plane->setRequest(kConfirmed);

                    return;
                }

                // Request confirmed
                if (plane->getRequest() == kConfirmed) {
                    // Set time remaining
                    plane->setTimeRemaining(plane->getEngine() == kJet? 1:2);
                }

                if (plane->getRequest() == kDenied) {
                    // Send message to fATC
                    string message = "Holding south on the one eighty radial, " + plane->getCallsign();
                    fATC.sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

                    // Set time remaining
                    plane->setTimeRemaining(3);
                }
            }

            // Plane is going to land
            else if (plane->getAltitude() == 0) {
                plane->setTimeRemaining(2);
                plane->setRequest(kConfirmed);
            }

            // Normal descend
            else {
                plane->setTimeRemaining(plane->getEngine() == kJet? 1:2);
                plane->setRequest(kConfirmed);
            }

        }

        // Plane has landed
        else if (status == kTaxiArrival) {

            if (plane->getRequest() == kIdle) {
                if (plane->getPosition().empty()) {

                    // Send message to fATC
                    string message = fAirport->getCallsign() + ", " + plane->getCallsign() + ", runway " +
                                     plane->getRunway()->getName() + " vacated.";
                    fATC.sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));
                }

                else {
                    // Get next runway
                    Runway* runway = fAirport->getNextRunway(plane);

                    // Send message to fATC
                    string message = fAirport->getCallsign() + ", " + plane->getCallsign() + ", holding short at " + runway->getName() + ".";
                    fATC.sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));
                }

                // Send the request
                fATC.sendRequest(fTime, plane);

                // Set request to pending
                plane->setRequest(kPending);

                return;
            }

            else if (plane->getRequest() == kPending) {
                return;
            }

            else if (plane->getRequest() == kAccepted) {

                // Plane just arrived
                if (plane->getPosition().empty()) {

                    // Change position
                    plane->setPosition(plane->getRunway()->getTaxiPoint());

                    // Plane arrived at first runway
                    if (plane->getPosition() == fAirport->getRunways()[0]->getTaxiPoint()) {
                        // Convert int to string
                        stringstream stream;
                        stream << plane->getGateID();

                        // Send message to fATC
                        string message = "Taxi to gate " + stream.str() + " via " + plane->getPosition() + ", " + plane->getCallsign() + ".";
                        fATC.sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));
                    }

                    else {
                        // Get next runway
                        Runway* next = fAirport->getNextRunway(plane);

                        // Send message to fATC
                        string message = "Taxi to holding point " + next->getName() + " via " + plane->getPosition() + ".";
                        fATC.sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));
                    }

                    // Change status to taxi arrival
                    plane->setStatus(kTaxiArrival);
                }


                // Plane is about to cross last runway
                else if (plane->getPosition() == fAirport->getRunways()[1]->getTaxiPoint()) {
                    // Get next runway
                    Runway* next = fAirport->getNextRunway(plane);

                    // Convert int to string
                    stringstream stream;
                    stream << plane->getGateID();

                    // Send message to fATC
                    string message = "Cleared to cross " + next->getName() + ", taxi to gate " + stream.str() + " via " + next->getTaxiPoint() + ", " + plane->getCallsign() + ".";
                    fATC.sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

                    // Set plane status to crossing
                    plane->setStatus(kCrossing);
                }

                // Plane crosses a runway
                else {
                    // Get next runway
                    Runway* next = fAirport->getNextRunway(plane);

                    // Temporarily set airplane position to next taxipoint to retrieve the runway after the next one
                    string original = plane->getPosition();
                    plane->setPosition(next->getTaxiPoint());
                    Runway* afterNext = fAirport->getNextRunway(plane);
                    plane->setPosition(original);

                    // Send message to fATC
                    string message = "Cleared to cross " + next->getName() + ", taxi to holding point " + afterNext->getName() + " via " + next->getTaxiPoint() + ", " + plane->getCallsign() + ".";
                    fATC.sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

                    // Set plane status to crossing
                    plane->setStatus(kCrossing);
                }

                // Set request to confirmed
                plane->setRequest(kConfirmed);

                return;
            }

            else if (plane->getRequest() == kConfirmed) {
                plane->setTimeRemaining(5);
            }

        }

        else if (status == kCrossing) {
            plane->setTimeRemaining(1);
        }

        else if (status == kDeboarding) {
            plane->setTimeRemaining(plane->getPassengers() / 2);
            plane->setRequest(kConfirmed);
        }
    }

    // Decrease time of operation
    plane->decreaseTimeRemaining();

    // If there's still time remaining for the operation, exit function
    if (plane->getTimeRemaining() != 0 or (plane->getRequest() != kConfirmed and plane->getRequest() != kDenied)) {
        return;
    }

    // Plane has finished approaching
    if (status == kApproaching) {
        // Set altitude
        plane->setAltitude(10);

        // Log event
        fLog << "[" << fTime.formatted() << "] " << plane->getCallsign() << " is approaching " << fAirport->getName() << " at " << plane->getAltitude() << ".000ft." << endl;

        // Change status
        plane->setStatus(kDescending);

        // Set request status to idle
        plane->setRequest(kIdle);
    }

    // Plane has finished descending
    else if (status == kDescending) {
        // Plane has landed
        if (plane->getAltitude() == 0) {
            // Log event
            fLog << "[" << fTime.formatted() << "] " << plane->getCallsign() << " has landed at " << fAirport->getName() << " on runway " << plane->getRunway()->getName() << endl;

            plane->getRunway()->setFree(true);

            // Change status
            plane->setStatus(kTaxiArrival);
        }

        else {

            if (plane->getAltitude() == 3) {
                fATC.set3occupied(false);
            }

            if (plane->getAltitude() == 5) {
                fATC.set5occupied(false);
            }

            // Decrease altitude
            plane->decreaseAltitude();

            // Plane is landing
            if (plane->getAltitude() == 0) {
                fLog << "[" << fTime.formatted() << "] " << plane->getCallsign() << " is landing at " << fAirport->getName() << " on runway " << plane->getRunway()->getName() << endl;
            }

            // Plane has circled
            else if ((plane->getAltitude() == 3 or plane->getAltitude() == 5) and plane->getRequest() == kDenied) {
                fLog << "[" << fTime.formatted() << "] " << plane->getCallsign() << " has circled at " << plane->getAltitude() << ".000ft." << endl;
            }

            // Normal descend
            else {
                fLog << "[" << fTime.formatted() << "] " << plane->getCallsign() << " descended to " << plane->getAltitude() << ".000ft." << endl;
            }
        }

        // Set request status to idle
        plane->setRequest(kIdle);
    }

    else if (status == kCrossing) {
        // Get the runway the plane just crossed
        Runway* runway = fAirport->getNextRunway(plane);

        string pos = plane->getPosition();

        // Log event
        fLog << "[" << fTime.formatted() << "] " << plane->getCallsign() << " crossed " << runway->getName() << "." << endl;

        // Set status of runway to free
        runway->setFree(true);

        // Change position
        plane->setPosition(runway->getTaxiPoint());

        // Chane status back to taxiing
        plane->setStatus(kTaxiArrival);

        // Don't change request status, as we can keep taxiing until the next crossing
    }

    else if (status == kTaxiArrival) {

        // Arrived at gate
        if (plane->getPosition() == fAirport->getRunways()[0]->getTaxiPoint()) {

            // Log event
            fLog << "[" << fTime.formatted() << "] " << plane->getCallsign() << " is standing at Gate " << plane->getGateID() << endl;

            // Change status to at airport
            plane->setStatus(kDeboarding);
        }

        // Arrived at taxipoint
        else {
            // Get runway
            Runway* runway = fAirport->getNextRunway(plane);

            // Log event
            fLog << "[" << fTime.formatted() << "] " << plane->getCallsign() << " has taxied to holding point at " << runway->getName() << endl;
        }

        // Set request status to idle
        plane->setRequest(kIdle);
    }

    else if (status == kDeboarding) {
        // Log event
        fLog << "[" << fTime.formatted() << "] " << plane->getPassengers() << " passengers exited " <<
             plane->getCallsign() << " at gate " << plane->getGateID() << " of " << fAirport->getName() << endl;

        // Change status
        plane->setStatus(kAirport);

        plane->setRequest(kIdle);
    }
}

void System::takeoff(Airplane *plane) const {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");

}

void System::gate(Airplane *plane) const {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");

}

void System::run() {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");
    REQUIRE(fAirport != NULL, "No airport in the simulation");

    while (!simulationFinished()) {
        // Process requests in fATC
        fATC.doHeartbeat(fTime);

        if (fTime == Time(12,29)) {
            cout << "what";
        }

        // Set up iterator
        vector<Flightplan*>::iterator flightplanItr;

        // Get flightplans
        vector<Flightplan *> flightplans = getFlightplans();

        // Loop over flightplans
        for (flightplanItr = flightplans.begin(); flightplanItr != flightplans.end(); ++flightplanItr) {

            // Get flightplan
            Flightplan* flightplan = *flightplanItr;

            // Get the event at this time
            EEvent event = flightplan->getEvent(fTime);

            // Get the airplane
            Airplane* airplane = flightplan->getAirplane();

            // Change airplane status according to flightplan
            // Plane has to be away to land
            if (airplane->getStatus() == kAway and event == kLand) {
                airplane->setStatus(kApproaching);
            }

            // Plane has to be at airport to takeoff
            if (airplane->getStatus() == kAirport and event == kTakeoff) {
                airplane->setStatus(kGate);
            }

            // Get the airplane status
            EPlaneStatus status = airplane->getStatus();

            // Perform necessary actions according to plane status
            if (status == kApproaching or status == kDescending or status == kCrossing or status == kDeboarding or status == kTaxiArrival) {
                land(airplane);
            }
            else if (airplane->getStatus() == kGate) {
                takeoff(airplane);
            }
        }

        // Advance time
        fTime.advance();
    }

    ENSURE(simulationFinished(), "Simulation is not finished yet, error occured");
}

bool System::simulationFinished() const {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");
    return !(fTime < fEndTime or fTime == fEndTime);
}


Airport* System::getAirport() const {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");
    return System::fAirport;
}

vector<Flightplan*> System::getFlightplans() const {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");
    return System::fFlightplans;
}

System::~System() {
    // Delete airport
    delete fAirport;

    // Delete all flightplans
    vector<Flightplan*>::iterator flightplanItr;
    vector<Flightplan*> flightplans = getFlightplans();
    for (flightplanItr = flightplans.begin(); flightplanItr != flightplans.end(); ++flightplanItr) {
        delete *flightplanItr;
    }
}