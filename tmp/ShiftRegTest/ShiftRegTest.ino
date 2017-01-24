
//set up the pins for communication with the shift register
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
int x; //create a counting variable

byte nixies = 255;

int charTable[] = {0,128,64,192,32,160,96,224,16,144,8,136,72,200,40,168,104,232,24,152,4,132,68,196,36,164,100,228,20,148,12,140,76,204,44,
172,108,236,28,156,2,130,66,194,34,162,98,226,
18,146,10,138,74,202,42,170,106,234,26,154,6,134,70,198,38,166,102,230,22,150,14,142,78,206,46,174,110,238,30,158,1,129,
65,193,33,161,97,225,17,145,9,137,73,201,41,169,105,233,25,153};

void setup() {
   pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  if (Serial.available() > 0) {
    // ASCII '0' through '9' characters are
    // represented by the values 48 through 57.
    // so if the user types a number from 0 through 9 in ASCII, 
    // you can subtract 48 to get the actual value:
    int bitToSet = Serial.read() - 48;

  // write to the shift register with the correct bit set high:
    registerWrite(bitToSet, HIGH);
  }
}

// This method sends bits to the shift register:

void registerWrite(int whichPin, int whichState) {
// the bits you want to send
  byte bitsToSend = 0;

  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);
Serial.println(whichPin);
if (whichPin == 1)
{
  Serial.println("Entered 1.");
  bitWrite(bitsToSend, 1, HIGH);
  bitWrite(bitsToSend, 6, HIGH);
} else if (whichPin == 2) {
  Serial.println("Entered 2.");
  bitWrite(bitsToSend, 1, LOW);
  bitWrite(bitsToSend, 6, LOW);
} else if (whichPin == 3) {
  Serial.println("Entered 3.");
  bitWrite(bitsToSend, 1, LOW);
  bitWrite(bitsToSend, 6, HIGH);
} else if (whichPin == 4) {
  Serial.println("Entered 4.");
  bitWrite(bitsToSend, 1, HIGH);
  bitWrite(bitsToSend, 6, LOW);
}

  // turn on the next highest bit in bitsToSend:
  //bitWrite(bitsToSend, whichPin, whichState);
Serial.print(bitsToSend);
  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

    // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

}
