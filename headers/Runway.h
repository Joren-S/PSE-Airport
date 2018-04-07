//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_RUNWAYS_H
#define PROJECTVLIEGVELD_RUNWAYS_H

#include <string>

using namespace std;

class Airport;

enum ERunwayType { kGrass, kAsphalt };

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

public:

    /**
     * Constructor
     */
    Runway(bool free, const string& name, Airport* airport, ERunwayType type, int length);

    /**
     * Default constructor
     * Set airport to NULL
     */
    Runway();

    // Getters and Setters
    ERunwayType getType() const;
    void setType(ERunwayType type);
    int getLength() const;
    void setLength(int length);
    const string &getFName() const;
    void setFName(const string &fName);
    Airport *getFAirport() const;
    void setFAirport(Airport *fAirport);
    bool isFree() const;
    void setFree(bool free);

};

#endif //PROJECTVLIEGVELD_RUNWAYS_H
