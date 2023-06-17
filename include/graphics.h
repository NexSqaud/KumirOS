#include <stddef.h>
#include <stdint.h>

#include <debug.h>
#include <string.h>

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
} __attribute__((packed)) Color;

int16_t vgaFontIndex(const char ch);
uint16_t vgaMeasureString(const char* str, uint8_t scale);

void vgaSetTargetBuffer(Color* buffer, size_t bufferWidth, size_t bufferHeight);

void vgaSetPixel(uint16_t x, uint16_t y, Color color);
void vgaDrawBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t bufferWidth, uint16_t bufferHeight, Color* buffer);
void vgaPrintString(uint16_t x, uint16_t y, const char* str, uint8_t scale, Color color);
void vgaPrintStringNew(uint16_t x, uint16_t y, String str, uint8_t scale, Color color);
void vgaFillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
void vgaDrawRectanle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t borderSize, Color color);
void vgaFillCircle(uint16_t x, uint16_t y, uint16_t radius, Color color);
void vgaClearBuffer(Color color);

void vgaDrawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t borderSize, uint8_t progress, Color color);

void vgaFlipBuffer(void);
void vgaInit(uint32_t frontBufferAddress);

#define FONT_WIDTH 5
#define FONT_HEIGHT 8

#endif