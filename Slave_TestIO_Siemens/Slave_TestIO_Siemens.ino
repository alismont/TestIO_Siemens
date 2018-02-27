// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>
// Test IO Siemens
// Arduino UNO 
// Master com I2C 

//--------------------------------


#include <Wire.h>

//--------------------------------
int Input[10];

char octetReceptionProc;
char caractereReceptionProc;
String chaineReception;
String chaineReceptionProc;
String MessageEntete;
int Index=0;
//*****************************************************************************************************************
void setup()
{

  Wire.begin(9);                // join i2c bus with address #9
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

//****************************************************************************************************************
void loop()
{
  delay(100);
}

//-------------------------------------------------------------
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{     
 //entête du message exemple "CONFIG" réception TBL Input[]
  while (Wire.available() != 8) 
  { 
        //Serial.println(Wire.available());
        octetReceptionProc = Wire.read();         
       caractereReceptionProc = char(octetReceptionProc);
      chaineReceptionProc = chaineReceptionProc + caractereReceptionProc;
  }
  
if (chaineReceptionProc=="CONFIG") {
        Serial.println("CONFIG"); 
        chaineReceptionProc="";
        int i=1;
        while (Wire.available())
        {
          int x = Wire.read();           // receive byte as an integer
          Input[i]=x;
          Serial.print("i:");
                    Serial.println(Input[i]);
          i++;
        }
}
}
  

//-----------------------------------------------------------

