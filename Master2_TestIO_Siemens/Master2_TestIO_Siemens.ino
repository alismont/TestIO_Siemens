// Test IO Siemens
// Arduino UNO 
// Master com I2C 

//--------------------------------------------------------------------------------------------------------
//------------------------------les bibliotheques---------------------------------------------------------
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "TimerOne.h"
#include <VSync.h>
#include "VirtuinoEthernet_WebServer.h"                           // Neccesary virtuino library for ethernet shield

//-----------------------------les declarations------------------------------------------------------------
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x05, 0xBD};                // Set the ethernet shield mac address.
IPAddress ip(192, 168, 4, 1);                                   // Set the ethernet shield ip address. Check your gateway ip address first
VirtuinoEthernet_WebServer virtuino(8000);                          // default port=8000
//--------------------------------

#define BPinitConf 7                                            //le bouton bousoir d'init est branché sur la pin 7
#define bits      8
//ValueSender<2> sender;
//ValueReceiver<2> receiver;

//  valueA and valueB are the incoming values
//int VersArd0, VersArd1;

char caractereReception;
char octetReception;
char octetReceptionProc;
char caractereReceptionProc;
String chaineReception;
String chaineReceptionProc,Mess;
int cpt;
//  doubleA and doubleB are the outgoing variables
//int doubleA, doubleB;

int Input[100];
int In1[8];
//int In1[bits], In2[bits], In3[bits], In4[bits], In5[bits], In6[bits], In7[bits], In8[bits], In9[bits], In10[bits];
//int In11[bits], In12[bits], In13[bits], In14[bits], In15[bits], In16[bits], In17[bits], In18[bits], In19[bits], In20[bits];
int InValid1[8];
int In2[8];
int InValid2[8];
int In3[8];
int InValid3[8];
int In4[8];
int InValid4[8];
int In5[8];
int InValid5[8];
int In6[8];
int InValid6[8];
int In7[8];
int InValid7[8];
int In8[8];
int InValid8[8];
int In9[8];
int InValid9[8];
int In10[8];
int InValid10[8];
int In11[8];
int InValid11[8];
int In12[8];
int InValid12[8];
int In13[8];
int InValid13[8];
int In14[8];
int InValid14[8];
int In15[8];
int InValid15[8];
int In16[8];
int InValid16[8];
int In17[8];
int InValid17[8];
int In18[8];
int InValid18[8];
int In19[8];
int InValid19[8];
int In20[8];
int InValid20[8];

int i;
byte x = 0;
int T[10],TDN[10];
boolean CptT[10];
boolean Condition=true;
int memoEcran,Ecran;

int B3[100]; // ONS



/* Broche CS de la carte SD */
const byte SDCARD_CS_PIN = 4; // TODO A remplacer suivant votre shield SD
File CONFIG; // fichier contenant les indexs des cartes input ex: I0/I1/I15/ fichier:0/1/15/
int Index;


//********************************************************************************************************
void setup()
{


  //sender.observe(VersArd0);
  //sender.observe(VersArd1);

 // receiver.observe(valueA);
 // receiver.observe(valueB);

// Init tempos
 Timer1.initialize(100000);         // initialize timer1
  Timer1.attachInterrupt(callback);


  
// Pin 7 -> BP init fichier CONFIG
pinMode(BPinitConf,INPUT);

// Init port série
      Serial.begin(9600); 
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
// Virtuino
   virtuino.DEBUG=false;                                                                                // Enable this line only if DEBUG=true

// Mac adresse   
  Ethernet.begin(mac, ip);
  virtuino.password="12345678"; 
  

// Init port SPI pour la carte SD
  pinMode(4, OUTPUT); // Arduino Mega
  
    /* Initialisation de la carte SD */
  //Serial.println("Init SD card... ");
  if (!SD.begin(SDCARD_CS_PIN)) {
    //Serial.println("FAIL");
    for(;;); //  appui sur bouton RESET
  }
  //Serial.println("OK1");  
  
  cpt=0;

}

