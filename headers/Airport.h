//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_AIRPORTS_H
#define PROJECTVLIEGVELD_AIRPORTS_H

#include <string>
#include <stack>
#include "DesignByContract.h"
#include "Runway.h"

using namespace std;

class Airport {
private:

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
     * Set int values to -1, to check if field are initialized
     */
    Airport();

    /**
     * Initializes the gateStack
     * REQUIRE: gateStack is empty
     */
    void initStack();

    /**
     * Returns the ID of a free gate
     * REQUIRE: at least one gate available
     */
    int getFreeGate();

    /**
     * Restores a gate, i.e. makes one available
     * REQUIRE: given gate was unavailable &&
     * gate exists
     * ENSURE: gate was added
     * @param id of gate
     */
    void restoreGate(int id);

    /**
     * Adds a runway to the airport
     * REQUIRE: runway not present &&
     * not nullptr
     * ENSURE: added in order
     */
    void addRunway(Runway*);

    /**
     * Return the first free runway in the airport
     */
    Runway* getFreeRunway() const;

    /**
     * Getter for the runways
     */
    vector<Runway*> getRunways() const;

    /**
     * Return the amount of runways the airport has
     */
    size_t amountOfRunways() const;

    /**
     * Checks if all the data members were initialized
     */
    bool complete() const;

    // Getters and Setters
    const string &getName() const;
    void setName(const string &fName);
    const string &getIata() const;
    void setIata(const string &fIata);
    const string &getCallsign() const;
    void setCallsign(const string &fCallsign);
    int getGates() const;
    void setGates(int fGates);

};


#endif //PROJECTVLIEGVELD_AIRPORTS_H
