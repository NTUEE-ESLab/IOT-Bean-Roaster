#ifndef MAX6675_H
#define MAX6675_H
#include "mbed.h"
class max6675
{    
  public:
  
    max6675(PinName miso, PinName sclk, PinName cs);
        
    // read temperature 0 Centigrade, 1 Fahrenheit       
    float gettemp(int cf);    
    
  private:  
    SPI max;
    DigitalOut _cs;
    Timer t;    
};

#endif