
/*  15-02-18 Projet Test I/O
 *   Auteurs: Brahim Boujamaoui
 */


/*========= Virtuino General methods ==============================================================================================================================  
*  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)       write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
*  int  vDigitalMemoryRead(int digitalMemoryIndex)                   read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
*  void vMemoryWrite(int memoryIndex, float value);                  write a value to Virtuino memory             (memoryIndex=0..31, value range as float value)
*  float vMemoryRead(int memoryIndex);                               read a value of  Virtuino memory             (memoryIndex=0..31, returned a float value
*  run();                                                            neccesary command to communicate with Virtuino android app  (on start of void loop)
*  int getPinValue(int pin);                                         read the value of a Pin. Usefull to read the value of a PWM pin
*  long lastCommunicationTime;                                       Stores the last communication with Virtuino time
*  void vDelay(long milliseconds); 
*/ ================================================================================================================================================================

#include <WiFi.h>

#include "Virtuino_ESP_WifiServer.h"

//--- SETTINGS ------------------------------------------------
const char* ssid = "bbox2-5de4";            // WIFI network SSID
const char* password = "ARLARLARL";            // WIFI network PASSWORD
WiFiServer server(8000);                      // Server port
//-------------------------------------------------------------


Virtuino_ESP_WifiServer virtuino(&server);



//============================================================== connectToWiFiNetwork
void connectToWiFiNetwork(){
  Serial.println("Connecting to "+String(ssid));
  
   // If you don't want to config IP manually disable the next four lines
  IPAddress ip(192, 168, 1, 50);            // where 150 is the desired IP Address   
  IPAddress gateway(192, 168, 1, 1);         // set gateway to match your network
 IPAddress subnet(255, 255, 255, 0);        // set subnet mask to match your network
  WiFi.config(ip, gateway, subnet);          // If you don't want to config IP manually disable this line
  
  WiFi.mode(WIFI_STA);                       // Config module as station only.
  WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
    }
   Serial.println("");
   Serial.println("WiFi connected");
   Serial.println(WiFi.localIP());
}

//=================================================================
void initAccessPoint(){
  Serial.print("Setting soft-AP ... ");                   // Default IP: 192.168.4.1
   WiFi.mode(WIFI_AP);                                     // Config module as Access point only.  Set WiFi.mode(WIFI_AP_STA); to config module as Acces point and station
   boolean result = WiFi.softAP("DFROBOT_WIFI_SHIED_V3", "12345678");      // SSID: Virtuino Network   Password:12345678
   if(result == true)  {
    Serial.println("Server Ready");
    Serial.println(WiFi.softAPIP());
   }
   else Serial.println("Failed!");
}




//============================================================== setup
//==============================================================
//==============================================================
void setup() {
   //----- Virtuino settings
  virtuino.DEBUG=true;                         // set this value TRUE to enable the serial monitor status
  virtuino.password="";                    // Set a password or prefix to your web server for more protection 
                                               // avoid special characters like ! $ = @ # % & * on your password. Use only numbers or text characters

  Serial.begin(9600);
 // connectToWiFiNetwork();
  initAccessPoint();
  server.begin();
 
  pinMode(2,OUTPUT);
 
}


//============================================================== loop
//==============================================================
//==============================================================
void loop() {
  virtuino.run();              

// // enter your loop code here.
// 
//  int value1 = random(100);    // replace the random values with your sensors' values
//  int value2 = random(100);
//
//  virtuino.vMemoryWrite(1,value1);   // write the value1 to virtual memory V1. On Virtuino panel add a value display to virtual pin V1
//  virtuino.vMemoryWrite(2,value2);   // write the value2 to virtual memory V2  On Virtuino panel add a value display to virtual pin V2
//  
//
//  //------ avoid to use delay() function in your code
//  virtuino.vDelay(5000);        // wait 5 seconds


}


