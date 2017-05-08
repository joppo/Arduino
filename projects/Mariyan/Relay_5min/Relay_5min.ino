#define CH1 4   // Connect Digital Pin 4 on Arduino to CH1 on Relay Module

 void setup(){
   //Setup all the Arduino Pins
   pinMode(CH1, OUTPUT);
   digitalWrite(CH1,LOW);

   delay(2000); //Wait 2 seconds before starting sequence
 }

 void loop(){
   digitalWrite(CH1, HIGH);
   delay(1000);
   digitalWrite(CH1, LOW);
   delay(1000);
 }