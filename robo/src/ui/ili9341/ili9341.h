#ifndef ST7789_H
#define ST7789_H

#include <stdint.h>

#define WIDTH  (320)
#define HEIGHT (240)

#define ILI9341_NOP       0x00 //no operation
#define ILI9341_SWRESET   0x01 //software reset
#define ILI9341_RDDST     0x09 //read display status
#define ILI9341_RDDPM     0x0A //read display power
#define ILI9341_RDDMADCTL 0x0B //read display
#define ILI9341_RDDCOLMOD 0x0C
#define ILI9341_RDDIM     0x0D
#define ILI9341_RDDSM     0x0E
#define ILI9341_RDDSDR    0x0F
#define ILI9341_SLPIN     0x10
#define ILI9341_SLPOUT    0x11
#define ILI9341_PTLON     0x12
#define ILI9341_NORON     0x13
#define ILI9341_INVOFF    0x20
#define ILI9341_INVON     0x21
#define ILI9341_GAMSET    0x25
#define ILI9341_DISPOFF   0x28
#define ILI9341_DISPON    0x29
#define ILI9341_CASET     0x2A
#define ILI9341_RASET     0x2B
#define ILI9341_RAMWR     0x2C
#define ILI9341_RAMRD     0x2E
#define ILI9341_PTLAR     0x30
#define ILI9341_VSCRDEF   0x33
#define ILI9341_TEOFF     0x34
#define ILI9341_TEON      0x35
#define ILI9341_MADCTL    0x36
#define ILI9341_VSCRSADD  0x37
#define ILI9341_IDMOFF    0x38
#define ILI9341_IDMON     0x39
#define ILI9341_COLMOD    0x3A
#define ILI9341_RAMWRC    0x3C
#define ILI9341_RAMRDC    0x3E
#define ILI9341_TESCAN    0x44
#define ILI9341_RDTESCAN  0x45
#define ILI9341_WRDISBV   0x51
#define ILI9341_RDDISBV   0x52
#define ILI9341_WRCTRLD   0x53
#define ILI9341_RDCTRLD   0x54
#define ILI9341_WRCACE    0x55
#define ILI9341_RDCABC    0x56
#define ILI9341_WRCABCMB  0x5E
#define ILI9341_RDCABCMB  0x5F
#define ILI9341_PORCTRL    (0xB2)
#define ILI9341_GCTRL      (0xB7)
#define ILI9341_VCOMS      (0xBB)
#define ILI9341_DLPOFFSAVE (0xBD)
#define ILI9341_LCMCTRL    (0xC0)  //LCM Control
#define ILI9341_VDVVRHEN   (0xC2)  //VDV and VRH Command Enable
#define ILI9341_VRHS       (0xC3)  //VRH Set
#define ILI9341_VDVSET     (0xC4)  //VDV Set
#define ILI9341_FRCTR2     (0xC6)  //Frame Rate Control in Normal Mode
#define ILI9341_PWCTRL1    (0xD0)   //Power Control 1
#define ILI9341_PVGAMCTRL  (0xE0)
#define ILI9341_NVGAMCTRL  (0xE1)
#define ILI9341_RDABCSDR  0x68
#define ILI9341_RDID1     0xDA
#define ILI9341_RDID2     0xDB
#define ILI9341_RDID3     0xDC

void ili9341_setDisplayWindow(int x0, int y0, int x1, int y1);
void ili9341_init(void);
void ili9341_enterSleep(void);
void ili9341_exitSleep(void);
void ili9341_setBrightness(uint16_t bright);
void ili9341_fillLCD(uint16_t color);
void ili9341_writePixel(uint16_t color);
void ili9341_writePixelAtPoint(uint16_t x, uint16_t y, uint16_t color);

#endif
