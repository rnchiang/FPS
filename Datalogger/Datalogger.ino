/*
  Modded SD card datalogger (RC)

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;   //SD Card
const int PowerLedPin =  12;     //On/Off LED pin
const int StrokeLedPin =  3;     //Stroke LED pin
int fileCount = 1;
char fileName [50];

File dataFile;

void setup()
{
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
  // ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  // initialize the LED pin as an output:
  pinMode(PowerLedPin, OUTPUT);
  // initialize the LED pin as an output:
  pinMode(StrokeLedPin, OUTPUT);
  
  create_file();
  
  delay(5000);
}

void loop()
{
  
  // if on, turn on RED LED
  digitalWrite(PowerLedPin, HIGH);
  //buttonState = HIGH; //real code: buttonState = digitalRead(buttonPin);
  // make a string for assembling the data to log:
  String dataString = "";

  // read sensors and append to the string:
  int analogPin = 2;
  int sensor = analogRead(analogPin);
  dataString += String(sensor);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);

    // If stroke taken, light GREEN LED
    if (sensor > 0) {
      digitalWrite(StrokeLedPin, HIGH);
    }
    else {
      digitalWrite(StrokeLedPin, LOW);
    }
    
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
    Serial.println(fileName);
  }

}

void create_file () {
  
  /// DATAFILE CREATION
  int n = sprintf(fileName, "datalog%d.txt", fileCount);
  if (SD.exists(fileName)){
    while(SD.exists(fileName)){
      Serial.print(fileName);Serial.println(" already exists.");
      fileCount++;
      n = sprintf(fileName, "datalog%d.txt", fileCount);
    }
  }
  Serial.print("Creating ... ");Serial.println(fileName);
  dataFile = SD.open(fileName, FILE_WRITE);
  dataFile.close(); 
  
}








