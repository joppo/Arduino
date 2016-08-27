#include "Wire.h"
#include "Arduino.h"
#include "TM1637Display.h"
#include <dht22.h>

dht DHT;
// DHT Sensor Setup
#define DHT22_PIN 5 // We have connected the DHT to Digital Pin 2

// Display connection pins (Digital Pins)
#define CLK 8
#define DIO 9

#define DS3231_I2C_ADDRESS 0x68

#define TEST_DELAY   500

TM1637Display display(CLK, DIO);
int photoResistorPin = 1;  //define a pin for Photo resistor
int buttonMode = 13;

String display_mode;

void setup()
{
  pinMode(buttonMode, INPUT);
  Wire.begin();

  Serial.begin(9600);
display_mode = "temperature";
  
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(30,17,1,2,22,8,16);
}

void loop()
{
//if hour is clicked - increment hour
//if minute is clicked - increment minute
//if mode button is clicked - change mode.
SetLEDBrightness();

Serial.println("LOOP BEFORE btn click");

ButtonClick();

//delay(TEST_DELAY);
Serial.println("LOOP after btn click");

  if (display_mode == "clock") {
    DisplayTime();
  } else {
    SetStatusOfDHT();
    DisplayDHT();

  }
}

void ButtonClick()
{
  int btn_click_value = 0;
  btn_click_value = digitalRead(buttonMode);

//by default is LOW
  if (btn_click_value == HIGH) {
      if (display_mode == "clock") {
        display_mode = "temperature";
      } else if (display_mode == "temperature")
      {
        display_mode = "clock";
      }
    
  }
}

void SetLEDBrightness()
{
  int photoResult = analogRead(photoResistorPin);

//Serial.print("PHOTORESISTOR:");
//Serial.println(photoResult);
  
  if (photoResult > 600)
  {
    display.setBrightness(0x0d);
    //Serial.println("highlight");
  } else if (photoResult > 400) {
    display.setBrightness(0x08);
    //Serial.println("med_light");
  } else
  {
    //Serial.println("lowlight");
    display.setBrightness(0x04);
  }
  //delay(250);
}

void SetStatusOfDHT()
{
    // READ DATA
  //Serial.print("DHT22, \t");
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
  //delay(300);
}

void DisplayDHT()
{
    // DISPLAY DATA
  //Serial.print(DHT.humidity);
  //Serial.print(",\t");
  //Serial.println(DHT.temperature);

  
  int v = (int)DHT.temperature;
  //Serial.print("temmp:");
  //Serial.println(v);
  int ones = v%10;
  v = v/10;
  int dec= v%10;
  int whole_digit = dec * 10 + ones;

  
display.setColon(false);
// Selectively set different digits
  uint8_t data[4];// = { 0xff, 0xff, 0xff, 0xff };
  data[0] = 0;
  data[1] = 0;
  data[2] = display.encodeDigit(dec);
  data[3] = display.encodeDigit(ones);
    display.setSegments(data, 4, 0);
  
  
  //display.showNumberDec(whole_digit, false, 2, 2);
  delay(TEST_DELAY);

  //display.showNumberDec(dec, false, 2, 2);
  //delay(TEST_DELAY);
  //display.showNumberDec(ones, false, 2, 3);
  //delay(TEST_DELAY);
}

void DisplayTime()
{
  //display.setBrightness(0x0f);

  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  //num, leading zero, length, pos
  display.showNumberDec(hour, false, 2, 0);
  //delay(TEST_DELAY);
  
  display.showNumberDec(minute, false, 2, 2);
  //delay(TEST_DELAY);

display.setColon(true);
delay(TEST_DELAY);

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



