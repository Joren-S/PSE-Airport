//
// Created by uauser on 3/1/18.
//

#ifndef PROJECTVLIEGVELD_SYSTEM_H
#define PROJECTVLIEGVELD_SYSTEM_H

#include "types.h"

using namespace std;

class System {
private:
    vector<airport*> airports;
    vector<airplane*> airplanes;
    vector<runway*> runways;
public:
    void setup(const string& filename);
    void log(const string& filename = "log.txt");
    void run();
};


#endif //PROJECTVLIEGVELD_SYSTEM_H
