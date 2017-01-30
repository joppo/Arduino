#include "Arduino.h"
#include <dht22.h>
#include "Wire.h"

//I2C
#define DS3231_I2C_ADDRESS 0x68
//END I2C

//Button pins
const int buttonMode = 14; //analogue pin 0
const int buttonHour = 15; //analogue pin 1
const int buttonMinute = 16; //analogue pin 2
//END Button pins

String clock_mode = "clock";
int number_AB_ToDisplay;
int number_CD_ToDisplay;

//LED pins
int ledClock = 1;
int ledTemperature = 2;
//END LED pins

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

//Control VARS
unsigned long controlClockBtnTime = 0
unsigned long controlHourBtnTime = 0;
unsigned long controlMinuteBtnTime = 0;
unsigned long controlReadClockTime = 0;
unsigned long controlReadDHTtime = 0;
//END Control VARS

//Interval VARS
const long clockBtnInterval = 500;
const long hourBtnInterval = 250;
const long minuteBtnInterval = 250;
const long readClockInterval = 700;
const long readDHTInterval = 700;
//END Interval VARS

void setup() {
  Wire.begin();
  pinMode(ledClock,OUTPUT);
  pinMode(ledTemperature,OUTPUT);
  
  pinMode(buttonMode, INPUT_PULLUP);
  pinMode(buttonHour, INPUT_PULLUP);
  pinMode(buttonMinute, INPUT_PULLUP);

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
  
  boolean pirActive = ReadPIR();
  if (pirActive == 0)
  {
    TurnOffTubes(); 
  } 
  else {
    
    //We display the tubes
    ClockModeClick();
    HourBtnClick();
    MinuteBtnClick();
    
    
    if (clock_mode == "clock")
    {
      ReadTime();
      
    } else {
      ReadDHT();    
    }
    
    byte b;
    b = GetShiftByte(number_CD_ToDisplay);
    digitalWrite(latchPin_h, LOW);
    shiftOut(dataPin_h, clockPin_h, MSBFIRST, b);
    digitalWrite(latchPin_h, HIGH);
    SlotEffect();
  }

  delay(500);

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

void ReadDHT()
{
  //begin read DHT
  unsigned long currentTime = millis();
  if (currentTime - controlReadDHTtime >= readDHTInterval)
  {
    controlReadDHTtime = currentTime;
    number_CD_ToDisplay = ReadDHT();
}

void ReadTime()
{
  //begin read Clock
  unsigned long currentTime = millis();
  if (currentTime - controlReadClockTime >= readClockInterval)
  {
    controlReadClockTime = currentTime;
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    number_AB_ToDisplay = hour;
    number_CD_ToDisplay = minute;
  }
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

void HourBtnClick()
{
  unsigned long currentTime = millis();
  if (currentTime - controlHourBtnTime >= hourBtnInterval)
  {
    controlHourBtnTime = currentTime;

    int btn_hour_value = 0;
    btn_hour_value = digitalRead(buttonHour);

    //by default is LOW
    if (btn_hour_value == HIGH) {
      //get hour
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    if (hour == 23)
      hour = 0;
    else
      hour = hour + 1;
      
    //set hour with increment by 1
    setDS3231time(second, minute, hour, dayOfWeek,dayOfMonth, month, year);
    }
  }
}

void ClockModeClick()
{
  unsigned long currentTime = millis();
  if (currentTime - controlClockBtnTime >= clockBtnInterval)
  {
    controlClockBtnTime = currentTime;
    
    int buttonModeResult = digitalRead(buttonMode);
    if (buttonModeResult == HIGH)
    {
      //Enter Temperature Mode
      clock_mode = "clock";
      Serial.println("Clock mode");
    } 
    else
    {
      //Enter Clock Mode
      clock_mode = "temperature";
      Serial.println("temp mode");      
    }
  }
}

void MinuteBtnClick()
{
  unsigned long currentTime = millis();
  if (currentTime - controlMinuteBtnTime >= minuteBtnInterval)
  {
    controlMinuteBtnTime = currentTime;

    int btn_minute_value = 0;
    btn_minute_value = digitalRead(buttonMinute);

    //by default is LOW
    if (btn_minute_value == HIGH) {
      //get minute
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    if (minute == 59)
      minute = 0;
    else
      minute = minute + 1;
      
    //set minute with increment by 1
    setDS3231time(second, minute, hour, dayOfWeek,dayOfMonth, month, year);
    }
  }
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
                   dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void readDS3231time(byte *second,
                    byte *minute,
                    byte *hour,
                    byte *dayOfWeek,
                    byte *dayOfMonth,
                    byte *month,
                    byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
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



