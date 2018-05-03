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

#include "Airplane.h"
#include "Runway.h"
#include "Time.h"
#include "Airport.h"

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
    ATCRequest(Time time, Airplane* plane) : fTime(time), fAirplane(plane) {}
};

class ATC {
private:

    /**
     * Pointer to the ATC object to check for proper initialization
     */
    ATC *fInitCheck;

    /**
     * Output stream.
     */
    ostream& fStream;

    /**
     * Queue of messages.
     */
    queue<ATCRequest*> fQueue;

    /**
    * Time when ATC was last active.
    */
    Time fLastActive;

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
     * The set of used squawk codes
     */
    set<int> fUsedCodes;
    
    /**
     * Bool indicating if tests are running
     * Gives back 0 for all squawk codes
     */
    bool fTest;

public:

    /**
     * Constructor
     * ENSURE(properlyInitialized(), "ATC was not properly initialized after constructing.");
     * @param stream: ostream to write to.
     */
    ATC(ostream& stream, bool test);

    /**
     * Destructor
     */
    ~ATC();

    /**
     * Checks if the object is properly initialized
     * @return: Boolean indicating if properly initialized or not.
     */
    bool properlyInitialized() const;

    /**
     * Send a request to the ATC.
     * REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling sendRequest.");
     * REQUIRE(source != NULL, "Source is NULL.");
     * ENSURE(getQueue()->back() == rqst, "Request wasn't queued properly.");
     * @param time: Time of the request.
     * @param source: Airplane that made the request.
     */
    void sendRequest(Time time, Airplane* source);

    /**
     * Return the amount of requests that are queued.
     * REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling getQueueSize.");
     * ENSURE(size >= 0, "Queue has a negative size.");
     * @return: Size of the queue.
     */
    int getQueueSize() const;

    /**
     * Get the next request that needs to be handled by the ATC.
     * REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling getNextRequest.");
     * ENSURE(getQueue()->front() != rqst, "Message wasn't removed from the queue.");
     * ENSURE(msg != NULL, "Request popped from queue is NULL.");
     * @return: Pointer to the request, NULL if queue is empty.
     */
    ATCRequest *getNextRequest();

    /**
     * Creates a correctly formatted ATC message when the contents are given.
     * @param time: Time of message.
     * @param source: Sender of message.
     * @param message: Content of message.
     * @return: Formatted message.
     */
    static string formatMessage(Time time, string source, string message);

    /**
     * Main function of the ATC, needs to be called every time we advance in time.
     * Handles requests and responds correctly.
     * REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling doHeartbeat.");
     * @param time: Time of "heartbeat".
     */
    void doHeartbeat(Time time);

    /**
     * Processes a request for approach.
     * REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processApproach.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processApproach(Airplane* airplane, Time time);

    /**
     * Processes a request for descend.
     * REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processDescend.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processDescend(Airplane* airplane, Time time);

    /**
     * Processes a request for taxiing at arrival.
     * REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling processTaxiArrival.");
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processTaxiArrival(Airplane* airplane, Time time);

    /**
     * Processes a request for IFR clearance.
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processIFRClearance(Airplane* airplane, Time time);

    /**
     * Processes a request for pushback.
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processPushback(Airplane* airplane, Time time);

    /**
     * Processes a request to start taxiing.
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processTaxiInitialise(Airplane* airplane, Time time);

    /**
     * Processes a request for a taxi instruction.
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processTaxiInstruction(Airplane* airplane, Time time);

    /**
     * Processes a request for takeoff when waiting at runway.
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processTakeOff(Airplane* airplane, Time time);

    /**
     * Processes a request for takeoff when waiting on runway.
     * @param airplane: the sender of the request
     * @param time: current time
     */
    void processTakeOffRunway(Airplane* airplane, Time time);

    /**
     * Write a message to the ATC stream.
     * REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling sendMessage.");
     * @param message: Message that needs to be send.
     */
    void sendMessage(const string &message);

    /**
     * Generates a squawk code for a given plane.
     * The returned code will not be generated for any other plane.
     * REQUIRE(this->properlyInitialized(), "ATC was not properly initialized when calling getSquawk.");
     */
    int getSquawk(Airplane*);

    /**
     * Getters and setters for the fields of the class.
     * REQUIRE(properlyInitialized(), "ATC wasn't properly initialized when calling getter/setter.");
     *
     * Setters:
     * ENSURE(fField == value, "Field wasn't set properly");
     */
    queue<ATCRequest*> *getQueue();
    void setLastActive(Time);
    Time getLastActive() const;
    Airport* getAirport() const;
    void setAirport(Airport*);
    bool get3occupied() const;
    void set3occupied(bool);
    bool get5occupied() const;
    void set5occupied(bool);
};


#endif //PROJECTVLIEGVELD_ATC_H