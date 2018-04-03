//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_SYSTEM_H
#define PROJECTVLIEGVELD_SYSTEM_H

#include "types.h"

using namespace std;

class System {
private:
    vector<Airport*> airports;
    vector<Airplane*> airplanes;
    vector<Runway*> runways;

    // add new objects
    bool addAirport(const string& name, const string& iata, const string& callsign, int numGates);
    bool addRunway(const string& name, const string& Airport);
    bool addAirplane(const string& number, const string& callsign, const string& model, int status);

    // helper functions
    Airport* findAirportByIATA(const string& iata);
    int runwaysInAirport(Airport *ap) const;
    Runway* getFreeRunway(Airport* ap) const;



    // use cases
    void land(Airplane*, Airport*) const;
    void gate(Airplane*, Airport*) const;
    void takeoff(Airplane*, Airport*) const;

public:
    // main
    void setup(const string& filename);
    void log(const string& filename = "../log.txt");
    void run();

    // debug info
    void info();

    // getters
    vector<Airport*> getAirports();
    vector<Runway*> getRunways();
    vector<Airplane*> getAirplanes();
};

#endif //PROJECTVLIEGVELD_SYSTEM_H
