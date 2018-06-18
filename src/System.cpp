//============================================================================
// Name        : System.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/System.h"

using namespace std;

System::System(ostream& atc, Time end): fEndTime(end), fATC(new ATC(atc, false)) {
    fAirport = NULL;
    fInitCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

System::System() {
    fAirport = NULL;
    fATC = NULL;
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



void System::run(ostream& log, const string& impressionName) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling run");
    REQUIRE(fAirport != NULL, "No airport in the simulation.");
    REQUIRE(!simulationFinished(), "Simulation is already finished");

    while (!simulationFinished()) {

        // Current time
        Time fTime = getATC()->getTime();

        // Each tick, we make sure our ATC handles requests.
        fATC->doHeartbeat(fTime);

        // Set up ostream for impression
        string name = impressionName + fTime.formatted();
        ofstream impression(name.c_str());

        // Each tick, we draw a graphical impression of the airport.
        getAirport()->drawImpression(fTime, impression, getFlightplans());


        // Get flightplans and set up iterator
        vector<Flightplan*>::iterator flightplanItr;

        // Get flightplans
        vector<Flightplan*> flightplans = getFlightplans();

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

            airplane->land(log, fATC);

            airplane->takeoff(log, fATC);

            airplane->decreaseTimeRemaining();
        }

        // Advance time
        fTime.advance();
        getATC()->setTime(fTime);
    }

    ENSURE(simulationFinished(), "Simulation is not finished yet, error occured");
}

bool System::simulationFinished() const {
    REQUIRE(this->properlyInitialized(), "System wasn't initialized when calling simulationFinished");
    REQUIRE(this->getATC()->properlyInitialized(), "ATC wasn't initialized when calling simulationFinished");
    Time fTime = getATC()->getTime();
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

    // Delete atc
    delete fATC;

    // Delete all flightplans, which in the destructor deletes the airplane
    vector<Flightplan*>::iterator flightplanItr;
    vector<Flightplan*> flightplans = getFlightplans();
    for (flightplanItr = flightplans.begin(); flightplanItr != flightplans.end(); ++flightplanItr) {
        delete *flightplanItr;
    }
}


void System::initializeATC(ostream &log, bool test) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling initializeATC");
    fATC = new ATC(log, test);
}

void System::setEndTime(Time end) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling setEndTime");
    fEndTime = end;
}

ATC* System::getATC() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling getATC");
    return fATC;
}