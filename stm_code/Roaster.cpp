#include "Roaster.h"
#include "mbed.h"


Roaster::Roaster(PinName TX,PinName RX):esp(TX, RX)
{
    esp.baud(115200);
    esp.format(
        8,
        SerialBase::None,
        1
    );
    
    
}

int Roaster::get_roast_level(){

    char msg[] = "-m\n";
    while (esp.readable())
    {
        esp.getc();
    }
    printf(msg);
    esp.printf(msg);
//    esp.write(msg, sizeof(msg));
    printf("msg sent");
    
   
    

    wait(3);
    printf("Start read\n");

        
        char buf[200] = {};
        buf[0] = esp.getc();
   
        
        return (int)buf[0];
//      
    
}
