//
// Created by uauser on 3/1/18.
//

#include "../headers/system.h"
#include "../headers/airport.h"
#include "../headers/airplane.h"
#include "../headers/runway.h"

#include <stdio.h>
#include <fstream>

using namespace std;

void System::setup(const string &filename) {
    int fieldCount = 0;
    TiXmlDocument xml;
    if (xml.LoadFile(filename.c_str())) {
        for (TiXmlElement *root = xml.FirstChildElement(); root != NULL; root = root->NextSiblingElement()) {

            // AIRPORTS

            if (strcmp(root->Value(), "AIRPORT") == 0) {
                fieldCount = 0;
                airport *tmp = new airport();
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
                if (fieldCount == 4) {
                    System::addAirport(tmp->getFName(), tmp->getFIata(), tmp->getFCallsign(), tmp->getFGates());
                }
            }

            //  RUNWAYS

            else if (strcmp(root->Value(), "RUNWAY") == 0) {
                fieldCount = 0;
                string iata;
                runway *tmp = new runway();
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
                if (fieldCount == 2) {
                    System::addRunway(tmp->getFName(), iata);
                }
            }

            // AIRPLANES

            else if (strcmp(root->Value(), "AIRPLANE") == 0) {
                fieldCount = 0;
                airplane *tmp = new airplane();
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
                        fieldCount++;
                    }
                }
                if (fieldCount == 4) {
                    // 0 = EPlaneStatus::kApproaching.
                    System::addAirplane(tmp->getFNumber(), tmp->getFCallsign(), tmp->getFModel(), 0);
                }
            }
        }
    }
    else cerr << xml.ErrorDesc() << endl;
    xml.Clear();
}

void System::log(const string &filename) {
    // TODO: make sure the Require compiles
//    REQUIRE(!airports.empty(), "List of airports cannot be empty");

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

runway* System::getFreeRunway(airport *ap) const {
    vector<runway*>::const_iterator itr;
    for (itr = runways.begin(); itr < runways.end(); ++itr) {
        runway *run = *itr;
        if (run->getFAirport() == ap) {
            return run;
        }
    }
    return nullptr;
}

void System::land(airplane *plane, airport *port) const {
    // Get free runway
    runway* run = getFreeRunway(port);

    // If there isn't one, error msg and return
    if (!run) {
        cerr << "No free runway! Divert " << plane->getFCallsign() << endl;
        return;
    }

    // Set height to 10.000 feet
    int heightInThousandsFeet = 10;

    // Initial message
    cout << plane->getFCallsign() << " is approaching " << port->getFName() << " at " << heightInThousandsFeet << ".000 ft." << endl;

    // Descend loop
    while (heightInThousandsFeet > 1000) {
        --heightInThousandsFeet;
        cout << plane->getFCallsign() << " descended to " << heightInThousandsFeet << ".000 ft." << endl;
    }

    // Land and taxi to gate
    cout << plane->getFCallsign() << " is landing at " << port->getFName() << " on " << run->getFName() << endl;
    cout << plane->getFCallsign() << " has landed at " << port->getFName() << " on " << run->getFName() << endl;
    cout << plane->getFCallsign() << " is taxiing to Gate 1" << endl;
    cout << plane->getFCallsign() << " is standing at Gate 1" << endl;
}

void System::takeoff(airplane *plane, airport *port) const {

}

void System::gate(airplane *plane, airport *port) const {

}

void System::run() {

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


// NEW OBJECTS

bool System::addAirport(const string& name, const string& iata, const string& callsign, int numGates) {
    airport *ap = new airport();
    ap->setFName(name);
    ap->setFIata(iata);
    ap->setFCallsign(callsign);
    ap->setFGates(numGates);
    System::airports.push_back(ap);
    return true;
}

bool System::addRunway(const string& name, const string& iata) {
    airport *home = System::findAirportByIATA(iata);
    if (home != 0) {
        runway *rw = new runway();
        rw->setFName(name);
        rw->setFAirport(home);
        System::runways.push_back(rw);
        return true;
    }
    return false;
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


void System::info() {
    // tijdelijke logging voor gemakkelijker te debuggen

    // airports
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

int System::runwaysInAirport(airport *ap) const {
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