//============================================================================
// Name        : Airport.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_AIRPORTS_H
#define PROJECTVLIEGVELD_AIRPORTS_H

#include <string>
#include <stack>
#include <vector>
#include <sstream>

#include "DesignByContract.h"
#include "Runway.h"
#include "Time.h"
#include "Flightplan.h"

using namespace std;

class Airport {
private:
    /**
     * Pointer to itself
     */
    Airport *fInitCheck;

    /**
     * Members indicating name, iata and callsign
     */
    string fName, fIata, fCallsign;

    /**
     * Amount of gates in the airport
     */
    int fGates;

    /**
     * Stack of available gates
     */
    stack<int> fGateStack;

    /**
     * Vector of all the runways the airport contains
     */
    vector<Runway*> fRunways;

public:

    /**
     * Default constructor
     * ENSURE(properlyInitialized(), "Airport wasn't properly initialized after constructing");
     */
    Airport();

    /**
     * Checks if the object is properly initialized
     * @return: Boolean indicating if properly initialized or not.
     */
    bool properlyInitialized() const;

    /**
     * Initializes the gateStack
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling initStack.");
     * REQUIRE(fGateStack.empty(), "Can't initialize gate stack, already in use.");
     */
    void initStack();

    /**
     * Returns the ID of a free gate
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getFreeGate.");
     * REQUIRE(!fGateStack.empty(), "Can't get free gate: no gate in stack.");
     * REQUIRE(fGates > 0, "Airport has no gates.");
     * ENSURE(id <= fGates && id > 0, "Gate has an invalid ID.");
     * ENSURE(fGateStack.top() != id, "Gate wasn't properly popped from the stack.");
     * @return: ID of a free gate.
     */
    int getFreeGate();

    /**
     * Restores a gate, making it available for use again.
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling restoreGate.");
     * REQUIRE(id <= fGates && id > 0, "Gate ID is invalid.");
     * REQUIRE(fGates > 0, "Airport has no gates.");
     * ENSURE(fGateStack.top() == id, "Gate was not properly added to stack");
     * @param id: ID of gate to restore
     */
    void restoreGate(int id);

    /**
     * Searches the runway with the given taxipoint.
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getRunway.");
     * @return: Pointer to the found runway, NULL if nothing is found.
     */
    Runway* getRunway(const string&) const;

    /**
     * Gets the next runway (for taxiing) based on what the airplane is doing.
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getNextRunway.");
     * REQUIRE(airplane != NULL, "Airplane cannot be NULL.");
     * @param airplane: Airplane that is taxiing.
     * @return: Pointer to the found runway, NULL if nothing is found.
     */
    Runway* getNextRunway(Airplane* airplane) const;

    /**
     * Adds a runway to the airport.
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling addRunway.");
     * REQUIRE(runway != NULL, "Runway cannot be NULL.");
     * ENSURE(!present, "Runway is already in system.");
     * ENSURE(fRunways.back() == runway, "Runway was not properly added to the system.");
     * @param runway: Pointer to runway that needs to be added.
     */
    void addRunway(Runway* runway);

    /**
     * Looks for a runway that is free and is suitable for a given airplane.
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getFreeRunway.");
     * REQUIRE(plane != NULL, "Plane object does not exist.");
     * @param plane: Plane that needs the runway.
     * @return: Pointer to runway, NULL if nothing is found.
     */
    Runway* getFreeRunway(Airplane* plane) const;

    /**
     * Return the amount of runways the airport has.
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling amountOfRunways.");
     * @return: Amount of runways.
     */
    size_t amountOfRunways() const;

    /**
     * Checks if all the data members were initialized
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling complete.");
     * @return: Boolean indicating if everything was initialized.
     */
    bool complete() const;

    /**
     * Generates a graphical impression for the current state of the airport.
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling drawImpression.");
     * @param time: time of the impression.
     * @param stream: ostream to write the impression to.
     */
    void drawImpression(Time time, ostream& stream, vector<Flightplan*> plans);

    /**
     * Set the amount of gates in the airport.
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
     * REQUIRE(fGates >= 0, "Number of gates cannot be negative!");
     * ENSURE(fGates == gates, "Field wasn't set properly");
     * @param fGates: Amount of gates.
     */
    void setGates(int fGates);

    /**
     * Getters and setters for the fields of the class.
     * REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
     *
     * Setters:
     * ENSURE(fField == value, "Field wasn't set properly");
     */
    const string &getName() const;
    void setName(const string &fName);
    const string &getIata() const;
    void setIata(const string &fIata);
    const string &getCallsign() const;
    void setCallsign(const string &fCallsign);
    int getGates() const;
    vector<Runway*> getRunways() const;
};


#endif //PROJECTVLIEGVELD_AIRPORTS_H
