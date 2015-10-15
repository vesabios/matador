//
//  Paint.cpp
//  matador
//
//  Created by vesabios on 10/8/15.
//
//

#include "ofMain.h"
#include "Paint.h"
#include "Map.h"
#include "Core.h"
#include "Console.h"
#include "Defines.h"

Paint paint;


void Paint::toggle() {
    if (visible) {
        hide();
    } else {
        show();
    }
}

void Paint::show() {
    visible = true;
    core->state = PaintState;
    load();
}


void Paint::hide() {
    visible = false;
    core->state = NormalState;
    save();
}

void Paint::update(const float delaTime) {
    
    if (visible) {
        menuPos += (62.0f - menuPos) * 0.2f;
    } else {
        menuPos += (85.0f - menuPos) * 0.2f;
    }
    

    
}


void Paint::keyPressed(int key) {
    
    if (key=='1') {
        brush = 1;
    } else if (key=='2') {
        brush = 2;
    } else if (key=='3') {
        brush = 3;
    } else if (key=='4') {
        brush = 4;
    } else if (key=='5') {
        brush = 5;
    } else if (key=='6') {
        brush = 6;
    } else if (key=='7') {
        brush = 7;
    } else if (key=='8') {
        brush = 8;
    } else if (key=='i') {
        
        int x = core->mousePos.x;
        int y = core->mousePos.y;

        
        fg = pixels[(x+y*80)*4];
        bg = pixels[(x+y*80)*4+1];
        c = pixels[(x+y*80)*4+2];
        
    } else if (key=='o') {
        ortho = true;
        
    }
    
}

void Paint::keyReleased(int key) {
    if (key=='o') {
        ortho = false;
        orthoAxis = AXIS_NONE;
    }
}




void Paint::mouseDragged(int inx, int iny, int button) {
    
    if (inx < (int)menuPos) {
        
        if (ortho) {
            printf("ortho\n");
            
            if (orthoAxis==AXIS_NONE) {

                if ((int)startMousePoint.x!=inx) {
                    orthoAxis = AXIS_HORIZONTAL;
                } else if ((int)startMousePoint.y!=iny) {
                    orthoAxis = AXIS_VERTICAL;
                }
                
                brushPixels(ofVec2i(inx, iny));

            } else if (orthoAxis == AXIS_HORIZONTAL) {
                printf("H\n");
                
                
                brushPixels(ofVec2i(inx, startMousePoint.y));
            } else if (orthoAxis == AXIS_VERTICAL) {
                
                printf("V\n");
                
                brushPixels(ofVec2i(startMousePoint.x, iny));
            }
            
        } else {
            printf("no ortho\n");

            brushPixels(ofVec2i(inx, iny));
            
        }
        
    }
    
}

void Paint::mousePressed(int inx, int iny, int button) {
    
    int x = (inx - (int)menuPos) -1;
    int y = (iny - 5);
    
    if ((x>=0) && (y>=0) && (x<16) && (y<16)) {
        c = x+y*16;
    }
    
    char r,g,b;
    
    y = (iny - 22);
    
    if ((y>=0) && (x>=0) && (y<18)) {
        if (x<12) {
            r = x % 6;
            g = y % 6;
            
            b = int(y / 6) * 2;
            b += int(x / 6);
            
            if (button==0) {
                fg = makeColor(r,g,b);
            } else if (button==2) {
                bg = makeColor(r,g,b);
            }
        }
    }
    
    
    if (inx < (int)menuPos) {
        brushPixels(ofVec2i(inx, iny));
    }
    
    startMousePoint = ofPoint(inx, iny);
    
    
}


void Paint::brushPixels(ofVec2i p) {
    
    switch (brush) {
        case 0:
        case 1:
        {
            placePixel(p);
            break;
        }
        case 2:
        {
            placePixel(p - ofVec2i(1,0));
            placePixel(p + ofVec2i(1,0));
            placePixel(p - ofVec2i(0,1));
            placePixel(p + ofVec2i(0,1));
            placePixel(p);
          
            break;
        }
        case 3: {
            
            for (int i=0; i<2; i++) {
                ofVec2i offset = ofVec2i(ofRandom(12)-6, ofRandom(12-6));
                placePixel(p + offset);
            }
            
            break;
        }
        case 4: {
            for (int i=0; i<5; i++) {
                ofVec2i offset = ofVec2i(ofRandom(6)-3, ofRandom(6-3));
                placePixel(p + offset);
            }
            break;
        }
        case 5: {
            for (int i=0; i<15; i++) {
                ofVec2i offset = ofVec2i(ofRandom(12)-6, ofRandom(12-6));
                placePixel(p + offset);
            }
            break;
        }
    }
    

    
}

void Paint::placePixel(ofVec2i p) {
    
    int x = p.x;
    int y = p.y;
    
    if (x>=0 && y>=0 && x<80 && y<50) {
        int idx = ((y*80)+x)*4;
        
        pixels[idx] = fg;
        
        // background color
        pixels[idx+1] = bg;
        


        
        // character
        pixels[idx+2] = c;
        
        // not used
        pixels[idx+3] = 0;
        
    }
    
    
}


void Paint::save() {
    
    
    ofFile f;

    f.open("art.bin", ofFile::WriteOnly, true);
    
    for(int i = 0; i < 80*50*4; i++) {
        f.write((const char *)&pixels[i], sizeof(BYTE));
    }
    
    f.close();
    
}


void Paint::load() {
  
    ofFile f;

    f.open("art.bin", ofFile::ReadOnly, true);
    if (f.exists()) {
        ofBuffer buffer(f);
        for (int i = 0; i < 80*50*4; i++) {
            pixels[i] = (BYTE)buffer.getBinaryBuffer()[i];
        }
    }
    
    f.close();
    
}


void Paint::render() {
    
    for (int i=0; i<80*50*4; i++) {
        console.pixels[i] = pixels[i];
    }
    

    
    
    if (menuPos<80) {
        ofRectangle r = ofRectangle((int)menuPos,2,28,40);
        
        BYTE lbg = makeColor(0,1,3);
        BYTE lfg = makeColor(5,5,5);
        
        console.drawBox(r, lbg);
        console.writeString((int)menuPos+1, 3, "Paint", makeColor(5,0,5), lbg);
        console.writeString((int)menuPos+7, 3, ofToString((int)c), makeColor(5,5,0), lbg);

        BYTE cc = 0;
        for (int y=0; y<16; y++) {
            for (int x=0; x<16; x++) {
                int sx = (int)menuPos+1+x;
                int sy = 5+y;
                if (c==cc) {
                    console.setPixel(sx, sy, fg, bg, cc++);
                } else {
                    console.setPixel(sx, sy, makeColor(3,3,3), 0, cc++);
                }
            }
        }
        
        for (int z=0; z<6; z++) {
            for (int y=0; y<6; y++) {
                for (int x=0; x<6; x++) {
                    int sx = (int)menuPos+1+x + ((z%2)*6);
                    int sy = 22+y + (int(z/2)*6);
                    
                    BYTE bgc = makeColor(x,y,z);
                    BYTE cc = toascii(' ');
                    if (bgc == bg) {
                        cc = toascii('B');
                        if (bgc == fg) {
                            cc == toascii('X');
                        }
                    } else if (bgc == fg) {
                        cc = toascii('F');
                    }
                    
                    console.setPixel(sx, sy, makeColor(5,5,5), bgc, cc);
                }
            }
        }
        

        
    }
    
}