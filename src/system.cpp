//
// Created by uauser on 3/1/18.
//

#include "../headers/system.h"

#include <fstream>

using namespace std;

void System::setup(const string &filename) {
    TiXmlDocument xml;
    string error = "Couldn't open " + filename + ".";
    REQUIRE(xml.LoadFile(filename.c_str()), error.c_str());
//    if (!xml.LoadFile(filename.c_str()))  {
//        throw runtime_error("Couldn't open " + filename + ".");
//    }
    int fieldCount = 0;

    // We iterate over all root elements.
    for (TiXmlElement *root = xml.FirstChildElement(); root != NULL; root = root->NextSiblingElement()) {

        // AIRPORTS

        if (strcmp(root->Value(), "AIRPORT") == 0) {
            fieldCount = 0;
            airport *tmp = new airport();

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

            // If there were 4 field (all fields present), add the airport to our system.
            if (fieldCount == 4) {
                System::addAirport(tmp->getFName(), tmp->getFIata(), tmp->getFCallsign(), tmp->getFGates());
            }
            else {
                cerr << "Missing field(s) for airport." << endl;
            }
            delete tmp;
        }

        //  RUNWAYS

        else if (strcmp(root->Value(), "RUNWAY") == 0) {
            fieldCount = 0;
            string iata;
            runway *tmp = new runway();

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

            // If there were 2 fields (all fields present), add the runway to our system.
            // It can still fail inside 'addRunway' if the airport tied to the IATA does not exist in our system.
            if (fieldCount == 2) {
                System::addRunway(tmp->getFName(), iata);
            }
            else {
                cerr << "Missing field(s) for runway." << endl;
            }
            delete tmp;
        }

        // AIRPLANES

        else if (strcmp(root->Value(), "AIRPLANE") == 0) {
            int ap_status = 0;
            fieldCount = 0;
            airplane *tmp = new airplane();

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

            // If there were 4 fields (all fields present), add the runway to our system.
            if (fieldCount == 4) {
                System::addAirplane(tmp->getFNumber(), tmp->getFCallsign(), tmp->getFModel(), ap_status);
            }
            else {
                cerr << "Missing field(s) for airplane." << endl;
            }
            delete tmp;
        }
    }

    // We are finished with our XML file, so we clear it.
    xml.Clear();
}

void System::log(const string &filename) {
    REQUIRE(!airports.empty(), "List of airports cannot be empty");

    // Output file
    ofstream out(filename.c_str());

    // Loop over airports
    vector<airport*>::iterator itr;
    for (itr = airports.begin(); itr < airports.end(); ++itr) {
        airport* cur_ap = *itr;
        out << "Airport: " << cur_ap->getFName() << " (" << cur_ap->getFIata() << ")\n";
        out << " -> gates: " << cur_ap->getFGates() << endl;
        out << " -> runways: " << runwaysInAirport(cur_ap) << endl;
        out << endl;
    }

    // Loop over airplanes
    vector<airplane *>::iterator itr_air;
    for (itr_air = airplanes.begin(); itr_air < airplanes.end(); ++itr_air) {
        airplane* cur_ap = *itr_air;
        out << "Airplane: " << cur_ap->getFCallsign() << " (" << cur_ap->getFNumber() << ")\n";
        out << " -> model: " << cur_ap->getFModel() << endl;
        out << endl;
    }

    // Close file
    out.close();
}

