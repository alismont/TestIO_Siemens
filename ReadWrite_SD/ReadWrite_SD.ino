/*
  SD card read/write
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
 */

//-------------------------
#include <SPI.h>
#include <SD.h>
File CONFIG;

//*********************************************************************************************************************
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

// Init carte SD
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
// open the file for WRITE:
  CONFIG = SD.open("CONFIG.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (CONFIG) {
    Serial.print("Writing to CONFIG.txt...");
    CONFIG.println("CONFIG 1, 2, 3.");
    // close the file:
    CONFIG.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening CONFIG.txt");
  }

  // re-open the file for READING:
  CONFIG = SD.open("CONFIG.txt");
  if (CONFIG) {
    Serial.println("CONFIG.txt:");

    // read from the file until there's nothing else in it:
    while (CONFIG.available()) {
      Serial.write(CONFIG.read());
    }
    // close the file:
    CONFIG.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening CONFIG.txt");
  }
}

//******************************************************************************************************************
void loop() {
  // nothing happens after setup
}


