#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>


// Definition of WHITE
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF

/*
Your Connections to an Uno (Through a Level Shifter)

 LED to 3.3V
 SCK to D13
 SDA to D11
 A0 to D8
 RST to D9
 CS to D10
 GND to GND
 VCC to 3.3V 
 */
 
#define CS 10
#define DC 9

// Declare an instance of the ILI9163
TFT_ILI9163C tft = TFT_ILI9163C(CS, 8, DC);  
//TFT_ILI9163C tft = TFT_ILI9163C(CS, DC);

void setup() {
  tft.begin();
  tft.setRotation(0);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);  
  tft.setTextSize(1);
  tft.println("Zhivko:");
  tft.setTextColor(YELLOW); 
  tft.setTextSize(1);
  tft.println("Test line 1");
  tft.setTextColor(RED);    
  tft.setTextSize(3);
//   tft.println(0xDEAD, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.println("BIG LINE");
  tft.setTextSize(2);
//   tft.println("I implore thee,");
//   tft.setTextSize(1);
//   tft.println("my foonting turlingdromes.");
//   tft.println("And hooptiously drangle me");
//   tft.println("with crinkly bindlewurdles,");
//   tft.println("Or I will rend thee");
//   tft.println("in the gobberwarts");
//   tft.println("with my blurglecruncheon,");
//   tft.println("see if I don't!");
  tft.defineScrollArea(23,50);
  //try load again with this commented out!
}

int t = 0;


void loop(void) {
  tft.scroll(t);
  if (t > 160) {
    t = 0;
  } 
  else {
    t++;
  }

  delay(10);
}


void testFilledRects() {
  int           n, i, i2,
  cx = (tft.width()  / 2),
  cy = (tft.height() / 2);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    tft.fillRect(cx-i2, cy-i2, i, i, random(0x0000,0xFFFF));
    tft.drawRect(cx-i2, cy-i2, i, i, random(0x0000,0xFFFF));
  }
}