//
// Created by uauser on 3/1/18.
//

#include "../headers/System.h"


void System::setup(const string &filename) {
    TiXmlDocument xml;
    string error = "Couldn't open " + filename + ".";

//    REQUIRE(xml.LoadFile(filename.c_str()), error.c_str());

    if (!xml.LoadFile(filename.c_str()))  {
        throw runtime_error("Couldn't open " + filename + ".");
    }

    int fieldCount = 0;

    // We iterate over all root elements.
    for (TiXmlElement *root = xml.FirstChildElement(); root != NULL; root = root->NextSiblingElement()) {

        // AIRPORTS

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
//    REQUIRE(!airports.empty(), "List of airports cannot be empty");

    // Output file
    ofstream out(filename.c_str());

    // Loop over airports
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

void System::land(Airplane *plane, Airport *port, ostream& out) const {
    string error = plane->getFCallsign() + " is not able to land.";
//    REQUIRE(plane->getFStatus() == kApproaching, error.c_str());

    // Check status
//    if (plane->getFStatus() != kApproaching) {
//        cerr << plane->getFCallsign() << " is not able to land." << endl;
//        return;
//    }

    // Get free Runway
    Runway* run = getFreeRunway(port);

    // If there isn't one, error msg and return
    if (!run) {
        cerr << "No free Runway! Divert " << plane->getFCallsign() << endl;
        return;
    }

    // Get gate
    int gate = port->getFreeGate();

    // Check if available
    if (gate < 0) {
        cerr << "No free gate! Divert " << plane->getFCallsign() << endl;
        return;
    }

    plane->setFGateID(gate);

    // Set height to 10.000 feet
    int heightInThousandsFeet = 10;

    // Initial message
    out << plane->getFCallsign() << " is approaching " << port->getFName() << " at " << heightInThousandsFeet << ".000 ft." << endl;

    // Descend loop
    while (heightInThousandsFeet > 1) {
        --heightInThousandsFeet;
        out << plane->getFCallsign() << " descended to " << heightInThousandsFeet << ".000 ft." << endl;
    }

    // Land and taxi to gate
    out << plane->getFCallsign() << " is landing at " << port->getFName() << " on Runway " << run->getFName() << endl;
    out << plane->getFCallsign() << " has landed at " << port->getFName() << " on Runway " << run->getFName() << endl;
    out << plane->getFCallsign() << " is taxiing to Gate " << plane->getFGateID() << endl;
    out << plane->getFCallsign() << " is standing at Gate " << plane->getFGateID() << endl << endl;

    // Set status to landed
    plane->setFStatus(kLanded);
}

void System::takeoff(Airplane *plane, Airport *port, ostream& out) const {
    string error = plane->getFCallsign() + " is not able to takeoff, not at gate.";
//    REQUIRE(plane->getFStatus() == kGate, error.c_str());

    // Check if plane is at gate
    if (plane->getFStatus() != kGate) {
        cerr << plane->getFCallsign() << " is not able to takeoff, not at gate." << endl;
        return;
    }

    // Get free Runway
    Runway* run = getFreeRunway(port);

    // If there isn't one, error msg and return
    if (!run) {
        cerr << "No free Runway! Stay on hold " << plane->getFCallsign() << endl;
        return;
    }

    // Set height to 0 feet
    int heightInThousandsFeet = 0;

    // Initial messages
    out << plane->getFCallsign() << " is standing at Gate " << plane->getFGateID() << endl;
    out << plane->getFCallsign() << " is taxiing to Runway " << run->getFName() << endl;
    out << plane->getFCallsign() << " is taking off at " << port->getFName() << " on Runway " << run->getFName() << endl;

    // Ascend loop
    while (heightInThousandsFeet < 5) {
        ++heightInThousandsFeet;
        out << plane->getFCallsign() << " ascended to " << heightInThousandsFeet << ".000 ft." << endl;
    }

    // Last message
    out << plane->getFCallsign() << " has left " << port->getFName() << endl << endl;

    // Set status to finished
    plane->setFStatus(kFinished);

    // Restore gate
    port->restoreGate(plane->getFGateID());

    // Set to no gate
    plane->setFGateID(-1);
}

void System::gate(Airplane *plane, Airport *port, ostream& out) const {
    string error = plane->getFCallsign() + " has not landed or is already at gate.";
//    REQUIRE(plane->getFStatus() == kLanded, error.c_str());

//    if (plane->getFStatus() != kLanded) {
//        cerr << plane->getFCallsign() << " has not landed or is already at gate." << endl;
//        return;
//    }
    out << plane->getFPassengers() << " passengers exited " << plane->getFCallsign() << " at gate " << plane->getFGateID() << " of " << port->getFName() << endl;
    out << plane->getFCallsign() << " has been checked for technical malfunctions" << endl;
    out << plane->getFCallsign() << " has been refueled" << endl;
    out << plane->getFPassengers() << " boarded " << plane->getFCallsign() << " at gate " << plane->getFGateID() << " of " << port->getFName() << endl << endl;
    plane->setFStatus(kGate);
}

void System::run() {
    string errormsg = "No airport available, can't run system";
//    REQUIRE(!airports.empty() && !simulationFinished(), errormsg.c_str());

    // Set up iterator
    vector<Airplane*>::iterator itr;

    // Pick first airport in the list
    Airport* airport = airports[0];

    // While the simulation is not finished
    while (!simulationFinished()) {
        // Loop over all the planes
        for (itr = airplanes.begin(); itr != airplanes.end(); itr++) {
            Airplane* plane = *itr;

            // If plane status is approaching, land
            if (plane->getFStatus() == kApproaching) {
                land(plane, airport);
            }

            // If plane status is gate, takeoff
            else if (plane->getFStatus() == kGate) {
                takeoff(plane, airport);
            }

            // If plane status is landed, go to gate
            else if (plane->getFStatus() == kLanded) {
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
    }
    ap->setFPassengers(4);
    System::airplanes.push_back(ap);
    return true;
}


// HELPER FUNCTIONS

Airport *System::findAirportByIATA(const string& iata) const {
    // Check all airports and if the Airport matches the IATA, return this Airport.
    vector<Airport *>::iterator itr;
    vector<Airport *> airports = System::getAirports();
    for (itr = airports.begin(); itr < airports.end(); ++itr) {
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

    // Delete all airports
    vector<Airport*>::iterator airportItr;
    for (airportItr = airports.begin(); airportItr != airports.end(); airportItr++) {
        delete *airportItr;
    }

    // Delete all runways
    vector<Runway*>::iterator runwayItr;
    for (runwayItr = runways.begin(); runwayItr != runways.end(); runwayItr++) {
        delete *runwayItr;
    }
}