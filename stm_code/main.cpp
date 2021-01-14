/* WiFi Example
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include "mbed.h"
#include "HTS221.h"
#include "max6675.h"
#include "Roaster.h"

#include "TCPSocket.h"
#include "TCPServer.h"
//#include "stm32l475e_iot01_accelero.h"


#define WIFI_IDW0XX1    2

#if (defined(TARGET_DISCO_L475VG_IOT01A) || defined(TARGET_DISCO_F413ZH))
#include "ISM43362Interface.h"
//ISM43362Interface wifi(MBED_CONF_APP_WIFI_SPI_MOSI, MBED_CONF_APP_WIFI_SPI_MISO, MBED_CONF_APP_WIFI_SPI_SCLK, MBED_CONF_APP_WIFI_SPI_NSS, MBED_CONF_APP_WIFI_RESET, MBED_CONF_APP_WIFI_DATAREADY, MBED_CONF_APP_WIFI_WAKEUP, false);
ISM43362Interface wifi(false);
#else // External WiFi modules

#if MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1
#include "SpwfSAInterface.h"
SpwfSAInterface wifi(MBED_CONF_APP_WIFI_TX, MBED_CONF_APP_WIFI_RX);
#endif // MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1

#endif

#define SCALE_MULTIPLIER    0.004



//used by HTS
    I2C i2c(PB_11, PB_10);
HTS221 hts221; 



const char *sec2str(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Unknown";
    }
}

int scan_demo(WiFiInterface *wifi)
{
    WiFiAccessPoint *ap;

    printf("Scan:\n");

    int count = wifi->scan(NULL,0);
    printf("%d networks available.\n", count);

    /* Limit number of network arbitrary to 15 */
    count = count < 15 ? count : 15;

    ap = new WiFiAccessPoint[count];
    count = wifi->scan(ap, count);
    for (int i = 0; i < count; i++)
    {
        printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\n", ap[i].get_ssid(),
               sec2str(ap[i].get_security()), ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
               ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5], ap[i].get_rssi(), ap[i].get_channel());
    }

    delete[] ap;
    return count;
}

void acc_server(NetworkInterface *net)
{
    /* 
    TCPServer socket;
    TCPSocket* client;*/
    TCPSocket socket;
    SocketAddress addr("192.168.0.39",65431);
    nsapi_error_t response;

    int16_t pDataXYZ[3] = {0};
    char recv_buffer[9];
    char acc_json[64];
    int sample_num = 0;

//    Roaster roaster(PD_5,PD_6);

    max6675 probe(D12, D13, D10);
    
//    max6675 probe(PD_3, PD_1, PD_5);

    // Open a socket on the network interface, and create a TCP connection to addr
    response = socket.open(net);
    if (0 != response){
        printf("Error opening: %d\n", response);
    }
    response = socket.connect(addr);
    
    if (0 != response){
        printf("Error connecting: %d\n", response);
    }


    socket.set_blocking(1);
    while (1){
        ++sample_num;
        //BSP_ACCELERO_AccGetXYZ(pDataXYZ);
        float x = 0.1, y = 0.3, z = 0.4;
        int humid = hts221.readHumidity();
        float temp = probe.gettemp(0);
        printf("Humid=%d\n",humid);
        printf("Temperature=%d\n",temp);
        
//        float roast_level = roaster.get_roast_level();
        float roast_level = 0;
        
        printf("Roast=%d\n",roast_level);
        
        int len = sprintf(acc_json,"{\"h\":%f,\"t\":%f,\"r\":%f,\"s\":%d}",(float)humid,
                                        temp, roast_level, sample_num);


        response = socket.send(acc_json,len);
        if (0 >= response){
            printf("Error seding: %d\n", response);
        }
        wait(0.9);
    

    }

 
    socket.close();
}

int main()
{
    
    int hts_ret = hts221.begin();
    

    printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
    //wifi.set_network("192.168.130.105","255.255.255.0","192.168.130.254");
    int ret = wifi.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error\n");
        return -1;
    }

    printf("Success\n\n");
    printf("MAC: %s\n", wifi.get_mac_address());
    printf("IP: %s\n", wifi.get_ip_address());
    printf("Netmask: %s\n", wifi.get_netmask());
    printf("Gateway: %s\n", wifi.get_gateway());
    printf("RSSI: %d\n\n", wifi.get_rssi());



    //BSP_ACCELERO_Init();    


    acc_server(&wifi);



}
