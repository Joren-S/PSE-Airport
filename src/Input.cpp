//
// Created by Max on 4/7/2018.
//

#include "../headers/Input.h"

Input::Input(const string &filename) {
    // Load xml file, program will end if failed
    TiXmlDocument xml;
    string error = "Couldn't open " + filename + ".";
    REQUIRE(xml.LoadFile(filename.c_str()), error.c_str());

    // We iterate over all root elements.
    for (TiXmlElement *root = xml.FirstChildElement(); root != NULL; root = root->NextSiblingElement()) {

        // Airports
        if (strcmp(root->Value(), "AIRPORT") == 0) {
            readAirport(root->FirstChildElement());
        }

        // Runways
        else if (strcmp(root->Value(), "RUNWAY") == 0) {
            readRunway(root->FirstChildElement());
        }

        // Airplanes
        else if (strcmp(root->Value(), "AIRPLANE") == 0) {
            readAirplane(root->FirstChildElement());
        }

        // Invalid element
        else {
            cerr << "Did not recognize element: " << root->Value() << endl;
        }
    }

    // We are finished with our XML file, so we clear it.
    xml.Clear();
}

void Input::readAirplane(TiXmlElement *elem) {
    // Keep track of how many fields the element has
    int fieldCount = 0;

    // Make new object
    Airplane *tmp = new Airplane();

    // Pointer to the associated flightplan
    Flightplan* flightplan;

    //  We iterate over all members, check if it's a valid element and if so, add it to our object.
    for (; elem != NULL; elem = elem->NextSiblingElement()) {

        if (strcmp(elem->Value(), "number") == 0) {
            // Check for duplicate data
            if (tmp->getNumber() != "") {
                cerr << "Duplicate data in Airplane." << endl;
                fieldCount = -1;
                break;
            }

            // Set airplane number
            tmp->setNumber(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "callsign") == 0) {
            // Check for duplicate data
            if (tmp->getCallsign() != "") {
                cerr << "Duplicate data in Airplane." << endl;
                fieldCount = -1;
                break;
            }

            // Set airplane callsign
            tmp->setCallsign(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "model") == 0) {
            // Check for duplicate data
            if (tmp->getModel() != "") {
                cerr << "Duplicate data in Airplane." << endl;
                fieldCount = -1;
                break;
            }

            // Set airplane model
            tmp->setModel(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "status") == 0) {
            // Check for duplicate data
            if (tmp->getStatus() != 0) {
                cerr << "Duplicate data in Airplane status." << endl;
                fieldCount = -1;
                break;
            }

            // Set status
            if (strcmp(elem->GetText(), "gate") == 0) {
                tmp->setStatus(kGate);
                tmp->setAltitude(0);
            }
            else if (strcmp(elem->GetText(), "approaching") == 0) {
                tmp->setStatus(kApproaching);
                tmp->setAltitude(10);
            }
            else {
                cerr << "Invalid data for Airplane status" << endl;
                fieldCount = -1;
                break;
            }

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "passengers") == 0) {
            // Check for duplicate data
            if (tmp->getPassengers() != -1) {
                cerr << "Duplicate data in Airplane." << endl;
                fieldCount = -1;
                break;
            }

            // Set airplane passengers
            tmp->setPassengers(atoi(elem->GetText()));

            // Increase fieldcount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "type") == 0) {
            // Check for duplicate data
            if (tmp->getType() != 0) {
                cerr << "Duplicate data in Airplane." << endl;
                fieldCount = -1;
                break;
            }

            // Set airplane type
            if (strcmp(elem->GetText(), "private") == 0) {
                tmp->setType(kPrivate);
            }
            else if (strcmp(elem->GetText(), "airline") == 0) {
                tmp->setType(kAirline);
            }
            else if (strcmp(elem->GetText(), "military") == 0) {
                tmp->setType(kMilitary);
            }
            else if (strcmp(elem->GetText(), "emergency") == 0) {
                tmp->setType(kEmergency);
            }
            else {
                cerr << "Invalid data for Airplane type" << endl;
                fieldCount = -1;
                break;
            }

            // Increase fieldcount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "engine") == 0) {
            // Check for duplicate data
            if (tmp->getEngine() != 0) {
                cerr << "Duplicate data in Airplane." << endl;
                fieldCount = -1;
                break;
            }

            // Set airplane engine
            if (strcmp(elem->GetText(), "propeller") == 0) {
                tmp->setEngine(kPropeller);
            }
            else if (strcmp(elem->GetText(), "jet") == 0) {
                tmp->setEngine(kJet);
            }
            else {
                cerr << "Invalid data for Airplane engine." << endl;
                fieldCount = -1;
                break;
            }

            // Increase fieldcount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "size") == 0) {
            // Check for duplicate data
            if (tmp->getSize() != 0) {
                cerr << "Duplicate data in Airplane." << endl;
                fieldCount = -1;
                break;
            }

            // Set airplane size
            if (strcmp(elem->GetText(), "small") == 0) {
                tmp->setSize(kSmall);
            }
            else if (strcmp(elem->GetText(), "medium") == 0) {
                tmp->setSize(kMedium);
            }
            else if (strcmp(elem->GetText(), "large") == 0) {
                tmp->setSize(kLarge);
            }
            else {
                cerr << "Invalid data for Airplane size." << endl;
                fieldCount = -1;
                break;
            }

            // Increase fieldcount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "FLIGHTPLAN") == 0) {
            flightplan = readFlightplan(elem->FirstChildElement());
            flightplan->setAirplane(tmp);
            if (flightplan == NULL) {
                fieldCount = -1;
                cerr << "Didn't add airplane because of invalid flightplan." << endl;
                break;
            }
            fieldCount++;
        }
        else {
            cerr << "Invalid field: " << elem->Value() << endl;
            fieldCount = -1;
            break;
        }
    }

    // If there were 4 field (all fields present), add the Airplane to our system.
    if (fieldCount == 9) {
        Input::addAirplane(tmp);
        return;
    }

    // Something went wrong, if the field count is -1, an error msg has already been logged
    // Else, there were missing fields
    if (fieldCount != -1) {
        cerr << "Missing field(s) for Airplane." << endl;
    }

    // Delete the object
    delete tmp;

    // Delete the flightplan
    delete flightplan;
    flightplans.pop_back();
}

