#ifndef ROASTER_H
#define ROASTER_H


#include "mbed.h"


class Roaster {
public:
    Roaster(PinName TX, PinName RX);
    // ~Roaster();
    int get_roast_level();
 
    

private:
    
    int rl;
    Serial esp;

};

#endif
