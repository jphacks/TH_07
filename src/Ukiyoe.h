//
//  Ukiyoe.h
//  ukiyoe_v0_6
//
//  Created by Shogo Tabata on 2015/11/28.
//
//

#ifndef __ukiyoe_v0_6__Ukiyoe__
#define __ukiyoe_v0_6__Ukiyoe__

#include <iostream>

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofMain.h"

#define CSVNUM 2
#define WIDTH 640
#define HEIGHT 480
#define DIST_N 500
#define DIST_F 1500
#define LEVEL 4

struct color{
    unsigned char r,g,b;
};


class Ukiyoe{
    ofxKinect kinect;
    ofImage img;
    ofxCvColorImage colimg;
    ofxCvGrayscaleImage gryimg;
    ofxCvGrayscaleImage edgimg;
    vector<color> c[2];
    unsigned char border[LEVEL];
    unsigned char post[LEVEL];
    
    void Posterize_trans(unsigned char & num);
public:
    unsigned char * Return();

    void loadcsv();
    int count_depth();
    void set();
    void camera_update();
    void filter_update();
    void dipth_update_pack();
    void ukiyoe_update_pack();
    void depth_update();
    void drawcamera(float y);
    void drawukiyoe(float y);
    void setcolor();
    void ColorDecider(unsigned char * ary,int cm);
    void Gaussian(unsigned char * ary);
    void Posterize(unsigned char * ary);
    void Mirror_alpha(unsigned char * ary);
    void Gaussian_alpha(unsigned char * ary);
};



#endif /* defined(__ukiyoe_v0_6__Ukiyoe__) */
