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
            plane->setStatus(kLanded);
        }
        else {
            fLog << plane->getCallsign() << " has descended to " << plane->getAltitude() << endl;
        }
    }
    else if (status == kCircling) {
        fLog << plane->getCallsign() << " is circling at " << plane->getAltitude() << endl;
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
        // Set up iterator
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

            // Set airplane status
            if (event == kLand) {
                airplane->setStatus(kApproaching);
            }
            if (event == kTakeoff) {
                airplane->setStatus(kGate);
            }

            EPlaneStatus status = airplane->getStatus();

            // Perform necessary actions according to plane status
            if (status == kApproaching or status == kDescending or status == kCircling) {
                land(airplane);
            }
            else if (airplane->getStatus() == kLanded) {
                gate(airplane);
            }
            else if (airplane->getStatus() == kGate) {
                takeoff(airplane);
            }
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