//
//  Dialog.cpp
//  matador
//
//  Created by vesabios on 10/22/15.
//
//

#include "Dialog.hpp"


Dialog dialog;


void Dialog::render() {
    
    if (active) {
        
        int MARGIN = 3;
        
        int maxLen = 0;
        for (int i=0; i<lines.size(); i++) {
            maxLen = MAX(maxLen, lines[i].length());
        }
        
        int w = maxLen+MARGIN*2;
        int h = lines.size()+MARGIN*2;
        
        
        
        ofRectangle r = ofRectangle(center.x - w/2, center.y - (h+2),w,h);
        
        Pixel p;
        p.bg = makeColor(2,1,0);
        
        
        for (int y=0; y<r.height; y++) {
            for (int x=0; x<r.width; x++) {
                
                p.c = toascii(' ');
                
                if (y==0) {
                    if (x==0) {
                        p.c = 3;
                        p.fg = makeColor(4,2,2);
                    } else if (x==1) {
                        p.c = 3;
                        p.fg = makeColor(4,2,1);
                    } else if (x==r.width-2) {
                        p.c = 1;
                        p.fg = makeColor(4,2,1);
                    } else if (x==r.width-1) {
                        p.c = 1;
                        p.fg = makeColor(4,2,2);
                    } else {
                        p.c = 12;
                        p.fg = makeColor(4,2,1);
                    }
                } else if (y==1) {
                    if (x==0) {
                        p.c = 3;
                        p.fg = makeColor(4,2,1);
                    } else if (x==1) {
                        p.c = 2;
                        p.fg = makeColor(5,3,1);
                    } else if (x==r.width-2) {
                        p.c = 4;
                        p.fg = makeColor(5,3,1);
                    } else if (x==r.width-1) {
                        p.c = 1;
                        p.fg = makeColor(4,2,1);
                    } else {
                        p.c = toascii(' ');
                        p.fg = makeColor(4,2,1);
                    }
                } else if (y==r.height-2) {
                    if (x==0) {
                        p.c = 4;
                        p.fg = makeColor(4,2,1);
                    } else if (x==1) {
                        p.c = 1;
                        p.fg = makeColor(5,3,1);
                    } else if (x==r.width-2) {
                        p.c = 3;
                        p.fg = makeColor(5,3,1);
                    } else if (x==r.width-1) {
                        p.c = 2;
                        p.fg = makeColor(4,2,1);
                    } else {
                        p.c = toascii(' ');
                        p.fg = makeColor(4,2,1);
                    }
                } else if (y==r.height-1) {
                    if (x==0) {
                        p.c = 4;
                        p.fg = makeColor(4,2,2);
                    } else if (x==1) {
                        p.c = 4;
                        p.fg = makeColor(4,2,1);
                    } else if (x==r.width-2) {
                        p.c = 2;
                        p.fg = makeColor(4,2,1);
                    } else if (x==r.width-1) {
                        p.c = 2;
                        p.fg = makeColor(4,2,2);
                    } else {
                        p.c = 10;
                        p.fg = makeColor(4,2,1);
                    }
                } else {
                    if (x==0) {
                        p.c = 11;
                        p.fg = makeColor(4,2,1);
                    } else if (x==r.width-1) {
                        p.c = 9;
                        p.fg = makeColor(4,2,1);
                    } else {
                        p.c = toascii(' ');
                        p.fg = makeColor(4,2,1);
                    }
                    
                }
                
                console.setPixel(r.getMinX()+x, r.getMinY()+y, p);
                
                
            }
            
        }
        
        
        // 3: 4,2,2  // TL
        // 3: 4,2,1
        // 12: 4,2,1   // top span
        // 1: 4,2,1
        // 1: 4,2,2
        
        
        // 3: 4,2,1
        // 2: 5,3,1
        // -- span
        // 4: 5,3,1
        // 1: 4,2,1
        
        
        // 11: 4,2,1
        // -- span
        // 9: 4,2,1
        
        
        // 4: 4,2,1
        // 1: 5,3,1
        // -- span
        // 3: 5,3,1
        // 2: 4,2,1
        
        // 4: 4,2,2
        // 4: 4,2,1
        // 10: 4,2,1 -- span
        // 2: 4,2,1
        // 2: 4,2,2
        
        
        for (int i=0; i<lines.size(); i++) {
            console.writeString(r.x+MARGIN, r.y+MARGIN+i, makeColor(5,4,3), lines[i]);
        }

    
    }

}