//*******************************************************************************************************
void loop()
{
// communication vers processing
while (Serial.available() > 0) { // si un caractère en réception
    octetReception = Serial.read(); // lit le 1er octet de la file d'attente
    if (octetReception == 13) { // si Octet reçu est le saut de ligne

      if (chaineReception == "Lu")  {
      cpt++;
      Mess= String(Ecran)+" OK/*";
        Serial.println(Mess);
      }   
      
      chaineReception = ""; 
 
      }
      else { 
      caractereReception = char(octetReception); 
      chaineReception = chaineReception + caractereReception; 
    }
    }




// Virtuino
     virtuino.run();
  // VersArd0++;//=Input[0];
 // VersArd1=Input[2];   
  //receiver.sync();
 // doubleA = valueA * 2;
 // doubleB = valueB * 2;
  //sender.sync();

// Changement & chargement si BP1  fichier "CONFIG.txt"
// Ecriture dans le tableau Input[] 
//OneShot
 int SimuBPinitConf=1;
  if (digitalRead(BPinitConf)) {
    if (!B3[1]) {
      B3[1] = 1;
    SDRead();    
    }
  }
  else {
    B3[1] = 0;
  }

  
// Delai en 1/10 de secondes 100=10s
  if (Condition & !TDN[8]==1) {
    CptT[8] = true;
  }
  else {
    //Serial.println("ELSE CptT[8] = true");
    T[8] =  0.0;
    TDN[8] =  0.0;
    CptT[8] = false;
  }
  if (T[8] >= 50) { // présélection de 600 donc 60 secondes
    TDN[8] =  1.0;   // bit down
    T[8] = 50;
  }
if (TDN[8]==1){

// action TON8
// ECRITURE INDEX DES CARTES D' ENTREES VERS TABLETTE
  for(i=1;i<Input[0];i++)
  {
        virtuino.vMemoryWrite(i,Input[i]);
        //println(Input[i]);
  }
}

// Test cartes
  Ecran=int(virtuino.vMemoryRead(0));
  if(int(virtuino.vMemoryRead(0))==1)
  {
    if(int(virtuino.vMemoryRead(30))==1) Ecran=1;
    if(int(virtuino.vMemoryRead(30))==2) Ecran=2;
  }
  switch (Ecran) {
  case 0:
    // statements
    break;
    
  case 1:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In1[i]);
        virtuino.vMemoryWrite(i+21, InValid1[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In1[i]);
        InValid1[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
  case 2:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In2[i]);
        virtuino.vMemoryWrite(i+21, InValid2[i]);  
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{    
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In2[i]);
        InValid2[i]=int(virtuino.vMemoryRead(i+21));
      }
         //   virtuino.vDigitalMemoryWrite(8,0);
    // } 
    break;
    
  case 3:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In3[i]);
        virtuino.vMemoryWrite(i+21, InValid3[i]);  
      }
      memoEcran=Ecran;
    }
        //if(virtuino.vDigitalMemoryRead(8))
       // {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In3[i]);
        InValid3[i]=int(virtuino.vMemoryRead(i+21));
      }
          //  virtuino.vDigitalMemoryWrite(8,0);
      //}
    break;
    
  case 4:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In4[i]);
        virtuino.vMemoryWrite(i+21, InValid4[i]);  
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In4[i]);
        InValid4[i]=int(virtuino.vMemoryRead(i+21));
      }
            //virtuino.vDigitalMemoryWrite(8,0);
     // }
    break;
    
      case 5:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In5[i]);
        virtuino.vMemoryWrite(i+21, InValid5[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In5[i]);
        InValid5[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
      case 6:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In6[i]);
        virtuino.vMemoryWrite(i+21, InValid6[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In6[i]);
        InValid6[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
      case 7:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In7[i]);
        virtuino.vMemoryWrite(i+21, InValid7[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In7[i]);
        InValid7[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
      case 8:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In8[i]);
        virtuino.vMemoryWrite(i+21, InValid8[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In8[i]);
        InValid8[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
      case 9:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In9[i]);
        virtuino.vMemoryWrite(i+21, InValid9[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In9[i]);
        InValid9[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
      case 10:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In10[i]);
        virtuino.vMemoryWrite(i+21, InValid10[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In10[i]);
        InValid10[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
     case 11:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In11[i]);
        virtuino.vMemoryWrite(i+21, InValid11[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In11[i]);
        InValid11[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
  case 12:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In12[i]);
        virtuino.vMemoryWrite(i+21, InValid12[i]);  
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{    
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In12[i]);
        InValid12[i]=int(virtuino.vMemoryRead(i+21));
      }
         //   virtuino.vDigitalMemoryWrite(8,0);
    // } 
    break;
    
  case 13:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In13[i]);
        virtuino.vMemoryWrite(i+21, InValid13[i]);  
      }
      memoEcran=Ecran;
    }
        //if(virtuino.vDigitalMemoryRead(8))
       // {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In13[i]);
        InValid13[i]=int(virtuino.vMemoryRead(i+21));
      }
          //  virtuino.vDigitalMemoryWrite(8,0);
      //}
    break;
    
  case 14:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In14[i]);
        virtuino.vMemoryWrite(i+21, InValid14[i]);  
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In14[i]);
        InValid14[i]=int(virtuino.vMemoryRead(i+21));
      }
            //virtuino.vDigitalMemoryWrite(8,0);
     // }
    break;
    
      case 15:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In15[i]);
        virtuino.vMemoryWrite(i+21, InValid15[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In15[i]);
        InValid15[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
      case 16:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In16[i]);
        virtuino.vMemoryWrite(i+21, InValid16[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In16[i]);
        InValid16[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
      case 17:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In17[i]);
        virtuino.vMemoryWrite(i+21, InValid17[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In17[i]);
        InValid17[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
      case 18:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In18[i]);
        virtuino.vMemoryWrite(i+21, InValid18[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In18[i]);
        InValid18[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
      case 19:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In19[i]);
        virtuino.vMemoryWrite(i+21, InValid19[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In19[i]);
        InValid19[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break;
    
      case 20:
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In20[i]);
        virtuino.vMemoryWrite(i+21, InValid20[i]);
 
      }
      memoEcran=Ecran;
    }
    //if(virtuino.vDigitalMemoryRead(8))
    //{
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In20[i]);
        InValid20[i]=int(virtuino.vMemoryRead(i+21));
      }
     // virtuino.vDigitalMemoryWrite(8,0);
      //}

    break; 
      
 } 

}



