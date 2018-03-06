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
int InAValid1[8];
int In2[8];
int InAValid2[8];
int In3[8];
int InAValid3[8];
int In4[8];
int InAValid4[8];
int In5[8];
int InAValid5[8];
int In6[8];
int InAValid6[8];
int In7[8];
int InAValid7[8];
int In8[8];
int InAValid8[8];
int In9[8];
int InAValid9[8];
int In10[8];
int InAValid10[8];
int In11[8];
int InAValid11[8];
int In12[8];
int InAValid12[8];
int In13[8];
int InAValid13[8];
int In14[8];
int InAValid14[8];
int In15[8];
int InAValid15[8];
int In16[8];
int InAValid16[8];
int In17[8];
int InAValid17[8];
int In18[8];
int InAValid18[8];
int In19[8];
int InAValid19[8];
int In20[8];
int InAValid20[8];

int i;
byte x = 0;
int T[10],TDN[10];
boolean CptT[10];
boolean Condition=true;
int memoEcran,Ecran;

int B3[100]; // ONS



/* Broche CS de la carte SD */
const byte SDCARD_CS_PIN = 4; // TODO A remplacer suivant votre shield SD
File CONFIGAI; // fichier contenant les indexs des cartes input ex: I0/I1/I15/ fichier:0/1/15/
int Index;
File myFile;
int IndexValid;
File InAValid1File;
File InAValid2File;
File InAValid3File;
File InAValid4File;
File InAValid5File;
File InAValid6File;
File InAValid7File;
File InAValid8File;
File InAValid9File;
File InAValid10File;
File InAValid11File;
File InAValid12File;
File InAValid13File;
File InAValid14File;
File InAValid15File;
File InAValid16File;
File InAValid17File;
File InAValid18File;
File InAValid19File;
File InAValid20File;

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

// Pin 7 -> BP init fichier CONFIGAI
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
    
        SDRead();    
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

