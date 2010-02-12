//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (c) 2006 Phillip Castaneda (pjcast -- www.wreckedgames.com)
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com 
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_INPUT_MANAGER_KEY_CODES_HPP_INCLUDED
#define ZEN_ENGINE_INPUT_MANAGER_KEY_CODES_HPP_INCLUDED

#include "Configuration.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

// Key codes derived from OIS library
enum KeyCode_enum
{
    KC_UNASSIGNED  = 0x00,                                                     // ""
    KC_ESCAPE      = 0x01,                                                     // "esc"
    KC_1           = 0x02,                                                     // "1"
    KC_2           = 0x03,                                                     // "2"
    KC_3           = 0x04,                                                     // "3"
    KC_4           = 0x05,                                                     // "4"
    KC_5           = 0x06,                                                     // "5"
    KC_6           = 0x07,                                                     // "6"
    KC_7           = 0x08,                                                     // "7"
    KC_8           = 0x09,                                                     // "8"
    KC_9           = 0x0A,                                                     // "9"
    KC_0           = 0x0B,                                                     // "0"
    KC_MINUS       = 0x0C,    // - on main keyboard                            // "minus"
    KC_EQUALS      = 0x0D,                                                     // "="
    KC_BACK        = 0x0E,    // backspace                                     // "bsp"
    KC_TAB         = 0x0F,                                                     // "tab"
    KC_Q           = 0x10,                                                     // "q"
    KC_W           = 0x11,                                                     // "w"
    KC_E           = 0x12,                                                     // "e"
    KC_R           = 0x13,                                                     // "r"
    KC_T           = 0x14,                                                     // "t"
    KC_Y           = 0x15,                                                     // "y"
    KC_U           = 0x16,                                                     // "u"
    KC_I           = 0x17,                                                     // "i"
    KC_O           = 0x18,                                                     // "o"
    KC_P           = 0x19,                                                     // "p"
    KC_LBRACKET    = 0x1A,                                                     // "["
    KC_RBRACKET    = 0x1B,                                                     // "]"
    KC_RETURN      = 0x1C,    // Enter on main keyboard                        // "enter"
    KC_LCONTROL    = 0x1D,                                                     // "lctrl"
    KC_A           = 0x1E,                                                     // "a"
    KC_S           = 0x1F,                                                     // "s"
    KC_D           = 0x20,                                                     // "d"
    KC_F           = 0x21,                                                     // "f"
    KC_G           = 0x22,                                                     // "g"
    KC_H           = 0x23,                                                     // "h"
    KC_J           = 0x24,                                                     // "j"
    KC_K           = 0x25,                                                     // "k"
    KC_L           = 0x26,                                                     // "l"
    KC_SEMICOLON   = 0x27,                                                     // ";"
    KC_APOSTROPHE  = 0x28,                                                     // "'"
    KC_GRAVE       = 0x29,    // accent                                        // "`"
    KC_LSHIFT      = 0x2A,                                                     // "lshift"
    KC_BACKSLASH   = 0x2B,                                                     // "\"
    KC_Z           = 0x2C,                                                     // "z"
    KC_X           = 0x2D,                                                     // "x"
    KC_C           = 0x2E,                                                     // "c"
    KC_V           = 0x2F,                                                     // "v"
    KC_B           = 0x30,                                                     // "b"
    KC_N           = 0x31,                                                     // "n"
    KC_M           = 0x32,                                                     // "m"
    KC_COMMA       = 0x33,                                                     // ","
    KC_PERIOD      = 0x34,    // . on main keyboard                            // "."
    KC_SLASH       = 0x35,    // / on main keyboard                            // "/"
    KC_RSHIFT      = 0x36,                                                     // "rshift"
    KC_MULTIPLY    = 0x37,    // * on numeric keypad                           // "nummult"
    KC_LMENU       = 0x38,    // left Alt                                      // "lalt"
    KC_SPACE       = 0x39,                                                     // "space"
    KC_CAPITAL     = 0x3A,                                                     // "capslock"
    KC_F1          = 0x3B,                                                     // "f1"
    KC_F2          = 0x3C,                                                     // "f2"
    KC_F3          = 0x3D,                                                     // "f3"
    KC_F4          = 0x3E,                                                     // "f4"
    KC_F5          = 0x3F,                                                     // "f5"
    KC_F6          = 0x40,                                                     // "f6"
    KC_F7          = 0x41,                                                     // "f7"
    KC_F8          = 0x42,                                                     // "f8"
    KC_F9          = 0x43,                                                     // "f9"
    KC_F10         = 0x44,                                                     // "f10"
    KC_NUMLOCK     = 0x45,                                                     // "numlock"
    KC_SCROLL      = 0x46,    // Scroll Lock                                   // "scrolllock"
    KC_NUMPAD7     = 0x47,                                                     // "num7"
    KC_NUMPAD8     = 0x48,                                                     // "num8"
    KC_NUMPAD9     = 0x49,                                                     // "num9"
    KC_SUBTRACT    = 0x4A,    // - on numeric keypad                           // "numminus"
    KC_NUMPAD4     = 0x4B,                                                     // "num4"
    KC_NUMPAD5     = 0x4C,                                                     // "num5"
    KC_NUMPAD6     = 0x4D,                                                     // "num6"
    KC_ADD         = 0x4E,    // + on numeric keypad                           // "numplus"
    KC_NUMPAD1     = 0x4F,                                                     // "num1"
    KC_NUMPAD2     = 0x50,                                                     // "num2"
    KC_NUMPAD3     = 0x51,                                                     // "num3"
    KC_NUMPAD0     = 0x52,                                                     // "num0"
    KC_DECIMAL     = 0x53,    // . on numeric keypad                           // "num."
    KC_OEM_102     = 0x56,    // < > | on UK/Germany keyboards                 // "???"
    KC_F11         = 0x57,                                                     // "f11"
    KC_F12         = 0x58,                                                     // "f12"
    KC_F13         = 0x64,    //                     (NEC PC98)                // "f13"
    KC_F14         = 0x65,    //                     (NEC PC98)                // "f14"
    KC_F15         = 0x66,    //                     (NEC PC98)                // "f15"
    KC_KANA        = 0x70,    // (Japanese keyboard)                           // "???"
    KC_ABNT_C1     = 0x73,    // / ? on Portugese (Brazilian) keyboards        // "???"
    KC_CONVERT     = 0x79,    // (Japanese keyboard)                           // "???"
    KC_NOCONVERT   = 0x7B,    // (Japanese keyboard)                           // "???"
    KC_YEN         = 0x7D,    // (Japanese keyboard)                           // "???"
    KC_ABNT_C2     = 0x7E,    // Numpad . on Portugese (Brazilian) keyboards   // "???"
    KC_NUMPADEQUALS= 0x8D,    // = on numeric keypad (NEC PC98)                // "num="
    KC_PREVTRACK   = 0x90,    // Previous Track (KC_CIRCUMFLEX on Japanese keyboard) // "???"
    KC_AT          = 0x91,    //                     (NEC PC98)                // "???"
    KC_COLON       = 0x92,    //                     (NEC PC98)                // "???"
    KC_UNDERLINE   = 0x93,    //                     (NEC PC98)                // "???"
    KC_KANJI       = 0x94,    // (Japanese keyboard)                           // "???"
    KC_STOP        = 0x95,    //                     (NEC PC98)                // "???"
    KC_AX          = 0x96,    //                     (Japan AX)                // "???"
    KC_UNLABELED   = 0x97,    //                        (J3100)                // "???"
    KC_NEXTTRACK   = 0x99,    // Next Track                                    // "???"
    KC_NUMPADENTER = 0x9C,    // Enter on numeric keypad                       // "numenter"
    KC_RCONTROL    = 0x9D,                                                     // "rctrl"
    KC_MUTE        = 0xA0,    // Mute                                          // "???"
    KC_CALCULATOR  = 0xA1,    // Calculator                                    // "???"
    KC_PLAYPAUSE   = 0xA2,    // Play / Pause                                  // "???"
    KC_MEDIASTOP   = 0xA4,    // Media Stop                                    // "???"
    KC_VOLUMEDOWN  = 0xAE,    // Volume -                                      // "???"
    KC_VOLUMEUP    = 0xB0,    // Volume +                                      // "???"
    KC_WEBHOME     = 0xB2,    // Web home                                      // "???"
    KC_NUMPADCOMMA = 0xB3,    // , on numeric keypad (NEC PC98)                // "???"
    KC_DIVIDE      = 0xB5,    // / on numeric keypad                           // "numdiv"
    KC_SYSRQ       = 0xB7,                                                     // "???"
    KC_RMENU       = 0xB8,    // right Alt                                     // "ralt"
    KC_PAUSE       = 0xC5,    // Pause                                         // "???"
    KC_HOME        = 0xC7,    // Home on arrow keypad                          // "home"
    KC_UP          = 0xC8,    // UpArrow on arrow keypad                       // "up"
    KC_PGUP        = 0xC9,    // PgUp on arrow keypad                          // "pgup"
    KC_LEFT        = 0xCB,    // LeftArrow on arrow keypad                     // "left"
    KC_RIGHT       = 0xCD,    // RightArrow on arrow keypad                    // "right"
    KC_END         = 0xCF,    // End on arrow keypad                           // "end"
    KC_DOWN        = 0xD0,    // DownArrow on arrow keypad                     // "down"
    KC_PGDOWN      = 0xD1,    // PgDn on arrow keypad                          // "pgdn"
    KC_INSERT      = 0xD2,    // Insert on arrow keypad                        // "ins"
    KC_DELETE      = 0xD3,    // Delete on arrow keypad                        // "del"
    KC_LWIN        = 0xDB,    // Left Windows key                              // "lwin
    KC_RWIN        = 0xDC,    // Right Windows key                             // "rwin"
    KC_APPS        = 0xDD,    // AppMenu key                                   // "???"
    KC_POWER       = 0xDE,    // System Power                                  // "???"
    KC_SLEEP       = 0xDF,    // System Sleep                                  // "???"
    KC_WAKE        = 0xE3,    // System Wake                                   // "???"
    KC_WEBSEARCH   = 0xE5,    // Web Search                                    // "???"
    KC_WEBFAVORITES= 0xE6,    // Web Favorites                                 // "???"
    KC_WEBREFRESH  = 0xE7,    // Web Refresh                                   // "???"
    KC_WEBSTOP     = 0xE8,    // Web Stop                                      // "???"
    KC_WEBFORWARD  = 0xE9,    // Web Forward                                   // "???"
    KC_WEBBACK     = 0xEA,    // Web Back                                      // "???"
    KC_MYCOMPUTER  = 0xEB,    // My Computer                                   // "???"
    KC_MAIL        = 0xEC,    // Mail                                          // "???"
    KC_MEDIASELECT = 0xED     // Media Select                                  // "???"
};  // enum KeyCode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_INPUT_MANAGER_KEY_CODES_HPP_INCLUDED
