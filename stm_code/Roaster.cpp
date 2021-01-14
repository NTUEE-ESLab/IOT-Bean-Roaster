#include "Roaster.h"
#include "mbed.h"


Roaster::Roaster(PinName TX,PinName RX)
:esp(TX, RX)
{
    esp.set_baud(115200);
    esp.set_format(
        8,
        BufferedSerial::None,
        1
    );
}

float Roaster::get_roast_level(){

    char msg[] = "-m\n";

    esp.write(msg, sizeof(msg));

    ThisThread::sleep_for("3s");

    if(esp.readable()){
        char buf[200] = {};
        uint32_t rb = esp.read(buf,sizeof(buf));

        if( rb >=4 ){
            rl = atof(buf);
        }

    }


    if(rl>150 || rl<0)return 0;
    else
    {
        return rl;
    }
    
}