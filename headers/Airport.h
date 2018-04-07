//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_AIRPORTS_H
#define PROJECTVLIEGVELD_AIRPORTS_H

#include <string>
#include <stack>
#include "DesignByContract.h"

using namespace std;

class Airport {
private:

    /**
     * Members indication name, iata and callsign
     */
    string fName, fIata, fCallsign;

    /**
     * Amount of gate in the airport
     */
    int fGates;

    /**
     * Stack of available gates
     */
    stack<int> fGateStack;

public:

    /**
     * Constructor
     */
    Airport(const string& name, const string& iata, const string& callsign, int gates);

    /**
     * Default constructor
     */
    Airport() {}

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

    // Getters and Setters
    const string &getFName() const;
    void setFName(const string &fName);
    const string &getFIata() const;
    void setFIata(const string &fIata);
    const string &getFCallsign() const;
    void setFCallsign(const string &fCallsign);
    int getFGates() const;
    void setFGates(int fGates);

};


#endif //PROJECTVLIEGVELD_AIRPORTS_H
