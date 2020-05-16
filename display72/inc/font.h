#ifndef _NFONT_
#define _NFONT_

#define uint8_t unsigned char

typedef struct _font_char_info {
    unsigned char width;
    unsigned char height;
    unsigned int offset;
}FONT_CHAR_INFO;

typedef struct _font_info {
    char startchar;
    char endchar;
    FONT_CHAR_INFO *descriptor;
    unsigned char *bitmap;
} FONT_INFO;

extern FONT_INFO verdana_36ptFontInfo;
extern FONT_INFO lucidaConsole_22ptFontInfo;
extern FONT_INFO lucidaConsole_18ptFontInfo;
extern FONT_INFO lucidaConsoleBold_18ptFontInfo;
extern FONT_INFO lucidaConsole_14ptFontInfo;
extern FONT_INFO lucidaConsole_12ptFontInfo;

extern FONT_INFO verdana_22ptFontInfo;
extern FONT_INFO verdana_18ptFontInfo;
extern FONT_INFO verdana_12ptFontInfo;
extern FONT_INFO verdana_10ptFontInfo;

#endif // _NFONT_
