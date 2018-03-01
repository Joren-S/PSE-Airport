//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_AIRPLANES_H
#define PROJECTVLIEGVELD_AIRPLANES_H

#include <string>
using namespace std;

enum EPlaneStatus { kApproaching, kLanded, kGate, kTaxiing, kFinished };


class airplane {
private:
    string fNumber, fCallsign, fModel;
    EPlaneStatus fStatus;
public:


    // Getters and Setters
    const string &getFNumber() const;

    void setFNumber(const string &fNumber);

    const string &getFCallsign() const;

    void setFCallsign(const string &fCallsign);

    const string &getFModel() const;

    void setFModel(const string &fModel);

    EPlaneStatus getFStatus() const;

    void setFStatus(EPlaneStatus fStatus);

};


#endif //PROJECTVLIEGVELD_AIRPLANES_H
