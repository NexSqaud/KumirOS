#include <graphics.h>

#include <debug.h>
#include <memory.h>

Color* frontBuffer = NULL;
Color* backBuffer = NULL;

size_t targetWidth = SCREEN_WIDTH;
size_t targetHeight = SCREEN_HEIGHT;
Color* targetBuffer = NULL;

static char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789>!?. ";

static uint8_t font[sizeof(alphabet) - 1][5] = {
    {0x7e, 0x09, 0x09, 0x09, 0x7e},
    {0x7f, 0x49, 0x49, 0x49, 0x36},
    {0x3e, 0x41, 0x41, 0x41, 0x22},
    {0x7f, 0x41, 0x41, 0x41, 0x3e},
    {0x7f, 0x49, 0x49, 0x49, 0x41},
    {0x7f, 0x09, 0x09, 0x09, 0x01},
    {0x3e, 0x41, 0x49, 0x49, 0x7a},
    {0x7f, 0x08, 0x08, 0x08, 0x7f},
    {0x00, 0x41, 0x7f, 0x41, 0x00},
    {0x20, 0x40, 0x41, 0x3f, 0x01},
    {0x7f, 0x08, 0x14, 0x22, 0x41},
    {0x7f, 0x40, 0x40, 0x40, 0x40},
    {0x7f, 0x02, 0x0c, 0x02, 0x7f},
    {0x7f, 0x04, 0x08, 0x10, 0x7f},
    {0x3e, 0x41, 0x41, 0x41, 0x3e},
    {0x7f, 0x09, 0x09, 0x09, 0x06},
    {0x3e, 0x41, 0x51, 0x21, 0x5e},
    {0x7f, 0x09, 0x19, 0x29, 0x46},
    {0x46, 0x49, 0x49, 0x49, 0x31},
    {0x01, 0x01, 0x7f, 0x01, 0x01},
    {0x3f, 0x40, 0x40, 0x40, 0x3f},
    {0x0f, 0x30, 0x40, 0x30, 0x0f},
    {0x3f, 0x40, 0x30, 0x40, 0x3f},
    {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x07, 0x08, 0x70, 0x08, 0x07},
    {0x61, 0x51, 0x49, 0x45, 0x43},
    {0x20, 0x54, 0x54, 0x54, 0x78},
    {0x7f, 0x48, 0x44, 0x44, 0x38},
    {0x38, 0x44, 0x44, 0x44, 0x20},
    {0x38, 0x44, 0x44, 0x48, 0x7f},
    {0x38, 0x54, 0x54, 0x54, 0x18},
    {0x08, 0x7e, 0x09, 0x01, 0x02},
    {0x0c, 0x52, 0x52, 0x52, 0x3e},
    {0x7f, 0x08, 0x04, 0x04, 0x78},
    {0x00, 0x44, 0x7d, 0x40, 0x00},
    {0x20, 0x40, 0x44, 0x3d, 0x00},
    {0x00, 0x7f, 0x10, 0x28, 0x44},
    {0x00, 0x41, 0x7f, 0x40, 0x00},
    {0x7c, 0x04, 0x18, 0x04, 0x78},
    {0x7c, 0x08, 0x04, 0x04, 0x78},
    {0x38, 0x44, 0x44, 0x44, 0x38},
    {0x7c, 0x14, 0x14, 0x14, 0x08},
    {0x08, 0x14, 0x14, 0x18, 0x7c},
    {0x7c, 0x08, 0x04, 0x04, 0x08},
    {0x48, 0x54, 0x54, 0x54, 0x20},
    {0x04, 0x3f, 0x44, 0x40, 0x20},
    {0x3c, 0x40, 0x40, 0x20, 0x7c},
    {0x1c, 0x20, 0x40, 0x20, 0x1c},
    {0x3c, 0x40, 0x30, 0x40, 0x3c},
    {0x44, 0x28, 0x10, 0x28, 0x44},
    {0x0c, 0x50, 0x50, 0x50, 0x3c},
    {0x44, 0x64, 0x54, 0x4c, 0x44},
    {0x3e, 0x51, 0x49, 0x45, 0x3e},
    {0x00, 0x42, 0x7f, 0x40, 0x00},
    {0x42, 0x61, 0x51, 0x49, 0x46},
    {0x21, 0x41, 0x45, 0x4b, 0x31},
    {0x18, 0x14, 0x12, 0x7f, 0x10},
    {0x27, 0x45, 0x45, 0x45, 0x39},
    {0x3c, 0x4a, 0x49, 0x49, 0x30},
    {0x01, 0x71, 0x09, 0x05, 0x03},
    {0x36, 0x49, 0x49, 0x49, 0x36},
    {0x06, 0x49, 0x49, 0x29, 0x1e},
    {0x00, 0x41, 0x22, 0x14, 0x08},
    {0x00, 0x00, 0x9f, 0x00, 0x00},
    {0x02, 0x01, 0x51, 0x09, 0x06},
    {0x00, 0x60, 0x60, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00},
};

