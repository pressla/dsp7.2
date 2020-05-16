/*
 * tft_highlevel.c
 *
 *  Created on: 13.08.2017
 *      Author: kurt
 */

#include <main.h>
#include <math.h>

void tft_copy_sd_to_flash(char *fn);

void TFT_setRegion(int xstart, int ystart, int xend, int yend)
{
int tmp;

	// x=0, y=0 ist links oben
	TFT_REG = 0x36;
	TFT_RAM = 0x02;

	if(xstart > xend)
	{
		tmp = xstart;
		xstart = xend;
		xend = tmp;
	}

	if(ystart > yend)
	{
		tmp = ystart;
		ystart = yend;
		yend = tmp;
	}

	TFT_REG=0x2A;
	TFT_RAM=xstart >> 8;
	TFT_RAM=xstart;
	TFT_RAM=xend >> 8;
	TFT_RAM=xend;

	TFT_REG=0x2B;
	TFT_RAM=ystart >> 8;
	TFT_RAM=ystart;
	TFT_RAM=yend >> 8;
	TFT_RAM=yend;
}

void TFT_Fill_Rectangle(int xs, int ys, int xe, int ye, unsigned int rgb)
{
    long len = (abs(xe-xs)+1)*(abs(ye-ys)+1);
    if(len <= 0 || len > (800*480)) return;

    TFT_setRegion(xs, ys, xe, ye);

    TFT_REG=0x2C;

    for(long pixel = 0; pixel < len; pixel++)
    	TFT_RAM = rgb;
}

void TFT_Fill_Screen(unsigned int rgb)
{
	TFT_Fill_Rectangle(0,0,TFT_WIDTH-1,TFT_HEIGHT-1,rgb);
}

long totalpiclen = 0;
// Bereite Ausgabe eines Bildes vor
void TFT_startPicture(int xs, int ys, int xe, int ye)
{
	totalpiclen = (abs(xe-xs)+1)*(abs(ye-ys)+1);

	if(totalpiclen <= 0 || totalpiclen > (800*480)) return;


	TFT_setRegion(xs, ys, xe, ye);

	// für diese Bildausgabe stelle x=0 und y=0 auf links unten
	TFT_REG = 0x36;
	TFT_RAM = 0x82;

	TFT_REG=0x2C;
}

// Zeiche einen Teil des vorbereiteten Bildes
void TFT_drawPictureLine(uint8_t *buf, int len)
{
uint16_t rgb;

	for(int pixel = 0; pixel < len; pixel+=2)
	{
		rgb = buf[pixel+1];
		rgb <<= 8;
		rgb |= buf[pixel];
		TFT_RAM = rgb;
	}
}

void TFT_SetPixel(int x, int y, unsigned int rgb)
{
	TFT_setRegion(x, y, x, y);

	TFT_REG=0x2C;
   	TFT_RAM = rgb;
}

void TFT_Hor_Line(int xs, int xe, int y, unsigned int rgb, unsigned int brgb, int linewidth, int pattern)
{
int i;

	TFT_setRegion(xs, y, xe, y+linewidth-1);

	TFT_REG=0x2C;

	for(i=0; i<(xe-xs)*linewidth; i++)
	{
		if(pattern != 0)
		{
			// punktierte Line
			if(!(i%pattern) || !((i-1)%pattern)) // immer 2 Punkte, da Einzelpunkte bei dem Display zum Flackern neigen
				TFT_RAM = rgb;
			else
				TFT_RAM = brgb;
		}
		else					// durchgezogene Linie
			TFT_RAM = rgb;
	}
}

void TFT_Vert_Line(int x, int ys, int ye, unsigned int rgb, unsigned int brgb, int linewidth, int pattern)
{
int i;

	if(ye < ys)
	{
		int h=ye;
		ye = ys;
		ys = h;
	}

	TFT_setRegion(x, ys, x+linewidth-1, ye);

	TFT_REG=0x2C;

	for(i=0; i<(ye-ys)*linewidth; i++)
	{
		if(pattern != 0)
		{
			// punktierte Line
			if(!(i%pattern) || !((i-1)%pattern)) // immer 2 Punkte, da Einzelpunkte bei dem Display zum Flackern neigen
				TFT_RAM = rgb;
			else
				TFT_RAM = brgb;
		}
		else					// durchgezogene Linie
			TFT_RAM = rgb;
	}
}

