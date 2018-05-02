//============================================================================
// Name        : Input.cpp
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#include "../headers/Input.h"

void Input::read(const string &filename, ostream& errorLog) {
    // Load xml file, program will end if failed
    TiXmlDocument xml;
    string error = "Couldn't open " + filename + ".";
    REQUIRE(xml.LoadFile(filename.c_str()), error.c_str());

    // We iterate over all root elements.
    for (TiXmlElement *root = xml.FirstChildElement(); root != NULL; root = root->NextSiblingElement()) {

        // Airports
        if (strcmp(root->Value(), "AIRPORT") == 0) {
            readAirport(root->FirstChildElement(), errorLog);
        }

        // Runways
        else if (strcmp(root->Value(), "RUNWAY") == 0) {
            readRunway(root->FirstChildElement(), errorLog);
        }

        // Airplanes
        else if (strcmp(root->Value(), "AIRPLANE") == 0) {
            readAirplane(root->FirstChildElement(), errorLog);
        }

        // Invalid element
        else {
            errorLog << "Did not recognize element: " << root->Value() << endl;
        }
    }

    // We are finished with our XML file, so we clear it.
    xml.Clear();
}

Input::Input() {
    fInitCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

void Input::readAirplane(TiXmlElement *elem, ostream& errorLog) {
    // Keep track of how many fields the element has
    int fieldCount = 0;

    // Make new object
    Airplane *tmp = new Airplane();

    // Pointer to the associated flightplan
    Flightplan* flightplan = NULL;

    //  We iterate over all members, check if it's a valid element and if so, add it to our object.
    for (; elem != NULL; elem = elem->NextSiblingElement()) {

        // Number
        if (strcmp(elem->Value(), "number") == 0) {
            tmp->setNumber(elem->GetText());
        }

        // Callsign
        else if (strcmp(elem->Value(), "callsign") == 0) {
            tmp->setCallsign(elem->GetText());
        }

        // Model
        else if (strcmp(elem->Value(), "model") == 0) {
            tmp->setModel(elem->GetText());
        }

        // Type
        else if (strcmp(elem->Value(), "type") == 0) {
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
                errorLog << "Invalid data for Airplane type" << endl;
                fieldCount = -1;
                break;
            }
        }

        // Engine
        else if (strcmp(elem->Value(), "engine") == 0) {
            if (strcmp(elem->GetText(), "propeller") == 0) {
                tmp->setEngine(kPropeller);
            }
            else if (strcmp(elem->GetText(), "jet") == 0) {
                tmp->setEngine(kJet);
            }
            else {
                errorLog << "Invalid data for Airplane engine." << endl;
                fieldCount = -1;
                break;
            }
        }

        // Size
        else if (strcmp(elem->Value(), "size") == 0) {
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
                errorLog << "Invalid data for Airplane size." << endl;
                fieldCount = -1;
                break;
            }
        }

        // Passengers
        else if (strcmp(elem->Value(), "passengers") == 0) {
            tmp->setPassengers(atoi(elem->GetText()));
        }

        // Fuel
        else if (strcmp(elem->Value(), "fuel") == 0) {
            tmp->setFuel(atoi(elem->GetText()));
        }

        // Flightplan
        else if (strcmp(elem->Value(), "FLIGHTPLAN") == 0) {
            // Check if there already was a flightplan
            if (flightplan != NULL) {
                fieldCount = -1;
                errorLog << "Didn't add airplane because of multiple flightplans." << endl;
                break;
            }

            // Read the flightplan, it returns a pointer to the object
            flightplan = readFlightplan(elem->FirstChildElement());

            // Flightplan is NULL when there was an error
            if (flightplan == NULL) {
                fieldCount = -1;
                errorLog << "Didn't add airplane because of invalid flightplan." << endl;
                break;
            }

            // Set gate and altitude of plane
            if (flightplan->getArrival() > flightplan->getDeparture()) {
                tmp->setStatus(kParked);
                tmp->setGateID(airports[0]->getFreeGate());
            }

            // Set plane in flightplan
            flightplan->setAirplane(tmp);
        }

        // Invalid field
        else {
            errorLog << "Invalid field: " << elem->Value() << endl;
            fieldCount = -1;
            break;
        }

        // Increase fieldcount
        fieldCount++;
    }

    // If all fields were present, no errors and airplane is complete, add the Airplane to our system.
    if (fieldCount == 9 and tmp->complete()) {
        return;
    }

    // Something went wrong, if the field count is -1, an error msg has already been logged
    // Else, there were missing fields
    if (fieldCount != -1) {
        errorLog << "Duplicate or missing field(s) for Airplane." << endl;
    }

    if (flightplan != NULL) {
        // Restore gate in airport
        if (tmp->getGateID() != -1) {
            airports[0]->restoreGate(tmp->getGateID());
        }

        // Delete the flightplan
        delete flightplan;

        // Pop flightplan from vector
        flightplans.pop_back();
    }
}

