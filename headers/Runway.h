//============================================================================
// Name        : Runway.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_RUNWAYS_H
#define PROJECTVLIEGVELD_RUNWAYS_H

#include <string>
#include "DesignByContract.h"
#include "Airplane.h"

using namespace std;

// Forward declaration
class Airport;

// Enums for the Runway class

enum ERunwayType { kAsphalt, kGrass, kDefaultRunType };

class Runway {
private:
    /**
     * Pointer to itself
     */
    Runway *fInitCheck;

    /**
     * bool indicating if the runway is free
     */
    bool fFree;

    /**
     * Name of the runway
     */
    string fName;

    /**
     * Pointer to the airport in which the runway is
     */
    Airport* fAirport;

    /**
     * Type of runway, either grass or asphalt
     */
    ERunwayType fType;

    /**
     * Length of the runway in meters
     */
    int fLength;

    /**
     * Taxipoint associated with runway
     */
    string fTaxiPoint;

    /**
     * Pointer to the runway tied to this airplane
     */
    Runway* fRunway;

public:
    
    /**
     * Constructor for the Runway class.
     * ENSURE(properlyInitialized(), "Runway wasn't properly initialized.");
     */
    Runway();

    /**
     * Checks if the object is properly initialized
     * @return: Boolean indicating if properly initialized or not.
     */
    bool properlyInitialized() const;

    /**
     * Checks if all the data members were initialized
     * REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
     * @return: Boolean indicating if all members were initialized
     */
    bool complete() const;

    /**
     * Check if this runway is valid for the provided airplane.
     * REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
     * REQUIRE(plane != NULL, "Plane object does not exist.");
     * @param plane: Airplane to check validity for.
     * @return: Boolean indicating if valid or not.
     */
    bool validForAirplane(Airplane* plane) const;

    /**
     * Getters and setters for the fields of the class.
     * REQUIRE(properlyInitialized(), "Runway wasn't properly initialized.");
     *
     * Setters:
     * ENSURE(fField == value, "Field wasn't set properly");
     */
    ERunwayType getType() const;
    void setType(ERunwayType type);
    int getLength() const;
    void setLength(int length);
    const string &getName() const;
    void setName(const string &fName);
    Airport *getAirport() const;
    void setAirport(Airport *fAirport);
    bool isFree() const;
    void setFree(bool free);
    string getTaxiPoint() const;
    void setTaxiPoint(const string&);

};

#endif //PROJECTVLIEGVELD_RUNWAYS_H
