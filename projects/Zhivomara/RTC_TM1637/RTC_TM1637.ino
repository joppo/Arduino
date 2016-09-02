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

const long dhtInterval = 700;
const long brightnessInterval = 700;
const long clockInterval = 700;
const long modeBtnInterval = 250;
const long clockBtnInterval = 250;
const long ledInterval = 250;
const long secondInterval = 1000;



unsigned long controlBrightnessTime = 0;
unsigned long controlDHTTime = 0;
unsigned long controlClockTime = 0;
unsigned long controlModeBtnTime = 0;
unsigned long controlHourBtnTime = 0;
unsigned long controlMinuteBtnTime = 0;
unsigned long controlMinuteMinusBtnTime = 0;
unsigned long ledTime = 0;
unsigned long controlSecondTime = 0;


TM1637Display display(CLK, DIO);
int photoResistorPin = 1;  //define a pin for Photo resistor
int buttonMode = 13;
int buttonHour = 12;
int buttonMinute = 11;
int buttonMinuteMinus = 10;
int greenLed = 2;
int yellowLed = 3;
int redLed = 4;

String display_mode;

void setup()
{
  pinMode(buttonMode, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  Wire.begin();

  Serial.begin(9600);
  display_mode = "clock";
  
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(30,17,1,2,22,8,16);
}

void loop()
{
//if hour is clicked - increment hour
//if minute is clicked - increment minute
//if mode button is clicked - change mode.
//delay(100);
SetLEDBrightness();

ModeBtnClick();
HourBtnClick();
MinuteBtnClick();
MinuteMinusBtnClick();

  if (display_mode == "clock") {
    DisplayTime();

    StartModeLeds(display_mode);
  } else if (display_mode == "temperature") {
    DisplayDHT(display_mode);

    StartModeLeds(display_mode);
  } else if (display_mode == "humidity")
  {
    DisplayDHT(display_mode);
    StartModeLeds(display_mode);
  }
}

void StartModeLeds(String led_mode)
{
  unsigned long currentTime = millis();
  if (currentTime - ledTime >= ledInterval)
  {
    ledTime = currentTime;
    if (led_mode == "clock") {
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
      digitalWrite(yellowLed, LOW);
    } else if (led_mode == "temperature")
    {
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, LOW);
      digitalWrite(yellowLed, HIGH);
    } else if (led_mode == "humidity")
    {
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      digitalWrite(yellowLed, LOW);
    }
  }
}

void HourBtnClick()
{
  unsigned long currentTime = millis();
  if (currentTime - controlHourBtnTime >= clockBtnInterval)
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

void MinuteBtnClick()
{
  unsigned long currentTime = millis();
  if (currentTime - controlMinuteBtnTime >= clockBtnInterval)
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

void MinuteMinusBtnClick()
{
  unsigned long currentTime = millis();
  if (currentTime - controlMinuteMinusBtnTime >= clockBtnInterval)
  {
    controlMinuteMinusBtnTime = currentTime;

    int btn_minute_minus_value = 0;
    btn_minute_minus_value = digitalRead(buttonMinuteMinus);

    //by default is LOW
    if (btn_minute_minus_value == HIGH) {
      //get minute
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    if (minute == 0)
      minute = 59;
    else
      minute = minute - 1;
      
    //set minute with increment by 1
    setDS3231time(second, minute, hour, dayOfWeek,dayOfMonth, month, year);
    }
  }
}

void ModeBtnClick()
{
  unsigned long currentTime = millis();
  
  if (currentTime - controlModeBtnTime >= modeBtnInterval)
  {
    controlModeBtnTime = currentTime;
  int btn_mode_value = 0;
  btn_mode_value = digitalRead(buttonMode);

  //by default is LOW
  if (btn_mode_value == HIGH) {
      if (display_mode == "clock") {
        display_mode = "temperature";
      } else if (display_mode == "temperature")
      {
        display_mode = "humidity";
      } else if (display_mode == "humidity")
      {
        display_mode = "clock";
      }
    
  }
  }
}

void SetLEDBrightness()
{
  int photoResult = analogRead(photoResistorPin);
  uint8_t brightness_value = 0x0d; //default value
  
  if (photoResult > 800)
  {
    brightness_value = 0x0d;
  } else if (photoResult > 700) {
    brightness_value = 0x0c;
  } else if (photoResult > 600) {
    brightness_value = 0x0b;
  } else if (photoResult > 500) {
    brightness_value = 0x0a;
  } else if (photoResult > 400) {
    brightness_value = 0x09;
  } else if (photoResult > 300) {
    brightness_value = 0x08;
  } else
  {
    //below 300
    brightness_value = 0x08;
  }
  
  unsigned long currentTime = millis();
  if (currentTime - controlBrightnessTime >= brightnessInterval)
  {
    controlBrightnessTime = currentTime;
    display.setBrightness(brightness_value);
  }
  
}

//mode should contain "temperature" or "humidity"
void DisplayDHT(String mode)
{
  unsigned long currentTime = millis();
  
  if (currentTime - controlDHTTime >= dhtInterval)
  {
    controlDHTTime = currentTime;

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

    if (mode == "temperature") {
      
      display.setColon(false);
      float wholeDigit = DHT.temperature;
      int v = (int)wholeDigit;
      int tens = ((int)(wholeDigit * 10)) % 10;
      
      int ones = v%10;
      v = v/10;
      int dec= v%10;
  
      // Selectively set different digits
      data[3] = display.encodeDigit(tens);
      
      //data[2] = SEG_C | SEG_D | SEG_E | SEG_G; //o
      data[2] = SEG_D; //_
      
      if (dec == 0)
        data[0] = 0;
      else
        data[0] = display.encodeDigit(dec);
      data[1] = display.encodeDigit(ones);
    } else {

      display.setColon(false);
      int h = (int)DHT.humidity;
      int ones = h%10;
      h = h/10;
      int dec = h%10;
      
      // Selectively set different digits
      data[0] = 0;
      data[1] = 0;
      if (dec == 0)
        data[2] = 0;
      else
        data[2] = display.encodeDigit(dec);
      data[3] = display.encodeDigit(ones);
      
      Serial.print("Humidity:");
      Serial.println(h);
    }
    display.setSegments(data, 4, 0);
  }
    
}

bool colonFlag = true;
void DisplayTime()
{
  unsigned long currentTime = millis();
  if (currentTime - controlClockTime >= clockInterval)
  {
    controlClockTime = currentTime;
    
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  
    //num, leading zero, length, pos
    display.showNumberDec(hour, true, 2, 0);
    display.showNumberDec(minute, true, 2, 2);
    //display.setColon(true);
  }

  if (currentTime - controlSecondTime >= secondInterval)
  {
    controlSecondTime = currentTime;
    
    display.setColon(colonFlag);
    colonFlag = !colonFlag;
  }
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



