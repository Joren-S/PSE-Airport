//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_SYSTEM_H
#define PROJECTVLIEGVELD_SYSTEM_H

#include "types.h"

using namespace std;

class System {
private:

    /**
     * Vector of pointers to all the airports
     */
    vector<Airport*> airports;

    /**
     * Vector of pointers to all the airplanes
     */
    vector<Airplane*> airplanes;

    /**
     * Vector of pointers to all the runways
     */
    vector<Runway*> runways;

    /**
     * Finds an airport with a specific IATA
     */
    Airport* findAirportByIATA(const string& iata) const;

    /**
     * Return how many runways an airport has
     */
    int runwaysInAirport(Airport *ap) const;

    /**
     * Get a free runway from a given airport if available.
     * Return a nullptr otherwise
     */
    Runway* getFreeRunway(Airport* ap) const;

    /**
     * Checks if the simulation has ended.
     * i.e. the status of all planes is kFinished
     */
    bool simulationFinished() const;

    /**
     * Lands an airplane on an airport.
     * Logs the info to an output stream, defaulted to "cout".
     * REQUIRE: plane status is approaching &&
     * a gate at target airport is available &&
     * a runway at target airport is available &&
     * at least one airport in the system
     * ENSURE: plane status is kLanded
     */
    void land(Airplane*, Airport*, ostream& = cout) const;

    /**
     * Performs duties at the gate for an airplane in an airport.
     * Logs the info to an output stream, defaulted to "cout".
     * REQUIRE: plane status is kLanded &&
     * at least one airport in the system
     * ENSURE: plane status is kGate
     */
    void gate(Airplane*, Airport*, ostream& = cout) const;

    /**
     * Performs a takeoff of a plane on an airport
     * Logs the info to an output stream, defaulted to "cout".
     * REQUIRE: plane status is kGate &&
     * a runway at target airport is available &&
     * at least one airport in the system
     * ENSURE: plane status is kFinished
     */
    void takeoff(Airplane*, Airport*, ostream& = cout) const;

public:

    /**
     * Default constructor
     */
    System() {};

    /**
     * Destructor
     */
    ~System();

    /**
     * Initializes the system using the given input file.
     * REQUIRE: valid input file
     * @param filename: name of the input file
     */
    void setup(const string& filename);

    /**
     * Logs information of the airports and airplanes to a
     * text file.
     * REQUIRE: at least one airport in the system
     * @param filename: name of the to be generated txt file
     */
    void log(const string& filename = "../log.txt");

    /**
     * Runs the complete simulation
     * REQUIRE: at least one airport in the system and
     * simulation not ended yet.
     * ENSURE: simulation ended
     */
    void run();

    /**
     * Adds an airport to the simulation with the given specifications
     * ENSURE: airport is added to the vector of airports
     * @return boolean indicating success
     */
    bool addAirport(const string& name, const string& iata, const string& callsign, int numGates);

    /**
     * Adds a runway to the simulation with the given specifications
     * REQUIRE: airport has to exist
     * ENSURE: runway is added to the vector of runways
     * @return boolean indicating success
     */
    bool addRunway(const string& name, const string& Airport);

    /**
     * Adds an airplane to the simulation with the given specifications
     * REQUIRE: available gate if the airplane should be at a gate
     * ENSURE: airplane is added to the vector of airplanes
     * @return boolean indicating success
     */
    bool addAirplane(const string& number, const string& callsign, const string& model, int status);

    /**
     * Getter for the airports in the simulation
     * @return vec of all airports
     */
    vector<Airport*> getAirports() const;

    /**
     * Getter for the runways in the simulation
     * @return vec of all runways
     */
    vector<Runway*> getRunways() const;

    /**
     * Getter for the airplanes in the simulation
     * @return vec of all airplanes
     */
    vector<Airplane*> getAirplanes() const;

};

#endif //PROJECTVLIEGVELD_SYSTEM_H
