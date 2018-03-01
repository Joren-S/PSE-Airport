//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_AIRPORTS_H
#define PROJECTVLIEGVELD_AIRPORTS_H

#include <string>

using namespace std;

class airport {
private:
    string fName, fIata, fCallsign;
    int fGates;
public:
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
