//
// Created by uauser on 3/1/18.
//

#include "../headers/System.h"

System::System(ostream& atc, Time end): fEndTime(end), fATC(new ATC(atc)) {
    fTime = Time();
    fAirport = NULL;
    fInitCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

System::System() {
    fAirport = NULL;
    fATC = NULL;
    fTime = Time();
    fInitCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

bool System::properlyInitialized() const {
    return fInitCheck == this;
}

void System::import(Input &input) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling import");
    fFlightplans = input.getFlightplans();
    fAirport = input.getAirports()[0];
    fATC->setAirport(fAirport);
}

void System::info(const string &filename) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling info");
    REQUIRE(fAirport != NULL, "No airport in the simulation");

    // Output file
    ofstream out(filename.c_str());

    // Log airport info
    out << "Airport: " << fAirport->getName() << " (" << fAirport->getIata() << ")\n";
    out << " -> gates: " << fAirport->getGates() << endl;
    out << " -> runways: " << fAirport->amountOfRunways() << endl;
    out << endl;

    // Set up iterator
    vector<Flightplan *>::iterator itr_air;

    // Get flightplans
    vector<Flightplan *> flightplans = getFlightplans();

    // Loop over all of them
    for (itr_air = flightplans.begin(); itr_air < flightplans.end(); ++itr_air) {
        // Get airplane
        Airplane* cur_ap = (*itr_air)->getAirplane();

        // Log information
        out << "Airplane: " << cur_ap->getCallsign() << " (" << cur_ap->getNumber() << ")\n";
        out << " -> model: " << cur_ap->getModel() << endl;
        out << " -> type: ";
        if (cur_ap->getType() == kPrivate) {
            out << "private";
        } else if (cur_ap->getType() == kAirline) {
            out << "airline";
        } else if (cur_ap->getType() == kMilitary) {
            out << "military";
        } else if (cur_ap->getType() == kEmergency) {
            out << "emergency";
        }
        out << endl << " -> engine: ";
        if (cur_ap->getEngine() == kJet) {
            out << "jet";
        } else if (cur_ap->getEngine() == kPropeller) {
            out << "propeller";
        }
        out << endl << " -> size: ";
        if (cur_ap->getSize() == kSmall) {
            out << "small";
        } else if (cur_ap->getSize() == kMedium) {
            out << "medium";
        } else if (cur_ap->getSize() == kLarge) {
            out << "large";
        }
        out << endl << " -> passengers: " << cur_ap->getPassengers() << endl;
        out << endl;
    }

    // Close file
    out.close();
}


void System::approach(Airplane *plane, ostream& log) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling approach");
    // Request has been accepted by fATC
    if (plane->getRequest() == kAccepted) {
        // Send message to fATC
        string message = "Descend and maintain five thousand feet, squawking SQUAWKCODE, " + plane->getCallsign() + ".";
        fATC->sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

        // Set the altitude of the plane
        plane->setAltitude(10);

        // Set status to descending
        plane->setStatus(kDescending);

        // Set time remaining for first descend
        plane->setTimeRemaining(plane->getEngine() == kJet? 1:2);

        // Log
        log << "[" << fTime.formatted() << "] " << plane->getCallsign() << " is approaching " << fAirport->getName() << " at " << plane->getAltitude() << ".000ft." << endl;

        // Set request to idle
        plane->setRequest(kIdle);
    }

    // Send a request if not yet done
    else if (plane->getRequest() == kIdle) {
        // Send message to fATC
        string message = fAirport->getCallsign() + ", " + plane->getCallsign() + ", arriving at " + fAirport->getName() + ".";
        fATC->sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

        // Send the request
        fATC->sendRequest(fTime, plane);

        // Set request to pending
        plane->setRequest(kPending);
    }
}