void TFT_DrawLine(int x0,int y0,int x1,int y1, unsigned int rgb, int linewidth)
{
int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
int err = (dx>dy ? dx : -dy)/2, e2;

	while(1)
	{
		if(linewidth == 1)
			TFT_SetPixel(x0,y0,rgb);
		else
			TFT_Fill_Rectangle(x0, y0, x0+linewidth-1, y0+linewidth-1, rgb);

		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

// Speicher für den Hintergrund bei Setzen eines Pixels
#define MAXBGSTORAGE	400
#define PIXELLINES	8
uint16_t storageBG[PIXELLINES][MAXBGSTORAGE];
int pixnum[PIXELLINES] = {-1,-1,-1,-1};		// Zähler für die Pixelnummer

void TFT_SetPixelSafeBG(int x, int y, int rgb, int linenum)
{
	TFT_setRegion(x, y, x, y);

	// Lese Wert des aktuellen Pixels
	TFT_REG=0x2E;
	storageBG[linenum][pixnum[linenum]] = TFT_RAM;
	if(pixnum[linenum] < MAXBGSTORAGE) pixnum[linenum]++;

	// setze das Pixel neu
	TFT_REG=0x2C;
   	TFT_RAM = rgb;
}

int lastline_x0[PIXELLINES];
int lastline_x1[PIXELLINES];
int lastline_y0[PIXELLINES];
int lastline_y1[PIXELLINES];
int last_linewidth;
int last_aboveY;

void TFT_DrawLine_aboveY(int x0,int y0,int x1,int y1, int aboveY, unsigned int rgb, int linewidth, int linenum)
{
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;

	// speichere aktuelle Line
	lastline_x0[linenum] = x0;
	lastline_x1[linenum] = x1;
	lastline_y0[linenum] = y0;
	lastline_y1[linenum] = y1;
	last_linewidth = linewidth;
	last_aboveY = aboveY;

	pixnum[linenum] = 0;
	while(1)
	{
		if(y0 < aboveY)
		{
			TFT_SetPixelSafeBG(x0,y0,rgb,linenum);
		}
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

// ersetze letzte Linie mit Hintergrund
void TFT_DeleteLastLine(int linenum)
{
	if(pixnum[linenum] == -1) return;

int x0 = lastline_x0[linenum];
int x1 = lastline_x1[linenum];
int y0 = lastline_y0[linenum];
int y1 = lastline_y1[linenum];

int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
int err = (dx>dy ? dx : -dy)/2, e2;

	pixnum[linenum] = 0;
	while(1)
	{
		if(y0 < last_aboveY)
		{
			TFT_SetPixel(x0,y0,storageBG[linenum][pixnum[linenum]]);
			if(pixnum[linenum] < MAXBGSTORAGE) pixnum[linenum]++;
		}
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void TFT_Rectangle(int xs, int ys, int xe, int ye, unsigned int rgb, int linewidth)
{
	TFT_Hor_Line(xs+linewidth,xe+1,ys,rgb, BLACK, linewidth,0);
	TFT_Hor_Line(xs+linewidth,xe+linewidth-1,ye,rgb, BLACK, linewidth,0);
	TFT_Vert_Line(xs,ys,ye+linewidth,rgb, BLACK, linewidth, 0);
	TFT_Vert_Line(xe,ys,ye+linewidth,rgb, BLACK, linewidth, 0);
}

void TFT_drawcircle(int x0, int y0, int radius)
{
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
    	TFT_SetPixel(x0 + x, y0 + y, BLACK);
    	TFT_SetPixel(x0 + y, y0 + x, BLACK);
    	TFT_SetPixel(x0 - y, y0 + x, BLACK);
    	TFT_SetPixel(x0 - x, y0 + y, BLACK);
    	TFT_SetPixel(x0 - x, y0 - y, BLACK);
    	TFT_SetPixel(x0 - y, y0 - x, BLACK);
    	TFT_SetPixel(x0 + y, y0 - x, BLACK);
    	TFT_SetPixel(x0 + x, y0 - y, BLACK);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += (-radius << 1) + dx;
        }
    }
}

void TFT_PolarToKartesian(int r, float phi, int *x, int *y)
{
float fphi = (360.0-phi) * 2.0 * 3.14159 / 360.0;

	*x = (int)((float)r * cos(fphi));
	*y = (int)((float)r * sin(fphi));
}

// Zeichne Bilder die mit GIMP als BMP mit der Spezialoption 565 gespeichert wurden
#define TFT_FILE_LINELEN 150 // mindestens 140 wegen Bitmapheader !!!

int TFT_DrawFile(char *fn, int x, int y)
{
	FATFS FatFs;
	FIL fil;
	uint8_t picbuf[TFT_FILE_LINELEN];
	UINT bytesread;
	int ret = 0;

	if (f_mount(&FatFs, "", 1) == FR_OK)
	{
		if (f_open(&fil, fn, FA_READ) == FR_OK)
		{
			// Lese Bitmapheader
			f_read(&fil,picbuf,140,&bytesread);
			int width = picbuf[18] + (((int)picbuf[19])<<8);
			int height = picbuf[22] + (((int)picbuf[23])<<8);

			TFT_startPicture(x,y,x+width-1,y+height-1);
			while(1)
			{
				f_read(&fil,picbuf,TFT_FILE_LINELEN,&bytesread);
				if(bytesread == 0) break;
				TFT_drawPictureLine(picbuf,bytesread);
				if(bytesread < TFT_FILE_LINELEN) break;
			}
			f_close(&fil);
			ret = 1;
		}
		f_mount(0, "", 1); //Unmount drive, don't forget this!
	}

	return ret;
}

/* =========  Icons in Flash ==========
 * drei Sektoren zu je 128 kB
 *
 * Start bei Sector 9,  0x080A0000 bis zum Ende des Flashs
 *
 * Sector 11,  0x080FFB50
 * hier beginnt die Bildtabelle (1200 Bytes max)
 * für jedes Bild: 8 Byte Bildname gefolgt von 4 Byte Adresse und 2 Byte Länge (MSB first)
 */

// kopiere die Icons von der SD Karte in den Flash

#define FLASHDATA_SECTOR9  0x080A0000
#define FLASHDATA_SECTOR10 0x080C0000
#define FLASHDATA_SECTOR11 0x080E0000
#define BMPTABLE_SECTOR11  0x080FFB50

#define MAXPICSIZE	60000	// Maximalgröße einer Bitmap

uint32_t flashaddress = FLASHDATA_SECTOR9;
uint32_t tableaddess = BMPTABLE_SECTOR11;

void copy_bmp_to_flash()
{
	// prüfe ob SD Karte vorhanden ist
	FATFS FatFs;
	if (f_mount(&FatFs, "", 1) != FR_OK) return;
	//f_mount(0, "", 1);

	DIR dir;
	FILINFO fi;
	FRESULT fr = f_findfirst (&dir, &fi, "", "*.cmp");
	if(fr != FR_OK || fi.fname[0] == 0 )
	{
		// keine bmp auf der SD Karte
		f_mount(0, "", 1);
		FLASH_Lock();
		return;
	}

	TFT_Fill_Screen(BLACK);
	tft_print(&lucidaConsole_22ptFontInfo, getLang(SD_to_Flash), 0,0, 2, WHITE , BLACK);

	tft_print_restclear(&lucidaConsole_22ptFontInfo, getLang(clear_flash), 0,80, 2, WHITE , BLACK, TFT_WIDTH);
	FLASH_Unlock();
	FLASH_EraseSector(FLASH_Sector_9, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);

	while(1)
	{
		tft_copy_sd_to_flash(fi.fname);
		fr = f_findnext(&dir, &fi);
		if(fr != FR_OK || fi.fname[0] == 0 ) break;
	}

	// und kopiere noch alle ZIP Dateien, falls vorhanden
	fr = f_findfirst (&dir, &fi, "", "*.zip");
	if(fr == FR_OK && fi.fname[0] != 0 )
	{
		while(1)
		{
			tft_copy_sd_to_flash(fi.fname);
			fr = f_findnext(&dir, &fi);
			if(fr != FR_OK || fi.fname[0] == 0 ) break;
		}
	}

	f_mount(0, "", 1);
	FLASH_Lock();

	char s[50];
	strcpy(s,getLang(SDFINISHED));
	sprintf(s+strlen(s),": %ld Bytes",BMPTABLE_SECTOR11-flashaddress);
	tft_print_restclear(&lucidaConsole_22ptFontInfo, s, 0,80, 2, WHITE , BLACK, TFT_WIDTH);
	delay_1ms(3000);
	TFT_Fill_Screen(BLACK);
}

void tft_copy_sd_to_flash(char *fn)
{
	// Lese das Bild in den Buffer
	FIL fil;
	uint8_t picbuf[MAXPICSIZE];
	UINT picsize = 0;
	uint32_t picaddress;

	//if (f_mount(&FatFs, "", 1) == FR_OK)
	{
		if (f_open(&fil, fn, FA_READ) == FR_OK)
		{
			f_read(&fil,picbuf,MAXPICSIZE,&picsize);
			f_close(&fil);
		}
	//	f_mount(0, "", 1); //Unmount drive, don't forget this!
	}
	if(picsize == 0) return; // Bild nicht gefunden

	tft_print_restclear(&lucidaConsole_22ptFontInfo, fn, 0,80, 2, WHITE , BLACK, TFT_WIDTH);

	// kopiere Bild ins Flash
	picaddress = flashaddress;
	for(int i=0; i<picsize; i++)
	{
		FLASH_ProgramByte(flashaddress++, picbuf[i]);
	}

	// erzeuge 8Byte Dateinamen
	char filename[8];
	memset(filename,0,8);
	for(int i=0; i<8; i++)
	{
		if(fn[i] == '.' || fn[0] == 0) break;
		filename[i] = toupper(fn[i]);
	}

	// Speichere den Namen
	for(int i=0; i<8; i++)
	{
		FLASH_ProgramByte(tableaddess++, filename[i]);
	}

	// Speichere die Adresse
	FLASH_ProgramByte(tableaddess++, picaddress >> 24);
	FLASH_ProgramByte(tableaddess++, picaddress >> 16);
	FLASH_ProgramByte(tableaddess++, picaddress >> 8);
	FLASH_ProgramByte(tableaddess++, picaddress);

	// Speichere die Länge
	FLASH_ProgramByte(tableaddess++, picsize >> 8);
	FLASH_ProgramByte(tableaddess++, picsize);
}

/*
 * Version für reine 5-6-5 Bitmaps unkomprimiert
 *
int TFT_DrawFlash(char *fn, int x, int y)
{
uint8_t *pflash = 0;
uint8_t *ptable = (unsigned char *)BMPTABLE_SECTOR11;
char filename[9];

	// filename muss 8 Zeichen haben, mit 0 aufgefüllt
	for(int i=0; i<strlen(fn); i++)
		fn[i] = toupper(fn[i]);
	memset(filename,0,9);
	strcpy(filename,fn);

	// suche das Bild in der Tabelle
	while((uint32_t)ptable < (0x08100000 - 8 - 4 -2))
	{
		if(!memcmp(ptable,filename,8))
		{
			// Bild gefunden
			// lese Adresse
			uint32_t adr = (uint32_t)(ptable[8])<<24;
			adr += (uint32_t)(ptable[9])<<16;
			adr += (uint32_t)(ptable[10])<<8;
			adr += ptable[11];
			pflash = (uint8_t *)adr;
			// lese Länge
			uint16_t len = (uint16_t)(ptable[12])<<8;
			len += ptable[13];
			// Lese Bitmapheader
			int width = pflash[18] + (((int)pflash[19])<<8);
			int height = pflash[22] + (((int)pflash[23])<<8);

			TFT_startPicture(x,y,x+width-1,y+height-1);
			TFT_drawPictureLine(pflash+140,len-140);
			return 1;

		}
		ptable += (8+4+2);
	}
	return 0;
}
*/


// zeichnet LZE(2Byte) komprimierte 5-6-5 Bitmaps
int TFT_DrawFlash(char *fn, int x, int y)
{
uint8_t *ptable = (unsigned char *)BMPTABLE_SECTOR11;
uint8_t *pflash = 0;
char filename[9];
int decomppos=0;
uint8_t anz;
int width=1, height;
uint16_t data;

	// filename muss 8 Zeichen haben, mit 0 aufgefüllt
	memset(filename,0,9);
	strncpy(filename,fn,8);
	for(int i=0; i<strlen(filename); i++)
		filename[i] = toupper(filename[i]);


	// suche das Bild in der Tabelle
	while((uint32_t)ptable < (0x08100000 - 8 - 4 -2))
	{
		if(!memcmp(ptable,filename,8))
		{
			// Bild gefunden
			// ptable zeigt auf die Dateitabelle in Sector 11:
			// 8 Byte Dateiname, 4 Byte Flashadresse, 2 Byte Dateilänge
			// lese Adresse
			uint32_t adr = (uint32_t)(ptable[8])<<24;
			adr += (uint32_t)(ptable[9])<<16;
			adr += (uint32_t)(ptable[10])<<8;
			adr += ptable[11];
			pflash = (uint8_t *)adr;
			// lese Länge
			uint16_t len = (uint16_t)(ptable[12])<<8;
			len += ptable[13];

			// pflash zeigt auf das erste Byte des komprimierten Bildes im Flash
			// gehe jetzt durch diese komprimierte Datei
			for(int complen = 0; complen < len; complen+=2)
			{
				data = *pflash++;
				data += (((int)*pflash++)<<8);
				anz = *pflash++;

				// anz: Anzahl der 16 Bit Werte
				for(int i=0; i<anz; i++)
				{
					// hier stehen die originalen Bytes zur Verfügung (in data), bearbeite sie
					// lese Höhe und Breite der Bitmap aus dem Header
					if(decomppos == 18) width = data;
					if(decomppos == 22)
					{
						height = data;
						// Header fertig gelesen, initialisiere das Zeichnen der Bitmap
						TFT_startPicture(x,y,x+width-1,y+height-2); //-2 bei der Höhe, weil sonst ein Pixel rechts oben stehen bleibt
					}
					// der Bildinhalt beginnt ab Byte 140
					if(decomppos >= 140)
					{
						TFT_RAM = data;
					}

					decomppos+=2;
				}
			}
			return 1;

		}
		ptable += (8+4+2);
	}
	return 0;
}
