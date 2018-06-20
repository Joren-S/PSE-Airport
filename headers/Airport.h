//============================================================================
// Name        : Airport.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_AIRPORTS_H
#define PROJECTVLIEGVELD_AIRPORTS_H

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "DesignByContract.h"
#include "Runway.h"
#include "Time.h"
#include "FlightPlan.h"
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
     * Checks if all the data members were initialized
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling complete.");
     * @return: Boolean indicating if everything was initialized.
     */
    bool complete() const;

    /**
     * Initializes the gateStack
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling initGates.");
     * \n REQUIRE(getGateMap().empty(), "Can't initialize gate map, already in use.");
     */
    void initGates();

    /**
     * Returns the ID of a free gate
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getFreeGate.");
     * \n REQUIRE(getGates() > 0, "Airport has no gates.");
     * \n REQUIRE(!getGateMap().empty(), "Gate map not initialized yet");
     * @return: ID of a free gate. -1 if nothing available
     */
    int getFreeGate();

    /**
     * Restores a gate, making it available for use again.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling restoreGate.");
     * \n REQUIRE(id <= getGates() && id > 0, "Gate ID is invalid.");
     * \n REQUIRE(getGateMap()[id], "Gate has to be in use to restore it");
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
     * Adds a runway to the airport. Present is a boolean indicating if the object is already in the getRunways()
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling addRunway.");
     * \n REQUIRE(runway != NULL, "Runway cannot be NULL.");
     * \n ENSURE(!present, "Runway is already in system.");
     * \n ENSURE(getRunways().back() == runway, "Runway was not properly added to the system.");
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
     * Generates a graphical impression for the current state of the airport.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling drawImpression.");
     * @param time: time of the impression.
     * @param plans: the flight plans to access the planes
     * @return string containing the impression.
     */
    std::string drawImpression(const Time& time, const std::vector<FlightPlan *>& plans) const;

    /**
     * Generates a string containing the info for use with the graphics engine
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling graphicsINI.");
     * @param plans: the flight plans
     * @return string containing ini file
     */
    std::string graphicsINI(const std::vector<FlightPlan *>& plans);

    /**
     * Set the amount of gates in the airport.
     * \n REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
     * \n REQUIRE(getGates() >= 0, "Number of gates cannot be negative!");
     * \n ENSURE(getGates() == gates, "Field wasn't set properly");
     * @param gates: Amount of gates.
     */
    void setGates(int gates);

    //////////////
    /// Getters and setters
    /// For all: REQUIRE(properlyInitialized(), "Airport wasn't properly initialized when calling getter/setter.");
    /// For setters: ENSURE(getField() == value, "Field wasn't set properly");
    /// Where getField() is flexible
    //////////////

    const std::string &getName() const;
    void setName(const std::string &fName);
    const std::string &getIata() const;
    void setIata(const std::string &fIata);
    const std::string &getCallsign() const;
    void setCallsign(const std::string &fCallsign);
    int getGates() const;
    std::vector<Runway*> getRunways() const;
    std::map<int, bool> getGateMap() const;

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
     * Map with all the gates and boolean indicating if free
     */
    std::map<int, bool> fGateMap;

    /**
     * Vector of all the runways the airport contains
     */
    std::vector<Runway*> fRunways;

};


#endif //PROJECTVLIEGVELD_AIRPORTS_H