__attribute__((always_inline)) inline void vgaSetFrontbufferPixel(uint16_t x, uint16_t y, Color color)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT || frontBuffer == NULL) return;

    frontBuffer[y * SCREEN_WIDTH + x] = color;
}

__attribute__((always_inline)) inline void vgaSetPixel(uint16_t x, uint16_t y, Color color)
{
    if (x >= targetWidth || y >= targetHeight || targetBuffer == NULL) return;

    targetBuffer[y * targetWidth + x] = color;
}

void vgaSetTargetBuffer(Color* buffer, size_t bufferWidth, size_t bufferHeight)
{
    if (buffer == NULL)
    {
        targetBuffer = backBuffer;
        targetWidth = SCREEN_WIDTH;
        targetHeight = SCREEN_HEIGHT;
    }
    else
    {
        targetBuffer = buffer;
        targetWidth = bufferWidth;
        targetHeight = bufferHeight;
    }
}

#if 0
void vgaDrawBuffer(uint16_t x, uint16_t y, uint16_t bufferWidth, uint16_t bufferHeight, Color* buffer)
{
    if (x > SCREEN_WIDTH || y > SCREEN_HEIGHT) return;

    uint16_t copyWidth = bufferWidth;
    uint16_t copyHeight = bufferHeight;

    if (x + bufferWidth > SCREEN_WIDTH) copyWidth = SCREEN_WIDTH - x;
    if (y + bufferHeight > SCREEN_HEIGHT) copyHeight = SCREEN_HEIGHT - y;

    for (int dy = 0; dy < copyHeight; dy++)
    {
        for (int dx = 0; dx < copyWidth; dx++)
        {
            backBuffer[(y + dy) * SCREEN_WIDTH + (x + dx)] = buffer[dy * bufferWidth + dx];
        }
    }
}
#else
void vgaDrawBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t bufferWidth, uint16_t bufferHeight, Color* buffer)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;

    uint16_t copyWidth = width;
    uint16_t copyHeight = height;

    if (x + copyWidth > SCREEN_WIDTH) copyWidth = SCREEN_WIDTH - x;
    if (y + copyHeight > SCREEN_HEIGHT) copyHeight = SCREEN_HEIGHT - y;

    float widthCoefficient = (float)bufferWidth / width;
    float heightCoefficient = (float)bufferHeight / height;

    if (width == SCREEN_WIDTH) widthCoefficient = 1.0f;
    if (height == SCREEN_HEIGHT) heightCoefficient = 1.0f;

    for (size_t dy = 0; dy < copyHeight; dy++)
    {
        for (size_t dx = 0; dx < copyWidth; dx++)
        {
            int px = (int)(dx * bufferWidth / width);
            int py = (int)(dy * bufferHeight / height);

            backBuffer[((y + dy) * SCREEN_WIDTH) + (x + dx)] = buffer[py * bufferWidth + px];
        }
    }

}
#endif
inline int16_t vgaFontIndex(const char ch)
{
    for (size_t i = 0; i < sizeof(alphabet) - 1; i++)
    {
        if (ch == alphabet[i]) return i;
    }

    return -1;
}

inline uint16_t vgaMeasureString(const char* str, uint8_t scale)
{
    uint16_t result = 0;
    while (*str)
    {
        result += (FONT_WIDTH + 1) * scale;
        str++;
    }
    return result;
}

