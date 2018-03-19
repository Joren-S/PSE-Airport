//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_SYSTEM_H
#define PROJECTVLIEGVELD_SYSTEM_H

#include "types.h"

using namespace std;

class System {
private:
    vector<airport*> airports;
    vector<airplane*> airplanes;
    vector<runway*> runways;

    // add new objects
    bool addAirport(const string& name, const string& iata, const string& callsign, int numGates);
    bool addRunway(const string& name, const string& airport);
    bool addAirplane(const string& number, const string& callsign, const string& model, int status);

    // helper functions
    airport* findAirportByIATA(const string& iata);
    int runwaysInAirport(airport *ap) const;
    runway* getFreeRunway(airport* ap) const;

    // getters
    vector<airport*> getAirports();
    vector<runway*> getRunways();
    vector<airplane*> getAirplanes();

    // use cases
    void land(airplane*, airport*) const;
    void gate(airplane*, airport*) const;
    void takeoff(airplane*, airport*) const;

public:
    // main
    void setup(const string& filename);
    void log(const string& filename = "../log.txt");
    void run();

    // debug info
    void info();
};

#endif //PROJECTVLIEGVELD_SYSTEM_H
