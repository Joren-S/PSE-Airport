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
#include "Graphics.h"

/**
 * Class that represents the airport in a simulation
 */
class Airport {
public:

    /**
     * Default constructor
     * \n ENSURE(properlyInitialized(), "Airport wasn't properly initialized after constructing");
     */
    Airport();

    /**
     * Destructor
     */
    ~Airport();

    /**
     * Checks if the object is properly initialized
     * @return: Boolean indicating if properly initialized or not.
     */
    bool properlyInitialized() const;

    /**
     * Initializes the gateStack
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling initStack.");
     * \n REQUIRE(fGateStack.empty(), "Can't initialize gate stack, already in use.");
     */
    void initStack();

    /**
     * Returns the ID of a free gate
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getFreeGate.");
     * \n REQUIRE(!fGateStack.empty(), "Can't get free gate: no gate in stack.");
     * \n REQUIRE(fGates > 0, "Airport has no gates.");
     * \n ENSURE(id <= fGates && id > 0, "Gate has an invalid ID.");
     * \n ENSURE(fGateStack.top() != id, "Gate wasn't properly popped from the stack.");
     * @return: ID of a free gate.
     */
    int getFreeGate();

    /**
     * Restores a gate, making it available for use again.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling restoreGate.");
     * \n REQUIRE(id <= fGates && id > 0, "Gate ID is invalid.");
     * \n REQUIRE(fGates > 0, "Airport has no gates.");
     * \n ENSURE(fGateStack.top() == id, "Gate was not properly added to stack");
     * @param id: ID of gate to restore
     */
    void restoreGate(int id);

    /**
     * Searches the runway with the given taxipoint.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getRunway.");
     * @return: Pointer to the found runway, NULL if nothing is found.
     */
    Runway* getRunway(const std::string&) const;

    /**
     * Gets the next runway (for taxiing) based on what the airplane is doing.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getNextRunway.");
     * \n REQUIRE(airplane != NULL, "Airplane cannot be NULL.");
     * @param airplane: Airplane that is taxiing.
     * @return: Pointer to the found runway, NULL if nothing is found.
     */
    Runway* getNextRunway(Airplane* airplane) const;

    /**
     * Adds a runway to the airport.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling addRunway.");
     * \n REQUIRE(runway != NULL, "Runway cannot be NULL.");
     * \n ENSURE(!present, "Runway is already in system.");
     * \n ENSURE(fRunways.back() == runway, "Runway was not properly added to the system.");
     * @param runway: Pointer to runway that needs to be added.
     */
    void addRunway(Runway* runway);

    /**
     * Looks for a runway that is free and is suitable for a given airplane.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getFreeRunway.");
     * \n REQUIRE(plane != NULL, "Plane object does not exist.");
     * @param plane: Plane that needs the runway.
     * @return: Pointer to runway, NULL if nothing is found.
     */
    Runway* getFreeRunway(Airplane* plane) const;

    /**
     * Return the amount of runways the airport has.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling amountOfRunways.");
     * @return: Amount of runways.
     */
    size_t amountOfRunways() const;

    /**
     * Checks if all the data members were initialized
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling complete.");
     * @return: Boolean indicating if everything was initialized.
     */
    bool complete() const;

    /**
     * Generates a graphical impression for the current state of the airport.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling drawImpression.");
     * @param time: time of the impression.
     * @param plans: the flight plans to access the planes
     * @return string containing the impression.
     */
    std::string drawImpression(const Time& time, const std::vector<Flightplan *>& plans) const;

    std::string graphicsINI(const std::vector<Flightplan *>& plans);

    /**
     * Set the amount of gates in the airport.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
     * \n REQUIRE(fGates >= 0, "Number of gates cannot be negative!");
     * \n ENSURE(fGates == gates, "Field wasn't set properly");
     * @param fGates: Amount of gates.
     */
    void setGates(int fGates);

    /**
     * Getters and setters for the fields of the class.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
     *
     * Setters:
     * \n ENSURE(fField == value, "Field wasn't set properly");
     */
    const std::string &getName() const;
    void setName(const std::string &fName);
    const std::string &getIata() const;
    void setIata(const std::string &fIata);
    const std::string &getCallsign() const;
    void setCallsign(const std::string &fCallsign);
    int getGates() const;
    std::vector<Runway*> getRunways() const;

private:

    /**
     * Pointer to itself
     */
    Airport *fInitCheck;

    /**
     * Members indicating name, iata and callsign
     */
    std::string fName, fIata, fCallsign;

    /**
     * Amount of gates in the airport
     */
    int fGates;

    /**
     * Stack of available gates
     */
    std::stack<int> fGateStack;

    /**
     * Vector of all the runways the airport contains
     */
    std::vector<Runway*> fRunways;

};


#endif //PROJECTVLIEGVELD_AIRPORTS_H