void vgaPrintString(uint16_t x, uint16_t y, const char* str, uint8_t scale, Color color)
{
#if 0
    String newStr =
    {
        .buffer = str,
        .count = stringLength(str),
        .length = stringLength(str)
    };

    vgaPrintStringNew(x, y, newStr, scale, color);

#else
    if (x >= targetWidth || y >= targetHeight || scale == 0) return;

    for (size_t i = 0; *str; i++, str++)
    {
        int16_t fontIndex = vgaFontIndex(*str);
        if (fontIndex == -1)
        {
            continue;
        }

        uint16_t dx = x + ((FONT_WIDTH + 1) * scale) * i;
        uint16_t dy = y;

        for (int x = 0; x < FONT_WIDTH * scale; x++)
        {
            for (int y = 0; y < FONT_HEIGHT * scale; y++)
            {
                uint8_t px = x / scale;
                uint8_t py = y / scale;

                if (font[fontIndex][px] & (1 << py))
                {
                    vgaSetPixel(dx + x, dy + y, color);
                }
            }
        }
    }
#endif
}

void vgaPrintStringNew(uint16_t x, uint16_t y, String str, uint8_t scale, Color color)
{
    if (x >= targetWidth || y >= targetHeight) return;

    for (size_t i = 0; i < str.count; i++)
    {
        int16_t fontIndex = vgaFontIndex(str.buffer[i]);
        if (fontIndex == -1)
        {
            continue;
        }

        uint16_t dx = x + ((FONT_WIDTH + 1) * scale) * i;
        uint16_t dy = y;

        for (int x = 0; x < FONT_WIDTH * scale; x++)
        {
            for (int y = 0; y < FONT_HEIGHT * scale; y++)
            {
                uint8_t px = x / scale;
                uint8_t py = y / scale;

                if (font[fontIndex][px] & (1 << py))
                {
                    vgaSetPixel(dx + x, dy + y, color);
                }
            }
        }
    }
}

void vgaFillCircle(uint16_t x, uint16_t y, uint16_t radius, Color color)
{
    if (x - radius >= targetWidth || y - radius >= targetHeight) return;

    for (int16_t dy = -(radius * 2); dy < radius * 2; dy++)
    {
        for (int16_t dx = -(radius * 2); dx < radius * 2; dx++)
        {
            if (dx * dx + dy * dy < radius * radius)
            {
                vgaSetPixel(x + dx, y + dy, color);
            }
        }
    }
}

void vgaFillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color)
{
    if (x >= targetWidth || y >= targetHeight) return;

    for (size_t dy = 0; dy < height; dy++)
    {
        for (size_t dx = 0; dx < width; dx++)
        {
            vgaSetPixel(x + dx, y + dy, color);
        }
    }
}

void vgaDrawRectanle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t borderSize, Color color)
{
    if (x >= targetWidth || y >= targetHeight) return;

    vgaFillRectangle(x, y, width, borderSize, color);
    vgaFillRectangle(x, y, borderSize, height, color);
    vgaFillRectangle(x + width - borderSize, y, borderSize, height, color);
    vgaFillRectangle(x, y + height - borderSize, width, borderSize, color);
}

void vgaDrawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t borderSize, uint8_t progress, Color color)
{
    static uint8_t borderOffset = 5;
    if (x >= targetWidth || y >= targetHeight) return;

    if (progress > 100) progress = 100;

    uint16_t maxWidth = width - (borderSize + borderOffset) * 2;

    uint16_t barX = x + borderSize + borderOffset;
    uint16_t barY = y + borderSize + borderOffset;

    uint16_t barWidth = (maxWidth * progress) / 100;
    uint16_t barHeight = height - (borderSize + borderOffset) * 2;

    vgaDrawRectanle(x, y, width, height, borderSize, color);
    vgaFillRectangle(barX, barY, barWidth, barHeight, color);
}

void vgaFlipBuffer(void)
{
    if (backBuffer == NULL) return;

    for (uint32_t i = 0; i < targetWidth * targetHeight; i++)
    {
        vgaSetFrontbufferPixel(i % targetWidth, i / targetWidth, targetBuffer[i]);
    }
}

void vgaClearBuffer(Color color)
{
    if (targetBuffer == NULL) return;

    for (int i = 0; i < targetWidth * targetHeight; i++)
    {
        targetBuffer[i] = color;
    }
}

void vgaInit(uint32_t frontBufferAddress)
{
    DEBUG_PRINTF("Front buffer address: %x", frontBufferAddress);

    frontBuffer = (Color*)frontBufferAddress;
    backBuffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Color));

    DEBUG_PRINTF("Back buffer address: %x", backBuffer);
}