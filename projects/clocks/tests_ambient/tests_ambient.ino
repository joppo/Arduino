void setup()
{
    Serial.begin(115200);
    // Serial.begin(9600);
    pinMode(6,OUTPUT);
}
void loop()
{ 
    digitalWrite(6,HIGH);
    delay(500);
    int a = analogRead(A0);
    Serial.println(a); 
}