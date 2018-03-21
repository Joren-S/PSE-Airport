//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_AIRPORTS_H
#define PROJECTVLIEGVELD_AIRPORTS_H

#include <string>
#include <stack>

using namespace std;

class airport {
private:
    string fName, fIata, fCallsign;
    int fGates;
    stack<int> fGateStack;

public:
//    airport(const string& name, const string& iata, const string& callsign, int numGates);

    void initStack();

    int getFreeGate();

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
