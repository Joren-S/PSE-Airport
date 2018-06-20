//============================================================================
// Name        : Airplane.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_AIRPLANES_H
#define PROJECTVLIEGVELD_AIRPLANES_H

#include <string>
#include <sstream>
#include <cmath>
#include "DesignByContract.h"

// Forward declarations
class ATC;
class Runway;

///
/// Enums for the Airplane class
///

enum EPlaneStatus   { kApproaching, kDescending, kTaxiArrival, kTaxiDeparture, kCircling, kDeboarding, kTechnicalCheck, kGate, kDeparture, kAway,
                      kAirport, kAscending, kPushback, kWaitingForDeparture, kParked, kCrossingArrival, kCrossingDeparture, kEmergencyLanding, kEmergencyLandingUrgent };

enum EPlaneType     { kPrivate, kAirline, kMilitary, kEmergency, kDefaultType };

enum EPlaneSize     { kSmall, kMedium, kLarge, kDefaultSize };

enum EPlaneEngine   { kPropeller, kJet, kDefaultEngine };

enum EPlaneRequest  { kPending, kAccepted, kDenied, kConfirmed, kAcceptedImmediate, kIdle };


/**
 * \brief: Class that represents an airplane in the simulation
 */
class Airplane {
public:

    /**
     * Default constructor
     * \n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    Airplane();

    /**
     * Checks if the object is properly initialized
     */
    bool properlyInitialized() const;

    /**
     * Calls the right flying function according to the status.
     * \n At the end, checks fuel and decreases time remaining
     * \n REQUIRE(properlyInitialized(), "Plane was not properly initialized when calling performNextStep.");
     * @param log: ostream used for logging events
     */
    void performNextStep(std::ostream& log);

    /**
    * Checks if all the data members were initialized
    * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane::complete");
    * @return boolean isComplete
    */
    bool complete() const;

    /**
     * Calculate the fuel cost for a plane
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling getFuelCost");
     * \n REQUIRE(getEngine != kDefaultEngine, "Invalid engine type for calculating fuel.");
     * \n REQUIRE(getSize != kDefaultSize, "Invalid size for calculating fuel.");
     * @return fuel cost
     */
    int getFuelCost();

    /**
    * Increases the plane's altitude by 1000
    * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling increaseAltitude");
    */
    void increaseAltitude();

    /**
     * Decreases the plane's altitude by 1000
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling decreaseAltitude");
     */
    void decreaseAltitude();

    /**
     * Decreases the time of the remaining operation by one.
     * \n If there's no operation busy, it does nothing.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling decreaseTimeRemaining");
     */
    void decreaseTimeRemaining();


    ////////////////////////////
    ///   Flying functions
    ///   Only use when really needed, else use Airplane::performNextStep
    ////////////////////////////

    /**
     * Performs the approach of a given plane.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling approach");
     * @param log: ostream used for logging events
     */
    void approach(std::ostream& log);

    /**
     * Performs the descend of a given plane.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling descend");
     * @param log: ostream used for logging events
     */
    void descend(std::ostream& log);

    /**
     * Performs the circling of a given plane.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling circle");
     * @param log: ostream used for logging events
     */
    void circle(std::ostream& log);

    /**
     * Performs the taxiing upon arrival of a given plane.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling taxiArrival");
     * @param log: ostream used for logging events
     */
    void taxiArrival(std::ostream& log);

    /**
     * Crosses a runway
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling crossArrival");
     * @param log: ostream used for logging events
     */
    void crossArrival(std::ostream& log);

    /**
     * Performs the deboarding of a given plane.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling deboard");
     * @param log: ostream used for logging events
     */
    void deboard(std::ostream& log);

    /**
     * Performs the technical check of the plane
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling technicalCheck");
     * @param log: ostream used for logging events
     */
    void technicalCheck(std::ostream& log);

    /**
     * Ascends an airplane to 5000ft.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling ascend.");
     * @param log: ostream used for logging events
     */
    void ascend(std::ostream& fLog);

    /**
     * Lets an airplane wait ON a runway before taking off.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling onRunway.");
     * @param log: ostream used for logging events
     */
    void onRunway(std::ostream& fLog);

    /**
     * Lets an airplane wait AT a runway until instructions are given.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling atRunway");
     * @param log: ostream used for logging events
     */
    void atRunway(std::ostream& fLog);

    /**
     * Performs a cross.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling taxiDepartureCross");
     * @param log: ostream used for logging events
     */
    void taxiDepartureCross(std::ostream& fLog);

