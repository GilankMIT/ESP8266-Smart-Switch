#include <ESP8266WiFi.h>        
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>        
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
/* Initialize relay. */
int relay_1;        
int relay_1_status;
int relay_1_mode;  

int relay_2;      
int relay_2_status; 
int relay_2_mode; 

int isDt;
void setup() {

    /* Initialize relay. */
    relay_1        = 4; //Relay 1
    relay_1_status = 0; //Initiliaze Relay 1 in OFF state || 0 = Off , 1 = On
    relay_1_mode   = 0; //Initialize Relay 1 in Manual Mode || 0 = Manual , 1 = Automatic Day time saver
    
    relay_2        = 5; //Relay 2
    relay_2_status = 0; //Initiliaze Relay 2 in OFF state || 0 = Off , 1 = On
    relay_2_mode   = 0; //Initialize Relay 2 in Manual Mode || 0 = Manual , 1 = Automatic Day time saver
    isDt           = 0;

    /* Initialize pin mode as OUTPUT */
    pinMode(relay_1, OUTPUT);
    pinMode(relay_2, OUTPUT);
    
    
    Serial.begin(115200);
    Serial.println("Menghubungkan...");
  
    WiFiManager wifiManager;
    wifiManager.autoConnect("Swiper");
  
    
    //if you get here you have connected to the WiFi
    Serial.println("Berhasil Terhubung");

    
}

void loop() {
HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://smartswitch.astorytowear.com/?param=CHECKDATA"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);


              relay_1_status = payload.substring(15, 16).toInt();
              relay_1_mode   = payload.substring(7, 8).toInt();
              relay_2_mode   = payload.substring(23, 24).toInt();
              relay_2_status = payload.substring(31, 32).toInt();
              isDt           = payload.substring(38, 39).toInt();

              if(relay_1_status > -1 && relay_1_mode > -1 && relay_2_status > -1 && relay_2_mode > -1){

                   if(relay_1_mode == 1){ //Automatic Mode
                     if(isDt == 1){
                      USE_SERIAL.println("RELAY 1 NYALA | MODE OTOMATIS");
                      digitalWrite(relay_1, HIGH);
                     }else{
                      USE_SERIAL.println("RELAY 1 MATI | MODE OTOMATIS");
                      digitalWrite(relay_1, LOW);
                     }
                   }else{   //Manual Mode
                    if(relay_1_status == 1){
                      USE_SERIAL.println("RELAY 1 NYALA | MODE MANUAL");
                      digitalWrite(relay_1, HIGH);
                    }else{
                      USE_SERIAL.println("RELAY 1 MATI | MODE MANUAL");
                      digitalWrite(relay_1, LOW);
                    }
                   }



                   if(relay_2_mode == 1){ //Automatic Mode
                     if(isDt == 1){
                      USE_SERIAL.println("RELAY 2 NYALA | MODE OTOMATIS");
                      digitalWrite(relay_2, HIGH);
                     }else{
                      USE_SERIAL.println("RELAY 2 MATI | MODE OTOMATIS");
                      digitalWrite(relay_2, LOW);
                     }
                   }else{   //Manual Mode
                    if(relay_2_status == 1){
                      USE_SERIAL.println("RELAY 2 NYALA | MODE MANUAL");
                      digitalWrite(relay_2, HIGH);
                    }else{
                      USE_SERIAL.println("RELAY 2 MATI | MODE MANUAL");
                      digitalWrite(relay_2, LOW);
                    }
                   }
              }

              
             }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();

        delay(4000);
}
