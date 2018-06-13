/* http://www.bajdi.com
   Analog reading of TCRT5000 sensor connected to Arduino Uno
   TCRT5000 pins: (see datasheet http://www.vishay.com/docs/83760/tcrt5000.pdf )
     C = Arduino analog pin A0 and to one end of 4K7 resistor, other end of resistor to 5V
     E = GND
     A = 100 ohm resistor, other end of resistor to 5V
     C = GND
*/

const int led =  11;      // the number of the LED pin
int tcrt;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop(){
  tcrt = analogRead(A0);
  Serial.println(tcrt);
  //Serial.println("test");
  analogWrite(led, tcrt/4);
  delay(1000);
}