void System::descend(Airplane *plane, ostream& log) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling descend");
    // Plane has landed
    if (plane->getAltitude() == 0) {
        // Log event
        log << "[" << fTime.formatted() << "] " << plane->getCallsign() << " has landed at " << fAirport->getName() << " on runway " << plane->getRunway()->getName() << endl;

        // Set runway to available
        plane->getRunway()->setFree(true);

        // Change status
        plane->setStatus(kTaxiArrival);

        // Set request to idle
        plane->setRequest(kIdle);

        return;
    }

    // Plane is descending
    if ((plane->getAltitude() != 5 and plane->getAltitude() != 3) or plane->getRequest() == kConfirmed) {
        // Decrease the altitude
        plane->decreaseAltitude();

        // If plane is at one of the key altitudes, send a request to atc
        if (plane->getAltitude() == 5 or plane->getAltitude() == 3) {
            fATC->sendRequest(fTime, plane);
        }

        // If altitude is 0, the plane has started landing
        if (plane->getAltitude() == 0) {
            log << "[" << fTime.formatted() << "] " << plane->getCallsign() << " is landing at " << fAirport->getName() << " on runway " << plane->getRunway()->getName() << endl;
            plane->setTimeRemaining(2);
        }

        // Normal descend
        else {
            log << "[" << fTime.formatted() << "] " << plane->getCallsign() << " descended to " << plane->getAltitude() << ".000ft." << endl;
            plane->setTimeRemaining(plane->getEngine() == kJet ? 1 : 2);
        }

        return;
    }

    // Request has been accepted by ATC
    if (plane->getRequest() == kAccepted) {
        // Pick the right message for the altitude
        string message;
        if (plane->getAltitude() == 5) message = "Descend and maintain three thousand feet, " + plane->getCallsign() + ".";
        else message = "Cleared ILS approach runway " + plane->getRunway()->getName() + ", " + plane->getCallsign() + ".";

        // Send message to fATC
        fATC->sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

        // Set time remaining for descend
        plane->setTimeRemaining(plane->getEngine() == kJet? 1:2);

        // Set status to confirmed
        plane->setRequest(kConfirmed);
    }

    // Request has been denied by ATC
    if (plane->getRequest() == kDenied) {
        // Send message to ATC
        string message = "Holding south on the one eighty radial, " + plane->getCallsign();
        fATC->sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

        // Set time remaining
        plane->setTimeRemaining(3);

        // Set status to circling
        plane->setStatus(kCircling);
    }
}

void System::circle(Airplane *plane, ostream& log) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling circle");
    // Log event
    log << "[" << fTime.formatted() << "] " << plane->getCallsign() << " has circled at " << plane->getAltitude() << ".000ft." << endl;

    // Make the altitude available again for circling
    if (plane->getAltitude() == 3) {
        fATC->set3occupied(false);
    }
    else {
        fATC->set3occupied(false);
    }

    // Send a new request to descend
    fATC->sendRequest(fTime, plane);

    // Set status to descending
    plane->setStatus(kDescending);
}


