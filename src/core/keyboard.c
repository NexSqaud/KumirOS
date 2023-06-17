#include <keyboard.h>

#include <debug.h>
#include <ports.h>
#include <idt.h>

#define KEY_PRESSED_HANDLERS_COUNT 10

const char* keyStrings[KeyboardKey_F12 + 1] =
{
    DEFINE_KEYBOARD_KEY_STRING(Escape),
    DEFINE_KEYBOARD_KEY_STRING(1),
    DEFINE_KEYBOARD_KEY_STRING(2),
    DEFINE_KEYBOARD_KEY_STRING(3),
    DEFINE_KEYBOARD_KEY_STRING(4),
    DEFINE_KEYBOARD_KEY_STRING(5),
    DEFINE_KEYBOARD_KEY_STRING(6),
    DEFINE_KEYBOARD_KEY_STRING(7),
    DEFINE_KEYBOARD_KEY_STRING(8),
    DEFINE_KEYBOARD_KEY_STRING(9),
    DEFINE_KEYBOARD_KEY_STRING(0),
    DEFINE_KEYBOARD_KEY_STRING(Minus),
    DEFINE_KEYBOARD_KEY_STRING(Equals),
    DEFINE_KEYBOARD_KEY_STRING(Backspace),
    DEFINE_KEYBOARD_KEY_STRING(Tab),
    DEFINE_KEYBOARD_KEY_STRING(Q),
    DEFINE_KEYBOARD_KEY_STRING(W),
    DEFINE_KEYBOARD_KEY_STRING(E),
    DEFINE_KEYBOARD_KEY_STRING(R),
    DEFINE_KEYBOARD_KEY_STRING(T),
    DEFINE_KEYBOARD_KEY_STRING(Y),
    DEFINE_KEYBOARD_KEY_STRING(U),
    DEFINE_KEYBOARD_KEY_STRING(I),
    DEFINE_KEYBOARD_KEY_STRING(O),
    DEFINE_KEYBOARD_KEY_STRING(P),
    DEFINE_KEYBOARD_KEY_STRING(OpenSquareBracets),
    DEFINE_KEYBOARD_KEY_STRING(CloseSqruareBracets),
    DEFINE_KEYBOARD_KEY_STRING(Enter),
    DEFINE_KEYBOARD_KEY_STRING(LeftControl),
    DEFINE_KEYBOARD_KEY_STRING(A),
    DEFINE_KEYBOARD_KEY_STRING(S),
    DEFINE_KEYBOARD_KEY_STRING(D),
    DEFINE_KEYBOARD_KEY_STRING(F),
    DEFINE_KEYBOARD_KEY_STRING(G),
    DEFINE_KEYBOARD_KEY_STRING(H),
    DEFINE_KEYBOARD_KEY_STRING(J),
    DEFINE_KEYBOARD_KEY_STRING(K),
    DEFINE_KEYBOARD_KEY_STRING(L),
    DEFINE_KEYBOARD_KEY_STRING(Semicolon),
    DEFINE_KEYBOARD_KEY_STRING(SingleQuote),
    DEFINE_KEYBOARD_KEY_STRING(BackTick),
    DEFINE_KEYBOARD_KEY_STRING(LeftShift),
    DEFINE_KEYBOARD_KEY_STRING(BackSlash),
    DEFINE_KEYBOARD_KEY_STRING(Z),
    DEFINE_KEYBOARD_KEY_STRING(X),
    DEFINE_KEYBOARD_KEY_STRING(C),
    DEFINE_KEYBOARD_KEY_STRING(V),
    DEFINE_KEYBOARD_KEY_STRING(B),
    DEFINE_KEYBOARD_KEY_STRING(N),
    DEFINE_KEYBOARD_KEY_STRING(M),
    DEFINE_KEYBOARD_KEY_STRING(Comma),
    DEFINE_KEYBOARD_KEY_STRING(Dot),
    DEFINE_KEYBOARD_KEY_STRING(Slash),
    DEFINE_KEYBOARD_KEY_STRING(RightShift),
    DEFINE_KEYBOARD_KEY_STRING(KeypadStar),
    DEFINE_KEYBOARD_KEY_STRING(LeftAlt),
    DEFINE_KEYBOARD_KEY_STRING(Space),
    DEFINE_KEYBOARD_KEY_STRING(CapsLock),
    DEFINE_KEYBOARD_KEY_STRING(F1),
    DEFINE_KEYBOARD_KEY_STRING(F2),
    DEFINE_KEYBOARD_KEY_STRING(F3),
    DEFINE_KEYBOARD_KEY_STRING(F4),
    DEFINE_KEYBOARD_KEY_STRING(F5),
    DEFINE_KEYBOARD_KEY_STRING(F6),
    DEFINE_KEYBOARD_KEY_STRING(F7),
    DEFINE_KEYBOARD_KEY_STRING(F8),
    DEFINE_KEYBOARD_KEY_STRING(F9),
    DEFINE_KEYBOARD_KEY_STRING(F10),
    DEFINE_KEYBOARD_KEY_STRING(NumLock),
    DEFINE_KEYBOARD_KEY_STRING(ScrollLock),
    DEFINE_KEYBOARD_KEY_STRING(Keypad7),
    DEFINE_KEYBOARD_KEY_STRING(Keypad8),
    DEFINE_KEYBOARD_KEY_STRING(Keypad9),
    DEFINE_KEYBOARD_KEY_STRING(KeypadMinus),
    DEFINE_KEYBOARD_KEY_STRING(Keypad4),
    DEFINE_KEYBOARD_KEY_STRING(Keypad5),
    DEFINE_KEYBOARD_KEY_STRING(Keypad6),
    DEFINE_KEYBOARD_KEY_STRING(KeypadPlus),
    DEFINE_KEYBOARD_KEY_STRING(Keypad1),
    DEFINE_KEYBOARD_KEY_STRING(Keypad2),
    DEFINE_KEYBOARD_KEY_STRING(Keypad3),
    DEFINE_KEYBOARD_KEY_STRING(Keypad0),
    DEFINE_KEYBOARD_KEY_STRING(KeypadDot),
    DEFINE_KEYBOARD_KEY_STRING(F11),
    DEFINE_KEYBOARD_KEY_STRING(F12),
};

