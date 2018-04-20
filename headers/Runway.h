//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_RUNWAYS_H
#define PROJECTVLIEGVELD_RUNWAYS_H

#include <string>

#include "Airplane.h"
#include "DesignByContract.h"

using namespace std;

class Airport;
class Airplane;

enum ERunwayType { kAsphalt, kGrass, kDefaultRunType };

class Runway {
private:

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

public:
    
    /**
     * Default constructor
     * Set airport to NULL
     */
    Runway();

    /**
     * Checks if all the data members were initialized
     */
    bool complete() const;

    /**
     * Check if this runway is valid for the provided airplane.
     * REQUIRE: plane exists
     */
    bool validForAirplane(Airplane* plane) const;

    // Getters and Setters
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
