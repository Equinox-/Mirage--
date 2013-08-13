#ifndef INCLUDED_INPUTTYPES_H
#define INCLUDED_INPUTTYPES_H

//====================================================================================================
// Filename:	InputTypes.h
// Created by:	Peter Chan
// Description:	Header containing a bunch on constants for inputs.
//====================================================================================================

//====================================================================================================
// Constants
//====================================================================================================

namespace Keys {
// Keyboard roll 1
const int ESCAPE = 9;
const int F1 = 168;
const int F2 = 169;
const int F3 = 170;
const int F4 = 171;
const int F5 = 172;
const int F6 = 173;
const int F7 = 174;
const int F8 = 175;
const int F9 = 176;
const int F10 = 177;
const int F11 = 178;
const int F12 = 179;

// Keyboard roll 2
const int GRAVE = '`';
const int ONE = '1';
const int TWO = '2';
const int THREE = '3';
const int FOUR = '4';
const int FIVE = '5';
const int SIX = '6';
const int SEVEN = '7';
const int EIGHT = '8';
const int NINE = '9';
const int ZERO = '0';
const int MINUS = '-';
const int EQUALS = '=';
const int BACKSPACE = '\b';

// Keyboard roll 3
const int TAB ='\t';
const int Q = 'q';
const int W = 'w';
const int E = 'e';
const int R = 'r';
const int T = 't';
const int Y = 'y';
const int U = 'u';
const int I = 'i';
const int O = 'o';
const int P = 'p';
const int LBRACKET = '[';
const int RBRACKET = ']';
const int BACKSLASH = '\\';

// Keyboard roll 4
const int A = 'a';
const int S = 's';
const int D = 'd';
const int F = 'f';
const int G = 'g';
const int H = 'h';
const int J = 'j';
const int K = 'k';
const int L = 'l';
const int SEMICOLON = ';';
const int APOSTROPHE = '\'';
const int ENTER = '\n';

// Keyboard roll 5
const int Z = 'z';
const int X = 'x';
const int C = 'c';
const int V = 'v';
const int B = 'b';
const int N = 'n';
const int M = 'm';
const int COMMA = ',';
const int PERIOD = '.';
const int SLASH = '/';

// Lock keys
const int CAPSLOCK = 66;
const int NUMLOCK = 77;
const int SCROLLLOCK = 78;

// Numpad
const int NUMPAD1 = 87;
const int NUMPAD2 = 88;
const int NUMPAD3 = 89;
const int NUMPAD4 = 83;
const int NUMPAD5 = 84;
const int NUMPAD6 = 85;
const int NUMPAD7 = 79;
const int NUMPAD8 = 80;
const int NUMPAD9 = 81;
const int NUMPAD0 = 90;
const int NUM_ADD = 86;
const int NUM_SUB = 82;
const int NUM_MUL = 63;
const int NUM_DIV = 106;
const int NUM_ENTER = 0x9C;
const int NUM_DECIMAL = 91;

// Navigation keys
const int INS = 118;
const int DEL = 119;
const int HOME = 110;
const int END = 115;
const int PGUP = 112;
const int PGDN = 117;

// Support keys
const int LSHIFT = 150;
const int RSHIFT = 162;
const int LCONTROL = 137;
const int RCONTROL = 105;
const int LALT = 164;
const int RALT = 108;
const int LWIN = 133;
const int RWIN = 135;
const int SPACE = ' ';

// Arrow keys
const int UP = 111;
const int DOWN = 116;
const int LEFT = 113;
const int RIGHT = 114;
}

//----------------------------------------------------------------------------------------------------

namespace Mouse {
const int LBUTTON = 1;
const int RBUTTON = 2;
const int MBUTTON = 4;
}

#endif // #ifndef INCLUDED_DXINPUT_H
