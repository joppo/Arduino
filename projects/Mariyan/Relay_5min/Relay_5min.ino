#define CH1 4   // Connect Digital Pin 4 on Arduino to CH1 on Relay Module

unsigned long signalLength = 2000;
unsigned long pauseBetweenSignals = 2000;
unsigned long loop_period = 300000; //5 minutes

 void setup(){
   
   pinMode(CH1, OUTPUT);
   digitalWrite(CH1,HIGH);
  delay(loop_period); //we wait for 5 minutes before signaling with the siren
 }

 void loop(){
   digitalWrite(CH1, LOW);
   delay(signalLength);
   digitalWrite(CH1, HIGH);
   delay(pauseBetweenSignals);
   
   digitalWrite(CH1, LOW);
   delay(signalLength);
   digitalWrite(CH1, HIGH);
   delay(pauseBetweenSignals);
   
   digitalWrite(CH1, LOW);
   delay(signalLength);
   digitalWrite(CH1, HIGH);
   
   long total_sleep_time = loop_period - signalLength*3 - pauseBetweenSignals*2;

   delay(total_sleep_time); //this is 5 minutes minus 10 seconds (the signaling takes 10 seconds to end)
 }