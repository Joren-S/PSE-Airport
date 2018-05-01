//============================================================================
// Name        : Airplane.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_AIRPLANES_H
#define PROJECTVLIEGVELD_AIRPLANES_H


#include <string>
#include "DesignByContract.h"

using namespace std;


class Runway; // Forward declaration


// Enums for the Airplane class

enum EPlaneStatus { kApproaching, kDescending, kTaxiArrival, kTaxiDeparture, kCircling, kDeboarding, kGate, kDeparture, kAway,
                    kAirport, kAscending, kPushback, kWaitingForDeparture, kParked, kCrossingArrival, kCrossingDeparture };

enum EPlaneType { kPrivate, kAirline, kMilitary, kEmergency, kDefaultType };

enum EPlaneSize { kSmall, kMedium, kLarge, kDefaultSize };

enum EPlaneEngine { kPropeller, kJet, kDefaultEngine };

enum EPlaneRequest { kPending, kAccepted, kDenied, kConfirmed, kAcceptedImmediate, kIdle };


/**
 * \brief: Class that represents an airplane in the simulation
 */
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
     * Status of last request.
     */
    EPlaneRequest fRequest;

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
     * Indicates the runway on which the airplane lands/takes off
     */
    Runway* fRunway;

    /**
     * Amount of fuel
     */
     int fFuel;

public:

    /**
     * Default constructor
     * ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    Airplane();

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Increases the plane's altitude by a given amount
     * REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling increaseAltitude");
     * REQUIRE(difference > 0, "Difference can't be negative");
     * ENSURE(fAltitude == oldAltitude + difference, "Altitude hasn't been increased correctly.");
     */
    void increaseAltitude(int difference = 1);

    /**
     * Decreases the plane's altitude by a given amount
     * REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling decreaseAltitude");
     * REQUIRE(difference > 0, "Difference can't be negative");
     * REQUIRE(fAltitude - difference >= 0, "New altitude can't be less than 0!");
     * ENSURE(fAltitude == oldAltitude - difference, "Altitude hasn't been decreased correctly.");
     */
    void decreaseAltitude(int difference = 1);

    /**
     * Decreases the time of the remaining operation by one.
     * If there's no operation busy, it does nothing.
     * REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling decreaseTimeRemaining");
     */
    void decreaseTimeRemaining();

    /**
     * Checks if all the data members were initialized
     * REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane::complete");
     */
    bool complete() const;

    /**
     * Setter for the altitude
     * REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * REQUIRE(altitude >= 0, "Altitude can't be negative");
     */
    void setAltitude(int altitude);

    /**
     * Setter for the time remaining on the operation
     * REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * REQUIRE(time >= 0, "Time remaining can't be negative");
     */
    void setTimeRemaining(int time);

    /**
     * Setter for the maximum amount of passengers
     * REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * REQUIRE(passengers >= 0, "Passenger amount can't be negative");
     */
    void setPassengers(int fPassengers);

    /**
     * Setter for the gate the plane's at
     * REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * REQUIRE(id >= -1, "Gate id can't be less than -1");
     */
    void setGateID(int id);

    /**
     * Setter for the maximum amount of fuel (in 10.000 units)
     * REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * REQUIRE(fuel > 0, "Fuel can't be less than 1");
     */
    void setFuel(int fuel);


    /**
     * Getters and setters for the fields of the class.
     * REQUIRE(properlyInitialized(), "Airplane wasn't initialized when calling Airplane getter/setter");
     *
     */
    EPlaneSize getSize() const;
    void setSize(EPlaneSize size);
    EPlaneType getType() const;
    void setType(EPlaneType type);
    EPlaneEngine getEngine() const;
    void setEngine(EPlaneEngine engine);
    int getAltitude() const;
    int getGateID() const;
    int getPassengers() const;
    const string &getNumber() const;
    void setNumber(const string &fNumber);
    const string &getCallsign() const;
    void setCallsign(const string &fCallsign);
    const string &getModel() const;
    void setModel(const string &fModel);
    EPlaneStatus getStatus() const;
    void setStatus(EPlaneStatus fStatus);
    int getTimeRemaining() const;
    void setPosition(const string&);
    const string &getPosition() const;
    void setRequest(EPlaneRequest);
    EPlaneRequest getRequest() const;
    Runway* getRunway() const;
    void setRunway(Runway*);
    int getFuel() const;

};


#endif //PROJECTVLIEGVELD_AIRPLANES_H