void System::taxiArrival(Airplane *plane, ostream& log) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling taxiArrival");
    // Arrived at gate
    if (plane->getPosition() == fAirport->getRunways()[0]->getTaxiPoint() and plane->getRequest() == kConfirmed) {
        // Log event
        log << "[" << fTime.formatted() << "] " << plane->getCallsign() << " is standing at Gate " << plane->getGateID() << endl;

        // Set time remaining for deboarding
        plane->setTimeRemaining(int(ceil(plane->getPassengers() / 2.0)));

        // Change status to at airport
        plane->setStatus(kDeboarding);
    }

    // Arrived at taxipoint
    else if (plane->getRequest() == kConfirmed) {
        // Get runway
        Runway* runway = fAirport->getNextRunway(plane);

        // Log event
        log << "[" << fTime.formatted() << "] " << plane->getCallsign() << " has taxied to holding point at " << runway->getName() << endl;

        // Set request to idle
        plane->setRequest(kIdle);

        return;
    }

    // Send a request
    if (plane->getRequest() == kIdle) {
        // Declare message to be sent to ATC
        string message;

        // Define it according to the position of the plane
        if (plane->getPosition().empty()) {
            message = fAirport->getCallsign() + ", " + plane->getCallsign() + ", runway " + plane->getRunway()->getName() + " vacated.";
        }
        else {
            Runway* runway = fAirport->getNextRunway(plane);
            message = fAirport->getCallsign() + ", " + plane->getCallsign() + ", holding short at " + runway->getName() + ".";
        }

        // Send the request and message
        fATC->sendRequest(fTime, plane);
        fATC->sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

        // Set request to pending
        plane->setRequest(kPending);

        return;
    }

    // Request has been accepted by ATC
    if (plane->getRequest() == kAccepted) {

        // Plane just arrived
        if (plane->getPosition().empty()) {

            // Change position
            plane->setPosition(plane->getRunway()->getTaxiPoint());

            // Declare message to be sent to ATC
            string message;

            // Define it according to the position of the plane
            if (plane->getPosition() == fAirport->getRunways()[0]->getTaxiPoint()) {
                // Convert int to string with stream
                stringstream stream;
                stream << plane->getGateID();
                message = "Taxi to gate " + stream.str() + " via " + plane->getPosition() + ", " + plane->getCallsign() + ".";
            }
            else {
                Runway* next = fAirport->getNextRunway(plane);
                message = "Taxi to holding point " + next->getName() + " via " + plane->getPosition() + "," + plane->getCallsign() + ".";
            }

            // Send the message
            fATC->sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

            // Change status to taxi arrival
            plane->setStatus(kTaxiArrival);

            // Set time remaining
            plane->setTimeRemaining(5);
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
            fATC->sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

            // Set plane status to crossing
            plane->setStatus(kCrossingArrival);
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
            fATC->sendMessage(ATC::formatMessage(fTime, plane->getCallsign(), message));

            // Set plane status to crossing
            plane->setStatus(kCrossingArrival);
        }

        // Set request to confirmed
        plane->setRequest(kConfirmed);
    }
}

void System::crossArrival(Airplane *plane, ostream& log) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling crossArrival");
    // Get the runway the plane just crossed
    Runway* runway = fAirport->getNextRunway(plane);

    // Log event
    log << "[" << fTime.formatted() << "] " << plane->getCallsign() << " crossed " << runway->getName() << "." << endl;

    // Make runway available again
    runway->setFree(true);

    // Set time remaining
    plane->setTimeRemaining(5);

    // Change position
    plane->setPosition(runway->getTaxiPoint());

    // Chane status back to taxiing
    plane->setStatus(kTaxiArrival);
}

void System::deboard(Airplane *plane, ostream& log) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling deboard");

    // Log event
    log << "[" << fTime.formatted() << "] " << plane->getPassengers() << " passengers exited " <<
         plane->getCallsign() << " at gate " << plane->getGateID() << " of " << fAirport->getName() << endl;

    // Change status
    plane->setStatus(kAirport);

    // Set request status to idle
    plane->setRequest(kIdle);
}

void System::land(Airplane *plane, ostream& log) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling land");

    // If time remaining is not 0, plane is still busy
    if (plane->getTimeRemaining() != 0) {
        return;
    }

    // Plane is approaching
    if (plane->getStatus() == kApproaching) {
        approach(plane, log);
    }

    // Plane is descending
    else if (plane->getStatus() == kDescending) {
        descend(plane, log);
    }

    // Plane is circling
    else if (plane->getStatus() == kCircling) {
        circle(plane, log);
    }

    // Plane is taxiing
    else if (plane->getStatus() == kTaxiArrival) {
        taxiArrival(plane, log);
    }

    // Plane is crossing a runway
    else if (plane->getStatus() == kCrossingArrival) {
        crossArrival(plane, log);
    }

    // Plane is deboarding
    else if (plane->getStatus() == kDeboarding) {
        deboard(plane, log);
    }
}



