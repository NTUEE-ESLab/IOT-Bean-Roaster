#ifndef ROASTER_H
#define ROASTER_H


#include "mbed.h"

class Roaster {
public:
    Roaster(PinName TX, PinName RX);
    // ~Roaster();
    float get_roast_level();
private:

    float rl;
    BufferedSerial esp;

};

#endif