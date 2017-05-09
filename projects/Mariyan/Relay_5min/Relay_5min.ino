#define CH1 4   // Connect Digital Pin 4 on Arduino to CH1 on Relay Module

uint signalLength = 2000;
uint pauseBetweenSignals = 2000;
uint loop_period = 300000; //5 minutes

 void setup(){
   
   pinMode(CH1, OUTPUT);
   digitalWrite(CH1,LOW);

   delay(loop_period); //we wait for 5 minutes before signaling with the siren
 }

 void loop(){
   digitalWrite(CH1, HIGH);
   delay(signalLength);
   digitalWrite(CH1, LOW);
   delay(pauseBetweenSignals);
   
   digitalWrite(CH1, HIGH);
   delay(signalLength);
   digitalWrite(CH1, LOW);
   delay(pauseBetweenSignals);
   
   digitalWrite(CH1, HIGH);
   delay(signalLength);
   digitalWrite(CH1, LOW);
   
   delay(loop_period - signalLength*3 - pauseBetweenSignals*2); //this is 5 minutes minus 10 seconds (the signaling takes 10 seconds to end)
 }