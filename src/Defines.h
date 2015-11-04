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

#define EQUIPMENT_SLOTS 9

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

/*
int XP_TABLE[5][20] =
   {{300,   600,    900,    1350,   1800,   2700,   3600,   5400,   7200,   10800,  -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1},
    {300,   600,    800,    1200,   1600,   2400,   3200,   4800,   6400,   9600,   12800,  -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1},
    {300,   500,    750,    1000,   1500,   2250,   3000,   4500,   6000,   9000,   12000,  18000,  -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1},
    {300,   450,    600,    900,    1200,   1800,   2700,   3600,   5400,   7200,   10800,  14400,  21600,  -1,     -1,     -1,     -1,     -1,     -1,     -1},
    {263,   350,    525,    700,    1050,   1400,   2100,   3150,   4200,   6300,   8400,   12600,  16800,  25200,  -1,     -1,     -1,     -1,     -1,     -1},
    {200,   300,    400,    600,    800,    1200,   1600,   2400,   3600,   4800,   7200,   9600,   14400,  19200,  28800,  -1,     -1,     -1,     -1,     -1},
    {-1,    225,    338,    450,    675,    900,    1350,   1800,   2700,   4050,   5400,   8100,   10800,  16200,  21600,  32400,  -1,     -1,     -1,     -1},
    {-1,    -1,     250,    375,    500,    750,    1000,   1500,   2000,   3000,   4500,   6000,   9000,   12000,  18000,  24000,  36000,  -1,     -1,     -1},
    {-1,    -1,     -1,     275,    413,    550,    825,    1100,   1650,   2200,   3300,   4950,
 */


static int mEven(float x)
{
    int r = 2.0f * (x/2.0f);
    if (x < r) r += -2;
    else if (x > r) r += 2;
    return r;
}

static int experience(const int &pc_level, const float &monster_cr)
{
    int x = pc_level;
    float y = monster_cr;
   
    int xp = 0;
    
    if (x < 3) x = 3;
    if ((x <= 6) && (y <= 1)) xp = 300 * y;
    else if (y < 1) xp = 0;
    
    // This formula looks nice, but 3.5 doesn't follow a smooth formula like 3.0 did.
    else xp = 6.25f * x * ( pow(2.0f , (float)(mEven(7- (x-y) ) /2) ) * ( 11-(x-y) - mEven(7-(x-y))) );
    
    // Below catches places where the formula fails for 3.5.
    if ((y == 4) || (y == 6) || (y == 8) || (y == 10) || (y == 12) ||
        (y == 14) ||(y == 16) ||(y == 18) ||(y == 20))
    {
        if (x <= 3)                     xp = 1350 * pow(2.0f,float(y-4.0f)/2.0f);
        else if (x == 5 && y >= 6)      xp = 2250 * pow(2.0f,float(y-6.0f)/2.0f);
        else if (x == 7 && y >= 8)      xp = 3150 * pow(2.0f,float(y-8.0f)/2.0f);
        else if (x == 9 && y >= 10)     xp = 4050 * pow(2.0f,float(y-10.0f)/2.0f);
        else if (x == 11 && y >= 12)    xp = 4950 * pow(2.0f,float(y-12.0f)/2.0f);
        else if (x == 13 && y >= 14)    xp = 5850 * pow(2.0f,float(y-14.0f)/2.0f);
        else if (x == 15 && y >= 16)    xp = 6750 * pow(2.0f,float(y-16.0f)/2.0f);
        else if (x == 17 && y >= 18)    xp = 7650 * pow(2.0f,float(y-18.0f)/2.0f);
        else if (x == 19 && y >= 20)    xp = 8550 * pow(2.0f,float(y-20.0f)/2.0f);
    }
    if ((y == 7) || (y == 9) || (y == 11) || (y == 13) || (y == 15) ||
        (y == 17) ||(y == 19))
    {
        if (x == 6)             xp = 2700 * pow(2.0f,(y-7.0f)/2.0f);
        if (x == 8 && y >= 9)   xp = 3600 * pow(2.0f,(y-9.0f)/2.0f);
        if (x == 10 && y >= 11) xp = 4500 * pow(2.0f,(y-11.0f)/2.0f);
        if (x == 12 && y >= 13) xp = 5400 * pow(2.0f,(y-13.0f)/2.0f);
        if (x == 14 && y >= 15) xp = 6300 * pow(2.0f,(y-15.0f)/2.0f);
        if (x == 16 && y >= 17) xp = 7200 * pow(2.0f,(y-17.0f)/2.0f);
        if (x == 18 && y >= 19) xp = 8100 * pow(2.0f,(y-19.0f)/2.0f);
    }
    
    if (y > 20) xp = 2 * experience(x, y-2.0f);
    // recursion should end this in short order.
    // This method is clean, and ensures any errors in the above
    // formulas for 3.5 are accounted for.
    
    // Finally we correct for out of bounds entries, doing this last to cut space on the
    // above formulas.
    if (x - y > 7) xp = 0;
    else if (y - x > 7) xp = 0;
    
    return xp;
}







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