keyPressedHandler handlers[KEY_PRESSED_HANDLERS_COUNT] = { 0 };

uint8_t keysPressed[(KeyboardKey_F12 + 1) / 8] = { 0 };

uint8_t getKeyboardBufferedScancode(void)
{
    return portReadByte(0x60);
}

void keyboardInterrupt(Registers regs)
{
    (void)regs;

    uint8_t bufferedScancode = getKeyboardBufferedScancode();
    uint8_t bufferedKeycode = (bufferedScancode & ~(1 << 7));

    if (bufferedScancode >> 7)
    {
        keysPressed[bufferedKeycode / 8] &= ~(1 << (bufferedKeycode % 8));
    }
    else
    {
        keysPressed[bufferedKeycode / 8] |= (1 << (bufferedKeycode % 8));

        for (uint32_t i = 0; i < KEY_PRESSED_HANDLERS_COUNT; i++)
        {
            if (handlers[i] != NULL)
            {
                handlers[i]((KeyboardKey)bufferedKeycode);
            }
        }
    }

    DEBUG_PRINTF("%cs toggled!", keyStrings[bufferedKeycode]);
}

void keyboardInit(void)
{
    interruptAddHandler(0x21, keyboardInterrupt);
}

bool keyboardIsAnyKeyPressed(void)
{
    for (int i = 0; i < (KeyboardKey_F12 + 1) / 8; i++)
    {
        if (keysPressed[i]) return true;
    }

    return false;
}

bool keyboardIsKeyPressed(KeyboardKey key)
{
    if (key > KeyboardKey_F12) return false;
    if (key <= KeyboardKey_None) return false;

    return (keysPressed[key / 8] >> (key % 8)) & 1;
}

void keyboardAddKeyPressedHandler(keyPressedHandler handler)
{
    for (int i = 0; i < KEY_PRESSED_HANDLERS_COUNT; i++)
    {
        if (handlers[i] == NULL)
        {
            handlers[i] = handler;
            break;
        }
    }
}
