//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_RUNWAYS_H
#define PROJECTVLIEGVELD_RUNWAYS_H

#include <string>
using namespace std;

class airport;

class runway {
private:
    bool fFree;
    string fName;
    airport *fAirport;
public:
    // Getters and Setters
    const string &getFName() const;

    void setFName(const string &fName);

    airport *getFAirport() const;

    void setFAirport(airport *fAirport);

    bool isFree() const;

    void setFree(bool free);

};

#endif //PROJECTVLIEGVELD_RUNWAYS_H