void Input::readRunway(TiXmlElement *elem, ostream& errorLog) {
    // Keep track of how many fields the element has
    int fieldCount = 0;

    // Make new object
    Runway *tmp = new Runway();

    //  We iterate over all members, check if it's a valid element and if so, add it to our object.
    for (; elem != NULL; elem = elem->NextSiblingElement()) {

        // Name
        if (strcmp(elem->Value(), "name") == 0) {
            tmp->setName(elem->GetText());
        }

        // Airport
        else if (strcmp(elem->Value(), "airport") == 0) {
            // Get airport
            Airport* airport = findAirportByIATA(elem->GetText());

            // No airport found
            if (airport == NULL) {
                errorLog << "Did not find airport with iata: " << elem->GetText() << endl;
                fieldCount = -1;
                break;
            }

            // Set runway airport
            tmp->setAirport(airport);
        }

        // Type
        else if (strcmp(elem->Value(), "type") == 0) {
            if (strcmp(elem->GetText(), "grass") == 0) {
                tmp->setType(kGrass);
            }
            else if (strcmp(elem->GetText(), "asphalt") == 0) {
                tmp->setType(kAsphalt);
            }
            else {
                errorLog << "Invalid data for Runway status." << endl;
                fieldCount = -1;
            }
        }

        // Length
        else if (strcmp(elem->Value(), "length") == 0) {
            tmp->setLength(atoi(elem->GetText()));
        }

        // Taxiroute
        else if (strcmp(elem->Value(), "TAXIROUTE") == 0) {
            //
            if (tmp->getAirport() == NULL) {
                errorLog << "Can't check taxiroute if runway doesn't specify airport first." << endl;
                fieldCount = -1;
                break;
            }
            // Read the taxiroute
            string taxipoint = readTaxiroute(elem->FirstChildElement(), tmp->getAirport());

            // Not valid
            if (taxipoint.empty()) {
                fieldCount = -1;
                break;
            }

            // Valid
            tmp->setTaxiPoint(taxipoint);
        }

        // Invalid field
        else {
            errorLog << "Invalid field: " << elem->Value() << endl;
            fieldCount = -1;
            break;
        }

        // Increase fieldcount
        fieldCount++;
    }

    // If all fields present, no errors and runway complete, add the Runway to our system.
    if (fieldCount == 5 and tmp->complete()) {
        Input::addRunway(tmp);
        return;
    }

    // Something went wrong, if the field count is -1, an error msg has already been logged
    // Else, there were missing fields
    if (fieldCount != -1) {
        errorLog << "Duplicate or missing field(s) for Runway." << endl;
    }

    // Delete the object
    delete tmp;
}

string Input::readTaxiroute(TiXmlElement *elem, Airport* airport, ostream& errorLog) {
    string returnstr;
    string error = "Inconsistent taxiroute data, runway will not be added.";
    bool taxipoint = true;
    unsigned int index = 0;
    vector<Runway*> runways = airport->getRunways();
    for (; elem != NULL; elem = elem->NextSiblingElement()) {

        // Check what type of element it is
        bool isTaxipoint = strcmp(elem->Value(), "taxipoint") == 0;
        bool isCrossing = strcmp(elem->Value(), "crossing") == 0;

        // Expected a taxipoint and got one
        if (taxipoint and isTaxipoint) {

            // If the index is <= the amount of runways, it's correct
            if (index >= runways.size()) {
                returnstr = elem->GetText();
            }

            else if (strcmp(runways[index]->getTaxiPoint().c_str(), elem->GetText()) != 0) {
                errorLog << error << endl;
                return "";
            }
        }

        // Expected a crossing and got one
        else if (!taxipoint and isCrossing) {

            if (index >= runways.size()) {
                errorLog << error << endl;
                return "";
            }

            bool correctCrossing = strcmp(runways[index]->getName().c_str(), elem->GetText()) == 0;

            if (!correctCrossing) {
                errorLog << error << endl;
                return "";
            }
        }

        else {
            errorLog << error << endl;
            return "";
        }

        // Increase index when needed
        if (!taxipoint) {
            index++;
        }

        // Invert taxipoint
        taxipoint = !taxipoint;
    }
    return returnstr;
}

