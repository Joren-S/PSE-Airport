//
// Created by Joren Servotte on 21.04.18.
//

#ifndef PROJECTVLIEGVELD_ATC_H
#define PROJECTVLIEGVELD_ATC_H

#include <queue>
#include "Time.h"


struct ATCMessage {
    // Used to store a message for the ATC alongside the original time.

    /**
     * Time message was sent.
     */
    Time fTime;

    /**
     * Content of message that was sent.
     */
    string fMessage;

    /**
     * Constructor.
     */
    ATCMessage(Time time, string msg) : fTime(time), fMessage(msg) {}
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
    queue<ATCMessage*> fQueue;

    /**
    * Time when ATC was last active.
    */
    Time fLastActive;

public:

    /**
    * Constructor
    * ENSURE: properlyInitialized
    */
    ATC(ostream& stream);

    /**
    * Checks if the object is properly initialized
    */
    bool properlyInitialized() const;

    /**
    * Send a message.
    * Message gets queued if it was unable to be sent.
    * REQUIRE: properlyInitialized
    * REQUIRE: time can't be before the last time the ATC was active (can't send messages in the past)
    * ENSURE: message was queued if unable to send
    */
    void sendMessage(Time, string, string);

    /**
    * Check if the ATC can send a message.
    */
    bool canSend(Time) const;

    /**
    * Return the amount of messages queued.
    * This is also the time in minutes it will take until all messages are sent.
    * REQUIRE: properlyInitialized
    * ENSURE: return >= 0
    */
    int getQueueSize() const;

    /**
    * Return the next queued message.
    * If a message is found (queue is not empty), said message is removed from the queue.
    * Returns NULL if queue is empty and no message was found.
    * REQUIRE: properlyInitialized
    * ENSURE: if queue is not empty -> return is not NULL
    * ENSURE: if queue is not empty -> return is no longer in queue
    */
    ATCMessage *getNextQueuedMessage() const;

    /**
    * Creates a correctly formatted ATC message when the contents are given.
    */
    string formatMessage(Time, string, string) const;

    /**
    * Check the queue for a queued message and send this message if possible.
     * REQUIRE: properlyInitialized.
    */
    void doHeartbeat(Time);


    // Getters and Setters
    queue<ATCMessage*> getQueue() const;
    void setLastActive(Time);
    Time getLastActive() const;
};


#endif //PROJECTVLIEGVELD_ATC_H