void System::land(airplane *plane, airport *port) const {
    string error = plane->getFCallsign() + " is not able to land.";
    REQUIRE(plane->getFStatus() == kApproaching, error.c_str());

    // Check status
//    if (plane->getFStatus() != kApproaching) {
//        cerr << plane->getFCallsign() << " is not able to land." << endl;
//        return;
//    }

    // Get free runway
    runway* run = getFreeRunway(port);

    // If there isn't one, error msg and return
    if (!run) {
        cerr << "No free runway! Divert " << plane->getFCallsign() << endl;
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
    cout << plane->getFCallsign() << " is approaching " << port->getFName() << " at " << heightInThousandsFeet << ".000 ft." << endl;

    // Descend loop
    while (heightInThousandsFeet > 1) {
        --heightInThousandsFeet;
        cout << plane->getFCallsign() << " descended to " << heightInThousandsFeet << ".000 ft." << endl;
    }

    // Land and taxi to gate
    cout << plane->getFCallsign() << " is landing at " << port->getFName() << " on runway " << run->getFName() << endl;
    cout << plane->getFCallsign() << " has landed at " << port->getFName() << " on runway " << run->getFName() << endl;
    cout << plane->getFCallsign() << " is taxiing to Gate " << plane->getFGateID() << endl;
    cout << plane->getFCallsign() << " is standing at Gate " << plane->getFGateID() << endl << endl;

    // Set status to landed
    plane->setFStatus(kLanded);
}

void System::takeoff(airplane *plane, airport *port) const {
    string error = plane->getFCallsign() + " is not able to takeoff, not at gate.";
    REQUIRE(plane->getFStatus() == kGate, error.c_str());

    // Check if plane is at gate
    if (plane->getFStatus() != kGate) {
        cerr << plane->getFCallsign() << " is not able to takeoff, not at gate." << endl;
        return;
    }

    // Get free runway
    runway* run = getFreeRunway(port);

    // If there isn't one, error msg and return
    if (!run) {
        cerr << "No free runway! Stay on hold " << plane->getFCallsign() << endl;
        return;
    }

    // Set height to 0 feet
    int heightInThousandsFeet = 0;

    // Initial messages
    cout << plane->getFCallsign() << " is standing at Gate " << plane->getFGateID() << endl;
    cout << plane->getFCallsign() << " is taxiing to runway " << run->getFName() << endl;
    cout << plane->getFCallsign() << " is taking off at " << port->getFName() << " on runway " << run->getFName() << endl;

    // Ascend loop
    while (heightInThousandsFeet < 5) {
        ++heightInThousandsFeet;
        cout << plane->getFCallsign() << " ascended to " << heightInThousandsFeet << ".000 ft." << endl;
    }

    // Last message
    cout << plane->getFCallsign() << " has left " << port->getFName() << endl << endl;

    // Set status to finished
    plane->setFStatus(kFinished);

    // Set to no gate
    plane->setFGateID(-1);
}

void System::gate(airplane *plane, airport *port) const {
    string error = plane->getFCallsign() + " has not landed, thus cannot be at gate.";
    REQUIRE(plane->getFStatus() == kLanded, error.c_str());

//    if (plane->getFStatus() != kLanded) {
//        cerr << plane->getFCallsign() << " has not landed, thus cannot be at gate." << endl;
//        return;
//    }
    cout << plane->getFPassengers() << " passengers exited " << plane->getFCallsign() << " at gate " << plane->getFGateID() << " of " << port->getFName() << endl;
    cout << plane->getFCallsign() << " has been checked for technical malfunctions" << endl;
    cout << plane->getFCallsign() << " has been refueled" << endl;
    cout << plane->getFPassengers() << " boarded " << plane->getFCallsign() << " at gate " << plane->getFGateID() << " of " << port->getFName() << endl << endl;
    plane->setFStatus(kGate);
}

void System::run() {
    vector<airplane*>::iterator itr;
    for (itr = airplanes.begin(); itr < airplanes.end(); ++itr) {
        land(*itr, airports[0]);
    }
    for (itr = airplanes.begin(); itr < airplanes.end(); ++itr) {
        gate(*itr, airports[0]);
    }
    for (itr = airplanes.begin(); itr < airplanes.end(); ++itr) {
        takeoff(*itr, airports[0]);
    }
}


// GETTERS

vector<airport*> System::getAirports() {
    return System::airports;
}

vector<runway*> System::getRunways() {
    return System::runways;
}

vector<airplane*> System::getAirplanes() {
    return System::airplanes;
}


// ADDING NEW OBJECTS

bool System::addAirport(const string& name, const string& iata, const string& callsign, int numGates) {
    airport *ap = new airport();
    ap->setFName(name);
    ap->setFIata(iata);
    ap->setFCallsign(callsign);
    ap->setFGates(numGates);
    ap->initStack();
    System::airports.push_back(ap);
    return true;
}

bool System::addRunway(const string& name, const string& iata) {
    airport *home = System::findAirportByIATA(iata);
    string error = "Can't add runway to non-existing airport.";
    REQUIRE(home != 0, error.c_str());

//    if (home == 0) {
//        cerr << "Can't add runway to non-existing airport." << endl;
//        return false;
//    }

    runway *rw = new runway();
    rw->setFName(name);
    rw->setFAirport(home);
    rw->setFree(true);
    System::runways.push_back(rw);
    return true;
}

bool System::addAirplane(const string& number, const string& callsign, const string& model, int status) {
    airplane *ap = new airplane();
    ap->setFNumber(number);
    ap->setFCallsign(callsign);
    ap->setFModel(model);
    ap->setFStatus((EPlaneStatus)status);
    ap->setFPassengers(4);
    System::airplanes.push_back(ap);
    return true;
}


// HELPER FUNCTIONS

airport *System::findAirportByIATA(const string& iata) {
    // Check all airports and if the airport matches the IATA, return this airport.
    vector<airport *>::iterator itr;
    vector<airport *> airports = System::getAirports();
    for (itr = airports.begin(); itr < airports.end(); ++itr) {
        airport* cur_ap = *itr;
        if (cur_ap->getFIata() == iata) {
            return cur_ap;
        }
    }
    return 0;
}

int System::runwaysInAirport(airport *ap) const {
    // Check all runways and if it's in the correct airport, increase our counter.
    // Finally, return this counter.
    int count = 0;
    vector<runway*>::const_iterator itr;
    for (itr = runways.begin(); itr < runways.end(); ++itr) {
        runway *run = *itr;
        if (run->getFAirport() == ap) {
            ++count;
        }
    }
    return count;
}

runway* System::getFreeRunway(airport *ap) const {
    // Check all runways and return the runway if it's in the correct airport and is free.
    vector<runway*>::const_iterator itr;
    for (itr = runways.begin(); itr < runways.end(); ++itr) {
        runway *run = *itr;
        if (run->isFree() && run->getFAirport() == ap) {
            return run;
        }
    }
    return NULL;
}


// DEBUGGING

void System::info() {
    // Temporary logging for easier debugging.

    // AIRPORTS
    cout << "AIRPORTS" << endl;
    vector<airport *>::iterator itr;
    vector<airport *> airports = System::getAirports();
    for (itr = airports.begin(); itr < airports.end(); ++itr) {
        airport* cur_ap = *itr;
        cout << "Name: " << cur_ap->getFName() << endl;
        cout << "IATA: " << cur_ap->getFIata() << endl;
        cout << "Callsign: " << cur_ap->getFCallsign() << endl;
        cout << "Gates: " << cur_ap->getFGates() << endl << endl;
    }

    // RUNWAYS
    cout << "RUNWAYS" << endl;
    vector<runway *>::iterator itr_run;
    vector<runway *> runways = System::getRunways();
    for (itr_run = runways.begin(); itr_run < runways.end(); ++itr_run) {
        runway* cur_rw = *itr_run;
        cout << "Name: " << cur_rw->getFName() << endl;
        cout << "IATA: " << cur_rw->getFAirport()->getFIata() << endl << endl;
    }

    // AIRPLANES
    cout << "AIRPLANES" << endl;
    vector<airplane *>::iterator itr_air;
    vector<airplane *> airplanes = System::getAirplanes();
    for (itr_air = airplanes.begin(); itr_air < airplanes.end(); ++itr_air) {
        airplane* cur_ap = *itr_air;
        cout << "Number: " << cur_ap->getFNumber() << endl;
        cout << "Callsign: " << cur_ap->getFCallsign() << endl;
        cout << "Model: " << cur_ap->getFModel() << endl;
        cout << "Status: " << cur_ap->getFStatus() << endl << endl;
    }
}