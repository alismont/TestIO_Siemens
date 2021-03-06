/*
    SimpleSender
        
    This example demonstrates how to synchronize a variable from your Processing sketch
    to a variable on your arduino.
    In this case we synchronize the 'mouseX' and 'mouseY' variables already provided by
    Processing but you could easily synchronize your own variables in the same way.
   
    This example should be used together with the SimpleReceiver example of the Arduino
    version of the VSync Library
    ( https://github.com/erniejunior/VSync )
    
    Author: Maximilian Ernestus
   
*/

import vsync.*;
import processing.serial.*;

//  To sync variable to a different device we need a ValueSender
ValueSender sender;
ValueReceiver receiver;

public int doubleA,doubleB,valueA,valueB;

void setup() 
{
  size(255,255);
  
  //  Hint: "/dev/ttyUSB0" is the serial port on my system. It might have
  //  a different name on yours. 
  //  It should be the same one that is checked under Tools->Serial Port in you Arduino IDE
  //  when uploading Arduino sketches.
  //  Look at http://processing.org/reference/libraries/serial/Serial.html if you still have trouble.
  Serial serial = new Serial(this, "COM5", 9600);
  
  //  Ininialize the ValueSender with this (to hook it to your sketch)
  //  and the serial interface you want to use.
  sender = new ValueSender(this, serial);
  receiver = new ValueReceiver(this, serial);
  
  //  Tell the ValueSender what variables to synchronize from the arduino to the Processing sketch.
  //  Be careful to always use the same order as on the other side!
  sender.observe("valueA");
  sender.observe("valueB");
  receiver.observe("doubleA");
  receiver.observe("doubleB");


}

void draw() 
{
  //  Nothing to be drawn for this example
  //println(mouseX);
  //println(mouseY);
   print("doubleA:");print(doubleA);print("  valueA: ");println(valueA);
   print("doubleB:");print(doubleB);print("  valueB: ");println(valueB);
}