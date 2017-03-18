//#include <dht22.h>
#include <ESP8266WiFi.h>
 
// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "ASSF6HC8PXDE12EE";
const char* wifiSSID = "M-Tel_D7D0";
const char* wifiPassword = "48575443EFD7D02A";

const char* server = "api.thingspeak.com";
unsigned int wifiSeqId = 0;
 
// Constants
const int pinP1 = 2; //D1;                   // Dust sensor P1 connected to digital pin D1
const int pinP2 = 16; //D2;                   // Dust sensor P2 connected to digital pin D2
const unsigned long sampleTime = 20000; // Length of the particle sampling period (60000ms = 60 seconds)


// Variables
int stateP1 = HIGH;                     // P1 dust detected? LOW = yes - HIGH = no
int stateP2 = HIGH;                     // P2 dust detected? LOW = yes - HIGH = no
volatile unsigned long startP1;         // P1 pulse duration start
volatile unsigned long startP2;         // P2 pulse duration start
volatile unsigned long lpoP1;           // Duration of time in sampling period during which
                                        // P1 particles are detected,
                                        // aka Low Pulse Occupancy (lpo)
volatile unsigned long lpoP2;           // Low Pulse Occupancy for P2
volatile int nP1 = 0;                   // Number of P1 reads
volatile int nP2 = 0;                   // Number of P2 reads
float ratioP1 = 0;                      // % of time a P1 particle is detected during the sampling period - P1 can be regarded as PM10
float ratioP2 = 0;                      // % of time a P1 particle is detected during the sampling period



// #define DHTPIN D2
// #define DHTTYPE DHT11 
 
// DHT dht(DHTPIN, DHTTYPE);
//ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
 
void setup() 
{
    //WiFiMulti.addAP(wifiSSID, wifiPassword);

    // Allow P1 and P2 to trigger interrupt
    attachInterrupt(digitalPinToInterrupt(pinP1), measureStateChange, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinP2), measureStateChange, CHANGE);

    pinMode(pinP1, INPUT);
    pinMode(pinP2, INPUT);

    // PPD42NS only
    digitalWrite(pinP1, HIGH);
    digitalWrite(pinP2, HIGH);

     Serial.begin(115200);
     delay(5000);
     WiFi.begin(wifiSSID, wifiPassword);
     WiFi.begin(wifiSSID, wifiPassword);
     while (WiFi.status() != WL_CONNECTED) 
     {
        delay(500);
     }
}
 
void loop() 
{
  Serial.println("start loop");

    // send data via WiFi:
    TalkToHub();

    // Reset lpo values:
    lpoP1 = 0;
    lpoP2 = 0;

    // start next sampling period:
    Serial.println("end loop before delay");
    delay(sampleTime);
    Serial.println("end loop after delay");

    //TalkToHub();
}

unsigned long limit(unsigned long x) {
  // PPD42NS only
  return (x < 10000ul || x > 90000ul) ? 0 : x;
  // return x;
}

/*
  The measureStateChange routine is called by a hardware interrupt on the pins
  whenever the state of P1 or P2 changes.
  It determines the type of change (low to high or high to low) and takes the
  appropriate action.
*/
void measureStateChange() {
  unsigned long us = micros();

  // determine which pins changed state and take appropriate action:
  if (stateP1 != digitalRead(pinP1)) {              // P1 has changed
    if ((stateP1 = digitalRead(pinP1)) == LOW) {    // a new pulse has started
      startP1 = us;                                 // start timing the pulse
      nP1++;                                        // increment number of P1 particles read
    } else {                                        // a pulse has ended
      lpoP1 += limit(us - startP1);                 // calculate the duration
    }
  }

  if (stateP2 != digitalRead(pinP2)) {              // P2 has changed
    if ((stateP2 = digitalRead(pinP2)) == LOW) {    // a new pulse has started
      startP2 = us;                                 // start timing the pulse
      nP2++;                                        // increment number of P2 particles read
    } else {                                        // a pulse has ended
      lpoP2 += limit(us - startP2);                 // calculate the duration
    }
  }
}

void TalkToHub()
{
    float p1_lpo_ratio = (float(lpoP1) / 1000) / float(sampleTime) * 100;
    float p2_lpo_ratio = (float(lpoP2) / 1000) / float(sampleTime) * 100;

    float concentration_p1 = 1.1 * pow(p1_lpo_ratio, 3) - 3.8 * pow(p1_lpo_ratio, 2) + 520 * p1_lpo_ratio + 0.62; // using spec sheet curve
    float concentration_p2 = 1.1 * pow(p2_lpo_ratio, 3) - 3.8 * pow(p2_lpo_ratio, 2) + 520 * p2_lpo_ratio + 0.62; // using spec sheet curve


    Serial.print("p1_lpo_ratio");
    Serial.println(p1_lpo_ratio);
    Serial.print("p2_lpo_ratio");
    Serial.println(p2_lpo_ratio);
    Serial.print("concentration_p1");
    Serial.println(concentration_p1);
    Serial.print("concentration_p2");
    Serial.println(concentration_p2);

    if (client.connect(server,80)) {
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(p1_lpo_ratio);
    postStr +="&field2=";
    postStr += String(concentration_p1);
    postStr +="&field3=";
    postStr += String(nP1);
    postStr +="&field4=";
    postStr += String(p2_lpo_ratio);
    postStr +="&field5=";
    postStr += String(concentration_p2);
    postStr +="&field6=";
    postStr += String(nP2);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    }
    client.stop();
    
    wifiSeqId++;                                       // Increment wifi sequence id.
}