void System::takeoff(Airplane *plane, ostream& fLog) const {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");
    REQUIRE(this->getAirport() != NULL, "No airport in the simulation.");
    REQUIRE(plane != NULL, "Plane object does not exist.");

    // if plane is still busy, we do nothing
    if (plane->getTimeRemaining() == 0) {

        // get needed information about airplane
        EPlaneSize size = plane->getSize();
        EPlaneStatus status = plane->getStatus();
        EPlaneRequest request = plane->getRequest();
        string planeCS = plane->getCallsign();
        string portCS = getAirport()->getCallsign();

        Runway *dest = plane->getRunway();

        // if plane has an "invalid" status, we do nothing
        if (status == kApproaching or status == kDescending or status == kTaxiArrival or status == kAway) {
            return;
        }


        // if plane is at airport, but not yet refueled and boarded
        if (status == kAirport) {

            // Check if plane already has IFR clearancy
            if (request == kAccepted) {
                // response
                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Cleared to " + dest->getName() + ", initial altitude five thousand, expecting one zero zero in ten, squawking SQUAWK" + ", " + planeCS + "."));

                // If so, we refuel and let passengers board

                // TODO: fuel

                plane->setTimeRemaining(int(ceil(plane->getPassengers() / 2.0)));

                // log
                fLog << "[" << fTime.formatted() << "] " << plane->getModel() << " has been refueled" << endl;
                fLog << "[" << fTime.formatted() << "] " << plane->getPassengers() << " passengers boarded " << plane->getModel() << " at Gate " << plane->getGateID() << " of " << getAirport()->getName() << endl;
                fLog << "[" << fTime.formatted() << "] " << plane->getModel() << " is standing at Gate" << plane->getGateID() << endl;

                // set status to kGate (= fully ready to start takeoff sequence) and return
                plane->setStatus(kGate);

                return;
            }

                // If plane hasn't sent a request yet, or the previous request was denied
            else if (request == kIdle or request == kDenied) {

                // Send new request to ATC, which takes 1 minute, and we change the status of the request
                fATC->sendRequest(fTime, plane);
                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", planeCS + ", " + planeCS + ", requesting IFR clearancy."));

                // Advance
                plane->setTimeRemaining(1);
                plane->setRequest(kPending);
                return;
            }

                // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
            else return;
        }

        // plane is done refueling and boarding
        if (status == kGate) {

            // Check if plane already has permission to start pushback
            if (request == kAccepted) {
                // response
                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Pushback approved, " + planeCS + "."));

                // log
                fLog << "[" << fTime.formatted() << "] " << plane->getModel() << " is being pushed back" << endl;

                // If so, we start the pushback and "forget" the previous request (kIdle)
                plane->setStatus(kPushback);
                plane->setRequest(kIdle);

                // Depending on the size, pushing back takes longer or shorter
                if (size == kSmall) {
                    plane->setTimeRemaining(1);
                }
                if (size == kMedium) {
                    plane->setTimeRemaining(2);
                }
                if (size == kLarge) {
                    plane->setTimeRemaining(3);
                }
                return;
            }

                // If plane hasn't sent a request yet, or the previous request was denied
            else if (request == kIdle or request == kDenied) {

                // Send new request to ATC, which takes 1 minute, and we change the status of the request
                fATC->sendRequest(fTime, plane);

                stringstream ss;
                ss << planeCS << ", " << planeCS << " at gate " << plane->getGateID() << ", requesting pushback.";

                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", ss.str()));
                plane->setTimeRemaining(1);
                plane->setRequest(kPending);
                return;
            }

                // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
            else return;
        }

        // plane is done pushing back
        if (status == kPushback) {

            // Check if plane already has permission to start taxiing
            if (request == kAccepted) {

                fLog << "[" << fTime.formatted() << "] " << plane->getModel() << " is taxiing to runway " << getAirport()->getRunway(plane->getRunway()->getTaxiPoint())->getName() << endl;

                plane->setPosition("");
                plane->setStatus(kTaxiDeparture);
                plane->setRequest(kIdle);
            }

                // If plane hasn't sent a request yet, or the previous request was denied
            else if (request == kIdle or request == kDenied) {

                // Send new request to ATC, which takes 1 minute, and we change the status of the request
                fATC->sendRequest(fTime, plane);
                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", planeCS + " is ready to taxi."));

                plane->setTimeRemaining(1);
                plane->setRequest(kPending);
                return;
            }

                // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
            else return;
        }

        // plane is at a taxipoint
        if (status == kTaxiDeparture) {

            string tp = plane->getPosition();
            Runway *cur_rw = getAirport()->getRunway(tp);
            Runway *next_rw = getAirport()->getNextRunway(plane);


            // If at start of taxi-procedure / gate after pushback
            if (tp.empty()) {

                // go to next taxipoint

                // GET FIRST RUNWAY

//                error


                plane->setPosition(" new taxipoint ");
                plane->setTimeRemaining(5);
                return;
            }

                // If taxiing
            else {

                // If at destination
                if (tp == plane->getRunway()->getTaxiPoint()) {
                    fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Taxi to runway " + next_rw->getName() + " via " + next_rw->getTaxiPoint() + ", " + planeCS + "."));
                    plane->setStatus(kWaitingForDeparture);
                }

                    // if not: cross current runway
                else {

                    // Check if plane already has permission to start crossing
                    if (request == kAccepted) {
                        // response
                        fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Cleared to cross " + cur_rw->getName() + ", taxi to holding point " + next_rw->getName() + " via " + next_rw->getTaxiPoint() + ", " + planeCS + "."));
                        plane->setStatus(kCrossingDeparture);
                        plane->setRequest(kIdle);
                        next_rw->setFree(false);
                        plane->setTimeRemaining(1); // crossing
                        return;
                    }

                    if (request == kDenied) {
                        fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Holding position, " + planeCS + "."));
                        plane->setTimeRemaining(1); // wait
                        return;
                    }

                        // If plane hasn't sent a request yet, or the previous request was denied and plane waited.
                    else if (request == kIdle) {

                        // Send new request to ATC, which takes 1 minute, and we change the status of the request
                        fATC->sendRequest(fTime, plane);
                        fATC->sendMessage(fATC->formatMessage(fTime, "AIR", portCS + ", " + planeCS + ", holding short at " + cur_rw->getName() + "."));
                        plane->setTimeRemaining(1);
                        plane->setRequest(kPending);
                        return;
                    }

                        // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
                    else return;

                }
            }
        }

        // plane is done crossing runway
        if (status == kCrossingDeparture) {

            // Set runway that was crossed to free
            getAirport()->getRunway(plane->getPosition())->setFree(true);

            // Set new position and status
            plane->setPosition(getAirport()->getNextRunway(plane)->getTaxiPoint());
            plane->setStatus(kTaxiDeparture);
            return;
        }

        // Plane is done taxiing and is waiting at runway
        if (status == kWaitingForDeparture) {

            // Check if plane already has permission to go on runway and start taking off
            if (request == kAcceptedImmediate) {
                plane->setStatus(kAscending);
                plane->setRequest(kIdle);
                plane->setTimeRemaining(1); // line up
                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Runway " + dest->getName() + " cleared for take-off, " + planeCS + "."));
                return;
            }

            // Check if plane already has permission to go on runway and wait
            if (request == kAccepted) {
                plane->setStatus(kDeparture);
                plane->setRequest(kIdle);
                plane->setTimeRemaining(1); // line up
                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Lining up runway " + dest->getName() + " and wait, " + planeCS + "."));
                return;
            }

            // plane has to wait
            if (request == kDenied) {
                plane->setTimeRemaining(1);
                plane->setRequest(kIdle);
                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Holding position, " + planeCS + "."));
                return;
            }

                // If plane hasn't sent a request yet, or the previous request was denied and plane has waited
            else if (request == kIdle) {

                // Send new request to ATC, which takes 1 minute, and we change the status of the request
                fATC->sendRequest(fTime, plane);
                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", portCS + ", " + planeCS + ", holding short at " + dest->getName() + "."));
                plane->setTimeRemaining(1);
                plane->setRequest(kPending);
                return;
            }

                // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
            else return;
        }

        // Plane is waiting on runway
        if (status == kDeparture) {

            // Check if plane already has permission to take off
            if (request == kAccepted) {
                plane->setStatus(kAscending);
                plane->setRequest(kIdle);
                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Runway " + dest->getName() + " cleared for take-off, " + planeCS + "."));

                // log
                fLog << "[" << fTime.formatted() << "] " << plane->getModel() << " is taking off at " << getAirport()->getName() << " on runway " << dest->getName() << endl;
            }

                // If plane hasn't sent a request yet, or the previous request was denied
            else if (request == kIdle or request == kDenied) {

                // Send new request to ATC, which takes 1 minute, and we change the status of the request
                // no message is needed since in this scenario (waiting on runway) the ATC will contact first.
                fATC->sendRequest(fTime, plane);
                plane->setTimeRemaining(1);
                plane->setRequest(kPending);
                return;
            }

                // If plane hasn't recieved an answer yet (= status is kPending): keep waiting
            else return;
        }

        // Plane is taking off
        if (status == kAscending) {

            // If plane is at a height < 5000 ft
            if (plane->getAltitude() < 5) {

                // Ascend 1000 ft
                plane->increaseAltitude(1);
                plane->setTimeRemaining(plane->getEngine() == kPropeller? 2 : 1);
                fLog << "[" << fTime.formatted() << "] " << plane->getModel() << " ascended to " << plane->getAltitude() << ".000 ft." << endl;
            }

                // plane is at a 5000 ft or higher
            else {

                // plane is finished, left airport
                fLog << "[" << fTime.formatted() << "] " << plane->getModel() << " has left " << getAirport()->getName() << endl;
                plane->setStatus(kAway);
            }
            return;
        }
    }
}


