//
// Created by uauser on 3/1/18.
//

#include "../headers/System.h"

System::System(ostream& atc, ostream& log, Time end): fEndTime(end), fATC(atc), fLog(log) {
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

void System::land(Airplane *plane) const {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");

    EPlaneStatus status = plane->getStatus();
    if (plane->getTimeRemaining() == 0) {
        if (status == kApproaching) {
            plane->setTimeRemaining(1);
        }
        else if (status == kDescending) {
            if (plane->getAltitude() == 0) {
                plane->setTimeRemaining(2);
            }
            else if (plane->getEngine() == kJet) {
                plane->setTimeRemaining(1);
            }
            else {
                plane->setTimeRemaining(2);
            }
        }
    }

    plane->decreaseTimeRemaining();

    if (plane->getTimeRemaining() != 0) {
        return;
    }

    if (status == kApproaching) {
        fATC << "[" << fTime.formatted() << "][" << plane->getCallsign() << endl;
        fATC << "$ " << fAirport->getCallsign() << ", " << plane->getCallsign() << ", arriving at " << fAirport->getName() << "." << endl;
        fLog << plane->getCallsign() << " approaching" << endl;
        plane->setAltitude(10);
        plane->setStatus(kDescending);
    }
    else if (status == kDescending) {
        plane->decreaseAltitude();
        if (plane->getAltitude() == 0) {
            fLog << plane->getCallsign() << " has landed" << endl;
            plane->setStatus(kTaxiArrival);
        }
        else {
            fLog << plane->getCallsign() << " has descended to " << plane->getAltitude() << endl;
        }
    }
}

void System::takeoff(Airplane *plane) const {
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
        Runway *dest = getAirport()->getRunway(plane->getDestination());

        // if plane has an "invalid" status, we do nothing
        if (status == kApproaching or status == kDescending or status == kTaxiArrival or status == kAway) {
            return;
        }


        // if plane is at airport, but not yet refueled and boarded
        if (status == kAirport) {

            // Check if plane already has IFR clearancy
            if (request == kAccepted) {
                // response
                fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Cleared to " + dest->getName() + ", initial altitude five thousand, expecting one zero zero in ten, squawking " + getSquawk() + ", " + planeCS + "."));

                // If so, we refuel and let passengers board
                plane->setTimeRemaining(ceil(plane->getPassengers() / 2) + 1);

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

                fLog << "[" << fTime.formatted() << "] " << plane->getModel() << " is taxiing to runway " << getAirport()->getRunway(plane->getDestination())->getName() << endl;

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

                error


                plane->setPosition(" new taxipoint ");
                plane->setTimeRemaining(5);
                return;
            }

            // If taxiing
            else {

                // If at destination
                if (tp == plane->getDestination()) {
                    fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Taxi to runway " + next_rw->getName() + " via " + next_rw->getTaxiPoint() + ", " + planeCS + "."));
                    plane->setStatus(kWaitingForDeparture);
                }

                // if not: cross current runway
                else {

                    // Check if plane already has permission to start crossing
                    if (request == kAccepted) {
                        // response
                        fATC->sendMessage(fATC->formatMessage(fTime, "AIR", "Cleared to cross " + cur_rw->getName() + ", taxi to holding point " + next_rw->getName() + " via " + next_rw->getTaxiPoint() + ", " + planeCS + "."));
                        plane->setStatus(kCrossing);
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
        if (status == kCrossing) {

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
                plane->setTimeRemaining(plane->getType() == kPropeller? 2 : 1);
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


void System::run() {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");
    REQUIRE(fAirport != NULL, "No airport in the simulation.");

    while (!simulationFinished()) {

        // Each tick, we make sure our ATC handles requests.
        fATC->doHeartbeat();


        // Get flightplans and set up iterator
        vector<Flightplan*>::iterator flightplanItr;
        vector<Flightplan *> flightplans = getFlightplans();

        // Loop over flightplans
        for (flightplanItr = flightplans.begin(); flightplanItr != flightplans.end(); ++flightplanItr) {

            // Change airplane status according to flightplan
            // Get flightplan, event and airplane
            Flightplan* flightplan = *flightplanItr;
            EEvent event = flightplan->getEvent(fTime);
            Airplane* airplane = flightplan->getAirplane();

            // Set airplane status
            if (event == kLand) {
                airplane->setStatus(kApproaching);
            }
            if (event == kTakeoff) {
                airplane->setStatus(kGate);
            }

            // Perform necessary actions according to plane status


            // we call this each tick, if plane doesn't need to land, nothing happens
            land(airplane);

            // we call this each tick, if plane doesn't need to takeoff, nothing happens
            takeoff(airplane);

            // Airplane did the step he had to do this minute, advance to next minute.
            airplane->decreaseTimeRemaining();
        }

        // Advance time
        fTime.advance();
        cout << fTime.formatted() << endl;
        if (fTime == Time(12,15)) {
            cout << "What" << endl;
        }
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