    /**
     * Performs the next taxi-step.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling taxiDepartureStep");
     * @param log: ostream used for logging events
     */
    void taxiDepartureStep(std::ostream& fLog);

    /**
     * Lets an airplane wait before taxiing.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling taxiDepartureStart");
     * @param log: ostream used for logging events
     */
    void taxiDepartureStart(std::ostream& fLog);

    /**
     * Pushes an airplane back from the gate.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling pushback");
     * @param log: ostream used for logging events
     */
    void pushback(std::ostream& fLog);

    /**
     * Refuels the airplane and lets passengers board.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling prepare");
     * @param log: ostream used for logging events
     */
    void prepare(std::ostream& fLog);

    /**
     * Check fuel, subtract fuelcost if needed, adjust squawk if needed, contact ATC if needed.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling checkFuel");
     * @param log: ostream used for logging events
     */
    void checkFuel(std::ostream& log);


    //////////////
    /// Getters and setters with special contracts/documentation
    //////////////


    /**
     * Setter for the altitude
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * \n REQUIRE(altitude >= 0, "Altitude can't be negative");
     */
    void setAltitude(int altitude);

    /**
     * Setter for the time remaining on the operation
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * \n REQUIRE(time >= 0, "Time remaining can't be negative");
     */
    void setTimeRemaining(int time);

    /**
     * Setter for the maximum amount of passengers
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * \n REQUIRE(passengers >= 0, "Passenger amount can't be negative");
     */
    void setPassengers(int fPassengers);

    /**
     * Setter for the gate the plane's at
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * \n REQUIRE(id >= -1, "Gate id can't be less than -1");
     */
    void setGateID(int id);

    /**
     * Setter for the maximum amount of fuel (in 10.000 units)
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * \n REQUIRE(fuel > 0, "Fuel can't be less than 1");
     */
    void setFuel(int fuel);

    /**
     * Setter for the squawk code
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * \n REQUIRE(squawk >= 0, "Squawk code can't be negative");
     */
    void setSquawk(int squawk);

    /**
     * Setter for the current amount of fuel
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane getter/setter");
     * \n REQUIRE(fuel >= 0, "Fuel can't be negative");
     * \n REQUIRE(fuel <= getFuel, "Fuel can't be more than the max. fuel");
     */
    void setCurFuel(int fuel);


    ////////////////////////////////////
    ///  Getters and setters
    ///  For each method: REQUIRE(properlyInitialized(), "Airplane wasn't initialized when calling Airplane getter/setter");
    ////////////////////////////////////


    EPlaneSize getSize() const;
    void setSize(EPlaneSize size);
    EPlaneType getType() const;
    void setType(EPlaneType type);
    EPlaneEngine getEngine() const;
    void setEngine(EPlaneEngine engine);
    int getAltitude() const;
    int getGateID() const;
    int getPassengers() const;
    const std::string &getNumber() const;
    void setNumber(const std::string &fNumber);
    const std::string &getCallsign() const;
    void setCallsign(const std::string &fCallsign);
    const std::string &getModel() const;
    void setModel(const std::string &fModel);
    EPlaneStatus getStatus() const;
    void setStatus(EPlaneStatus fStatus);
    int getTimeRemaining() const;
    void setPosition(const std::string&);
    const std::string &getPosition() const;
    void setRequest(EPlaneRequest);
    EPlaneRequest getRequest() const;
    Runway* getRunway() const;
    void setRunway(Runway*);
    int getFuel() const;
    int getCurFuel() const;
    int getSquawk() const;
    void setATC(ATC *atc);

private:

    /**
     * Members indicating number, callsign, position(=taxipoint) and model
     */
    std::string fNumber, fCallsign, fModel, fPosition;

    /**
     * Indicates the runway on which the airplane lands/takes off
     */
    Runway* fRunway;

    /**
     * ATC of the airport this plane is interacting with.
     */
    ATC *fATC;

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
     * Altitude in feet
     */
    int fAltitude;

    /**
     * Time remaining of current operation.
     */
    int fTimeRemaining;
    /**
     * Amount of fuel
     */
    int fFuel;

    /**
     * Current amount of fuel
     */
     int fCurFuel;

    /**
     * Squawk code of the airplane
     */
    int fSquawk;

    /**
     * Old squawk code for restoration incase of emergency.
     */
    int fOldSquawk;

    /**
     * Pointer to itself
     */
    Airplane* fInitCheck;

};


#endif //PROJECTVLIEGVELD_AIRPLANES_H
