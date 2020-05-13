/*
 * font.c
 *
 *  Created on: 17.08.2017
 *      Author: kurt
 *
 *      Erzeugen der Fonts mit: The Dot Factory
 *      Flip/Rotate: alles 0
 *      Padding Removal: Y=Fixed X=Tighest (oder Fixed für Fixfonts)
 *      Line Wrap: at column
 *      Comments: alles ankreuten, Style: Cpp
 *      Byte: BitLayout:RowMajor, Order:MsbFirst, Format:Hex, Leading: 0x
 *		Descriptors: Generate ankreuzen, CharWid: in Bits, CharHeigth: in Bits, Font Height: don't display
 *		 			 Multiple..: nicht ankreuzen
 *		 			 Image wid,heigth: in Bytes
 *		 			 Space char gen: ankreuzen, Pix: 2
 *
 */

#include <main.h>
// chr ... gesuchter Buchstabe
// returns: pointer auf Bitmap
// *width, *height ... Größe des Buchstabens in Pixel (bits)
unsigned char *getFont(FONT_INFO *pfont, char chr, unsigned char *width, unsigned char *height)
{
	FONT_CHAR_INFO *charinfo = &pfont->descriptor[chr-' '];

	*width = charinfo->width;
	*height = charinfo->height;

	return pfont->bitmap + charinfo->offset;
}

void tft_print_restclear(FONT_INFO *pfont, char *text, int xpos, int ypos, int abstand, unsigned int fcol, unsigned int bcol, unsigned int restclear)
 {
	 unsigned char idx;
	 unsigned char x,y;
	 unsigned char width, height=0;
	 unsigned int bytenum;
	 unsigned int pixcol;
	 unsigned int xright = 0;
	 unsigned char *fdata;

	 for(idx = 0; idx < strlen(text); idx++)
	 {
		 // hole die Bitmap
		 fdata = getFont(pfont, text[idx], &width, &height);

		 // Definiere den benötigten Zeichenbereich
		 TFT_setRegion(xpos,ypos,xpos+width-1,ypos+height-1);
		 TFT_REG=0x2C;

		 xright = xpos+width-1;	// merke rechteste Position

		 for(y=0; y<height; y++)
		 {
			 bytenum = y;                // diese beiden Zeilen getrennt lassen wegen 16 bit Berechnung
			 bytenum *= (((int)width+7)/8);
			 for(x=0; x<width; x++)
			 {
				 if(fdata[bytenum+x/8] & (1<<(7-(x%8))))
					 pixcol = fcol;
				 else
					 pixcol = bcol;

				 TFT_RAM = pixcol;
			 }
		 }
		 // Abstand zwischen den Buchstaben
		 // Fülle diesen mit der Hintergrundfarbe
		 if(abstand)
		 {
			 TFT_Fill_Rectangle(xpos+width, ypos, xpos+width+abstand, ypos+height-1, bcol);
		 }
		 xpos += (width + abstand);
	 }

	 // lösche von rechtester Position bis Position restclear
	 if(restclear > xright && restclear < TFT_WIDTH)
	 //tft_FillColor(xright+1, ypos, restclear-xright, height, bred,bgreen,bblue);
	 TFT_Fill_Rectangle(xright+1, ypos, restclear, ypos+height-1, bcol);
 }

 void tft_print(FONT_INFO *pfont, char *text, int xpos, int ypos, int abstand, unsigned int fcol, unsigned int bcol)
 {
	 tft_print_restclear(pfont,text,xpos,ypos,abstand,fcol, bcol,0);
 }

 // ermittle die Breite eines Textes in Pixel
 void tft_textwidth_pixel(FONT_INFO *pfont, char *text, int *w, int *h)
 {
	 *w = *h = 0;
	 unsigned char idx;
	 unsigned char width, height=0;

	 for(idx = 0; idx < strlen(text); idx++)
	 {
		 // hole die größe eines Buchstabens
		 getFont(pfont, text[idx], &width, &height);
		 *w += width;
		 *h = height;
	 }
 }
