// Test IO Siemens
// Arduino UNO 
// Master com I2C 

//--------------------------------------------------------------------------------------------------------
//------------------------------les bibliotheques---------------------------------------------------------
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "TimerOne.h"
#include <Timer.h>
#include <VSync.h>
#include "VirtuinoEthernet_WebServer.h"                           // Neccesary virtuino library for ethernet shield
#include <Ethernet.h>
#include "Settimino.h"
//-----------------------------les declarations------------------------------------------------------------
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x05, 0xBD};                // Set the ethernet shield mac address.
IPAddress Local(192, 168, 4, 10); 
IPAddress PLC(192, 168, 4, 4); // PLC Address                                  // Set the ethernet shield ip address. Check your gateway ip address first
VirtuinoEthernet_WebServer virtuino(8000);                          // default port=8000
//--------------------------------------------------------------------------------------------------------

#define BPinitConf 7                                            //le bouton bousoir d'init est branché sur la pin 7
#define bits      8
//ValueSender<2> sender;
//ValueReceiver<2> receiver;

//  valueA and valueB are the incoming values
//int VersArd0, VersArd1;
int FirstScan = 1;

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
File myFile;
int IndexValid;
File InValid1File;
File InValid2File;
File InValid3File;
File InValid4File;
File InValid5File;
File InValid6File;
File InValid7File;
File InValid8File;
File InValid9File;
File InValid10File;
File InValid11File;
File InValid12File;
File InValid13File;
File InValid14File;
File InValid15File;
File InValid16File;
File InValid17File;
File InValid18File;
File InValid19File;
File InValid20File;

int IndexCarte=0;
S7Client Client(_S7WIRED);;
unsigned long Elapsed; // To calc the execution time
byte Buffer[1024];
int DBs;
int Buf;
String NumDB;
int IndFile;
int PasApas;
int LU = 0;

int SYNCHRO = 0;
String TramTBL = "";
int SQLecture = 0;
int SQEcriture = 0;
char charVal[10];

