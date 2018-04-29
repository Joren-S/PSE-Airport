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

#include <cmath>

void System::takeoff(Airplane *plane) const {
    REQUIRE(this->properlyInitialized(), "System was not properly initialized.");
    REQUIRE(this->getAirport() != NULL, "No airport in the simulation.");
    REQUIRE(plane != NULL, "Plane object does not exist.");

    // if plane is still busy, we do nothing
    if (plane->getTimeRemaining() == 0) {

        // if plane has an "invalid" status, we do nothing
        EPlaneSize size = plane->getSize();
        EPlaneStatus status = plane->getStatus();
        EPlaneRequest request = plane->getRequest();

        if (status == kApproaching or status == kDescending or status == kTaxiArrival or status == kAway) {
            return;
        }

        // pre-takeoff
        if (status == kAirport) {
            //refuel and let passengers board
            plane->setTimeRemaining(ceil(plane->getPassengers() / 2) + 1);
            plane->setStatus(kGate);
            return;
        }

        // plane is fully ready and at gate
        if (status == kGate) {

            // plane has permission to start pushback
            if (request == kAccepted) {
                plane->setStatus(kPushback);
                plane->setRequest(kIdle);
                if (size == kSmall) {
                    plane->setTimeRemaining(1);
                }
                if (size == kMedium) {
                    plane->setTimeRemaining(2);
                }
                if (size == kLarge) {
                    plane->setTimeRemaining(3);
                }
            }

            // plane was denied or hasn't requested yet
            else if (request == kIdle or request == kDenied) {
                // send request
                fATC->sendRequest(fTime, plane);
                plane->setTimeRemaining(1);
                plane->setRequest(kPending);
                return;
            }
            // plane hasn't recieved an answer yet: keep waiting
            else return;
        }

        if (status == kPushback) {

            // plane has permission to start taxiing
            if (request == kAccepted) {
                plane->setStatus(kTaxiDeparture);
                plane->setRequest(kIdle);
            }

            // plane was denied or hasn't requested yet
            else if (request == kIdle or request == kDenied) {
                // send request
                fATC->sendRequest(fTime, plane);
                plane->setTimeRemaining(1);
                plane->setRequest(kPending);
                return;
            }
            // plane hasn't recieved an answer yet: keep waiting
            else return;
        }

        if (status == kTaxiDeparture) {
            // Taxi to destination
            if (plane->doNextTaxiStep()) {
                plane->setStatus(kDeparture);
            }
            else return;
        }

        if (status == kCrossing) {
            // we immediately set it to kTaxiDeparture since we can never be stuck in kCrossing
            // also, kCrossing only takes 1 minute.
            plane->setStatus(kTaxiDeparture);
            return;
        }

        if (status == kDeparture) {
            // plane has permission to start taking off
            if (request == kAccepted) {
                plane->setStatus(kAscending);
                plane->setRequest(kIdle);
            }

            // plane was denied or hasn't requested yet
            else if (request == kIdle or request == kDenied) {
                // send request
                fATC->sendRequest(fTime, plane);
                plane->setTimeRemaining(1);
                plane->setRequest(kPending);
                return;
            }
            // plane hasn't recieved an answer yet: keep waiting
            else return;
        }

        if (status == kAscending) {
            if (plane->getAltitude() < 5) {
                plane->increaseAltitude(1);
                plane->setTimeRemaining(plane->getType() == kPropeller? 2 : 1);
            }
            else {
                plane->setStatus(kAway);
            }
            return;
        }
    }
}

/*
 * fATC in System needs to become an ATC class instead of an ostream
 * so not fATC << msg, but fATC->fStream << msg (or overloader)
 *
 * also: doHeartbeat at beginning of WHILE in RUN
 * same for decreaseTimeRemaining, NOT in land or takeoff, but at end of plane iteration
 * */


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