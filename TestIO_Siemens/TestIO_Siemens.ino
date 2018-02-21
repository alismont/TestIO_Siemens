
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
================================================================================================================================================================*/ 


#include <WiFi.h>
#include "Virtuino_ESP_WifiServer.h"

//========== SETTING SI UTILISATION DU RESEAU  BBOX ================== 
const char* ssid = "bbox2-5de4";            // WIFI network SSID
const char* password = "ARLARLARL";            // WIFI network PASSWORD
WiFiServer server(8000);                      // Server port
//====================================================================


Virtuino_ESP_WifiServer virtuino(&server);

//================= VARIABLE =========================================

int Index_Panel=0,Memo_Index_Panel=0; // 0=Menu 1=Panel_IO1....etc
int Input[3]; // contient l'index de chaque cartes entrées ici 2
boolean IN1[7]; // Etat des bits 0-7 de la 1er carte d'entrée
boolean IN2[7]; // Etat des bits 0-7 de la 2er carte d'entrée
int Input1Valid[7],Input2Valid[7];
//====================================================================
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
   boolean result = WiFi.softAP("Virtuino Network", "12345678");      // SSID: Virtuino Network   Password:12345678
   if(result == true)  {
    Serial.println("Server Ready");
    Serial.println(WiFi.softAPIP());
   }
   else Serial.println("Failed!");
}


//+++++++++++++++++++ SEUP ++++++++++++++++++++++++++++++++++++++++++
void setup() {
   //----- Virtuino settings
  virtuino.DEBUG=false;                         // set this value TRUE to enable the serial monitor status
  virtuino.password="";                    // Set a password or prefix to your web server for more protection 
                                               // avoid special characters like ! $ = @ # % & * on your password. Use only numbers or text characters

  Serial.begin(9600);
 // connectToWiFiNetwork();
  initAccessPoint();
  server.begin();
  

 
}

//++++++++++++++++++ LOOP ++++++++++++++++++++++++++++++++++++++++++
void loop() {
  virtuino.run();              

//-----------------------
// Lecture fichier Index INPUT OUPUT
// A Faire -> Tableau Input[i];
Input[1]=0;
Input[2]=6;

//-----------------------
 // Lecture Index Panel
 //0=Menu
Index_Panel=virtuino.vMemoryRead(0);

//Menu
if(Index_Panel==0){

 virtuino.vMemoryWrite(1, Input[1]); // 1er carte INPUT 0-7 bits  V1= index 1ere carte ENTREE
 virtuino.vMemoryWrite(2, Input[2]); // 1er carte INPUT 0-7 bits  V2= index 2ere carte ENTREE
}

//2em Panel traitement 1er carte d'entrée
if(Index_Panel==1){
   virtuino.vMemoryWrite(1, Input[1]); // 1er carte INPUT 0-7 bits  V1= index 1ere carte ENTREE

//Ecriture de l ETAT des tests pour cet Index, précédent
if (Memo_Index_Panel!=Index_Panel){
 virtuino.vMemoryWrite(10,Input1Valid[0]);
 virtuino.vMemoryWrite(11,Input1Valid[1]);
 virtuino.vMemoryWrite(12,Input1Valid[2]); 
 virtuino.vMemoryWrite(13,Input1Valid[3]); 
 virtuino.vMemoryWrite(14,Input1Valid[4]); 
 virtuino.vMemoryWrite(15,Input1Valid[5]); 
 virtuino.vMemoryWrite(16,Input1Valid[6]); 
 virtuino.vMemoryWrite(17,Input1Valid[7]);
   Memo_Index_Panel=Index_Panel;
}
  
// visualisation état bits 0-7 I index DV0 à DV7
virtuino.vDigitalMemoryWrite(0,IN1[0]);
virtuino.vDigitalMemoryWrite(1,IN1[1]);
virtuino.vDigitalMemoryWrite(2,IN1[2]);
virtuino.vDigitalMemoryWrite(3,IN1[3]);
virtuino.vDigitalMemoryWrite(4,IN1[4]);
virtuino.vDigitalMemoryWrite(5,IN1[5]);
virtuino.vDigitalMemoryWrite(6,IN1[6]);
virtuino.vDigitalMemoryWrite(7,IN1[7]);

//Validation

if (virtuino.vDigitalMemoryRead(8)){
    Input1Valid[0] = virtuino.vMemoryRead(10);
    Input1Valid[1] = virtuino.vMemoryRead(11);
    Input1Valid[2] = virtuino.vMemoryRead(12);
    Input1Valid[3] = virtuino.vMemoryRead(13);
    Input1Valid[4] = virtuino.vMemoryRead(14);
    Input1Valid[5] = virtuino.vMemoryRead(15);
    Input1Valid[6] = virtuino.vMemoryRead(16);
    Input1Valid[7] = virtuino.vMemoryRead(17);
    virtuino.vDigitalMemoryWrite(8,0);
}
}

//3em Panel traitement 1er carte d'entrée
// visualisation état bits 0-7 I index DV0 à DV7
if(Index_Panel==2){
   virtuino.vMemoryWrite(2, Input[2]); // 1er carte INPUT 0-7 bits  V2= index 2ere carte ENTREE

//Ecriture de l ETAT des tests pour cet Index, précédent
if (Memo_Index_Panel!=Index_Panel){
 virtuino.vMemoryWrite(10,Input2Valid[0]);
 virtuino.vMemoryWrite(11,Input2Valid[1]);
 virtuino.vMemoryWrite(12,Input2Valid[2]); 
 virtuino.vMemoryWrite(13,Input2Valid[3]); 
 virtuino.vMemoryWrite(14,Input2Valid[4]); 
 virtuino.vMemoryWrite(15,Input2Valid[5]); 
 virtuino.vMemoryWrite(16,Input2Valid[6]); 
 virtuino.vMemoryWrite(17,Input2Valid[7]);
   Memo_Index_Panel=Index_Panel;
}
   
virtuino.vDigitalMemoryWrite(0,IN2[0]);
virtuino.vDigitalMemoryWrite(1,IN2[1]);
virtuino.vDigitalMemoryWrite(2,IN2[2]);
virtuino.vDigitalMemoryWrite(3,IN2[3]);
virtuino.vDigitalMemoryWrite(4,IN2[4]);
virtuino.vDigitalMemoryWrite(5,IN2[5]);
virtuino.vDigitalMemoryWrite(6,IN2[6]);
virtuino.vDigitalMemoryWrite(7,IN2[7]);

if (virtuino.vDigitalMemoryRead(8)){
    Input2Valid[0] = virtuino.vMemoryRead(10);
    Input2Valid[0] = virtuino.vMemoryRead(11);
    Input2Valid[0] = virtuino.vMemoryRead(12);
    Input2Valid[0] = virtuino.vMemoryRead(13);
    Input2Valid[0] = virtuino.vMemoryRead(14);
    Input2Valid[0] = virtuino.vMemoryRead(15);
    Input2Valid[0] = virtuino.vMemoryRead(16);
    Input2Valid[0] = virtuino.vMemoryRead(17);
        virtuino.vDigitalMemoryWrite(8,0);
}
}


  //------ avoid to use delay() function in your code
  virtuino.vDelay(1000);        // wait 5 seconds


delay(100);
}


