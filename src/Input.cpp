//
// Created by Max on 4/7/2018.
//

#include "../headers/Input.h"

Input::Input(const string &filename) {
    // Load xml file, program will end if failed
    TiXmlDocument xml;
    string error = "Couldn't open " + filename + ".";
//    REQUIRE(xml.LoadFile(filename.c_str()), error.c_str());

    // Temporary for when DesignByContract.h doesn't work
    if (!xml.LoadFile(filename.c_str()))  {
        cerr << "Couldn't open " + filename + "." << endl;
        return;
    }

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

    //  We iterate over all members, check if it's a valid element and if so, add it to our object.
    for (; elem != NULL; elem = elem->NextSiblingElement()) {

        if (strcmp(elem->Value(), "number") == 0) {
            // Check for duplicate data
            if (tmp->getFNumber() != "") {
                cerr << "Duplicate data in Airplane." << endl;
                fieldCount = -1;
                break;
            }

            // Set airplane number
            tmp->setFNumber(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "callsign") == 0) {
            // Check for duplicate data
            if (tmp->getFCallsign() != "") {
                cerr << "Duplicate data in Airplane." << endl;
                fieldCount = -1;
                break;
            }

            // Set airplane callsign
            tmp->setFCallsign(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "model") == 0) {
            // Check for duplicate data
            if (tmp->getFModel() != "") {
                cerr << "Duplicate data in Airplane." << endl;
                fieldCount = -1;
                break;
            }

            // Set airplane model
            tmp->setFModel(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "status") == 0) {
            // Check for duplicate data
            if (tmp->getFStatus() != 0) {
                cerr << "Duplicate data in Airplane status." << endl;
                fieldCount = -1;
                break;
            }

            // Set status
            if (strcmp(elem->GetText(), "Gate") == 0) {
                tmp->setFStatus(kGate);
                tmp->setAltitude(0);
            }
            else if (strcmp(elem->GetText(), "Approaching") == 0) {
                tmp->setFStatus(kApproaching);
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
        else {
            cerr << "Invalid field: " << elem->Value() << endl;
            fieldCount = -1;
            break;
        }
    }

    // If there were 4 field (all fields present), add the Airplane to our system.
    if (fieldCount == 4) {
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
            if (tmp->getFName() != "") {
                cerr << "Duplicate data in Runway." << endl;
                fieldCount = -1;
                break;
            }

            // Set runway name
            tmp->setFName(elem->GetText());

            // Increase fieldcount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "airport") == 0) {
            // Check for duplicate data
            if (tmp->getFAirport() != NULL) {
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
            tmp->setFAirport(airport);

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
            if (tmp->getFName() != "") {
                cerr << "Duplicate data in Airport." << endl;
                fieldCount = -1;
                break;
            }

            // Set airport name
            tmp->setFName(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "iata") == 0) {
            // Check for duplicate data
            if (tmp->getFIata() != "") {
                cerr << "Duplicate data in Airport." << endl;
                fieldCount = -1;
                break;
            }

            // Set airport iata
            tmp->setFIata(elem->GetText());

            // Increase fieldCount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "callsign") == 0) {
            // Check for duplicate data
            if (tmp->getFCallsign() != "") {
                cerr << "Duplicate data in Airport." << endl;
                fieldCount = -1;
                break;
            }

            // Set airport callsign
            tmp->setFCallsign(elem->GetText());

            // Increase fieldcount
            fieldCount++;
        }
        else if (strcmp(elem->Value(), "gates") == 0) {
            // Check for duplicate data
            if (tmp->getFGates() != -1) {
                cerr << "Duplicate data in Airport." << endl;
                fieldCount = -1;
                break;
            }

            // Set airport gates
            tmp->setFGates(atoi(elem->GetText()));

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

void Input::addAirport(Airport *airport) {
    // Initialize gateStack
    airport->initStack();

    // Add to vec
    airports.push_back(airport);

    // Check if succesfully added
    string error = "Airplane was not added to simulation.";
//    ENSURE(airports.back() == airport, error.c_str());
}

void Input::addRunway(Runway *runway) {
    runways.push_back(runway);

    // Check if succesfully added
    string error = "Runway was not added to simulation.";
//    ENSURE(runways.back() == runway, error.c_str());
}

void Input::addAirplane(Airplane *airplane) {
    // An airport has to be available
    string error = "No airport available so can't add airplane";
//    REQUIRE(!airports.empty(), error.c_str());

    // Get airport
    Airport* airport = airports[0];

    // Set gate
    if (airplane->getFStatus() == kGate) {
        // Get gate from airport, if nothing available it will end the program
        airplane->setFGateID(airport->getFreeGate());
    }
    else {
        airplane->setFGateID(-1);
    }

    // Add the airplane to the simulation
    airplanes.push_back(airplane);

    // Make sure the airplane has been added
    error = "Airplane was not added to simulation.";
//    ENSURE(airplanes.back() == airplane, error.c_str());
}

Airport *Input::findAirportByIATA(const string& iata) const {
    // Check all Airports and if the Airport matches the IATA, return this Airport.
    vector<Airport*>::const_iterator itr;
    for (itr = airports.begin(); itr < airports.end(); ++itr) {
        Airport* cur_ap = *itr;
        if (cur_ap->getFIata() == iata) {
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

vector<Airplane*> Input::getAirplanes() const {
    return Input::airplanes;
}