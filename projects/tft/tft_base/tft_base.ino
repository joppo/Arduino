#include <TFT.h>
#include <SPI.h>

#define cs 10
#define dc 9
#define rst 8

TFT scr = TFT(cs, dc, rst);


int r = 0;
int g = 255;
int b = 0;


void setup()
{
	scr.begin();
	scr.background(0,0,0);
	scr.setTextSize(1);
	//scr.stroke(r, g, b);
	scr.textWrap("TEST line perspective1 perspective2 perspective3", 6, 10);
	//scr.fill(r, g, b);
	drawMenuTabs(4, 1, r, g, b);
	//scr.text("6tastliv, uspeshen v:", 6, 20);
	//scr.text("bashtinstvoto,spaneto,", 6, 30);
	//scr.text("football-a, 6ah-a", 6, 40);
	//scr.text("i da namirash vreme", 6, 50);
	//scr.text("za priklucheniq sys", 6, 60);
	//scr.text("cqloto si (i ne samo)", 6, 70);
	//scr.text("domochadie", 6, 80);
	//scr.text("Nazdrave", 6, 90);

}

void loop()
{
	delay(200);
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
