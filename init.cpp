#include "headers/system.h"


int main() {
    System *sys = new System();

    try {
        sys->setup("../input.xml");
        sys->run();
        sys->log();
    }
    catch (runtime_error &error) {
        cerr << error.what() << endl;
    }

    delete sys;
    return 0;
}
