//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_AIRPLANES_H
#define PROJECTVLIEGVELD_AIRPLANES_H

#include <string>
#include "DesignByContract.h"

using namespace std;

enum EPlaneStatus { kApproaching, kLanded, kGate, kFinished };

class Airplane {
private:

    /**
     * Members indication number, callsign and moder
     */
    string fNumber, fCallsign, fModel;

    /**
     * Status of plane
     */
    EPlaneStatus fStatus;

    /**
     * Amount of passengers
     */
    int fPassengers;

    /**
     * Gate where the airplane is at.
     * If the value is -1, the airplane is not at a gate
     */
    int fGateID;

    /**
     * Altitude in 1000feet
     */
    int fAltitude;

public:

    /**
     * Constructor
     */
    Airplane(const string& number, const string& callsign, const string& model, EPlaneStatus status, int passengers);

    /**
     * Default constructor
     * Set int values to -1, to check if field are initialized
     * Set status to approaching
     */
    Airplane();

    /**
     * Increases the plane's altitude by a given amount
     * ENSURE: altitude has been raised by the difference
     */
    void increaseAltitude(int difference);

    /**
     * Decreases the plane's altitude by a given amount
     * REQUIRE: new altitude can't be less than 0
     * ENSURE: altitude has been decreased by the difference
     */
    void decreaseAltitude(int difference);

    // Getters and Setters
    int getAltitude() const;
    void setAltitude(int altitude);
    int getFGateID() const;
    void setFGateID(int id);
    int getFPassengers() const;
    void setFPassengers(int fPassengers);
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