// Changement & chargement si BP1  fichier "CONFIGAI.txt"
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

      In1[0]=(256*Buffer[0])+Buffer[1] ;
      In1[1]=(256*Buffer[2])+Buffer[3] ;
      In1[2]=(256*Buffer[4])+Buffer[5] ;
      In1[3]=(256*Buffer[6])+Buffer[7] ;

    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In1[i]);
        virtuino.vMemoryWrite(i+21, InAValid1[i]);
 
      }
      memoEcran=Ecran;
    }
   
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In1[i]);
        InAValid1[i]=int(virtuino.vMemoryRead(i+21));
      }
    if(virtuino.vDigitalMemoryRead(8)) // VALIDATION
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

      In2[0]=(256*Buffer[0])+Buffer[1] ;
      In2[1]=(256*Buffer[2])+Buffer[3] ;
      In2[2]=(256*Buffer[4])+Buffer[5] ;
      In2[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In2[i]);
        virtuino.vMemoryWrite(i+21, InAValid2[i]);  
      }
      memoEcran=Ecran;
    }
  
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In2[i]);
        InAValid2[i]=int(virtuino.vMemoryRead(i+21));
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
      In3[0]=(256*Buffer[0])+Buffer[1] ;
      In3[1]=(256*Buffer[2])+Buffer[3] ;
      In3[2]=(256*Buffer[4])+Buffer[5] ;
      In3[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In3[i]);
        virtuino.vMemoryWrite(i+21, InAValid3[i]);  
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In3[i]);
        InAValid3[i]=int(virtuino.vMemoryRead(i+21));
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
      In4[0]=(256*Buffer[0])+Buffer[1] ;
      In4[1]=(256*Buffer[2])+Buffer[3] ;
      In4[2]=(256*Buffer[4])+Buffer[5] ;
      In4[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In4[i]);
        virtuino.vMemoryWrite(i+21, InAValid4[i]);  
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In4[i]);
        InAValid4[i]=int(virtuino.vMemoryRead(i+21));
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
      In5[0]=(256*Buffer[0])+Buffer[1] ;
      In5[1]=(256*Buffer[2])+Buffer[3] ;
      In5[2]=(256*Buffer[4])+Buffer[5] ;
      In5[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In5[i]);
        virtuino.vMemoryWrite(i+21, InAValid5[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In5[i]);
        InAValid5[i]=int(virtuino.vMemoryRead(i+21));
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
      In6[0]=(256*Buffer[0])+Buffer[1] ;
      In6[1]=(256*Buffer[2])+Buffer[3] ;
      In6[2]=(256*Buffer[4])+Buffer[5] ;
      In6[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In6[i]);
        virtuino.vMemoryWrite(i+21, InAValid6[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In6[i]);
        InAValid6[i]=int(virtuino.vMemoryRead(i+21));
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
      In7[0]=(256*Buffer[0])+Buffer[1] ;
      In7[1]=(256*Buffer[2])+Buffer[3] ;
      In7[2]=(256*Buffer[4])+Buffer[5] ;
      In7[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In7[i]);
        virtuino.vMemoryWrite(i+21, InAValid7[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In7[i]);
        InAValid7[i]=int(virtuino.vMemoryRead(i+21));
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
      In8[0]=(256*Buffer[0])+Buffer[1] ;
      In8[1]=(256*Buffer[2])+Buffer[3] ;
      In8[2]=(256*Buffer[4])+Buffer[5] ;
      In8[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In8[i]);
        virtuino.vMemoryWrite(i+21, InAValid8[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In8[i]);
        InAValid8[i]=int(virtuino.vMemoryRead(i+21));
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
      In9[0]=(256*Buffer[0])+Buffer[1] ;
      In9[1]=(256*Buffer[2])+Buffer[3] ;
      In9[2]=(256*Buffer[4])+Buffer[5] ;
      In9[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In9[i]);
        virtuino.vMemoryWrite(i+21, InAValid9[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In9[i]);
        InAValid9[i]=int(virtuino.vMemoryRead(i+21));
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
      In10[0]=(256*Buffer[0])+Buffer[1] ;
      In10[1]=(256*Buffer[2])+Buffer[3] ;
      In10[2]=(256*Buffer[4])+Buffer[5] ;
      In10[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In10[i]);
        virtuino.vMemoryWrite(i+21, InAValid10[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In10[i]);
        InAValid10[i]=int(virtuino.vMemoryRead(i+21));
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
      In11[0]=(256*Buffer[0])+Buffer[1] ;
      In11[1]=(256*Buffer[2])+Buffer[3] ;
      In11[2]=(256*Buffer[4])+Buffer[5] ;
      In11[3]=(256*Buffer[6])+Buffer[7] ;
       
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In11[i]);
        virtuino.vMemoryWrite(i+21, InAValid11[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In11[i]);
        InAValid11[i]=int(virtuino.vMemoryRead(i+21));
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
      In12[0]=(256*Buffer[0])+Buffer[1] ;
      In12[1]=(256*Buffer[2])+Buffer[3] ;
      In12[2]=(256*Buffer[4])+Buffer[5] ;
      In12[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In12[i]);
        virtuino.vMemoryWrite(i+21, InAValid12[i]);  
      }
      memoEcran=Ecran;
    }
   
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In12[i]);
        InAValid12[i]=int(virtuino.vMemoryRead(i+21));
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
      In13[0]=(256*Buffer[0])+Buffer[1] ;
      In13[1]=(256*Buffer[2])+Buffer[3] ;
      In13[2]=(256*Buffer[4])+Buffer[5] ;
      In13[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In13[i]);
        virtuino.vMemoryWrite(i+21, InAValid13[i]);  
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In13[i]);
        InAValid13[i]=int(virtuino.vMemoryRead(i+21));
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
      In14[0]=(256*Buffer[0])+Buffer[1] ;
      In14[1]=(256*Buffer[2])+Buffer[3] ;
      In14[2]=(256*Buffer[4])+Buffer[5] ;
      In14[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In14[i]);
        virtuino.vMemoryWrite(i+21, InAValid14[i]);  
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In14[i]);
        InAValid14[i]=int(virtuino.vMemoryRead(i+21));
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
      In15[0]=(256*Buffer[0])+Buffer[1] ;
      In15[1]=(256*Buffer[2])+Buffer[3] ;
      In15[2]=(256*Buffer[4])+Buffer[5] ;
      In15[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In15[i]);
        virtuino.vMemoryWrite(i+21, InAValid15[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In15[i]);
        InAValid15[i]=int(virtuino.vMemoryRead(i+21));
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
      In16[0]=(256*Buffer[0])+Buffer[1] ;
      In16[1]=(256*Buffer[2])+Buffer[3] ;
      In16[2]=(256*Buffer[4])+Buffer[5] ;
      In16[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In16[i]);
        virtuino.vMemoryWrite(i+21, InAValid16[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In16[i]);
        InAValid16[i]=int(virtuino.vMemoryRead(i+21));
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
      In17[0]=(256*Buffer[0])+Buffer[1] ;
      In17[1]=(256*Buffer[2])+Buffer[3] ;
      In17[2]=(256*Buffer[4])+Buffer[5] ;
      In17[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In17[i]);
        virtuino.vMemoryWrite(i+21, InAValid17[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In17[i]);
        InAValid17[i]=int(virtuino.vMemoryRead(i+21));
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
      In18[0]=(256*Buffer[0])+Buffer[1] ;
      In18[1]=(256*Buffer[2])+Buffer[3] ;
      In18[2]=(256*Buffer[4])+Buffer[5] ;
      In18[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In18[i]);
        virtuino.vMemoryWrite(i+21, InAValid18[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In18[i]);
        InAValid18[i]=int(virtuino.vMemoryRead(i+21));
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
      In19[0]=(256*Buffer[0])+Buffer[1] ;
      In19[1]=(256*Buffer[2])+Buffer[3] ;
      In19[2]=(256*Buffer[4])+Buffer[5] ;
      In19[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In19[i]);
        virtuino.vMemoryWrite(i+21, InAValid19[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In19[i]);
        InAValid19[i]=int(virtuino.vMemoryRead(i+21));
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
      In20[0]=(256*Buffer[0])+Buffer[1] ;
      In20[1]=(256*Buffer[2])+Buffer[3] ;
      In20[2]=(256*Buffer[4])+Buffer[5] ;
      In20[3]=(256*Buffer[6])+Buffer[7] ;
      
    if(Ecran!=memoEcran)
    {
      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In20[i]);
        virtuino.vMemoryWrite(i+21, InAValid20[i]);
 
      }
      memoEcran=Ecran;
    }

      for(i=0;i<4;i++)
      {
        virtuino.vDigitalMemoryWrite(i, In20[i]);
        InAValid20[i]=int(virtuino.vMemoryRead(i+21));
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
   
  Size=8;
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
  Result=Client.ReadArea(S7AreaPE, // We are requesting PE access
                         0,    // DB Number
                         IndexCarte,        // Start from byte N.0
                         8,     // We need "Size" bytes
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
//Lecture CONFIGAI IN index des cartes INPUT[] à partir de l'index 1
// fichier CONFIGAI 0/4/8/  -> Input[1]=0;Input[2]=1; Input[3]=12;  
void SDRead()
{
// Lecture fichier Input[]
  CONFIGAI = SD.open("CONFIGAI.txt", FILE_READ);
  Index = 1;
      Serial.print("Debut:......"); 
          Serial.println(CONFIGAI); 
  if (CONFIGAI) {
    Serial.println(" CONFIGAI ouvert en lecture");
  //  Serial.println("Debut:");   
    while (CONFIGAI.available()) {
      octetReceptionProc = CONFIGAI.read();    
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
    CONFIGAI.close();
  }
  

 // Lecture fichier InAValid1[]
  InAValid1File = SD.open("InAVal1.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid1File); 
  if (InAValid1File) {
    Serial.println(" InAValid1File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid1File.available()) {
      octetReceptionProc = InAValid1File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid1[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid1[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid1File.close();
  }
  
   // Lecture fichier InAValid2[]
  InAValid2File = SD.open("InAVal2.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid2File); 
  if (InAValid2File) {
    Serial.println(" InAValid2File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid2File.available()) {
      octetReceptionProc = InAValid2File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid2[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid2[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid2File.close();
  }
 
  // Lecture fichier InAValid20[]
  InAValid20File = SD.open("InAVal20.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid20File); 
  if (InAValid20File) {
    Serial.println(" InAValid20File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid20File.available()) {
      octetReceptionProc = InAValid20File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid20[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid20[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid20File.close();
  }
  
  // Lecture fichier InAValid19[]
  InAValid19File = SD.open("InAVal19.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid19File); 
  if (InAValid19File) {
    Serial.println(" InAValid19File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid19File.available()) {
      octetReceptionProc = InAValid19File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid19[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid19[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid19File.close();
  }
  
  // Lecture fichier InAValid18[]
  InAValid18File = SD.open("InAVal18.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid18File); 
  if (InAValid18File) {
    Serial.println(" InAValid18File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid18File.available()) {
      octetReceptionProc = InAValid18File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid18[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid18[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid18File.close();
  }
  
  // Lecture fichier InAValid17[]
  InAValid17File = SD.open("InAVal17.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid17File); 
  if (InAValid17File) {
    Serial.println(" InAValid17File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid17File.available()) {
      octetReceptionProc = InAValid17File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid17[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid17[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid17File.close();
  }
  
  // Lecture fichier InAValid16[]
  InAValid16File = SD.open("InAVal16.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid16File); 
  if (InAValid16File) {
    Serial.println(" InAValid16File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid16File.available()) {
      octetReceptionProc = InAValid16File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid16[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid16[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid16File.close();
  }
  
  // Lecture fichier InAValid15[]
  InAValid15File = SD.open("InAVal15.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid15File); 
  if (InAValid15File) {
    Serial.println(" InAValid15File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid15File.available()) {
      octetReceptionProc = InAValid15File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid15[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid15[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid15File.close();
  }
  
  // Lecture fichier InAValid14[]
  InAValid14File = SD.open("InAVal14.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid14File); 
  if (InAValid14File) {
    Serial.println(" InAValid14File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid14File.available()) {
      octetReceptionProc = InAValid14File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid14[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid14[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid14File.close();
  }
  
  // Lecture fichier InAValid13[]
  InAValid13File = SD.open("InAVal13.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid13File); 
  if (InAValid13File) {
    Serial.println(" InAValid13File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid13File.available()) {
      octetReceptionProc = InAValid13File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid13[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid13[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid13File.close();
  }
  
  // Lecture fichier InAValid12[]
  InAValid12File = SD.open("InAVal12.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid12File); 
  if (InAValid12File) {
    Serial.println(" InAValid12File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid12File.available()) {
      octetReceptionProc = InAValid12File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid12[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid12[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid12File.close();
  }
  
  // Lecture fichier InAValid11[]
  InAValid11File = SD.open("InAVal11.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid11File); 
  if (InAValid11File) {
    Serial.println(" InAValid11File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid11File.available()) {
      octetReceptionProc = InAValid11File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid11[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid11[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid11File.close();
  }
  
  // Lecture fichier InAValid10[]
  InAValid10File = SD.open("InAVal10.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid10File); 
  if (InAValid10File) {
    Serial.println(" InAValid10File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid10File.available()) {
      octetReceptionProc = InAValid10File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid10[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid10[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid10File.close();
  }
  
  // Lecture fichier InAValid9[]
  InAValid9File = SD.open("InAVal9.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid9File); 
  if (InAValid9File) {
    Serial.println(" InAValid9File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid9File.available()) {
      octetReceptionProc = InAValid9File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid9[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid9[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid9File.close();
  }
  
  // Lecture fichier InAValid8[]
  InAValid8File = SD.open("InAVal8.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid8File); 
  if (InAValid8File) {
    Serial.println(" InAValid8File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid8File.available()) {
      octetReceptionProc = InAValid8File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid8[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid8[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid8File.close();
  }
  
  // Lecture fichier InAValid7[]
  InAValid7File = SD.open("InAVal7.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid7File); 
  if (InAValid7File) {
    Serial.println(" InAValid7File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid7File.available()) {
      octetReceptionProc = InAValid7File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid7[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid7[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid7File.close();
  }
  
  // Lecture fichier InAValid6[]
  InAValid6File = SD.open("InAVal6.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid6File); 
  if (InAValid6File) {
    Serial.println(" InAValid6File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid6File.available()) {
      octetReceptionProc = InAValid6File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid6[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid6[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid6File.close();
  }
  
  // Lecture fichier InAValid5[]
  InAValid5File = SD.open("InAVal5.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid5File); 
  if (InAValid5File) {
    Serial.println(" InAValid5File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid5File.available()) {
      octetReceptionProc = InAValid5File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid5[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid5[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid5File.close();
  }
  
  // Lecture fichier InAValid4[]
  InAValid4File = SD.open("InAVal4.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid4File); 
  if (InAValid4File) {
    Serial.println(" InAValid4File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid4File.available()) {
      octetReceptionProc = InAValid4File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid4[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid4[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid4File.close();
  }
  
  // Lecture fichier InAValid3[]
  InAValid3File = SD.open("InAVal3.txt", FILE_READ);
  IndexValid = 0;
      Serial.print("Debut:......"); 
          Serial.println(InAValid3File); 
  if (InAValid3File) {
    Serial.println(" InAValid3File ouvert en lecture");
    Serial.println("Debut:");   
    while (InAValid3File.available()) {
      octetReceptionProc = InAValid3File.read();    
        Serial.println(octetReceptionProc);
       if (octetReceptionProc == '/') {
          InAValid3[IndexValid] = (chaineReceptionProc.toInt());
          Serial.println(InAValid3[IndexValid]);
        IndexValid++;
        chaineReceptionProc="";
      }
    else {
      caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;  
      }
        }   
    // close the file:

    InAValid3File.close();
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
  SD.remove("InAVal1.txt");
  InAValid1File = SD.open("InAVal1.txt", FILE_WRITE);
  InAValid1File.seek(0);
  Serial.print("InAValid1");Serial.println(InAValid1File);
  if (InAValid1File) {

    for (int i = 0; i <4; i++) {
     
        InAValid1File.print(InAValid1[i]);
        InAValid1File.print("/");
   Serial.print(InAValid1[i]);
    }
    InAValid1File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}
 //-------------------------
void SDWriteFile2() {
  SD.remove("InAVal2.txt");
  InAValid2File = SD.open("InAVal2.txt", FILE_WRITE);
  InAValid2File.seek(0);
  Serial.print("InAValid2");Serial.println(InAValid2File);
  if (InAValid2File) {

    for (int i = 0; i <4; i++) {
     
        InAValid2File.print(InAValid2[i]);
        InAValid2File.print("/");
   Serial.print(InAValid2[i]);
    }
    InAValid2File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile3() {
  SD.remove("InAVal3.txt");
  InAValid3File = SD.open("InAVal3.txt", FILE_WRITE);
  InAValid3File.seek(0);
  Serial.print("InAValid3");Serial.println(InAValid3File);
  if (InAValid3File) {

    for (int i = 0; i <4; i++) {
     
        InAValid3File.print(InAValid3[i]);
        InAValid3File.print("/");
   Serial.print(InAValid3[i]);
    }
    InAValid3File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile4() {
  SD.remove("InAVal4.txt");
  InAValid4File = SD.open("InAVal4.txt", FILE_WRITE);
  InAValid4File.seek(0);
  Serial.print("InAValid4");Serial.println(InAValid4File);
  if (InAValid4File) {

    for (int i = 0; i <4; i++) {
     
        InAValid4File.print(InAValid4[i]);
        InAValid4File.print("/");
   Serial.print(InAValid4[i]);
    }
    InAValid4File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}
//-------------------------
void SDWriteFile5() {
  SD.remove("InAVal5.txt");
  InAValid5File = SD.open("InAVal5.txt", FILE_WRITE);
  InAValid5File.seek(0);
  Serial.print("InAValid5");Serial.println(InAValid5File);
  if (InAValid5File) {

    for (int i = 0; i <4; i++) {
     
        InAValid5File.print(InAValid5[i]);
        InAValid5File.print("/");
   Serial.print(InAValid5[i]);
    }
    InAValid5File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile6() {
  SD.remove("InAVal6.txt");
  InAValid6File = SD.open("InAVal6.txt", FILE_WRITE);
  InAValid6File.seek(0);
  Serial.print("InAValid6");Serial.println(InAValid6File);
  if (InAValid6File) {

    for (int i = 0; i <4; i++) {
     
        InAValid6File.print(InAValid6[i]);
        InAValid6File.print("/");
   Serial.print(InAValid6[i]);
    }
    InAValid6File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile7() {
  SD.remove("InAVal7.txt");
  InAValid7File = SD.open("InAVal7.txt", FILE_WRITE);
  InAValid7File.seek(0);
  Serial.print("InAValid7");Serial.println(InAValid7File);
  if (InAValid7File) {

    for (int i = 0; i <4; i++) {
     
        InAValid7File.print(InAValid7[i]);
        InAValid7File.print("/");
   Serial.print(InAValid7[i]);
    }
    InAValid7File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile8() {
  SD.remove("InAVal8.txt");
  InAValid8File = SD.open("InAVal8.txt", FILE_WRITE);
  InAValid8File.seek(0);
  Serial.print("InAValid8");Serial.println(InAValid8File);
  if (InAValid8File) {

    for (int i = 0; i <4; i++) {
     
        InAValid8File.print(InAValid8[i]);
        InAValid8File.print("/");
   Serial.print(InAValid8[i]);
    }
    InAValid8File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile9() {
  SD.remove("InAVal9.txt");
  InAValid9File = SD.open("InAVal9.txt", FILE_WRITE);
  InAValid9File.seek(0);
  Serial.print("InAValid9");Serial.println(InAValid9File);
  if (InAValid9File) {

    for (int i = 0; i <4; i++) {
     
        InAValid9File.print(InAValid9[i]);
        InAValid9File.print("/");
   Serial.print(InAValid9[i]);
    }
    InAValid9File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile10() {
  SD.remove("InAVal10.txt");
  InAValid10File = SD.open("InAVal10.txt", FILE_WRITE);
  InAValid10File.seek(0);
  Serial.print("InAValid10");Serial.println(InAValid10File);
  if (InAValid10File) {

    for (int i = 0; i <4; i++) {
     
        InAValid10File.print(InAValid10[i]);
        InAValid10File.print("/");
   Serial.print(InAValid10[i]);
    }
    InAValid10File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile11() {
  SD.remove("InAVal11.txt");
  InAValid11File = SD.open("InAVal11.txt", FILE_WRITE);
  InAValid11File.seek(0);
  Serial.print("InAValid11");Serial.println(InAValid11File);
  if (InAValid11File) {

    for (int i = 0; i <4; i++) {
     
        InAValid11File.print(InAValid11[i]);
        InAValid11File.print("/");
   Serial.print(InAValid11[i]);
    }
    InAValid11File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile12() {
  SD.remove("InAVal12.txt");
  InAValid12File = SD.open("InAVal12.txt", FILE_WRITE);
  InAValid12File.seek(0);
  Serial.print("InAValid12");Serial.println(InAValid12File);
  if (InAValid12File) {

    for (int i = 0; i <4; i++) {
     
        InAValid12File.print(InAValid12[i]);
        InAValid12File.print("/");
   Serial.print(InAValid12[i]);
    }
    InAValid12File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile13() {
  SD.remove("InAVal13.txt");
  InAValid13File = SD.open("InAVal13.txt", FILE_WRITE);
  InAValid13File.seek(0);
  Serial.print("InAValid13");Serial.println(InAValid13File);
  if (InAValid13File) {

    for (int i = 0; i <4; i++) {
     
        InAValid13File.print(InAValid13[i]);
        InAValid13File.print("/");
   Serial.print(InAValid13[i]);
    }
    InAValid13File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile14() {
  SD.remove("InAVal14.txt");
  InAValid14File = SD.open("InAVal14.txt", FILE_WRITE);
  InAValid14File.seek(0);
  Serial.print("InAValid14");Serial.println(InAValid14File);
  if (InAValid14File) {

    for (int i = 0; i <4; i++) {
     
        InAValid14File.print(InAValid14[i]);
        InAValid14File.print("/");
   Serial.print(InAValid14[i]);
    }
    InAValid14File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile15() {
  SD.remove("InAVal15.txt");
  InAValid15File = SD.open("InAVal15.txt", FILE_WRITE);
  InAValid15File.seek(0);
  Serial.print("InAValid15");Serial.println(InAValid15File);
  if (InAValid15File) {

    for (int i = 0; i <4; i++) {
     
        InAValid15File.print(InAValid15[i]);
        InAValid15File.print("/");
   Serial.print(InAValid15[i]);
    }
    InAValid15File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile16() {
  SD.remove("InAVal16.txt");
  InAValid16File = SD.open("InAVal16.txt", FILE_WRITE);
  InAValid16File.seek(0);
  Serial.print("InAValid16");Serial.println(InAValid16File);
  if (InAValid16File) {

    for (int i = 0; i <4; i++) {
     
        InAValid16File.print(InAValid16[i]);
        InAValid16File.print("/");
   Serial.print(InAValid16[i]);
    }
    InAValid16File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile17() {
  SD.remove("InAVal17.txt");
  InAValid17File = SD.open("InAVal17.txt", FILE_WRITE);
  InAValid17File.seek(0);
  Serial.print("InAValid17");Serial.println(InAValid17File);
  if (InAValid17File) {

    for (int i = 0; i <4; i++) {
     
        InAValid17File.print(InAValid17[i]);
        InAValid17File.print("/");
   Serial.print(InAValid17[i]);
    }
    InAValid17File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }
}

//-------------------------
void SDWriteFile18() {
  SD.remove("InAVal18.txt");
  InAValid18File = SD.open("InAVal18.txt", FILE_WRITE);
  InAValid18File.seek(0);
  Serial.print("InAValid18");Serial.println(InAValid18File);
  if (InAValid18File) {

    for (int i = 0; i <4; i++) {
     
        InAValid18File.print(InAValid18[i]);
        InAValid18File.print("/");
   Serial.print(InAValid18[i]);
    }
    InAValid18File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile19() {
  SD.remove("InAVal19.txt");
  InAValid19File = SD.open("InAVal19.txt", FILE_WRITE);
  InAValid19File.seek(0);
  Serial.print("InAValid19");Serial.println(InAValid19File);
  if (InAValid19File) {

    for (int i = 0; i <4; i++) {
     
        InAValid19File.print(InAValid19[i]);
        InAValid19File.print("/");
   Serial.print(InAValid19[i]);
    }
    InAValid19File.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening FILE");
  }

}
//-------------------------
void SDWriteFile20() {
  SD.remove("InAVal20.txt");
  InAValid20File = SD.open("InAVal20.txt", FILE_WRITE);
  InAValid20File.seek(0);
  Serial.print("InAValid20");Serial.println(InAValid20File);
  if (InAValid20File) {

    for (int i = 0; i <4; i++) {
     
        InAValid20File.print(InAValid20[i]);
        InAValid20File.print("/");
   Serial.print(InAValid20[i]);
    }
    InAValid20File.close();
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

  Serial.print("[ Dumping "); Serial.print(Length);
  Serial.println(" bytes ]==========================");
  IndFile = Length;
  for (i = 0; i < Length; i++)
  {
    cnt++;
    if (buf[i] < 0x10)
      Serial.print("0");
    Serial.print(buf[i], HEX);
    Buf = buf[i];
    Serial.print(" ");
    if (cnt == 16)
    {
      cnt = 0;
      Serial.println();
    }
  }
  Serial.println("===============================================");
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
*  int getPInAValue(int pin);                                         read the value of a Pin. Usefull to read the value of a PWM pin
*  long lastCommunicationTime;                                       Stores the last communication with Virtuino time
*  void vDelay(long milliseconds); 
*/




