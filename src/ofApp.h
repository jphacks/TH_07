#pragma once
#include "ofMain.h"
#include "Ukiyoe.h"
#define BGNUM 2
class ofApp : public ofBaseApp{

	public:
    void title(float y);
    
		void setup();
		void update();
		void draw();
        void CreateImage();
    void snap();
    void CountDown();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        ofImage bg[BGNUM];
    ofImage kumo[2];
    ofImage sharaku;
    ofImage te;
    ofImage texture;
    ofImage finish;
    Ukiyoe ukiyoe;
    int mode;
    ofTrueTypeFont font;
    ofTrueTypeFont font2;
    ofSoundPlayer bgm,bgm2,cam,taiko,iyo,tsutsumi;
};
