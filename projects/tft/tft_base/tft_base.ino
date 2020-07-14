#include <TFT.h>
#include <SPI.h>

#define cs 10
#define dc 9
#define rst 8

TFT scr = TFT(cs, dc, rst);


int r = 0;
int g = 255;
int b = 0;
int selectedTab = 0;


void setup()
{
	scr.begin();
	scr.background(0,0,0);
	scr.setTextSize(1);
	//scr.stroke(r, g, b);
	scr.textWrap("TEST line perspective1 perspective2 perspective3", 6, 10);
	//scr.fill(r, g, b);
	drawMenuTabs(4, 1, r, g, b);
	switch (selectedTab) {
		case 0 : { drawMain(); }
		case 1 : { drawTempGraph(); }
		case 2 : { drawTemp(); }
		case 3 : { drawClock(); }
	}
}

void loop()
{
	delay(200);
}

void drawMain()
{
	drawMainClock(r, g, b);
	drawMainTemperature(r, g, b);
	drawMainHumidity(r, g, b);
}

void drawMainHumidity(int r_hu, int g_hu, int b_hu)
{
	scr.setTextSize(1);
	scr.text("Relative Humidity: 56%", 15, 64);
}

void drawMainClock(int r_cl, int g_cl, int b_cl)
{
	scr.setTextSize(5);
	scr.text("23:51", 10, 90);
}

void drawMainTemperature(int r_temp, int g_temp, int b_temp)
{
	scr.setTextSize(6);
	scr.text("24", 60, 5);
	scr.circle(140, 9, 9);
	scr.circle(140, 9, 8);
	scr.circle(140, 9, 7);
	scr.circle(140, 9, 6);
}

void drawMenuTabs(int total_rects, int selected, int r_rect, int g_rect, int b_rect)
{
	scr.stroke(r_rect, g_rect, b_rect);
	int offset = 1;
	for (int i=0; i < total_rects; i++)
	{
		if (i == selected)
		{
			scr.fill(r_rect, g_rect, b_rect);
		} else {
			scr.noFill();
		}
		scr.rect(1, offset, 8, 8);	
		offset += 14;	

	}

}
