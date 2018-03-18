#include "headers/system.h"


int main() {
    System* sys = new System();
    sys->setup("../input.xml");
    sys->run();
    return 0;
}

