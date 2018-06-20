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

// Forward declaration
class Airport;


/**
 * Enum that represents the type of the runway material
 */
enum ERunwayType { kAsphalt, kGrass, kDefaultRunType };


/**
 * Class that represents a runway in an airport
 */
class Runway {
public:
    
    /**
     * Constructor for the Runway class.
     * \n ENSURE(properlyInitialized(), "Runway wasn't properly initialized after constructing.");
     */
    Runway();

    /**
     * Checks if all the data members were initialized
     * \n REQUIRE(properlyInitialized(), "Runway wasn't properly initialized when calling complete.");
     * @return: Boolean indicating if all members were initialized
     */
    bool complete() const;

    /**
     * Checks if the object is properly initialized
     * @return: Boolean indicating if properly initialized or not.
     */
    bool properlyInitialized() const;

    /**
     * Check if this runway is valid for the provided airplane.
     * \n REQUIRE(properlyInitialized(), "Runway wasn't properly initialized when calling validForAirplane.");
     * \n REQUIRE(plane != NULL, "Plane object does not exist.");
     * @param plane: Airplane to check validity for.
     * @return: Boolean indicating if valid or not.
     */
    bool validForAirplane(Airplane* plane) const;

    ////////////////
    ///  Getters and setters for the fields of the class.
    ///  For all:
    ///  REQUIRE(properlyInitialized(), "Runway wasn't properly initialized when calling getter/setter.");
    ///  For setters;
    ///  ENSURE(getField == value, "Field wasn't set properly");
    ///  where getField is specific for the member
    ///////////////

    ERunwayType getType() const;
    void setType(ERunwayType type);
    int getLength() const;
    void setLength(int length);
    const std::string &getName() const;
    void setName(const std::string &fName);
    bool isFree() const;
    void setFree(bool free);
    std::string getTaxiPoint() const;
    void setTaxiPoint(const std::string&);
    Airport *getAirport() const;
    void setAirport(Airport *fAirport);

private:

    /**
     * Name of the runway
     */
    std::string fName;

    /**
     * Taxipoint associated with runway
     */
    std::string fTaxiPoint;

    /**
     * Type of runway, either grass or asphalt
     */
    ERunwayType fType;

    /**
     * Length of the runway in meters
     */
    int fLength;

    /**
     * bool indicating if the runway is free
     */
    bool fFree;

    /**
     * Pointer to the airport in which the runway is
     */
    Airport* fAirport;

    /**
     * Pointer to itself
     */
    Runway *fInitCheck;

};

#endif //PROJECTVLIEGVELD_RUNWAYS_H
