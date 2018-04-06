//
// Created by uauser on 3/1/18.
//

#include "../headers/System.h"


void System::setup(const string &filename) {
    TiXmlDocument xml;
    string error = "Couldn't open " + filename + ".";
//    REQUIRE(xml.LoadFile(filename.c_str()), error.c_str());

    // Temporary for when DesignByContract.h doesn't work
    if (!xml.LoadFile(filename.c_str()))  {
        cerr << "Couldn't open " + filename + "." << endl;
        return;
    }

    int fieldCount = 0;

    // We iterate over all root elements.
    for (TiXmlElement *root = xml.FirstChildElement(); root != NULL; root = root->NextSiblingElement()) {

        // AirportS

        if (strcmp(root->Value(), "AIRPORT") == 0) {
            fieldCount = 0;
            Airport *tmp = new Airport();

            //  We iterate over all members, check if it's a valid element and if so, add it to our object.
            for (TiXmlElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

                if (strcmp(elem->Value(), "name") == 0) {
                    tmp->setFName(elem->GetText());
                    fieldCount++;
                }
                else if (strcmp(elem->Value(), "iata") == 0) {
                    tmp->setFIata(elem->GetText());
                    fieldCount++;
                }
                else if (strcmp(elem->Value(), "callsign") == 0) {
                    tmp->setFCallsign(elem->GetText());
                    fieldCount++;
                }
                else if (strcmp(elem->Value(), "gates") == 0) {
                    tmp->setFGates(atoi(elem->GetText()));
                    fieldCount++;
                }
            }

            // If there were 4 field (all fields present), add the Airport to our system.
            if (fieldCount == 4) {
                System::addAirport(tmp->getFName(), tmp->getFIata(), tmp->getFCallsign(), tmp->getFGates());
            }
            else {
                cerr << "Missing field(s) for Airport." << endl;
            }
            delete tmp;
        }

        //  RUNWAYS

        else if (strcmp(root->Value(), "RUNWAY") == 0) {
            fieldCount = 0;
            string iata;
            Runway *tmp = new Runway();

            //  We iterate over all members, check if it's a valid element and if so, add it to our object.
            for (TiXmlElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

                if (strcmp(elem->Value(), "name") == 0) {
                    tmp->setFName(elem->GetText());
                    fieldCount++;
                }
                else if (strcmp(elem->Value(), "airport") == 0) {
                    iata = elem->GetText();
                    fieldCount++;
                }
            }

            // If there were 2 fields (all fields present), add the Runway to our system.
            // It can still fail inside 'addRunway' if the Airport tied to the IATA does not exist in our system.
            if (fieldCount == 2) {
                System::addRunway(tmp->getFName(), iata);
            }
            else {
                cerr << "Missing field(s) for Runway." << endl;
            }
            delete tmp;
        }

        // AIRPLANES

        else if (strcmp(root->Value(), "AIRPLANE") == 0) {
            int ap_status = 0;
            fieldCount = 0;
            Airplane *tmp = new Airplane();

            //  We iterate over all members, check if it's a valid element and if so, add it to our object.
            for (TiXmlElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

                if (strcmp(elem->Value(), "number") == 0) {
                    tmp->setFNumber(elem->GetText());
                    fieldCount++;
                }
                else if (strcmp(elem->Value(), "callsign") == 0) {
                    tmp->setFCallsign(elem->GetText());
                    fieldCount++;
                }
                else if (strcmp(elem->Value(), "model") == 0) {
                    tmp->setFModel(elem->GetText());
                    fieldCount++;
                }
                else if (strcmp(elem->Value(), "status") == 0) {
                    string status = elem->GetText();
                    // By default, the status is kApproaching.
                    // We also allow the initial state to be 'at gate' and change ap_status to kGate if so.
                    ap_status = 0;
                    if (status == "Gate") {
                        ap_status = 2;
                    }
                    fieldCount++;
                }
            }

            // If there were 4 fields (all fields present), add the Runway to our system.
            if (fieldCount == 4) {
                System::addAirplane(tmp->getFNumber(), tmp->getFCallsign(), tmp->getFModel(), ap_status);
            }
            else {
                cerr << "Missing field(s) for Airplane." << endl;
            }
            delete tmp;
        }
    }

    // We are finished with our XML file, so we clear it.
    xml.Clear();
}

