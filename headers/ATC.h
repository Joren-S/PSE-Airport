//============================================================================
// Name        : ATC.h
// Author      : Joren Servotte, Max Van Houcke
// Description : Airport simulation, Project Software Engineering
//============================================================================

#ifndef PROJECTVLIEGVELD_ATC_H
#define PROJECTVLIEGVELD_ATC_H

#include <stdlib.h>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include "Runway.h"
#include "Time.h"
#include "Airport.h"
#include "Airplane.h"

struct ATCRequest {

    /**
     * Time message was sent.
     */
    Time fTime;

    /**
     * Airplane that sent request
     */
    Airplane* fAirplane;

    /**
     * Constructor.
     */
    ATCRequest(Time time, Airplane* plane);

};

struct Comparator {
    bool operator()(const ATCRequest* lhs, const ATCRequest* rhs);
};



class ATC {
public:

    /**
     * Constructor
     * \n ENSURE(properlyInitialized(), "ATC was not properly initialized after constructing.");
     * @param stream: std::ostream to write to.
     */
    ATC(std::ostream& stream);

    /**
     * Destructor
     */
    ~ATC();

    /**
     * Main function of the ATC, needs to be called every time we advance in time.
     * \n Handles requests and responds correctly.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling doHeartbeat.");
     */
    void doHeartbeat();

    /**
     * Checks if the object is properly initialized
     * @return: Boolean indicating if properly initialized or not.
     */
    bool properlyInitialized() const;

    /**
     * Send a request to the ATC.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling sendRequest.");
     * \n REQUIRE(source != NULL, "Source is NULL.");
     * @param time: Time of the request.
     * @param source: Airplane that made the request.
     */
    void sendRequest(Time time, Airplane* source);

    /**
     * Return the amount of requests that are queued.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling getQueueSize.");
     * @return: Size of the std::priority_queue.
     */
    int getQueueSize() const;

    /**
     * Get the next request that needs to be handled by the ATC.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling getNextRequest.");
     * @return: Pointer to the request, NULL if priority_queue is empty.
     */
    ATCRequest *getNextRequest();

    /**
     * Creates a correctly formatted ATC message when the contents are given.
     * @param time: Time of message.
     * @param source: Sender of message.
     * @param message: Content of message.
     * @return: Formatted message.
     */
    static std::string formatMessage(Time time, std::string source, std::string message);

    /**
     * Processes a request for approach.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processApproach.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processApproach(Airplane* airplane);

    /**
     * Processes a request for descend.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processDescend.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processDescend(Airplane* airplane);

    /**
     * Processes a request for taxiing at arrival.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processTaxiArrival.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processTaxiArrival(Airplane* airplane);

    /**
     * Processes a request for IFR clearance.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processIFRClearancy.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processIFRClearance(Airplane* airplane);

    /**
     * Processes a request for pushback.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processPushback.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processPushback(Airplane* airplane);

    /**
     * Processes a request to start taxiing.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processTaxiInitialise.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processTaxiInitialise(Airplane* airplane);

    /**
     * Processes a request for a taxi instruction.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processTaxiInstruction.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processTaxiInstruction(Airplane* airplane);

    /**
     * Processes a request for takeoff when waiting at runway.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processTakeoff.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processTakeOff(Airplane* airplane);

    /**
     * Processes a request for takeoff when waiting on runway.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processTakeoffRunway.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processTakeOffRunway(Airplane* airplane);

    /**
     * Processes a request for an emergency landing.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processTakeoffRunway.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processEmergency(Airplane* airplane);

    /**
     * Processes a request for an urgent emergency landing.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processUrgentEmergency.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processUrgentEmergency(Airplane* airplane);

    /**
     * Write a message to the ATC stream.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling sendMessage.");
     * @param message: Message that needs to be send.
     */
    void sendMessage(const std::string &message);

    /**
     * Generates a squawk code for a given plane.
     * The returned code will not be generated for any other plane.
     * \n REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling getSquawk.");
     * @param airplane: airplane to generate squawk for.
     * @return squawk code
     */
    int getSquawk(Airplane* airplane);

    /**
     * Getter for the current time
     * \n REQUIRE(this->properlyInitialized(), "ATC was't initialized when calling getTime");
     * @return current time
     */
    Time getTime() const;

    /**
    * Setter for the current time
    * \n REQUIRE(this->properlyInitialized(), "ATC was't initialized when calling setTime");
    * @param time: time to set
    */
    void setTime(Time time);

    ////////////////
    ///  Getters and setters for the fields of the class.
    ///  For all:
    ///  REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
    ///  For setters;
    ///  ENSURE(getField == value, "Field wasn't std::set properly");
    ///  where getField is specific for the member
    ///////////////

    std::priority_queue<ATCRequest*, std::vector<ATCRequest*>, Comparator> *getQueue();
    Airport* getAirport() const;
    void setAirport(Airport*);
    bool get3occupied() const;
    void set3occupied(bool);
    bool get5occupied() const;
    void set5occupied(bool);
    void setTestMode(bool);
    bool isTestMode() const;

private:

    /**
     * Output stream.
     */
    std::ostream& fStream;

    /**
     * Queue of messages.
     */
    std::priority_queue<ATCRequest*, std::vector<ATCRequest*>, Comparator> fQueue;

    /**
     * Object that keeps track of current time
     */
    Time fTime;

    /**
     * Bool if 3.000ft is occupied.
     */
    bool f3Occupied;

    /**
     * Bool if 5.000ft is occupied.
     */
    bool f5Occupied;

    /**
     * Pointer to the airport the ATC is located in.
     */
    Airport* fAirport;

    /**
     * The std::set of used squawk codes
     */
    std::set<int> fUsedCodes;

    /**
     * Bool indicating if tests are running
     * Gives back 0 for all squawk codes
     */
    bool fTest;

    /**
    * Pointer to the ATC object to check for proper initialization
    */
    ATC *fInitCheck;

};


#endif //PROJECTVLIEGVELD_ATC_H