void Input::readAirport(TiXmlElement *elem, ostream& errorLog) {
    // Keep track of how many fields the element has
    int fieldCount = 0;

    // Make new object
    Airport *tmp = new Airport();

    //  We iterate over all members, check if it's a valid element and if so, add it to our object.
    for (; elem != NULL; elem = elem->NextSiblingElement()) {

        // Name
        if (strcmp(elem->Value(), "name") == 0) {
            tmp->setName(elem->GetText());
        }

        // IATA
        else if (strcmp(elem->Value(), "iata") == 0) {
            tmp->setIata(elem->GetText());
        }

        // Callsign
        else if (strcmp(elem->Value(), "callsign") == 0) {
            tmp->setCallsign(elem->GetText());
        }

        // Gates
        else if (strcmp(elem->Value(), "gates") == 0) {
            tmp->setGates(atoi(elem->GetText()));
        }

        // Invalid field
        else {
            errorLog << "Invalid field: " << elem->Value() << endl;
            fieldCount = -1;
            break;
        }

        // Increase fieldcount
        fieldCount++;
    }

    // If there were 4 field (all fields present), add the Airport to our system.
    if (fieldCount == 4 and tmp->complete()) {
        Input::addAirport(tmp);
        return;
    }

    // Something went wrong, if the field count is -1, an error msg has already been logged
    // Else, there were missing fields
    if (fieldCount != -1) {
        errorLog << "Duplicate or missing field(s) for Airport." << endl;
    }

    // Delete the object
    delete tmp;
}

Flightplan* Input::readFlightplan(TiXmlElement *elem, ostream& errorLog) {
    // Keep track of how many fields the element has
    int fieldCount = 0;

    // Make new object
    Flightplan *tmp = new Flightplan();

    //  We iterate over all members, check if it's a valid element and if so, add it to our object.
    for (; elem != NULL; elem = elem->NextSiblingElement()) {

        // Destination
        if (strcmp(elem->Value(), "destination") == 0) {
            tmp->setDestination(elem->GetText());
        }

        // Departure
        else if (strcmp(elem->Value(), "departure") == 0) {
            tmp->setDeparture(atoi(elem->GetText()));
        }

        // Arrival
        else if (strcmp(elem->Value(), "arrival") == 0) {
            tmp->setArrival(atoi(elem->GetText()));
        }

        // Interval
        else if (strcmp(elem->Value(), "interval") == 0) {
            tmp->setInterval(atoi(elem->GetText()));
        }

        // Invalid field
        else {
            errorLog << "Invalid field: " << elem->Value() << endl;
            fieldCount = -1;
            break;
        }

        // Increase fieldcount
        fieldCount++;
    }

    // If there were 4 field (all fields present), add the Airport to our system.
    if (fieldCount == 4 and tmp->complete()) {
        Input::addFlightplan(tmp);
        return tmp;
    }

    // Something went wrong, if the field count is -1, an error msg has already been logged
    // Else, there were missing fields
    if (fieldCount != -1) {
        errorLog << "Duplicate or missing field(s) for Flightplan." << endl;
    }

    // Delete the object
    delete tmp;

    return NULL;
}

void Input::addAirport(Airport *airport) {
    REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addAirport");
    REQUIRE(airport->complete(), "Airport has to be completely initialized to add it to the simulation");
    // Initialize gateStack
    airport->initStack();

    // Add to vec
    airports.push_back(airport);

    ENSURE(airports.back() == airport, "Airplane was not added to simulation.");
}

void Input::addRunway(Runway *runway) {
    REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addRunway");
    REQUIRE(runway->complete(), "Runway has to be completely initialized to add it to the simulation");
    runway->getAirport()->addRunway(runway);
    ENSURE(runway->getAirport()->getRunways().back() == runway, "Runway was not added to the airport");
}

void Input::addFlightplan(Flightplan *flightplan) {
    REQUIRE(this->properlyInitialized(), "Input was't initialized when calling addFlightplan");
    REQUIRE(flightplan->complete(), "Flightplan has to be completely initialized to add it to the simulation");
    flightplans.push_back(flightplan);
    ENSURE(flightplans.back() == flightplan, "Flightplan was not added to simulation.");
}

Airport *Input::findAirportByIATA(const string& iata) const {
    REQUIRE(this->properlyInitialized(), "Input was't initialized when calling findAirportByIATA");
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
    REQUIRE(this->properlyInitialized(), "Input was't initialized when calling getAirports");
    return Input::airports;
}

vector<Flightplan*> Input::getFlightplans() const {
    REQUIRE(this->properlyInitialized(), "Input was't initialized when calling getFlightplans");
    return flightplans;
}

bool Input::properlyInitialized() const {
    return fInitCheck == this;
}