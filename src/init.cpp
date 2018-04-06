#include "../headers/System.h"

int main() {
    System system;
    system.setup("../input.xml");
    system.run();
    system.log();
    return 0;
}