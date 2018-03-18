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
public:
    // main
    void setup(const string& filename);
    void log(const string& filename = "log.txt");
    void run();

    // add new objects
    bool addAirport(const string& name, const string& iata, const string& callsign, int numGates);
    bool addRunway(const string& name, const string& airport);
    bool addAirplane(const string& number, const string& callsign, const string& model, int status);

    // helper functions
    airport *findAirportByIATA(const string& iata);

    // getters
    vector<airport*> getAirports();
    vector<runway*> getRunways();
    vector<airplane*> getAirplanes();
};

#endif //PROJECTVLIEGVELD_SYSTEM_H
