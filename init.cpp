#include "headers/system.h"


int main() {
    System* sys = new System();
    sys->setup("../input.xml");
    sys->run();
    sys->log();
    return 0;
}
