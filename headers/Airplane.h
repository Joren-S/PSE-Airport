//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_AIRPLANES_H
#define PROJECTVLIEGVELD_AIRPLANES_H

#include <string>
#include "DesignByContract.h"

using namespace std;

enum EPlaneStatus { kApproaching, kDescending, kTaxiArrival, kTaxiDeparture, kGate, kDeparture, kAway, kAirport, kCrossing, kAscending, kPushback, kWaitingForDeparture };

enum EPlaneType { kPrivate, kAirline, kMilitary, kEmergency, kDefaultType };

enum EPlaneSize { kSmall, kMedium, kLarge, kDefaultSize };

enum EPlaneEngine { kPropeller, kJet, kDefaultEngine };

enum EPlaneRequest { kPending, kAccepted, kDenied, kIdle, kAcceptedImmediate };


class Airplane {
private:
    /**
     * Pointer to itself
     */
    Airplane *fInitCheck;

    /**
     * Members indicating number, callsign and model
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
     * Amount of passengers it can hold
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

    /**
     * Time remaining of current operation.
     */
    int fTimeRemaining;

    /**
     * Indicates the taxipoint it's at right now.
     */
    string fPosition;

    /**
     * Status of last request.
     */
    EPlaneRequest fRequest;

    /**
     * Taxipoint tied to destination runway for takeoff.
     */
     string fDestination;

public:

    /**
     * Default constructor
     * Set int values to -1, to check if field are initialized
     * Set status to approaching
     */
    Airplane();

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Increases the plane's altitude by a given amount
     * ENSURE: altitude has been raised by the difference
     */
    void increaseAltitude(int difference = 1);

    /**
     * Decreases the plane's altitude by a given amount
     * REQUIRE: new altitude can't be less than 0
     * ENSURE: altitude has been decreased by the difference
     */
    void decreaseAltitude(int difference = 1);


    void decreaseTimeRemaining();


    /**
     * Checks if all the data members were initialized
     */
    bool complete() const;

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
    void setTimeRemaining(int time);
    int getTimeRemaining() const;
    void setPosition(const string&);
    const string &getPosition() const;
    void setRequest(EPlaneRequest);
    EPlaneRequest getRequest() const;
    void setDestination(const string&);
    const string &getDestination() const;

};


#endif //PROJECTVLIEGVELD_AIRPLANES_H
