//============================================================================
// Name        : System.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/System.h"

using namespace std;

System::System(const Input& input, ostream& atc, const Time& end): fEndTime(end) {
    REQUIRE(!input.getAirports().empty(), "There has to be an airport in the input to start the simulation");
    
    // Make new atc
    fATC = new ATC(atc, false);

    // Get input
    fAirport = input.getAirports()[0];
    fATC->setAirport(fAirport);
    fFlightPlans = input.getFlightPlans();

    // Set atc for all airplanes
    for (unsigned int i=0; i<fFlightPlans.size(); i++) {
        fFlightPlans[i]->getAirplane()->setATC(fATC);
    }

    fInitCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

bool System::properlyInitialized() const {
    return fInitCheck == this;
}

void System::info(ostream& out) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling info");
    REQUIRE(fAirport != NULL, "No airport in the simulation");

    // Log airport info
    out << "Airport: " << fAirport->getName() << " (" << fAirport->getIata() << ")\n";
    out << " -> gates: " << fAirport->getGates() << endl;
    out << " -> runways: " << fAirport->amountOfRunways() << endl;
    out << endl;
    
    // Loop over flight plans
    vector<FlightPlan *>::iterator itr_air;
    vector<FlightPlan *> flightplans = getFlightPlans();
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
}



void System::run(ostream& log, const string& impressionName, const string& iniName) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling run");
    REQUIRE(getAirport() != NULL, "No airport in the simulation.");
    REQUIRE(!simulationFinished(), "Simulation is already finished");

    while (!simulationFinished()) {
        Time fTime = getATC()->getTime();

        // Each tick, we make sure our ATC handles requests.
        fATC->doHeartbeat(fTime);

        // Draw impression
        string name = impressionName + fTime.formatted();
        ofstream impression(name.c_str());
        impression << getAirport()->drawImpression(fTime, getFlightPlans());
        impression.close();

        // Generate an ini file for use with the graphics engine
        name = iniName + fTime.formatted() + ".ini";
        ofstream graphics(name.c_str());
        graphics << getAirport()->graphicsINI(getFlightPlans());
        graphics.close();

        // Loop over flight plans
        vector<FlightPlan*>::iterator itr;
        vector<FlightPlan*> flightPlans = getFlightPlans();
        for (itr = flightPlans.begin(); itr != flightPlans.end(); ++itr)
        {
            // Get info
            FlightPlan* flightplan = *itr;
            EEvent event = flightplan->getEvent(fTime);
            Airplane* airplane = flightplan->getAirplane();

            // Change status according to flight plan if plane is available
            if (airplane->getStatus() == kAway and event == kLand) {
                airplane->setStatus(kApproaching);
            }
            else if (airplane->getStatus() == kParked and event == kTakeoff) {
                airplane->setStatus(kAirport);
            }

            airplane->performNextStep(log);
        }

        // Advance time
        fTime.advance();
        getATC()->setTime(fTime);
    }

    ENSURE(simulationFinished(), "Simulation is not finished yet, error occurred");
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

vector<FlightPlan*> System::getFlightPlans() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling getFlightPlans");
    return System::fFlightPlans;
}

System::~System() {
    // Delete airport
    delete fAirport;

    // Delete atc
    delete fATC;

    // Delete all flightplans, which in the destructor deletes the airplane
    vector<FlightPlan*>::iterator itr;
    vector<FlightPlan*> flightplans = getFlightPlans();
    for (itr = flightplans.begin(); itr != flightplans.end(); ++itr) {
        delete *itr;
    }
}

ATC* System::getATC() const {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling getATC");
    return fATC;
}

void System::generateImages(Time start, Time end) {
    REQUIRE(this->properlyInitialized(), "System was't initialized when calling generateImages");
    string command = "../graphics/engine ";
    for (; start < end; start.advance()) {
        command += "../output/ini/graphics" + start.formatted() + ".ini ";
    }
    system(command.c_str());
}