//========================================================================================================
void SDWrite() {
  boolean RemoveFile;
  RemoveFile = SD.exists("CONFIG.txt");

  if (RemoveFile) {
    SD.remove("CONFIG.txt");
  }
  CONFIG = SD.open("CONFIG.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (CONFIG) { //println(" CONFIG s'ouvre en ecriture");

    for (int i = 0; i <= Input[0]; i++) {
     
        //CONFIG.println(Input[i]);
   //Serial.println(Input[i]);
    }
    
  }
    
   //Serial.println(" CONFIG ne s'ouvre pas  en ecriture");
    
    CONFIG.close();
  
  
}
//-----------------------------------------------
//Lecture Config IN index des cartes INPUT[] à partir de l'index 1
// fichier CONFIG 0/1/12/  -> Input[1]=0;Input[2]=1; Input[3]=12;  
void SDRead()
{
  CONFIG = SD.open("CONFIG.txt", FILE_READ);
  Index = 1;
     // Serial.print("Debut:......"); 
       //   Serial.println(CONFIG); 
  if (CONFIG) {
  //  Serial.println(" CONFIG ouvert en lecture");
  //  Serial.println("Debut:");   
    while (CONFIG.available()) {
      octetReceptionProc = CONFIG.read();    
        //Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          Input[Index] = (chaineReceptionProc.toInt());
         // Serial.println(Input[Index]);
        Index++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;

    }
    }   
    // close the file:
    Input[0]=Index;
    CONFIG.close();
  } else {
  }

}
//-----------------------
 void callback() {

  if (CptT[8]) {

    T[8] =  T[8] + 1.0;
            //Serial.println(T[8]);
  }
  }
  
  
  
// MEMOIRE
//----------------------
// Temporisation  TON
/*
if (Condition) {
    CptT[8] = true;
  }
  else {
    T[8] =  0.0;
    TDN[8] =  0.0;
    CptT[8] = false;
  }
  if (T[8] >= 600) { // présélection de 600 donc 60 secondes
    TDN[8] =  1.0;   // bit down
    T[8] = 600;
  }

}

// Temprisation TOF
if (Condition) {
    T[8] =  0.0;
    TDN[8] =  1.0;  // bit down
    CptT[8] = false;
  }
  else {
    CptT[8] = true;
  }

  if (T[8] >= 100) { //présélection de 100 donc 10 secondes
    TDN[8] =  0.0;
    T[8] = 100;
  }
*/
/* I2C CLASS
 * Wire.requestFrom(9,2)
 *
 */
//-------------------------------
  //virtuino.vMemoryWrite(20,mouseX);
  // virtuino.vMemoryWrite(21,mouseY);
  // mouseX++;
  // mouseY++;
  //
  //       //analogValue= 69;//int(virtuino.vMemoryRead(22));  
  //    //Serial.println(analogValue); 
  //-----------------------------
  
/*========= Virtuino General methods ==============================================================================================================================  
*  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)       write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
*  int  vDigitalMemoryRead(int digitalMemoryIndex)                   read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
*  void vMemoryWrite(int memoryIndex, float value);                  write a value to Virtuino memory             (memoryIndex=0..31, value range as float value)
*  float vMemoryRead(int memoryIndex);                               read a value of  Virtuino memory             (memoryIndex=0..31, returned a float value
*  run();                                                            neccesary command to communicate with Virtuino android app  (on start of void loop)
*  int getPinValue(int pin);                                         read the value of a Pin. Usefull to read the value of a PWM pin
*  long lastCommunicationTime;                                       Stores the last communication with Virtuino time
*  void vDelay(long milliseconds); 
*/




