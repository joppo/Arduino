#include "Wire.h"
#include "Arduino.h"
#include "TM1637Display.h"

// Display connection pins (Digital Pins)
#define CLK 8
#define DIO 9

#define DS3231_I2C_ADDRESS 0x68

#define TEST_DELAY   2000

TM1637Display display(CLK, DIO);


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(30,17,1,2,22,8,16);
}


void loop()
{
//if hour is clicked - increment hour
//if minute is clicked - increment minute
  
  DisplayTime();
}


void DisplayTime()
{
  display.setBrightness(0x0f);

  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  //num, leading zero, length, pos
  display.showNumberDec(hour, false, 2, 0);
  delay(TEST_DELAY);
  
  display.showNumberDec(minute, false, 2, 2);
  delay(TEST_DELAY);

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



