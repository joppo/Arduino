
//set up the pins for communication with the shift register
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

byte nixies = 255;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);

}

void loop() {
    byte b;
    b = GetShiftByte(12);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, b);
    digitalWrite(latchPin, HIGH);
    delay(4000);
    
    
    SlotEffect();
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

// This method sends bits to the shift register:

void SlotEffect()
{
    for (int s = 2; s <= 9; s++)
    {
      int slot_n;
      slot_n = s + s * 10;
      byte b;
      b = GetShiftByte(slot_n);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, b);
      digitalWrite(latchPin, HIGH);
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
  } else {
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
  } else if (n == 1) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, HIGH); 
  } else if (n == 2) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, HIGH);
    bitWrite(b, d_pos, LOW); 
  } else if (n == 3) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, HIGH);
    bitWrite(b, d_pos, HIGH);
  } else if (n == 4) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, HIGH);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, LOW);
  } else if (n == 5) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, HIGH);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, HIGH);
  } else if (n == 6) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, HIGH);
    bitWrite(b, c_pos, HIGH);
    bitWrite(b, d_pos, LOW);
  } else if (n == 7) {
    bitWrite(b, a_pos, LOW);
    bitWrite(b, b_pos, HIGH);
    bitWrite(b, c_pos, HIGH);
    bitWrite(b, d_pos, HIGH);
  } else if (n == 8) {
    bitWrite(b, a_pos, HIGH);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, LOW);
  } else if (n == 9) {
    bitWrite(b, a_pos, HIGH);
    bitWrite(b, b_pos, LOW);
    bitWrite(b, c_pos, LOW);
    bitWrite(b, d_pos, HIGH);
  }

  return b;
}


