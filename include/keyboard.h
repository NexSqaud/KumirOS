#include <stddef.h>

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define DEFINE_KEYBOARD_KEY_STRING(x) [KeyboardKey_ ## x] = stringify(x) \

typedef enum {
    KeyboardKey_None,
    KeyboardKey_Escape,
    KeyboardKey_1,
    KeyboardKey_2,
    KeyboardKey_3,
    KeyboardKey_4,
    KeyboardKey_5,
    KeyboardKey_6,
    KeyboardKey_7,
    KeyboardKey_8,
    KeyboardKey_9,
    KeyboardKey_0,
    KeyboardKey_Minus,
    KeyboardKey_Equals,
    KeyboardKey_Backspace,
    KeyboardKey_Tab,
    KeyboardKey_Q,
    KeyboardKey_W,
    KeyboardKey_E,
    KeyboardKey_R,
    KeyboardKey_T,
    KeyboardKey_Y,
    KeyboardKey_U,
    KeyboardKey_I,
    KeyboardKey_O,
    KeyboardKey_P,
    KeyboardKey_OpenSquareBracets,
    KeyboardKey_CloseSqruareBracets,
    KeyboardKey_Enter,
    KeyboardKey_LeftControl,
    KeyboardKey_A,
    KeyboardKey_S,
    KeyboardKey_D,
    KeyboardKey_F,
    KeyboardKey_G,
    KeyboardKey_H,
    KeyboardKey_J,
    KeyboardKey_K,
    KeyboardKey_L,
    KeyboardKey_Semicolon,
    KeyboardKey_SingleQuote,
    KeyboardKey_BackTick,
    KeyboardKey_LeftShift,
    KeyboardKey_BackSlash,
    KeyboardKey_Z,
    KeyboardKey_X,
    KeyboardKey_C,
    KeyboardKey_V,
    KeyboardKey_B,
    KeyboardKey_N,
    KeyboardKey_M,
    KeyboardKey_Comma,
    KeyboardKey_Dot,
    KeyboardKey_Slash,
    KeyboardKey_RightShift,
    KeyboardKey_KeypadStar,
    KeyboardKey_LeftAlt,
    KeyboardKey_Space,
    KeyboardKey_CapsLock,
    KeyboardKey_F1,
    KeyboardKey_F2,
    KeyboardKey_F3,
    KeyboardKey_F4,
    KeyboardKey_F5,
    KeyboardKey_F6,
    KeyboardKey_F7,
    KeyboardKey_F8,
    KeyboardKey_F9,
    KeyboardKey_F10,
    KeyboardKey_NumLock,
    KeyboardKey_ScrollLock,
    KeyboardKey_Keypad7,
    KeyboardKey_Keypad8,
    KeyboardKey_Keypad9,
    KeyboardKey_KeypadMinus,
    KeyboardKey_Keypad4,
    KeyboardKey_Keypad5,
    KeyboardKey_Keypad6,
    KeyboardKey_KeypadPlus,
    KeyboardKey_Keypad1,
    KeyboardKey_Keypad2,
    KeyboardKey_Keypad3,
    KeyboardKey_Keypad0,
    KeyboardKey_KeypadDot,

    KeyboardKey_F11 = 0x57,
    KeyboardKey_F12,
} KeyboardKey;

typedef void(*keyPressedHandler)(KeyboardKey);

extern const char* keyStrings[KeyboardKey_F12 + 1];

void keyboardInit(void);
bool keyboardIsAnyKeyPressed(void);
bool keyboardIsKeyPressed(KeyboardKey key);

void keyboardAddKeyPressedHandler(keyPressedHandler handler);

#endif