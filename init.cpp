#include "headers/system.h"
#define FNAME ("test.xml")


int main() {
    System* sys = new System();
    sys->setup(FNAME);
    sys->run();
    return 0;
}

