//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_AIRPLANES_H
#define PROJECTVLIEGVELD_AIRPLANES_H

#include <string>
#include "DesignByContract.h"

using namespace std;
// type, engine, size and passengers

enum EPlaneStatus { kApproaching, kLanded, kGate, kFinished };

enum EPlaneType { kPrivate, kAirline, kMilitary, kEmergency };

enum EPlaneSize { kSmall, kMedium, kLarge };

enum EPlaneEngine { kPropeller, kJet };

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
     * Size of plane
     */
    EPlaneSize fSize;

    /**
     * Engine type of plane
     */
    EPlaneEngine fEngine;

    /**
     * Type of plane
     */
    EPlaneType fType;

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
    EPlaneSize getSize() const;
    void setSize(EPlaneSize size);
    EPlaneType getType() const;
    void setType(EPlaneType type);
    EPlaneEngine getEngine() const;
    void setEngine(EPlaneEngine engine);
    int getAltitude() const;
    void setAltitude(int altitude);
    int getGateID() const;
    void setGateID(int id);
    int getPassengers() const;
    void setPassengers(int fPassengers);
    const string &getNumber() const;
    void setNumber(const string &fNumber);
    const string &getCallsign() const;
    void setCallsign(const string &fCallsign);
    const string &getModel() const;
    void setModel(const string &fModel);
    EPlaneStatus getStatus() const;
    void setStatus(EPlaneStatus fStatus);

};


#endif //PROJECTVLIEGVELD_AIRPLANES_H
