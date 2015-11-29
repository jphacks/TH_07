#include "ofApp.h"
ofVec2f croud[2];
float flow = 0;
float flowacc=0;
int tim=0;
int countdown=5;
int countdowntime=0;
float movc=0;
float movs=0;
int pattern = 0;
//--------------------------------------------------------------
void ofApp::setup(){
    bg[0].loadImage("haikei-nami.png");
    bg[1].loadImage("haikei-nami.png");
    kumo[0].loadImage("leftkumo.png");
    kumo[1].loadImage("rightkumo.png");
    sharaku.loadImage("ukiyoe.png");
    te.loadImage("te.png");
    texture.loadImage("washi.jpg");
    ukiyoe.set();
    iyo.loadSound("iyou.mp3");
    cam.loadSound("camera.mp3");
    taiko.loadSound("wadaiko.mp3");
    tsutsumi.loadSound("kodutumi.wav");
    bgm.loadSound("bgm.mp3");
    bgm2.loadSound("bgm2.mp3");
    mode = 0;
    font.loadFont("Brush Strokes.ttf",182);
    font2.loadFont("衡山毛筆フォント草書.TTF",102);
    croud[0].x = -200;
    croud[0].y = 100;
    croud[1].x = ofGetWidth()-200;
    croud[1].y = 300;
    ofBackground(255);
    finish.allocate(WIDTH, HEIGHT,OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::snap(){
   ukiyoe.ukiyoe_update_pack();
}
void ofApp::update(){
    if(mode == 0){
        if(bgm.getIsPlaying()==false)bgm.play();
        countdown=7;
        countdowntime++;
    if(countdowntime>=100)ukiyoe.camera_update();
    tim++;
    if(tim>=720)tim=0;
    movc=cos(tim*PI/360);
    movs=sin(tim*PI/360);
    croud[0].x =-100 + movc*100;
    croud[1].x =ofGetWidth()-800 + movs*100;
        if(ukiyoe.count_depth()>50000 && countdowntime>=100){
            countdowntime=100;
            if(flowacc>-25)flowacc--;
            if(flow<=-1000){
                bgm.stop();
                if(bgm2.getIsPlaying()==false)bgm2.play();
                mode=1;
            }
        }
        else if(flow<0){
            if(flowacc<25)flowacc++;
        }
        
    flow+=flowacc;
        if(flow>0)flow=0;
        if(flow<-1000)flow=-1000;
        
    }
    
    if(mode == 0)ukiyoe.dipth_update_pack();
    if(mode == 1){
        countdowntime++;
        if(countdowntime>30){
            taiko.play();
            countdown--;
            countdowntime=0;
        }
        ukiyoe.camera_update();
        ukiyoe.dipth_update_pack();
        if(countdown<=0){
            bgm2.stop();
            cam.play();
            mode=2;
            snap();
            ofBackground(233,219,205);
        }
    }
    if(mode==2){
        countdowntime++;
        if(countdowntime==127){
            iyo.play();
            CreateImage();
        }
        if(countdowntime>250){
            flowacc=25;
            mode=0;
             ofBackground(255);
            countdowntime=0;
        }
    }
    //
}
void ofApp::title(float y){
    ofPushMatrix();
    ofTranslate(0, y);
    texture.draw(0,0,ofGetWidth(),1000);
    for(int i=0;i<2;i++)kumo[i].draw(croud[i].x,croud[i].y);
    //bg.draw(0,0);
    sharaku.draw(-20+10*movc, ofGetHeight()-sharaku.height/1.5,sharaku.width/1.5,sharaku.height/1.5);
    te.draw(-20+10*movc, ofGetHeight()-te.height/1.5+15*movs,te.width/1.5,te.height/1.5);
    ofSetColor(0,0,0);
    font.drawString("UKIYOE", ofGetWidth()/2-font.stringWidth("UKIYOE")/2,300+movc*9);
    font.drawString("CAMERA", ofGetWidth()/2-font.stringWidth("CAMERA")/2,450+movs*9);
    ofSetColor(138,118,65);
    font.drawString("UKIYOE", ofGetWidth()/2-font.stringWidth("UKIYOE")/2,298+movc*9);
    font.drawString("CAMERA", ofGetWidth()/2-font.stringWidth("CAMERA")/2,448+movs*9);
    ofPopMatrix();
    ofSetColor(255,255,255);
}
void ofApp::CountDown(){
    ofPushMatrix();
    if(countdown>3){
    ofSetColor(0,0,0);
    font2.drawString("hold stay!", ofGetWidth()/2 - font2.stringWidth("hold stay!")/2,200+movc*9);
    ofSetColor(238,138,85);
    font2.drawString("hold stay!", ofGetWidth()/2 - font2.stringWidth("hold stay!")/2,198+movc*9);
    }
    else{
    ofSetColor(0,0,0,255-(countdowntime*8));
    font2.drawString(ofToString(countdown), ofGetWidth()/2-font.stringWidth(ofToString(countdown))/2,200);
    }
    ofPopMatrix();
    ofSetColor(255,255,255);
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);
    
    if(mode == 0){
        title(flow);
        
        ukiyoe.drawukiyoe(1000 + flow);
       // ofDrawBitmapString(ofToString(ukiyoe.count_depth()), 200,200);
       //ukiyoe.drawcamera();
    }
    
    if(mode==1){
        ukiyoe.drawukiyoe(0);
        CountDown();
    }
    if(mode==2){
        ofSetColor(255,255,255);
        bg[pattern].draw(0,0);
        ukiyoe.drawukiyoe(0);
        if(countdowntime<127){
            ofSetColor(255,255,255,255-countdowntime*2);
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
        }else{
            ofSetColor(0,0,0);
            font.drawString("OMIGOTO", ofGetWidth()/2-font.stringWidth("OMIGOTO")/2,ofGetHeight()-120);
            ofSetColor(108,88,35);
            font.drawString("OMIGOTO", ofGetWidth()/2-font.stringWidth("OMIGOTO")/2,ofGetHeight()-118);
        }
        ofSetColor(255,255,255,50);
        texture.draw(0,0,ofGetWidth(),ofGetHeight());
    }
    
    
    
}
void ofApp::CreateImage(){
    int da =ofGetDay();
    int mo=ofGetMonth();
    int ye=ofGetYear();
    int ho=ofGetHours();
    int mi=ofGetMinutes();
    int se=ofGetSeconds();
    string s ="photo/"+ofToString(ye)+"_"+ofToString(mo)+"_"+ofToString(da)+"_"+ofToString(ho)+"_"+ofToString(mi)+"_"+ofToString(se)+".png";
    cout<<s;
    finish.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    finish.saveImage(s);
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
