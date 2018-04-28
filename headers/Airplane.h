//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_AIRPLANES_H
#define PROJECTVLIEGVELD_AIRPLANES_H

// STL
#include <string>

// Libraries
#include "DesignByContract.h"

// Forward declaration
class Runway;

// Enums for the Airplane class

enum EPlaneStatus { kApproaching, kDescending, kTaxiArrival, kTaxiDeparture, kDeboarding, kGate, kDeparture, kAway, kAirport, kCrossing };

enum EPlaneType { kPrivate, kAirline, kMilitary, kEmergency, kDefaultType };

enum EPlaneSize { kSmall, kMedium, kLarge, kDefaultSize };

enum EPlaneEngine { kPropeller, kJet, kDefaultEngine };

enum EPlaneRequest { kPending, kAccepted, kDenied, kConfirmed, kIdle };


class Airplane {
private:

    /**
     * Members indicating number, callsign and model
     */
    std::string fNumber, fCallsign, fModel;

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
    std::string fPosition;

    /**
     * Indicates the runway on which the airplane lands/takes off
     */
    Runway* fRunway;

    /**
     * Status of last request.
     */
    EPlaneRequest fRequest;

public:

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
    const std::string &getNumber() const;
    void setNumber(const std::string &fNumber);
    const std::string &getCallsign() const;
    void setCallsign(const std::string &fCallsign);
    const std::string &getModel() const;
    void setModel(const std::string &fModel);
    EPlaneStatus getStatus() const;
    void setStatus(EPlaneStatus fStatus);
    void setTimeRemaining(int time);
    int getTimeRemaining() const;
    void setPosition(const std::string&);
    const std::string &getPosition() const;
    void setRequest(EPlaneRequest);
    EPlaneRequest getRequest() const;
    Runway* getRunway() const;
    void setRunway(Runway*);

};


#endif //PROJECTVLIEGVELD_AIRPLANES_H