void Input::readRunway(TiXmlElement *elem) {
    // Keep track of how many fields the element has
    int fieldCount = 0;

    // Make new object
    Runway *tmp = new Runway();

    //  We iterate over all members, check if it's a valid element and if so, add it to our object.
    for (; elem != NULL; elem = elem->NextSiblingElement()) {

        if (strcmp(elem->Value(), "name") == 0) {
            // Check for duplicate data
            if (tmp->getName() != "") {
                cerr << "Duplicate data in Runway." << endl;
                fieldCount = -1;
                break;
            }

            // Set runway name
            tmp->setName(elem->GetText());

            // Increase fieldcount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "airport") == 0) {
            // Check for duplicate data
            if (tmp->getAirport() != NULL) {
                cerr << "Duplicate data in Runway." << endl;
                fieldCount = -1;
                break;
            }

            // Get airport from IATA
            Airport* airport = findAirportByIATA(elem->GetText());

            // No airport found
            if (airport == NULL) {
                cerr << "Did not find airport with iata: " << elem->GetText() << endl;
                fieldCount = -1;
                break;
            }

            // Set runway airport
            tmp->setAirport(airport);

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "type") == 0) {
            // Check for duplicate data
            if (tmp->getType() != 0) {
                cerr << "Duplicate data in Runway." << endl;
                fieldCount = -1;
                break;
            }

            // Set type
            if (strcmp(elem->GetText(), "grass") == 0) {
                tmp->setType(kGrass);
            }
            else if (strcmp(elem->GetText(), "asphalt") == 0) {
                tmp->setType(kAsphalt);
            }
            else {
                cerr << "Invalid data for Runway status." << endl;
                fieldCount = -1;
            }

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "length") == 0) {
            // Check for duplicate data
            if (tmp->getLength() != -1) {
                cerr << "Duplicate data in Runway." << endl;
                fieldCount = -1;
                break;
            }

            // Set runway length
            tmp->setLength(atoi(elem->GetText()));

            // Increase fieldCount
            fieldCount++;
        }
        else {
            cerr << "Invalid field: " << elem->Value() << endl;
            fieldCount = -1;
            break;
        }
    }

    // If there were 2 fields (all fields present), add the Runway to our system.
    if (fieldCount == 4) {
        Input::addRunway(tmp);
        return;
    }

    // Something went wrong, if the field count is -1, an error msg has already been logged
    // Else, there were missing fields
    if (fieldCount != -1) {
        cerr << "Missing field(s) for Runway." << endl;
    }

    // Delete the object
    delete tmp;
}