void System::run(ostream& log) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling run");
    REQUIRE(fAirport != NULL, "No airport in the simulation.");
    REQUIRE(!simulationFinished(), "Simulation is already finished");

    while (!simulationFinished()) {

        // Each tick, we make sure our ATC handles requests.
        fATC->doHeartbeat(fTime);


        // Get flightplans and set up iterator
        vector<Flightplan*>::iterator flightplanItr;

        // Get flightplans
        vector<Flightplan *> flightplans = getFlightplans();

        // Loop over flightplans
        for (flightplanItr = flightplans.begin(); flightplanItr != flightplans.end(); ++flightplanItr) {

            // Change airplane status according to flightplan
            // Get flightplan
            Flightplan* flightplan = *flightplanItr;

            // Get the event at this time
            EEvent event = flightplan->getEvent(fTime);

            // Get the airplane
            Airplane* airplane = flightplan->getAirplane();

            if (airplane->getStatus() == kAway and event == kLand) {
                airplane->setStatus(kApproaching);
            }

            // Plane has to be at airport to takeoff
            if (airplane->getStatus() == kParked and event == kTakeoff) {
                airplane->setStatus(kAirport);
            }

            land(airplane, log);

            takeoff(airplane, log);

            airplane->decreaseTimeRemaining();
        }

        // Advance time
        fTime.advance();
    }

    ENSURE(simulationFinished(), "Simulation is not finished yet, error occured");
}

bool System::simulationFinished() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling simulationFinished");
    return !(fTime < fEndTime or fTime == fEndTime);
}


Airport* System::getAirport() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling getAirport");
    return System::fAirport;
}

vector<Flightplan*> System::getFlightplans() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling getFlightplans");
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


void System::initializeATC(ostream &log) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling initializeATC");
    fATC = new ATC(log);
}

void System::setEndTime(Time end) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling setEndTime");
    fEndTime = end;
}
