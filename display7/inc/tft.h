/*
 * tft.h
 *
 *  Created on: 19.09.2017
 *      Author: kurt
 */

#ifndef TFT_H_
#define TFT_H_

#include <font.h>

void init_TFT_and_Perif();
void TFT_Fill_Screen(unsigned int rgb);
void TFT_Rectangle(int xs, int ys, int xe, int ye, unsigned int rgb, int linewidth);
void TFT_setRegion(int xstart, int ystart, int xend, int yend);
void TFT_Fill_Rectangle(int xs, int ys, int xe, int ye, unsigned int rgb);
void tft_print(FONT_INFO *pfont, char *text, int xpos, int ypos, int abstand, unsigned int fcol, unsigned int bcol);
void tft_print_restclear(FONT_INFO *pfont, char *text, int xpos, int ypos, int abstand, unsigned int fcol, unsigned int bcol, unsigned int restclear);
int TFT_DrawFile(char *fn, int x, int y);
void TFT_Vert_Line(int x, int ys, int ye, unsigned int rgb, unsigned int brgb, int linewidth, int pattern);
void TFT_Hor_Line(int xs, int xe, int y, unsigned int rgb, unsigned int brgb, int linewidth, int pattern);
void tft_textwidth_pixel(FONT_INFO *pfont, char *text, int *w, int *h);
void tft_preload_symbols();
void copy_bmp_to_flash();
int TFT_DrawFlash(char *fn, int x, int y);
void setDisplayamKopf();
void TFT_drawcircle(int x0, int y0, int radius);
void TFT_PolarToKartesian(int r, float phi, int *x, int *y);
void TFT_DrawLine(int x0,int y0,int x1,int y1, unsigned int rgb, int linewidth);
void TFT_DeleteLastLine(int linenum);
void TFT_DrawLine_aboveY(int x0,int y0,int x1,int y1, int aboveY, unsigned int rgb, int linewidth, int linenum);

#define TFT_WIDTH				800ULL
#define TFT_HEIGHT				480ULL

#define TFT_REG      (*((volatile unsigned short *) 0x60000000))
#define TFT_RAM      (*((volatile unsigned short *) 0x60020000))

#define RGB16(r,g,b)	( (((r>>3)&0x1f)<<11) | (((g>>2)&0x3f)<<5) | ((b>>3)&0x1f) )

#define BLACK	RGB16(0,0,0)
#define WHITE	RGB16(255,255,255)
#define BLUE	RGB16(0,0,255)
#define LIGHTBLUE	RGB16(160,160,255)
#define LIGHTBLUEANT RGB16(120,120,215)
#define DARKBLUE	RGB16(0,0,60)
#define RED		RGB16(255,0,0)
#define YELLOW	RGB16(255,255,0)
#define GREEN	RGB16(0,255,0)
#define DARKGREY RGB16(70,70,70)
#define LIGHTGREY RGB16(128,128,128)
#define BARLABELBLUE RGB16(200,200,255)

#endif /* TFT_H_ */