void Input::readAirport(TiXmlElement *elem) {
    // Keep track of how many fields the element has
    int fieldCount = 0;

    // Make new object
    Airport *tmp = new Airport();

    //  We iterate over all members, check if it's a valid element and if so, add it to our object.
    for (; elem != NULL; elem = elem->NextSiblingElement()) {

        if (strcmp(elem->Value(), "name") == 0) {
            // Check for duplicate data
            if (tmp->getName() != "") {
                cerr << "Duplicate data in Airport." << endl;
                fieldCount = -1;
                break;
            }

            // Set airport name
            tmp->setName(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "iata") == 0) {
            // Check for duplicate data
            if (tmp->getIata() != "") {
                cerr << "Duplicate data in Airport." << endl;
                fieldCount = -1;
                break;
            }

            // Set airport iata
            tmp->setIata(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "callsign") == 0) {
            // Check for duplicate data
            if (tmp->getCallsign() != "") {
                cerr << "Duplicate data in Airport." << endl;
                fieldCount = -1;
                break;
            }

            // Set airport callsign
            tmp->setCallsign(elem->GetText());

            // Increase fieldcount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "gates") == 0) {
            // Check for duplicate data
            if (tmp->getGates() != -1) {
                cerr << "Duplicate data in Airport." << endl;
                fieldCount = -1;
                break;
            }

            // Set airport gates
            tmp->setGates(atoi(elem->GetText()));

            // Increase fieldcount
            fieldCount++;
        }
        else {
            cerr << "Invalid field: " << elem->Value() << endl;
            fieldCount = -1;
            break;
        }
    }

    // If there were 4 field (all fields present), add the Airport to our system.
    if (fieldCount == 4) {
        Input::addAirport(tmp);
        return;
    }

    // Something went wrong, if the field count is -1, an error msg has already been logged
    // Else, there were missing fields
    if (fieldCount != -1) {
        cerr << "Missing field(s) for Airport." << endl;
    }

    // Delete the object
    delete tmp;
}

Flightplan* Input::readFlightplan(TiXmlElement *elem) {
    // Keep track of how many fields the element has
    int fieldCount = 0;

    // Make new object
    Flightplan *tmp = new Flightplan();

    //  We iterate over all members, check if it's a valid element and if so, add it to our object.
    for (; elem != NULL; elem = elem->NextSiblingElement()) {
        if (strcmp(elem->Value(), "destination") == 0) {
            // Check for duplicate data
            if (tmp->getDestination() != "") {
                cerr << "Duplicate data in Flightplan." << endl;
                fieldCount = -1;
                break;
            }

            // Set destination
            tmp->setDestination(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "departure") == 0) {
            // Check for duplicate data
            if (tmp->getDeparture() != -1) {
                cerr << "Duplicate data in Flightplan." << endl;
                fieldCount = -1;
                break;
            }

            // Set destination
            tmp->setDeparture(atoi(elem->GetText()));

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "arrival") == 0) {
            // Check for duplicate data
            if (tmp->getArrival() != -1) {
                cerr << "Duplicate data in Flightplan." << endl;
                fieldCount = -1;
                break;
            }

            // Set destination
            tmp->setArrival(atoi(elem->GetText()));

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "interval") == 0) {
            // Check for duplicate data
            if (tmp->getInterval() != -1) {
                cerr << "Duplicate data in Flightplan." << endl;
                fieldCount = -1;
                break;
            }

            // Set destination
            tmp->setInterval(atoi(elem->GetText()));

            // Increase fieldCount
            fieldCount++;
        }

        else {
            cerr << "Invalid field: " << elem->Value() << endl;
            fieldCount = -1;
            break;
        }
    }

    // If there were 4 field (all fields present), add the Airport to our system.
    if (fieldCount == 4) {
        Input::addFlightplan(tmp);
        return tmp;
    }

    // Something went wrong, if the field count is -1, an error msg has already been logged
    // Else, there were missing fields
    if (fieldCount != -1) {
        cerr << "Missing field(s) for Airport." << endl;
    }

    // Delete the object
    delete tmp;

    return NULL;
}

void Input::addAirport(Airport *airport) {
    // Initialize gateStack
    airport->initStack();

    // Add to vec
    airports.push_back(airport);

    ENSURE(airports.back() == airport, "Airplane was not added to simulation.");
}

void Input::addRunway(Runway *runway) {
    runways.push_back(runway);
    ENSURE(runways.back() == runway, "Runway was not added to simulation.");
}

void Input::addFlightplan(Flightplan *flightplan) {
    flightplans.push_back(flightplan);
    ENSURE(flightplans.back() == flightplan, "Flightplan was not added to simulation.");
}


void Input::addAirplane(Airplane *airplane) {
    REQUIRE(!airports.empty(), "No airport available so can't add airplane");

    // Get airport
    Airport* airport = airports[0];

    // Set gate
    if (airplane->getStatus() == kGate) {
        // Get gate from airport, if nothing available it will end the program
        airplane->setGateID(airport->getFreeGate());
    }
    else {
        airplane->setGateID(-1);
    }
}

Airport *Input::findAirportByIATA(const string& iata) const {
    // Check all Airports and if the Airport matches the IATA, return this Airport.
    vector<Airport*>::const_iterator itr;
    for (itr = airports.begin(); itr < airports.end(); ++itr) {
        Airport* cur_ap = *itr;
        if (cur_ap->getIata() == iata) {
            return cur_ap;
        }
    }
    return NULL;
}

vector<Airport*> Input::getAirports() const {
    return Input::airports;
}

vector<Runway*> Input::getRunways() const {
    return Input::runways;
}

vector<Flightplan*> Input::getFlightplans() const {
    return flightplans;
}