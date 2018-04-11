//
// Created by uauser on 3/1/18.
//

#include "../headers/System.h"

System::System(Input &input) {
    airports = input.getAirports();
    airplanes = input.getAirplanes();
    runways = input.getRunways();
}

void System::log(const string &filename) {
//    REQUIRE(!airports.empty(), "List of Airports cannot be empty");

    // Output file
    ofstream out(filename.c_str());

    // Loop over Airports
    vector<Airport*>::iterator itr;
    for (itr = airports.begin(); itr < airports.end(); ++itr) {
        Airport* cur_ap = *itr;
        out << "Airport: " << cur_ap->getName() << " (" << cur_ap->getIata() << ")\n";
        out << " -> gates: " << cur_ap->getGates() << endl;
        out << " -> runways: " << runwaysInAirport(cur_ap) << endl;
        out << endl;
    }

    // Loop over airplanes
    vector<Airplane *>::iterator itr_air;
    for (itr_air = airplanes.begin(); itr_air < airplanes.end(); ++itr_air) {
        Airplane* cur_ap = *itr_air;
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

void System::land(Airplane *plane, Airport *airport, ostream& out) const {
    // Correct status
    string error = plane->getCallsign() + " is not able to land, wrong status.";
    // REQUIRE(plane->getStatus() == kApproaching, error.c_str());

    // Airport available
    error = plane->getCallsign() + " is not able to land, no airport available.";
    // REQUIRE(!airports.empty() && airport != NULL, error.c_str());

    // Runway available
    Runway* runway = getFreeRunway(airport);
    error = plane->getCallsign() + " is not able to land, no open runway.";
    // REQUIRE(runway != NULL, error.c_str());
    
    // Gate available
    int gate = airport->getFreeGate();
    error = plane->getCallsign() + " is not able to land, no gate available.";
    // REQUIRE(gate != -1, error.c_str());

    // Set runway to unavailable
    runway->setFree(false);

    // Set plane's gate ID
    plane->setGateID(gate);

    // Initial message
    out << plane->getCallsign() << " is approaching " << airport->getName() << " at " << plane->getAltitude() << ".000 ft." << endl;

    // Descend loop
    while (plane->getAltitude() > 1) {
        // Decrease altitude by 1.000 feet
        plane->decreaseAltitude(1);

        // Log
        out << plane->getCallsign() << " descended to " << plane->getAltitude() << ".000 ft." << endl;
    }

    // Land and taxi to gate
    out << plane->getCallsign() << " is landing at " << airport->getName() << " on Runway " << runway->getName() << endl;
    out << plane->getCallsign() << " has landed at " << airport->getName() << " on Runway " << runway->getName() << endl;
    out << plane->getCallsign() << " is taxiing to Gate " << plane->getGateID() << endl;
    out << plane->getCallsign() << " is standing at Gate " << plane->getGateID() << endl << endl;

    // Set status to landed
    plane->setStatus(kLanded);

    // Make runway available again
    runway->setFree(true);

    // Succesfully landed
    error = "Plane status has not been changed correctly";
    // ENSURE(plane->getStatus() == kLanded, error.c_str());
}

void System::takeoff(Airplane *plane, Airport *airport, ostream& out) const {
    // Correct status
    string error = plane->getCallsign() + " is not able to takeoff, not at gate.";
    // REQUIRE(plane->getStatus() == kGate, error.c_str());
    
    // Runway available
    Runway* runway = getFreeRunway(airport);
    error = plane->getCallsign() + " is not able to land because of no open runway.";
    // REQUIRE(runway != NULL, error.c_str());

    // Airport available
    error = plane->getCallsign() + " is not able to land, no airport available.";
    // REQUIRE(!airports.empty() && airport != NULL, error.c_str());

    // Set runway to unavailable
    runway->setFree(false);

    // Initial messages
    out << plane->getCallsign() << " is standing at Gate " << plane->getGateID() << endl;
    out << plane->getCallsign() << " is taxiing to Runway " << runway->getName() << endl;
    out << plane->getCallsign() << " is taking off at " << airport->getName() << " on Runway " << runway->getName() << endl;

    // Ascend loop
    while (plane->getAltitude() < 5) {
        // Increase altitude by 1.000 feet
        plane->increaseAltitude(1);

        // Log
        out << plane->getCallsign() << " ascended to " << plane->getAltitude() << ".000 ft." << endl;
    }

    // Last message
    out << plane->getCallsign() << " has left " << airport->getName() << endl << endl;

    // Set status to finished
    plane->setStatus(kFinished);

    // Restore gate
    airport->restoreGate(plane->getGateID());

    // Set plane to no gate
    plane->setGateID(-1);

    // Make runway available again
    runway->setFree(true);

    // Succesful takeoff
    error = "Plane status has not been changed correctly";
    // ENSURE(plane->getStatus() == kFinished, error.c_str());
}

void System::gate(Airplane *plane, Airport *airport, ostream& out) const {
    // Correct status
    string error = plane->getCallsign() + " has not landed or is already at gate.";
    // REQUIRE(plane->getStatus() == kLanded, error.c_str());

    // Airport available
    error = plane->getCallsign() + " is not able to land, no airport available.";
    // REQUIRE(!airports.empty() && airport != NULL, error.c_str());

    // Log
    out << plane->getPassengers() << " passengers exited " << plane->getCallsign() << " at gate " << plane->getGateID() << " of " << airport->getName() << endl;
    out << plane->getCallsign() << " has been checked for technical malfunctions" << endl;
    out << plane->getCallsign() << " has been refueled" << endl;
    out << plane->getPassengers() << " boarded " << plane->getCallsign() << " at gate " << plane->getGateID() << " of " << airport->getName() << endl << endl;
    plane->setStatus(kGate);

    // Succesfully performed duties at gate
    error = "Plane status has not been changed correctly";
    // ENSURE(plane->getStatus() == kGate, error.c_str());
}

void System::run() {
    // Airport available and simulation not finished
    string errormsg = "No Airport available, can't run system";
//    REQUIRE(!airports.empty() && !simulationFinished(), errormsg.c_str());

    // Set up iterator
    vector<Airplane*>::iterator itr;

    // Pick first Airport in the list
    Airport* airport = airports[0];

    // While the simulation is not finished
    while (!simulationFinished()) {
        // Loop over all the planes
        for (itr = airplanes.begin(); itr != airplanes.end(); itr++) {
            Airplane* plane = *itr;

            // If plane status is approaching, land
            if (plane->getStatus() == kApproaching) {
                land(plane, airport);
            }

            // If plane status is gate, takeoff
            else if (plane->getStatus() == kGate) {
                takeoff(plane, airport);
            }

            // If plane status is landed, go to gate
            else if (plane->getStatus() == kLanded) {
                gate(plane, airport);
            }
        }
    }

    errormsg = "Simulation is not finished yet, error occured";
//    ENSURE(simulationFinished(), errormsg);
}

bool System::simulationFinished() const {
    std::vector<Airplane*>::const_iterator itr;
    for (itr = airplanes.begin(); itr != airplanes.end(); itr++) {
        if ((*itr)->getStatus() != kFinished) {
            return false;
        }
    }
    return true;
}


// GETTERS

vector<Airport*> System::getAirports() const {
    return System::airports;
}

vector<Runway*> System::getRunways() const {
    return System::runways;
}

vector<Airplane*> System::getAirplanes() const {
    return System::airplanes;
}

// HELPER FUNCTIONS

int System::runwaysInAirport(Airport *ap) const {
    // Check all runways and if it's in the correct Airport, increase our counter.
    // Finally, return this counter.
    int count = 0;
    vector<Runway*>::const_iterator itr;
    for (itr = runways.begin(); itr < runways.end(); ++itr) {
        Runway *run = *itr;
        if (run->getAirport() == ap) {
            ++count;
        }
    }
    return count;
}

Runway* System::getFreeRunway(Airport *ap) const {
    // Check all runways and return the Runway if it's in the correct Airport and is free.
    vector<Runway*>::const_iterator itr;
    for (itr = runways.begin(); itr < runways.end(); ++itr) {
        Runway *run = *itr;
        if (run->isFree() && run->getAirport() == ap) {
            return run;
        }
    }
    return NULL;
}

System::~System() {
    // Delete all planes
    vector<Airplane*>::iterator planeItr;
    for (planeItr = airplanes.begin(); planeItr != airplanes.end(); planeItr++) {
        delete *planeItr;
    }

    // Delete all Airports
    vector<Airport*>::iterator AirportItr;
    for (AirportItr = airports.begin(); AirportItr != airports.end(); AirportItr++) {
        delete *AirportItr;
    }

    // Delete all runways
    vector<Runway*>::iterator runwayItr;
    for (runwayItr = runways.begin(); runwayItr != runways.end(); runwayItr++) {
        delete *runwayItr;
    }
}