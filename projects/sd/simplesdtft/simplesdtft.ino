// include the necessary libraries
#include <SPI.h>
#include <SD.h>
#include <TFT.h> // Hardware-specific library

// pin definition for the Uno
#define SD_CS  11
#define LCD_CS 10
#define DC    9
#define RESET    8  

// pin definition for the Leonardo
// #define SD_CS  8
// #define LCD_CS 7
// #define DC   0
// #define RESET  1

TFT myScreen = TFT(LCD_CS, DC, RESET);

// this variable represents the image to be drawn on screen
PImage image;

void setup() {
  // initialize the serial port
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial line to be ready
    // needed for the Leonardo
  }

  // try to access the SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("OK!");

  // initialize and clear the GLCD screen
  myScreen.begin();
  myScreen.background(255, 255, 255);

  // load the image from the SD card
  image = myScreen.loadImage("logo.bmp");

  // check if the image loaded properly
  if (image.isValid() != true) {
    Serial.println("error while loading arduino.bmp");
  }

  //write the image on screen
  myScreen.image(image, 0, 0);
}

void loop(){
// nothing happening here
}