void System::log(const string &filename) {
//    REQUIRE(!Airports.empty(), "List of Airports cannot be empty");

    // Output file
    ofstream out(filename.c_str());

    // Loop over Airports
    vector<Airport*>::iterator itr;
    for (itr = airports.begin(); itr < airports.end(); ++itr) {
        Airport* cur_ap = *itr;
        out << "Airport: " << cur_ap->getFName() << " (" << cur_ap->getFIata() << ")\n";
        out << " -> gates: " << cur_ap->getFGates() << endl;
        out << " -> runways: " << runwaysInAirport(cur_ap) << endl;
        out << endl;
    }

    // Loop over airplanes
    vector<Airplane *>::iterator itr_air;
    for (itr_air = airplanes.begin(); itr_air < airplanes.end(); ++itr_air) {
        Airplane* cur_ap = *itr_air;
        out << "Airplane: " << cur_ap->getFCallsign() << " (" << cur_ap->getFNumber() << ")\n";
        out << " -> model: " << cur_ap->getFModel() << endl;
        out << endl;
    }

    // Close file
    out.close();
}

void System::land(Airplane *plane, Airport *airport, ostream& out) const {
    // Correct status
    string error = plane->getFCallsign() + " is not able to land, wrong status.";
    // REQUIRE(plane->getFStatus() == kApproaching, error.c_str());

    // Airport available
    error = plane->getFCallsign() + " is not able to land, no airport available.";
    // REQUIRE(!airports.empty() && airport != NULL, error.c_str());

    // Runway available
    Runway* runway = getFreeRunway(airport);
    error = plane->getFCallsign() + " is not able to land, no open runway.";
    // REQUIRE(runway != NULL, error.c_str());
    
    // Gate available
    int gate = airport->getFreeGate();
    error = plane->getFCallsign() + " is not able to land, no gate available.";
    // REQUIRE(gate != -1, error.c_str());

    // Set plane's gate ID
    plane->setFGateID(gate);

    // Initial message
    out << plane->getFCallsign() << " is approaching " << airport->getFName() << " at " << plane->getAltitude() << ".000 ft." << endl;

    // Descend loop
    while (plane->getAltitude() > 1) {
        // Decrease altitude by 1.000 feet
        plane->decreaseAltitude(1);

        // Log
        out << plane->getFCallsign() << " descended to " << plane->getAltitude() << ".000 ft." << endl;
    }

    // Land and taxi to gate
    out << plane->getFCallsign() << " is landing at " << airport->getFName() << " on Runway " << runway->getFName() << endl;
    out << plane->getFCallsign() << " has landed at " << airport->getFName() << " on Runway " << runway->getFName() << endl;
    out << plane->getFCallsign() << " is taxiing to Gate " << plane->getFGateID() << endl;
    out << plane->getFCallsign() << " is standing at Gate " << plane->getFGateID() << endl << endl;

    // Set status to landed
    plane->setFStatus(kLanded);

    // Succesfully landed
    error = "Plane status has not been changed correctly";
    // ENSURE(plane->getFStatus() == kLanded, error.c_str());
}

void System::takeoff(Airplane *plane, Airport *airport, ostream& out) const {
    // Correct status
    string error = plane->getFCallsign() + " is not able to takeoff, not at gate.";
    // REQUIRE(plane->getFStatus() == kGate, error.c_str());
    
    // Runway available
    Runway* runway = getFreeRunway(airport);
    error = plane->getFCallsign() + " is not able to land because of no open runway.";
    // REQUIRE(runway != NULL, error.c_str());

    // Airport available
    error = plane->getFCallsign() + " is not able to land, no airport available.";
    // REQUIRE(!airports.empty() && airport != NULL, error.c_str());

    // Initial messages
    out << plane->getFCallsign() << " is standing at Gate " << plane->getFGateID() << endl;
    out << plane->getFCallsign() << " is taxiing to Runway " << runway->getFName() << endl;
    out << plane->getFCallsign() << " is taking off at " << airport->getFName() << " on Runway " << runway->getFName() << endl;

    // Ascend loop
    while (plane->getAltitude() < 5) {
        // Increase altitude by 1.000 feet
        plane->increaseAltitude(1);

        // Log
        out << plane->getFCallsign() << " ascended to " << plane->getAltitude() << ".000 ft." << endl;
    }

    // Last message
    out << plane->getFCallsign() << " has left " << airport->getFName() << endl << endl;

    // Set status to finished
    plane->setFStatus(kFinished);

    // Restore gate
    airport->restoreGate(plane->getFGateID());

    // Set plane to no gate
    plane->setFGateID(-1);

    // Succesful takeoff
    error = "Plane status has not been changed correctly";
    // ENSURE(plane->getFStatus() == kFinished, error.c_str());
}