int valMemo = 0, valTest = 0;

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
  Ethernet.begin(mac, Local);
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
//--------------------------------Wired Ethernet Shield Initialization    
    // Start the Ethernet Library
    Ethernet.begin(mac, Local);
    // Setup Time, someone said me to leave 2000 because some 
    // rubbish compatible boards are a bit deaf.
    delay(10000); 
    Serial.println("");
    Serial.println("Cable connected");  
    Serial.print("Local IP address : ");
    Serial.println(Ethernet.localIP());
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
  if (T[8] >= 1) { // présélection de 600 donc 60 secondes
    TDN[8] =  1.0;   // bit down
    T[8] = 1;
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
    if(int(virtuino.vMemoryRead(30))==3) Ecran=3;
    if(int(virtuino.vMemoryRead(30))==4) Ecran=4;
    if(int(virtuino.vMemoryRead(30))==5) Ecran=5;
    if(int(virtuino.vMemoryRead(30))==6) Ecran=6;
    if(int(virtuino.vMemoryRead(30))==7) Ecran=7;
    if(int(virtuino.vMemoryRead(30))==8) Ecran=8;
    if(int(virtuino.vMemoryRead(30))==9) Ecran=9;
    if(int(virtuino.vMemoryRead(30))==10) Ecran=10;
    if(int(virtuino.vMemoryRead(30))==11) Ecran=11;
    if(int(virtuino.vMemoryRead(30))==12) Ecran=12;
    if(int(virtuino.vMemoryRead(30))==13) Ecran=13;
    if(int(virtuino.vMemoryRead(30))==14) Ecran=14;
    if(int(virtuino.vMemoryRead(30))==15) Ecran=15;
    if(int(virtuino.vMemoryRead(30))==16) Ecran=16;
    if(int(virtuino.vMemoryRead(30))==17) Ecran=17;
    if(int(virtuino.vMemoryRead(30))==18) Ecran=18;
    if(int(virtuino.vMemoryRead(30))==19) Ecran=19;
    if(int(virtuino.vMemoryRead(30))==20) Ecran=20;
  }
  switch (Ecran) {
  case 0:
    // statements
    break;
    
  case 1: 
  // Communication S7
  virtuino.vMemoryWrite(31, Input[1]);
  IndexCarte=Input[1];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In1[i]=bitRead(Buffer[0], i) ; 
       }  
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In1[i]);
        virtuino.vMemoryWrite(i+21, InValid1[i]);
 
      }
      memoEcran=Ecran;
    }
   
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In1[i]);
        InValid1[i]=int(virtuino.vMemoryRead(i+21));
      }
    if(virtuino.vDigitalMemoryRead(8))
    {
SDWriteFile1();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
  case 2:
    // Communication S7
  virtuino.vMemoryWrite(31, Input[2]);
  IndexCarte=Input[2];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In2[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In2[i]);
        virtuino.vMemoryWrite(i+21, InValid2[i]);  
      }
      memoEcran=Ecran;
    }
  
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In2[i]);
        InValid2[i]=int(virtuino.vMemoryRead(i+21));
      }
    if(virtuino.vDigitalMemoryRead(8))
    {
SDWriteFile2();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
  case 3:
    // Communication S7
  virtuino.vMemoryWrite(31, Input[3]);
  IndexCarte=Input[3];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In3[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In3[i]);
        virtuino.vMemoryWrite(i+21, InValid3[i]);  
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In3[i]);
        InValid3[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {
SDWriteFile3();
      virtuino.vDigitalMemoryWrite(8,0);
      }
    break;
    
  case 4:
      // Communication S7
  virtuino.vMemoryWrite(31, Input[4]);
  IndexCarte=Input[4];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In4[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In4[i]);
        virtuino.vMemoryWrite(i+21, InValid4[i]);  
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In4[i]);
        InValid4[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {
SDWriteFile4();
      virtuino.vDigitalMemoryWrite(8,0);
      }
    break;
    
      case 5:
            // Communication S7
  virtuino.vMemoryWrite(31, Input[5]);
  IndexCarte=Input[5];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In5[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In5[i]);
        virtuino.vMemoryWrite(i+21, InValid5[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In5[i]);
        InValid5[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile5();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
      case 6:
                  // Communication S7
  virtuino.vMemoryWrite(31, Input[6]);
  IndexCarte=Input[6];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In6[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In6[i]);
        virtuino.vMemoryWrite(i+21, InValid6[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In6[i]);
        InValid6[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile6();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
      case 7:
                  // Communication S7
  virtuino.vMemoryWrite(31, Input[7]);
  IndexCarte=Input[7];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In7[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In7[i]);
        virtuino.vMemoryWrite(i+21, InValid7[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In7[i]);
        InValid7[i]=int(virtuino.vMemoryRead(i+21));
      }
                if(virtuino.vDigitalMemoryRead(8))
          {
SDWriteFile7();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
      case 8:
                  // Communication S7
  virtuino.vMemoryWrite(31, Input[8]);
  IndexCarte=Input[8];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In8[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In8[i]);
        virtuino.vMemoryWrite(i+21, InValid8[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In8[i]);
        InValid8[i]=int(virtuino.vMemoryRead(i+21));
      }
                if(virtuino.vDigitalMemoryRead(8))
          {
SDWriteFile8();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
      case 9:
            // Communication S7
  virtuino.vMemoryWrite(31, Input[9]);
  IndexCarte=Input[9];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In9[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In9[i]);
        virtuino.vMemoryWrite(i+21, InValid9[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In9[i]);
        InValid9[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile9();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
      case 10:
                 // Communication S7
  virtuino.vMemoryWrite(31, Input[10]);
  IndexCarte=Input[10];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In10[i]=bitRead(Buffer[0], i) ; 
       }
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In10[i]);
        virtuino.vMemoryWrite(i+21, InValid10[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In10[i]);
        InValid10[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile10();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
     case 11:
  // Communication S7
  virtuino.vMemoryWrite(31, Input[11]);
  IndexCarte=Input[11];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In11[i]=bitRead(Buffer[0], i) ; 
       }
       
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In11[i]);
        virtuino.vMemoryWrite(i+21, InValid11[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In11[i]);
        InValid11[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile11();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
  case 12:
                   // Communication S7
  virtuino.vMemoryWrite(31, Input[12]);
  IndexCarte=Input[12];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In12[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In12[i]);
        virtuino.vMemoryWrite(i+21, InValid12[i]);  
      }
      memoEcran=Ecran;
    }
   
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In12[i]);
        InValid12[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile12();
      virtuino.vDigitalMemoryWrite(8,0);
      }
    break;
    
  case 13:
                   // Communication S7
  virtuino.vMemoryWrite(31, Input[13]);
  IndexCarte=Input[13];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In13[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In13[i]);
        virtuino.vMemoryWrite(i+21, InValid13[i]);  
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In13[i]);
        InValid13[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile13();
      virtuino.vDigitalMemoryWrite(8,0);
      }
    break;
    
  case 14:
                   // Communication S7
  virtuino.vMemoryWrite(31, Input[14]);
  IndexCarte=Input[14];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In14[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In14[i]);
        virtuino.vMemoryWrite(i+21, InValid14[i]);  
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In14[i]);
        InValid14[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile14();
      virtuino.vDigitalMemoryWrite(8,0);
      }
    break;
    
      case 15:
                       // Communication S7
  virtuino.vMemoryWrite(31, Input[15]);
  IndexCarte=Input[15];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In15[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In15[i]);
        virtuino.vMemoryWrite(i+21, InValid15[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In15[i]);
        InValid15[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile15();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
      case 16:
                       // Communication S7
  virtuino.vMemoryWrite(31, Input[16]);
  IndexCarte=Input[16];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In16[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In16[i]);
        virtuino.vMemoryWrite(i+21, InValid16[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In16[i]);
        InValid16[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile16();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
      case 17:
                       // Communication S7
  virtuino.vMemoryWrite(31, Input[17]);
  IndexCarte=Input[17];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In17[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In17[i]);
        virtuino.vMemoryWrite(i+21, InValid17[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In17[i]);
        InValid17[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile17();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
      case 18:
                       // Communication S7
  virtuino.vMemoryWrite(31, Input[18]);
  IndexCarte=Input[18];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In18[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In18[i]);
        virtuino.vMemoryWrite(i+21, InValid18[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In18[i]);
        InValid18[i]=int(virtuino.vMemoryRead(i+21));
      }
                if(virtuino.vDigitalMemoryRead(8))
          {
SDWriteFile18();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
      case 19:
                       // Communication S7
  virtuino.vMemoryWrite(31, Input[19]);
  IndexCarte=Input[19];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In19[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In19[i]);
        virtuino.vMemoryWrite(i+21, InValid19[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In19[i]);
        InValid19[i]=int(virtuino.vMemoryRead(i+21));
      }
          if(virtuino.vDigitalMemoryRead(8))
          {      
SDWriteFile19();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break;
    
      case 20:
                       // Communication S7
  virtuino.vMemoryWrite(31, Input[20]);
  IndexCarte=Input[20];
        Senttimino();
        for(i=0;i<8;i++)
        {
          In20[i]=bitRead(Buffer[0], i) ; 
       }
    if(Ecran!=memoEcran)
    {
      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In20[i]);
        virtuino.vMemoryWrite(i+21, InValid20[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<8;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In20[i]);
        InValid20[i]=int(virtuino.vMemoryRead(i+21));
      }
                if(virtuino.vDigitalMemoryRead(8))
          {
SDWriteFile20();
      virtuino.vDigitalMemoryWrite(8,0);
      }

    break; 
      
 } 

 FirstScan = 0;

 
}

//===============================================================================================
void Senttimino() {
  int Size, Result;
  void *Target;
   
  Size=1;
  Target = &Buffer; // Uses a larger buffer

  
  // Connection
  while (!Client.Connected)
  {
    if (!Connect())
      delay(1);
  }
  
  //Serial.print("Reading ");Serial.print(Size);Serial.print(" bytes from DB");Serial.println(DBNum);
  // Get the current tick
  //Serial.print("Reading ");
  MarkTime();
  Result=Client.ReadArea(S7AreaPE, // We are requesting DB access
                         0,    // DB Number
                         IndexCarte,        // Start from byte N.0
                         1,     // We need "Size" bytes
                         Target);  // Put them into our target (Buffer or PDU)
  if (Result==0)
  {
    ShowTime();
    Dump(Target, Size);
  }
  else {
    CheckError(Result);
    
  //delay(500);  
  }
}

//-----------------------------------------------
//Lecture Config IN index des cartes INPUT[] à partir de l'index 1
// fichier CONFIG 0/1/12/  -> Input[1]=0;Input[2]=1; Input[3]=12;  
void SDRead()
{
// Lecture fichier Input[]
  CONFIG = SD.open("CONFIG.txt", FILE_READ);
  Index = 1;
      Serial.print("Debut:......"); 
          Serial.println(CONFIG); 
  if (CONFIG) {
    Serial.println(" CONFIG ouvert en lecture");
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
  }
  

 // Lecture fichier InValid1[]
  InValid1File = SD.open("InValid1.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InValid1File); 
  if (InValid1File) {
    Serial.println(" InValid1File ouvert en lecture");
    Serial.println("Debut:");   
    while (InValid1File.available()) {
      octetReceptionProc = InValid1File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InValid1[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InValid1[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InValid1File.close();
  }
 
}
//-----------------------
 void callback() {

  if (CptT[8]) {

    T[8] =  T[8] + 1.0;
            //Serial.println(T[8]);
  }
  }
  
  //-------------------------
void SDWriteFile1() {
  SD.remove("InValid1.txt");
  InValid1File = SD.open("InValid1.txt", FILE_WRITE);
  InValid1File.seek(0);
  Serial.print("InValid1");Serial.println(InValid1File);
  if (InValid1File) {

    for (int i = 0; i <8; i++) {
     
        InValid1File.print(InValid1[i]);
        InValid1File.print("/");
   Serial.print(InValid1[i]);
    }
    InValid1File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}
 //-------------------------
void SDWriteFile2() {
  SD.remove("InValid2.txt");
  InValid2File = SD.open("InValid2.txt", FILE_WRITE);
  InValid2File.seek(0);
  Serial.print("InValid2");Serial.println(InValid2File);
  if (InValid2File) {

    for (int i = 0; i <8; i++) {
     
        InValid2File.print(InValid2[i]);
        InValid2File.print("/");
   Serial.print(InValid2[i]);
    }
    InValid2File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile3() {
  SD.remove("InValid3.txt");
  InValid3File = SD.open("InValid3.txt", FILE_WRITE);
  InValid3File.seek(0);
  Serial.print("InValid3");Serial.println(InValid3File);
  if (InValid3File) {

    for (int i = 0; i <8; i++) {
     
        InValid3File.print(InValid3[i]);
        InValid3File.print("/");
   Serial.print(InValid3[i]);
    }
    InValid3File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile4() {
  SD.remove("InValid4.txt");
  InValid4File = SD.open("InValid4.txt", FILE_WRITE);
  InValid4File.seek(0);
  Serial.print("InValid4");Serial.println(InValid4File);
  if (InValid4File) {

    for (int i = 0; i <8; i++) {
     
        InValid4File.print(InValid4[i]);
        InValid4File.print("/");
   Serial.print(InValid4[i]);
    }
    InValid4File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}
//-------------------------
void SDWriteFile5() {
  SD.remove("InValid5.txt");
  InValid5File = SD.open("InValid5.txt", FILE_WRITE);
  InValid5File.seek(0);
  Serial.print("InValid5");Serial.println(InValid5File);
  if (InValid5File) {

    for (int i = 0; i <8; i++) {
     
        InValid5File.print(InValid5[i]);
        InValid5File.print("/");
   Serial.print(InValid5[i]);
    }
    InValid5File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile6() {
  SD.remove("InValid6.txt");
  InValid6File = SD.open("InValid6.txt", FILE_WRITE);
  InValid6File.seek(0);
  Serial.print("InValid6");Serial.println(InValid6File);
  if (InValid6File) {

    for (int i = 0; i <8; i++) {
     
        InValid6File.print(InValid6[i]);
        InValid6File.print("/");
   Serial.print(InValid6[i]);
    }
    InValid6File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile7() {
  SD.remove("InValid7.txt");
  InValid7File = SD.open("InValid7.txt", FILE_WRITE);
  InValid7File.seek(0);
  Serial.print("InValid7");Serial.println(InValid7File);
  if (InValid7File) {

    for (int i = 0; i <8; i++) {
     
        InValid7File.print(InValid7[i]);
        InValid7File.print("/");
   Serial.print(InValid7[i]);
    }
    InValid7File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile8() {
  SD.remove("InValid8.txt");
  InValid8File = SD.open("InValid8.txt", FILE_WRITE);
  InValid8File.seek(0);
  Serial.print("InValid8");Serial.println(InValid8File);
  if (InValid8File) {

    for (int i = 0; i <8; i++) {
     
        InValid8File.print(InValid8[i]);
        InValid8File.print("/");
   Serial.print(InValid8[i]);
    }
    InValid8File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile9() {
  SD.remove("InValid9.txt");
  InValid9File = SD.open("InValid9.txt", FILE_WRITE);
  InValid9File.seek(0);
  Serial.print("InValid9");Serial.println(InValid9File);
  if (InValid9File) {

    for (int i = 0; i <8; i++) {
     
        InValid9File.print(InValid9[i]);
        InValid9File.print("/");
   Serial.print(InValid9[i]);
    }
    InValid9File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile10() {
  SD.remove("InValid10.txt");
  InValid10File = SD.open("InValid10.txt", FILE_WRITE);
  InValid10File.seek(0);
  Serial.print("InValid10");Serial.println(InValid10File);
  if (InValid10File) {

    for (int i = 0; i <8; i++) {
     
        InValid10File.print(InValid10[i]);
        InValid10File.print("/");
   Serial.print(InValid10[i]);
    }
    InValid10File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile11() {
  SD.remove("InValid11.txt");
  InValid11File = SD.open("InValid11.txt", FILE_WRITE);
  InValid11File.seek(0);
  Serial.print("InValid11");Serial.println(InValid11File);
  if (InValid11File) {

    for (int i = 0; i <8; i++) {
     
        InValid11File.print(InValid11[i]);
        InValid11File.print("/");
   Serial.print(InValid11[i]);
    }
    InValid11File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile12() {
  SD.remove("InValid12.txt");
  InValid12File = SD.open("InValid12.txt", FILE_WRITE);
  InValid12File.seek(0);
  Serial.print("InValid12");Serial.println(InValid12File);
  if (InValid12File) {

    for (int i = 0; i <8; i++) {
     
        InValid12File.print(InValid12[i]);
        InValid12File.print("/");
   Serial.print(InValid12[i]);
    }
    InValid12File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile13() {
  SD.remove("InValid13.txt");
  InValid13File = SD.open("InValid13.txt", FILE_WRITE);
  InValid13File.seek(0);
  Serial.print("InValid13");Serial.println(InValid13File);
  if (InValid13File) {

    for (int i = 0; i <8; i++) {
     
        InValid13File.print(InValid13[i]);
        InValid13File.print("/");
   Serial.print(InValid13[i]);
    }
    InValid13File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile14() {
  SD.remove("InValid14.txt");
  InValid14File = SD.open("InValid14.txt", FILE_WRITE);
  InValid14File.seek(0);
  Serial.print("InValid14");Serial.println(InValid14File);
  if (InValid14File) {

    for (int i = 0; i <8; i++) {
     
        InValid14File.print(InValid14[i]);
        InValid14File.print("/");
   Serial.print(InValid14[i]);
    }
    InValid14File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile15() {
  SD.remove("InValid15.txt");
  InValid15File = SD.open("InValid15.txt", FILE_WRITE);
  InValid15File.seek(0);
  Serial.print("InValid15");Serial.println(InValid15File);
  if (InValid15File) {

    for (int i = 0; i <8; i++) {
     
        InValid15File.print(InValid15[i]);
        InValid15File.print("/");
   Serial.print(InValid15[i]);
    }
    InValid15File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile16() {
  SD.remove("InValid16.txt");
  InValid16File = SD.open("InValid16.txt", FILE_WRITE);
  InValid16File.seek(0);
  Serial.print("InValid16");Serial.println(InValid16File);
  if (InValid16File) {

    for (int i = 0; i <8; i++) {
     
        InValid16File.print(InValid16[i]);
        InValid16File.print("/");
   Serial.print(InValid16[i]);
    }
    InValid16File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile17() {
  SD.remove("InValid17.txt");
  InValid17File = SD.open("InValid17.txt", FILE_WRITE);
  InValid17File.seek(0);
  Serial.print("InValid17");Serial.println(InValid17File);
  if (InValid17File) {

    for (int i = 0; i <8; i++) {
     
        InValid17File.print(InValid17[i]);
        InValid17File.print("/");
   Serial.print(InValid17[i]);
    }
    InValid17File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile18() {
  SD.remove("InValid18.txt");
  InValid18File = SD.open("InValid18.txt", FILE_WRITE);
  InValid18File.seek(0);
  Serial.print("InValid18");Serial.println(InValid18File);
  if (InValid18File) {

    for (int i = 0; i <8; i++) {
     
        InValid18File.print(InValid18[i]);
        InValid18File.print("/");
   Serial.print(InValid18[i]);
    }
    InValid18File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile19() {
  SD.remove("InValid19.txt");
  InValid19File = SD.open("InValid19.txt", FILE_WRITE);
  InValid19File.seek(0);
  Serial.print("InValid19");Serial.println(InValid19File);
  if (InValid19File) {

    for (int i = 0; i <8; i++) {
     
        InValid19File.print(InValid19[i]);
        InValid19File.print("/");
   Serial.print(InValid19[i]);
    }
    InValid19File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile20() {
  SD.remove("InValid20.txt");
  InValid20File = SD.open("InValid20.txt", FILE_WRITE);
  InValid20File.seek(0);
  Serial.print("InValid20");Serial.println(InValid20File);
  if (InValid20File) {

    for (int i = 0; i <8; i++) {
     
        InValid20File.print(InValid20[i]);
        InValid20File.print("/");
   Serial.print(InValid20[i]);
    }
    InValid20File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

 //-------------------------
//----------------------------
void Lecture() {


  if ((LU == 1) and (SYNCHRO == 1)) {
    LU = 0;
    TramTBL = "";

    dtostrf( SQLecture, 1, 0, charVal);
    TramTBL = TramTBL + "D01" +  charVal;
    TramTBL = TramTBL + "F";

    dtostrf( valMemo, 1, 0, charVal);
    TramTBL = TramTBL + "D02" +  charVal;
    TramTBL = TramTBL + "F";

    dtostrf( valTest, 1, 0, charVal);
    TramTBL = TramTBL + "D03" + charVal;
    TramTBL = TramTBL + "F";

    //Serial.print(TramTBL);
    //Serial.println(TramTBL);

  }
  //delay(1);
}


//----------------------------------------------------------------------
// Connects to the PLC
//----------------------------------------------------------------------

bool Connect()
{
    int Result=Client.ConnectTo(PLC, 
                                  0,  // Rack (see the doc.)
                                  0); // Slot (see the doc.)
    Serial.print("Connecting to ");Serial.println(PLC);  
    if (Result==0) 
    {
      Serial.print("Connected ! PDU Length = ");Serial.println(Client.GetPDULength());
    }
    else
      Serial.println("Connection error");
    return Result==0;
} 
  
//----------------------------------------------------------------------
// Buffer VERS plc
//----------------------------------------------------------------------
void DumpVersPLC(void *Buffer, int Length)
{


}

//----------------------------------------------------------------------
// Dumps a buffer
//----------------------------------------------------------------------
void Dump(void *Buffer, int Length)
{
  int i, cnt = 0;
  pbyte buf;

  if (Buffer != NULL)
    buf = pbyte(Buffer);
  else
    buf = pbyte(&PDU.DATA[0]);

  //Serial.print("[ Dumping "); Serial.print(Length);
  //Serial.println(" bytes ]==========================");
  IndFile = Length;
  for (i = 0; i < Length; i++)
  {
    cnt++;
    if (buf[i] < 0x10)
      //Serial.print("0");
    //Serial.print(buf[i], HEX);
    Buf = buf[i];
    //Serial.print(" ");
    if (cnt == 16)
    {
      cnt = 0;
      //Serial.println();
    }
  }
  //Serial.println("===============================================");
}
//----------------------------------------------------------------------
// Prints the Error number
//----------------------------------------------------------------------
void CheckError(int ErrNo)
{
  Serial.print("Error No. 0x");
  Serial.println(ErrNo, HEX);

  // Checks if it's a Severe Error => we need to disconnect
  if (ErrNo & 0x00FF)
  {
    Serial.println("SEVERE ERROR, disconnecting.");
    Client.Disconnect();
  }
}
//----------------------
// Profiling routines
//
void MarkTime()
{
  Elapsed = millis();
  // Calcs the time
  Elapsed = millis() - Elapsed;
  //Serial.print("Job time (ms) : ");
  //Serial.println(Elapsed);
}  
  
//----------------------------------------------------------------------
void ShowTime()
{
  // Calcs the time
  Elapsed=millis()-Elapsed;
  //Serial.print("Job time (ms) : ");
  //Serial.println(Elapsed);   
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




