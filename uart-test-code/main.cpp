/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "HTS221.h"

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE                                                  32

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led(LED1);
static DigitalOut led2(LED2);

// Create a BufferedSerial object with a default baud rate.
static BufferedSerial usbdbg(USBTX, USBRX);
static BufferedSerial esp(D1,D0);

I2C i2c(PB_11, PB_10);

int main(void)
{
    
    usbdbg.set_baud(115200);
    usbdbg.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );
    

    
    esp.set_baud(115200);
    esp.set_format(
        8,
        BufferedSerial::None,
        1
    );

    static HTS221 hts221; 
    int ret = hts221.begin();
    usbdbg.write(&ret,1);

    // Application buffer to receive the data
    char buf[MAXIMUM_BUFFER_SIZE] = {0};
    char buf2[MAXIMUM_BUFFER_SIZE] = {0};

    while (1) {
             
        
        if(usbdbg.readable()){
            uint32_t num2 = usbdbg.read(buf2,sizeof(buf2));
            esp.write(buf2,num2);    
            led2 = !led2;


        }
        
        //no input, read will return EAGAIN = enum(11)
        
        if ( esp.readable()) {
            uint32_t num = esp.read(buf, sizeof(buf));
            // Toggle the LED.
            led = !led;

            // Print the esp msg to the terminal.
            usbdbg.write(buf, num);
            

        }
        
            int humid = hts221.readHumidity();
            char temp_char[10] = {};
//            sprintf(temp,"%ld",humid);
            usbdbg.write(&humid,1);
            char blank = '\n';
            usbdbg.write(&blank,1);
            int temp = (int)hts221.readTemperature();
//            sprintf(temp_char,"%lf",temp);
            usbdbg.write(&temp,1);
            ThisThread::sleep_for(1000);

        
    }


}
