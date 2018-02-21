// Test IO Siemens
// Arduino UNO 
// Slave com I2C 

//--------------------------------
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "TimerOne.h"
//--------------------------------

#define BPinitConf 7

int Input[100];

int T[10],TDN[10];
boolean CptT[10];
boolean Condition=true;

byte x = 0;

int B3[100]; // ONS

char octetReceptionProc;
char caractereReceptionProc;
String chaineReception;
String chaineReceptionProc;

/* Broche CS de la carte SD */
const byte SDCARD_CS_PIN = 4; // TODO A remplacer suivant votre shield SD
File CONFIG; // fichier contenant les indexs des cartes input ex: I0/I1/I15/ fichier:0/1/15/
int Index;

//********************************************************************************************************
void setup()
{
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
// Démarrage I2C    
  Wire.begin(); // join i2c bus (address optional for master)
  
// Init port SPI pour la carte SD
  pinMode(4, OUTPUT); // Arduino Mega
  
    /* Initialisation de la carte SD */
  Serial.println("Init SD card... ");
  if (!SD.begin(SDCARD_CS_PIN)) {
    Serial.println("FAIL");
    for(;;); //  appui sur bouton RESET
  }
  Serial.println("OK1");

  
}

//*******************************************************************************************************
void loop()
{
  
// Changement & chargement si BP1  fichier "CONFIG.txt"
// Ecriture dans le tableau Input[] 
//OneShot
 
  if (digitalRead(BPinitConf)==1) {
    if (!B3[1]) {
      B3[1] = 1;
    SDRead();    
    }
  }
  else {
    B3[1] = 0;
  }

  
// Transmission I2C
// Delai 10000
  if (Condition & !TDN[8]==1) {
    CptT[8] = true;
  }
  else {
    Serial.println("ELSE CptT[8] = true");
    T[8] =  0.0;
    TDN[8] =  0.0;
    CptT[8] = false;
  }
  if (T[8] >= 10) { // présélection de 600 donc 60 secondes
    TDN[8] =  1.0;   // bit down
    T[8] = 10;
  }
if (TDN[8]==1){
  Serial.println("TDN8 à 1");

  Wire.beginTransmission(9);  // transmit to device #9
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte
  Wire.endTransmission();     // stop transmitting
  Serial.println("Transmit"); 
  x++;
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
  if (CONFIG) { Serial.println(" CONFIG s'ouvre en ecriture");

    for (int i = 0; i <= Input[0]; i++) {
     
        //CONFIG.println(Input[i]);
   Serial.println(Input[i]);
    }
    
  }
    
   Serial.println(" CONFIG ne s'ouvre pas  en ecriture");
    
    CONFIG.close();
  
  
}
//-----------------------------------------------
//Lecture Config IN index des cartes INPUT[] à partir de l'index 1
// fichier CONFIG 0/1/12/  -> Input[1]=0;Input[2]=1; Input[3]=12;  
void SDRead()
{
  CONFIG = SD.open("CONFIG.txt", FILE_READ);
  Index = 1;
  String inString;
  if (CONFIG) {
    Serial.println(" CONFIG ouvert en lecture");
    Serial.println("Debut:");   
    while (CONFIG.available()) {
      octetReceptionProc = CONFIG.read();    
        //Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          Input[Index] = (chaineReceptionProc.toInt());
          Serial.println(Input[Index]);
        Index++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;

    }
    }   
    // close the file:
    CONFIG.close();
  } else {
  }

}
//-----------------------
 void callback() {

  if (CptT[8]) {

    T[8] =  T[8] + 1.0;
            Serial.println(T[8]);
  }
  }
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
