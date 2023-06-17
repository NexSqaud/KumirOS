#include <stdlib_c.h>

#include <graphics.h>
#include <memory.h>
#include <rtc.h>
#include <utf8.h>
#include <wstring.h>

size_t virtualScreenWidth = SCREEN_WIDTH;
size_t virtualScreenHeight = SCREEN_HEIGHT;

size_t penThickness = 0;
Color penColor = { 0 };
Color brushColor = { 0 };

Color* painterBuffer = NULL;

Color recordToColor(const __kumir_scalar* color)
{
    Color result;
    __kumir_variant* fields = (__kumir_variant*)color->data.u.fields;
    result.red = fields[0].i;
    result.green = fields[1].i;
    result.blue = fields[2].i;

    return result;
}

bool updateScreen = false;

void pushBufferToScreen(void)
{
    if (painterBuffer == NULL || !updateScreen) return;

    vgaSetTargetBuffer(NULL, 0, 0);
    vgaDrawBuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, virtualScreenWidth, virtualScreenHeight, painterBuffer);
    vgaFlipBuffer();
    vgaSetTargetBuffer(painterBuffer, virtualScreenWidth, virtualScreenHeight);

    updateScreen = false;
}

void Painter_update_screen(void)
{
    if(rtcGetTicksCount() % RTC_CLOCK_FREQUENCY == 0)
    {
        updateScreen = true;
    }
}

EXTERN void Painter_new_page(const __kumir_scalar* width, const __kumir_scalar* height, const __kumir_scalar* fillColor)
{
    __kumir_check_value_defined(width);
    __kumir_check_value_defined(height);
    __kumir_check_value_defined(fillColor);

    if (width->data.i < 0 || height->data.i < 0)
    {
        __kumir_abort_on_error("Virtual screen width or height less than zero");
    }

    virtualScreenWidth = width->data.i;
    virtualScreenHeight = height->data.i;

    Color clearColor = recordToColor(fillColor);

    if (painterBuffer != NULL)
    {
        free(painterBuffer);
    }

    painterBuffer = malloc(virtualScreenWidth * virtualScreenHeight * sizeof(Color));

    DEBUG_PRINTF("Painter back buffer address: %x", painterBuffer);

    vgaSetTargetBuffer(painterBuffer, virtualScreenWidth, virtualScreenHeight);
    vgaClearBuffer(clearColor);
    pushBufferToScreen();
}

EXTERN void Painter_point(const __kumir_scalar* x, const __kumir_scalar* y, const __kumir_scalar* color)
{
    __kumir_check_value_defined(x);
    __kumir_check_value_defined(y);
    __kumir_check_value_defined(color);

    Color pixelColor = recordToColor(color);

    vgaSetPixel(x->data.i, y->data.i, pixelColor);
    pushBufferToScreen();
}

EXTERN void Painter_set_pen(const __kumir_scalar* thickness, const __kumir_scalar* color)
{
    __kumir_check_value_defined(thickness);
    __kumir_check_value_defined(color);

    penThickness = thickness->data.i;
    penColor = recordToColor(color);
}

EXTERN void Painter_set_brush(const __kumir_scalar* color)
{
    __kumir_check_value_defined(color);

    brushColor = recordToColor(color);
}

EXTERN void Painter_rectangle(const __kumir_scalar* x1, const __kumir_scalar* y1, const __kumir_scalar* x2, const __kumir_scalar* y2)
{
    __kumir_check_value_defined(x1);
    __kumir_check_value_defined(y1);
    __kumir_check_value_defined(x2);
    __kumir_check_value_defined(y2);

    size_t rx1 = x1->data.i;
    size_t ry1 = y1->data.i;
    size_t rx2 = x2->data.i;
    size_t ry2 = y2->data.i;

    vgaFillRectangle(rx1, ry1, rx2 - rx1, ry2 - ry1, brushColor);
    vgaDrawRectanle(rx1, ry1, rx2 - rx1, ry2 - ry1, penThickness, penColor);
    pushBufferToScreen();
}

EXTERN void Painter_write(const __kumir_scalar* x, const __kumir_scalar* y, const __kumir_scalar* text)
{
    __kumir_check_value_defined(x);
    __kumir_check_value_defined(y);
    __kumir_check_value_defined(text);

    size_t size = wideStringLength(text->data.s) * sizeof(wchar_t);
    char* buffer = malloc(size);
    wcharToUtf8(text->data.s, SIZE_MAX, buffer, size);

    vgaPrintString(x->data.i, y->data.i, buffer, penThickness, penColor);
    pushBufferToScreen();
}

EXTERN void Painter_text_width(__kumir_scalar* result, const __kumir_scalar* text)
{
    __kumir_check_value_defined(text);

    size_t size = wideStringLength(text->data.s) * sizeof(wchar_t);
    char* buffer = malloc(size);
    wcharToUtf8(text->data.s, SIZE_MAX, buffer, size);
    size_t textWidth = vgaMeasureString(buffer, penThickness);

    result->defined = true;
    result->type = __KUMIR_INT;
    result->data.i = textWidth;
}

EXTERN void Painter_circle(const __kumir_scalar* x, const __kumir_scalar* y, const __kumir_scalar* radius)
{
    __kumir_check_value_defined(x);
    __kumir_check_value_defined(y);
    __kumir_check_value_defined(radius);

    vgaFillCircle(x->data.i, y->data.i, radius->data.i, brushColor);
    pushBufferToScreen();
}

EXTERN void Painter_RGB(__kumir_scalar* result, const __kumir_scalar* red, const __kumir_scalar* green, const __kumir_scalar* blue)
{
    __kumir_check_value_defined(red);
    __kumir_check_value_defined(green);
    __kumir_check_value_defined(blue);

    __kumir_create_record(result, 4);
    __kumir_add_record_field(result, 0, red);
    __kumir_add_record_field(result, 1, green);
    __kumir_add_record_field(result, 2, blue);

    __kumir_variant* values = (__kumir_variant*)(result->data.u.fields);
    values[0].i = IMAX(IMIN(values[0].i, 255), 0);
    values[1].i = IMAX(IMIN(values[1].i, 255), 0);
    values[2].i = IMAX(IMIN(values[2].i, 255), 0);
}

EXTERN void Painter_split_to_RGB(const __kumir_scalar* color, __kumir_scalar* red, __kumir_scalar* green, __kumir_scalar* blue)
{
    __kumir_check_value_defined(color);

    __kumir_create_int(red, __kumir_get_record_field(color, 0).i);
    __kumir_create_int(green, __kumir_get_record_field(color, 1).i);
    __kumir_create_int(blue, __kumir_get_record_field(color, 2).i);
}