void System::gate(Airplane *plane, Airport *airport, ostream& out) const {
    // Correct status
    string error = plane->getFCallsign() + " has not landed or is already at gate.";
    // REQUIRE(plane->getFStatus() == kLanded, error.c_str());

    // Airport available
    error = plane->getFCallsign() + " is not able to land, no airport available.";
    // REQUIRE(!airports.empty() && airport != NULL, error.c_str());

    // Log
    out << plane->getFPassengers() << " passengers exited " << plane->getFCallsign() << " at gate " << plane->getFGateID() << " of " << airport->getFName() << endl;
    out << plane->getFCallsign() << " has been checked for technical malfunctions" << endl;
    out << plane->getFCallsign() << " has been refueled" << endl;
    out << plane->getFPassengers() << " boarded " << plane->getFCallsign() << " at gate " << plane->getFGateID() << " of " << airport->getFName() << endl << endl;
    plane->setFStatus(kGate);

    // Succesfully performed duties at gate
    error = "Plane status has not been changed correctly";
    // ENSURE(plane->getFStatus() == kGate, error.c_str());
}

void System::run() {
    // Airport available and simulation not finished
    string errormsg = "No Airport available, can't run system";
//    REQUIRE(!Airports.empty() && !simulationFinished(), errormsg.c_str());

    // Set up iterator
    vector<Airplane*>::iterator itr;

    // Pick first Airport in the list
    Airport* Airport = airports[0];

    // While the simulation is not finished
    while (!simulationFinished()) {
        // Loop over all the planes
        for (itr = airplanes.begin(); itr != airplanes.end(); itr++) {
            Airplane* plane = *itr;

            // If plane status is approaching, land
            if (plane->getFStatus() == kApproaching) {
                land(plane, Airport);
            }

            // If plane status is gate, takeoff
            else if (plane->getFStatus() == kGate) {
                takeoff(plane, Airport);
            }

            // If plane status is landed, go to gate
            else if (plane->getFStatus() == kLanded) {
                gate(plane, Airport);
            }
        }
    }

    errormsg = "Simulation is not finished yet, error occured";
//    ENSURE(simulationFinished(), errormsg);
}

bool System::simulationFinished() const {
    std::vector<Airplane*>::const_iterator itr;
    for (itr = airplanes.begin(); itr != airplanes.end(); itr++) {
        if ((*itr)->getFStatus() != kFinished) {
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


// ADDING NEW OBJECTS

bool System::addAirport(const string& name, const string& iata, const string& callsign, int numGates) {
    Airport *ap = new Airport();
    ap->setFName(name);
    ap->setFIata(iata);
    ap->setFCallsign(callsign);
    ap->setFGates(numGates);
    ap->initStack();
    System::airports.push_back(ap);
    return true;
}

bool System::addRunway(const string& name, const string& iata) {
    Airport *home = System::findAirportByIATA(iata);
    string error = "Can't add Runway to non-existing Airport.";
//    REQUIRE(home != 0, error.c_str());

//    if (home == 0) {
//        cerr << "Can't add Runway to non-existing Airport." << endl;
//        return false;
//    }

    Runway *rw = new Runway();
    rw->setFName(name);
    rw->setFAirport(home);
    rw->setFree(true);
    System::runways.push_back(rw);
    return true;
}

bool System::addAirplane(const string& number, const string& callsign, const string& model, int status) {
    Airplane *ap = new Airplane();
    ap->setFNumber(number);
    ap->setFCallsign(callsign);
    ap->setFModel(model);
    ap->setFStatus((EPlaneStatus)status);
    ap->setFGateID(-1);
    if (ap->getFStatus() == kGate) {
        ap->setFGateID(findAirportByIATA("ANR")->getFreeGate());
        ap->setAltitude(0);
    }
    else {
        ap->setAltitude(10);
    }
    ap->setFPassengers(4);
    System::airplanes.push_back(ap);
    return true;
}


// HELPER FUNCTIONS

Airport *System::findAirportByIATA(const string& iata) const {
    // Check all Airports and if the Airport matches the IATA, return this Airport.
    vector<Airport *>::iterator itr;
    vector<Airport *> Airports = System::getAirports();
    for (itr = Airports.begin(); itr < Airports.end(); ++itr) {
        Airport* cur_ap = *itr;
        if (cur_ap->getFIata() == iata) {
            return cur_ap;
        }
    }
    return 0;
}

int System::runwaysInAirport(Airport *ap) const {
    // Check all runways and if it's in the correct Airport, increase our counter.
    // Finally, return this counter.
    int count = 0;
    vector<Runway*>::const_iterator itr;
    for (itr = runways.begin(); itr < runways.end(); ++itr) {
        Runway *run = *itr;
        if (run->getFAirport() == ap) {
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
        if (run->isFree() && run->getFAirport() == ap) {
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