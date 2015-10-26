//
//  Defines.h
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef Defines_h
#define Defines_h

#include "ofMain.h"
#include "ofVec2i.h"


typedef unsigned char BYTE;  // 1byte
typedef unsigned short WORD; // 2bytes
typedef unsigned long DWORD; // 4bytes

typedef int DEBT;



#define CHAR_BLANK 0
#define CHAR_PLAYER 64
#define CHAR_LEFT 123
#define CHAR_RIGHT 124
#define CHAR_DOWN 125
#define CHAR_UP 126
#define CHAR_HEART 152
#define CHAR_SPEECH 156
#define CHAR_TEXT 158
#define CHAR_PLANTS0 168
#define CHAR_PLANTS1 178
#define CHAR_PLANTS2 183
#define CHAR_PLANTS3 191
#define CHAR_EMPTYBOX 219
#define CHAR_PLANTS4 200
#define CHAR_DITHER0 240
#define CHAR_DITHER1 241
#define CHAR_DITHER2 242
#define CHAR_DITHER3 243


#define DEBT_TURN_THRESHOLD 30
#define TRAVERSE_BLOCKED 0
#define TRAVERSE_NORMAL 100
#define TRAVERSE_SLIPPERY 160


#define VOID_LOCATION 20000

#define OMIT_VALUE 10000

#define FEET_PER_TILE 5

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 50

#define TIME_TO_DEBT_SCALAR 300.0f

enum GameState {
    NORMAL_STATE,
    EQUIP_STATE,
    EDIT_STATE,
    PAINT_STATE,
    INSPECT_STATE
};


enum InteractionType {
    None,
    Stare,
    Read,
    Take,
    Use,
    Attack
};

enum Race {
    Human,
    Elf,
    Dwarf,
    Halfling
    
};

enum Axis {
    AXIS_NONE,
    AXIS_HORIZONTAL,
    AXIS_VERTICAL
};





struct Pixel {
public:
    BYTE fg;
    BYTE bg;
    BYTE c;
    BYTE a;
    Pixel(BYTE f, BYTE b, BYTE c, BYTE a) : fg(f), bg(b), c(c), a(a) {}

    Pixel() : fg(0), bg(0), c(0), a(255) {}
};




static unsigned char makeColor(unsigned char r, unsigned char g, unsigned char b) {
    return  (MAX(0,(MIN(5,r)) *36)  + (MAX(0,(MIN(5,g))*6)) + (MAX(0,MIN(5,b))));
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
    return std::max(lower, std::min(n, upper));
}


#endif /* Defines_h */
