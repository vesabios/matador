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

enum GameState {
    NormalState,
    EditState,
    PaintState,
    InspectState
};


enum InteractionType {
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





struct Pixel {
public:
    BYTE fg;
    BYTE bg;
    BYTE c;
    Pixel() : fg(0), bg(0), c(0) {}
};




static unsigned char makeColor(unsigned char r, unsigned char g, unsigned char b) {
    return  ((MIN(6,r) *36)  + ((MIN(6,g)*6)) + (MIN(6,b)));
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
    return std::max(lower, std::min(n, upper));
}


#endif /* Defines_h */
