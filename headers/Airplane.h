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

class ATC;
class Runway; // Forward declaration


// Enums for the Airplane class

enum EPlaneStatus { kApproaching, kDescending, kTaxiArrival, kTaxiDeparture, kCircling, kDeboarding, kTechnicalCheck, kGate, kDeparture, kAway,
                    kAirport, kAscending, kPushback, kWaitingForDeparture, kParked, kCrossingArrival, kCrossingDeparture, kEmergencyLanding, kEmergencyLandingUrgent };

enum EPlaneType { kPrivate, kAirline, kMilitary, kEmergency, kDefaultType };

enum EPlaneSize { kSmall, kMedium, kLarge, kDefaultSize };

enum EPlaneEngine { kPropeller, kJet, kDefaultEngine };

enum EPlaneRequest { kPending, kAccepted, kDenied, kConfirmed, kAcceptedImmediate, kIdle };


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
     * Increases the plane's altitude by a given amount
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling increaseAltitude");
     * \n REQUIRE(difference > 0, "Difference can't be negative");
     * \n ENSURE(fAltitude == oldAltitude + difference, "Altitude hasn't been increased correctly.");
     */
    void increaseAltitude(int difference = 1000);

    /**
     * Decreases the plane's altitude by a given amount
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling decreaseAltitude");
     * \n REQUIRE(difference > 0, "Difference can't be negative");
     * \n REQUIRE(fAltitude - difference >= 0, "New altitude can't be less than 0!");
     * \n ENSURE(fAltitude == oldAltitude - difference, "Altitude hasn't been decreased correctly.");
     */
    void decreaseAltitude(int difference = 1000);

    /**
     * Decreases the time of the remaining operation by one.
     * If there's no operation busy, it does nothing.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling decreaseTimeRemaining");
     */
    void decreaseTimeRemaining();

    /**
     * Checks if all the data members were initialized
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling Airplane::complete");
     */
    bool complete() const;

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
     * \n REQUIRE(fuel <= fFuel, "Fuel can't be more than the max. fuel");
     */
    void setCurFuel(int fuel);

    /**
     * Performs the approach of a given plane.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling approach");
     */
    void approach(std::ostream& log, ATC *fATC);

    /**
     * Performs the descend of a given plane.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling descend");
     */
    void descend(std::ostream& log, ATC *fATC);

    /**
     * Performs the circling of a given plane.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling circle");
     */
    void circle(std::ostream& log, ATC *fATC);

    /**
     * Performs the taxiing upon arrival of a given plane.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling taxiArrival");
     */
    void taxiArrival(std::ostream& log, ATC *fATC);

    /**
     * Crosses a runway
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling crossArrival");
     */
    void crossArrival(std::ostream& log, ATC *fATC);

    /**
     * Performs the deboarding of a given plane.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling deboard");
     */
    void deboard(std::ostream& log, ATC *fATC);

    /**
     * Performs the technical check of the plane
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling technicalCheck");
     */
    void technicalCheck(std::ostream& log, ATC *fATC);

    /**
     * Lands an airplane on the airport of the simulation.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was't initialized when calling land");
     */
    void land(std::ostream& log, ATC *fATC);

    /**
     * Ascends an airplane to 5000ft.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling ascend.");
     */
    void ascend(std::ostream& fLog, ATC *fATC);

    /**
     * Lets an airplane wait ON a runway before taking off.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling onRunway.");
     */
    void onRunway(std::ostream& fLog, ATC *fATC);

    /**
     * Lets an airplane wait AT a runway until instructions are given.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling atRunway");
     */
    void atRunway(std::ostream& fLog, ATC *fATC);

    /**
     * Performs a cross.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling taxiDepartureCross");
     */
    void taxiDepartureCross(std::ostream& fLog, ATC *fATC);

    /**
     * Performs the next taxi-step.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling taxiDepartureStep");
     */
    void taxiDepartureStep(std::ostream& fLog, ATC *fATC);

    /**
     * Lets an airplane wait before taxiing.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling taxiDepartureStart");
     */
    void taxiDepartureStart(std::ostream& fLog, ATC *fATC);

    /**
     * Pushes an airplane back from the gate.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling pushback");
     */
    void pushback(std::ostream& fLog, ATC *fATC);

    /**
     * Refuels the airplane and lets passengers board.
     * Events are logged to the given std::ostream&.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling prepare");
     */
    void prepare(std::ostream& fLog, ATC *fATC);

    /**
     * Performs a takeoff of a plane on an airport
     * Logs the info to an output stream, defaulted to "cout".
     * \n REQUIRE: plane status is kGate &&
     * a runway at target airport is available &&
     * at least one airport in the system
     * \n ENSURE: plane status is kFinished
     */
    void takeoff(std::ostream& log, ATC *fATC);

    /**
     * Calculate the fuel cost for a plane
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling getFuelCost");
     * \n REQUIRE(fEngine != kDefaultEngine, "Invalid engine type for calculating fuel.");
     * \n REQUIRE(fSize != kDefaultSize, "Invalid size for calculating fuel.");
     */
    int getFuelCost();

    /**
     * Check fuel, subtract fuelcost if needed, adjust squawk if needed, contact ATC if needed.
     * \n REQUIRE(this->properlyInitialized(), "Airplane was not properly initialized when calling checkFuel");
     */
    void checkFuel(std::ostream& log, ATC *fATC);

    /**
     * Getters and setters for the fields of the class.
     * \n REQUIRE(properlyInitialized(), "Airplane wasn't initialized when calling Airplane getter/setter");
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

private:

    /**
     * Pointer to itself
     */
    Airplane *fInitCheck;

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
    std::string fPosition;

    /**
     * Indicates the runway on which the airplane lands/takes off
     */
    Runway* fRunway;

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

};


#endif //PROJECTVLIEGVELD_AIRPLANES_H
