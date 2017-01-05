#include "Arduino.h"
#include <dht22.h>

//BEGIN NOT CONCLUSIVE
const int switchBtnPIN = 3;
//END NOT CONCLUSIVE

//BEGIN Yellow LED 1 pin
const int ledPIN = 14; //analogue pin 0
//END Yellow LED 1 pin

//BEGIN ShiftOut Stuff
//set up the pins for communication with the shift register
int latchPin_h = 8;
int clockPin_h = 12;
int dataPin_h = 11;

int latchPin_m = 9;
int clockPin_m = 10;
int dataPin_m = 13;
//END ShiftOut Stuff

//BEGIN DHT22
dht DHT;
#define DHT22_PIN 5 // We have connected the DHT to Digital Pin 2
//END DHT22

//BEGIN PIR
int inputPIRPin = 4;
int pirState = LOW; // we start, assuming no motion detected
//END PIR


byte nixies = 255;

void setup() {

  pinMode(ledPIN,OUTPUT); 

  pinMode(switchBtnPIN, OUTPUT);

  pinMode(latchPin_h, OUTPUT);
  pinMode(dataPin_h, OUTPUT);
  pinMode(clockPin_h, OUTPUT);

  pinMode(latchPin_m, OUTPUT);
  pinMode(dataPin_m, OUTPUT);
  pinMode(clockPin_m, OUTPUT);

  pinMode(inputPIRPin, INPUT);     // declare PIR port as input

  Serial.begin(9600);

}

void loop() {

  digitalWrite(ledPIN, HIGH);

  int switchBtnState = digitalRead(switchBtnPIN);
  if (switchBtnState == HIGH)
  {
    Serial.println("btn pressed");
  } 
  else
  {
    Serial.println("btn NOT pressed");      
  }

  boolean pirActive = ReadPIR();
  //Serial.print("PIR:");
  //Serial.println(pirActive);
  if (pirActive == 0)
  {
    TurnOffTubes(); 
  } 
  else {
    int n_ToDisplay;
    n_ToDisplay = ReadDHT();
    byte b;
    b = GetShiftByte(n_ToDisplay);
    Serial.println(n_ToDisplay);
    digitalWrite(latchPin_h, LOW);
    shiftOut(dataPin_h, clockPin_h, MSBFIRST, b);
    digitalWrite(latchPin_h, HIGH);
    SlotEffect();
  }

  delay(2000);




  //delay(5000);

  //if (Serial.available() > 0) {
  // ASCII '0' through '9' characters are
  // represented by the values 48 through 57.
  // so if the user types a number from 0 through 9 in ASCII, 
  // you can subtract 48 to get the actual value:
  //int bitToSet = Serial.read() - 48;
  //SlotEffect();
  // write to the shift register with the correct bit set high:
  //registerWrite(bitToSet, HIGH);

  //}
  //registerWrite(5, HIGH);
}

void TurnOffTubes()
{
  byte b = GetEmptyByte();
  digitalWrite(latchPin_h, LOW);
  shiftOut(dataPin_h, clockPin_h, MSBFIRST, b);
  digitalWrite(latchPin_h, HIGH);
}

// This method sends bits to the shift register:

boolean ReadPIR()
{
  int pirVal = 0;
  pirVal = digitalRead(inputPIRPin);
  //Serial.print(pirVal);
  if (pirVal == HIGH)
  {
    //Serial.println("  true");
    return true; 
  } 
  else {
    //Serial.println("  false");
    return false; 
  }
}

int ReadDHT()
{
  int chk = DHT.read22(DHT22_PIN);

  switch (chk)
  {
  case DHTLIB_OK:  
    break;
  case DHTLIB_ERROR_CHECKSUM: 
    break;
  case DHTLIB_ERROR_TIMEOUT: 
    break;
  default: 
    break;
  }
  uint8_t data[4]; 
  float wholeDigit = DHT.temperature;
  int v = (int)wholeDigit;
  return v;
}

void SlotEffect()
{
  for (int s = 2; s <= 9; s++)
  {
    int slot_n;
    slot_n = s + s * 10;
    byte b;
    b = GetShiftByte(slot_n);
    digitalWrite(latchPin_h, LOW);
    shiftOut(dataPin_h, clockPin_h, MSBFIRST, b);
    digitalWrite(latchPin_h, HIGH);

    digitalWrite(latchPin_m, LOW);
    shiftOut(dataPin_m, clockPin_m, MSBFIRST, b);
    digitalWrite(latchPin_m, HIGH);

    delay(50);
  }
}

byte GetShiftByte(int n)
{
  byte b = 0;
  int ones = n%10;
  n = n/10;
  int dec = n%10;

  //Serial.print("ones:");
  //Serial.println(ones);
  //Serial.print("dec:");
  //Serial.println(dec);

  b = GetSingleDigit(true, b, ones);
  b = GetSingleDigit(false, b, dec);
  return b;
}

byte GetEmptyByte()
{
  byte b;
  bitWrite(b, 0, HIGH);
  bitWrite(b, 1, HIGH);
  bitWrite(b, 2, HIGH);
  bitWrite(b, 3, HIGH);
  bitWrite(b, 4, HIGH);
  bitWrite(b, 5, HIGH);
  bitWrite(b, 6, HIGH);
  bitWrite(b, 7, HIGH);
  return b;
}

byte GetSingleDigit(boolean isUnits, byte b, int n)
{
  int a_pos, b_pos, c_pos, d_pos;
  //Serial.print("n:");Serial.println(n);
  //Serial.print("isUnits:");Serial.println(isUnits);
  if (isUnits) {
    //Serial.println("is units = true");
    a_pos = 0;
    b_pos = 1;
    c_pos = 2;
    d_pos = 3;
  } 
  else {
    //Serial.println("is units = false");
    a_pos = 4;
    b_pos = 5;
    c_pos = 6;
    d_pos = 7; 
  }

  if (n == 0) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, LOW); 
  } 
  else if (n == 1) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, HIGH); 
  } 
  else if (n == 2) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, HIGH);
    bitWrite(b, d_pos, LOW); 
  } 
  else if (n == 3) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, HIGH);
    bitWrite(b, d_pos, HIGH);
  } 
  else if (n == 4) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, HIGH);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, LOW);
  } 
  else if (n == 5) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, HIGH);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, HIGH);
  } 
  else if (n == 6) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, HIGH);
    bitWrite(b, c_pos, HIGH);
    bitWrite(b, d_pos, LOW);
  } 
  else if (n == 7) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, HIGH);
    bitWrite(b, c_pos, HIGH);
    bitWrite(b, d_pos, HIGH);
  } 
  else if (n == 8) {
    bitWrite(b, a_pos, HIGH);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, LOW);
  } 
  else if (n == 9) {
    bitWrite(b, a_pos, HIGH);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, HIGH);
  }

  return b;
}



