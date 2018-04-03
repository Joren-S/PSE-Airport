//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_RUNWAYS_H
#define PROJECTVLIEGVELD_RUNWAYS_H

#include <string>
using namespace std;

class Airport;

class Runway {
private:
    bool fFree;
    string fName;
    Airport *fAirport;
public:
    // Getters and Setters
    const string &getFName() const;

    void setFName(const string &fName);

    Airport *getFAirport() const;

    void setFAirport(Airport *fAirport);

    bool isFree() const;

    void setFree(bool free);

};

#endif //PROJECTVLIEGVELD_RUNWAYS_H
