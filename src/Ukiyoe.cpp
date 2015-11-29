//
//  Ukiyoe.cpp
//  ukiyoe_v0_6
//
//  Created by Shogo Tabata on 2015/11/28.
//
//

#include "Ukiyoe.h"
extern int pattern;

color C(unsigned char r,unsigned char g,unsigned char b){
    color rc;
    rc.r=r;
    rc.g=g;
    rc.b=b;
    return rc;
}
int gouth_denom[5][5]=
{
    {1,4,6,4,1},
    {4,16,24,16,4},
    {6,24,36,24,6},
    {4,16,24,26,4},
    {1,4,6,4,1}
};
int gouth_numer[5][5]=
{
    {255,255,255,255,255},
    {255,255,255,255,255},
    {255,255,255,255,255},
    {255,255,255,255,255},
    {255,255,255,255,255}
};
void Ukiyoe::loadcsv(){
    for(int i=0;i<CSVNUM;i++){
    FILE *fp;
    char filename[100];
    sprintf(filename,"../../../data/iro%d.csv",i);
    int r, g, b;
    int ret;
    
    fp = fopen( filename, "r" );
    if( fp == NULL ){
        printf( "%sファイルが開けません¥n", filename );
    }
    
    while( ( ret = fscanf( fp, "%d,%d,%d",&r, &g, &b) ) != EOF ){
        cout<<r<<","<<g<<","<<b<<"\n";
        c[i].push_back(C(r,g,b));
    }
    fclose( fp );
    }
    
}
/////////////////////////main/////////////////////////////////////

void Ukiyoe::set(){
    setcolor();
    loadcsv();
    kinect.init();
    kinect.open();
    img.allocate(WIDTH,HEIGHT,OF_IMAGE_COLOR_ALPHA);
    colimg.allocate(WIDTH, HEIGHT);
    gryimg.allocate(WIDTH, HEIGHT);
    edgimg.allocate(WIDTH, HEIGHT);
}

void Ukiyoe::camera_update(){
    kinect.update();
}
void Ukiyoe::filter_update(){
   }
void Ukiyoe::depth_update(){
    unsigned char *cdata = kinect.getPixels();
    unsigned char *ip = colimg.getPixels();
    for (int i = 0;  i< HEIGHT; i++) {
        for(int j=0;j< WIDTH;j++){
            for(int k=0;k<3;k++){
                ip[i*3*WIDTH+j*3+k] = cdata[i*3*WIDTH+j*3+k];
            }
        }
    }
}
void Ukiyoe::dipth_update_pack(){
    depth_update();
    unsigned char *ip = colimg.getPixels();
    unsigned char *ipa = img.getPixels();
    for (int i = 0;  i< HEIGHT; i++) {
        for(int j=0;j< WIDTH;j++){
            for(int k=0;k<3;k++){
                ipa[i*4*WIDTH+j*4+k] = ip[i*3*WIDTH+j*3+k];
            }
            if(j>600)ipa[i*4*WIDTH+j*4+3] = 0;
            else if(DIST_N<=kinect.getDistanceAt(j, i)&&kinect.getDistanceAt(j, i)<=DIST_F){
                
                ipa[i*4*WIDTH+j*4+3] = 255;
            }
            else if(kinect.getDistanceAt(j, i)==0)ipa[i*4*WIDTH+j*4+3] = 255;
            else ipa[i*4*WIDTH+j*4+3] = 0;
        }
    }
    Mirror_alpha(ipa);
    img.update();
}
void Ukiyoe::ukiyoe_update_pack(){
    unsigned char *cdata = kinect.getPixels();
    unsigned char *ip = colimg.getPixels();
    unsigned char *ipa = img.getPixels();
    unsigned char *edata = edgimg.getPixels();
    
    depth_update();
    
    gryimg = colimg;
    gryimg.blur();
    cvCanny( gryimg.getCvImage(), edgimg.getCvImage(), 30, 220);
    edgimg.dilate();
   
    Posterize(ip);
    for (int k = 0; k <WIDTH * HEIGHT; k++){
        if(edata[k]!=0){
            ip[k*3]=0;
            ip[k*3+1]=0;
            ip[k*3+2]=0;
        }
    }
    for (int i = 0;  i< HEIGHT; i++) {
        for(int j=0;j< WIDTH;j++){
            for(int k=0;k<3;k++){
                ipa[i*4*WIDTH+j*4+k] = ip[i*3*WIDTH+j*3+k];
            }
            if(j>600)ipa[i*4*WIDTH+j*4+3] = 0;
            else if(DIST_N<=kinect.getDistanceAt(j, i)&&kinect.getDistanceAt(j, i)<=DIST_F){
                
                ipa[i*4*WIDTH+j*4+3] = 255;
            }
            else if(kinect.getDistanceAt(j, i)==0)ipa[i*4*WIDTH+j*4+3] = 255;
            else ipa[i*4*WIDTH+j*4+3] = 0;
        }
    }
    
    ColorDecider(ipa, 4);
    Gaussian_alpha(ipa);
    for (int k = 0; k <WIDTH * HEIGHT; k++){
        if(edata[k]!=0){
            ipa[k*4]=0;
            ipa[k*4+1]=0;
            ipa[k*4+2]=0;
        }
    }
    Mirror_alpha(ipa);
    //Gaussian_alpha(ipa);
    colimg.flagImageChanged();
    img.update();

}
void Ukiyoe::drawcamera(float y){
    kinect.draw(ofGetWidth()/2+(WIDTH * 1.8)/2,y,-WIDTH * 1.8,HEIGHT* 1.8);
}
void Ukiyoe::drawukiyoe(float y){
    img.draw(ofGetWidth()/2-(WIDTH * 1.8)/2,y,WIDTH * 1.8,HEIGHT* 1.8);
    //kinect.drawDepth(ofGetWidth()/2-(WIDTH * 1.8)/2,y);
}
void Ukiyoe::setcolor(){
    /*
    c.push_back(C(255,255,238));
    c.push_back(C(51,51,34));
    c.push_back(C(85,102,85));
    c.push_back(C(255,238,153));
    c.push_back(C(204,136,17));
     */
    post[0]=0;
    post[1]=180;
    post[2]=230;
    post[3]=255;
    
    border[0]=70;
    border[1]=130;
    border[2]=190;
    border[3]=255;
}
int Ukiyoe::count_depth(){
    unsigned char *ipa = img.getPixels();
    int cd = 0;
    for(int i=0; i<WIDTH*HEIGHT;i++){
        if(ipa[i*4+3]>=100)cd++;
        
    }
    return cd;
    
}
///////////////

///////////////

void Ukiyoe::ColorDecider(unsigned char * ary,int cm){
    
    
    for(int i=0;i<WIDTH*HEIGHT;i++){
        int dic=0;
        int neer=1000;
        for(int l=0;l<c[pattern].size();l++){
            
            int dist = sqrt(pow((double)(c[pattern][l].r-ary[i*cm]),2)+pow((double)(c[pattern][l].g-ary[i*cm+1]),2)+pow((double)(c[pattern][l].b-ary[i*cm+2]),2));
            
            if(neer>dist){
                dic = l;
                neer=dist;
            }
            
        }
        //   if(i==0)cout << dic<<"\n";
        ary[i*cm] = c[0][dic].r;
        ary[i*cm+1] = c[0][dic].g;
        ary[i*cm+2] = c[0][dic].b;
        
    }
    
}

void Ukiyoe::Gaussian(unsigned char * ary){
    unsigned char * ret = new unsigned char[WIDTH * HEIGHT * 3];
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            
            for(int m=0;m<3;m++){
                int gouth=0;
                for(int k=-2;k<3;k++){
                    for(int l=-2;l<3;l++){
                        if(i+k>=0&&j+l>=0&&i+k<HEIGHT&&j+l<WIDTH)gouth += (int)ary[(i+k)*3*WIDTH+(j+l)*3+m]*gouth_denom[k+2][l+2]/gouth_numer[k+2][l+2];
                    }
                }
                if(gouth<=255)ret[i*3*WIDTH+j*3+m]= gouth;
                else ret[i*3*WIDTH+j*3+m]=255;
            }
        }
    }
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            ary[i*3*WIDTH+j*3]= ret[i*3*WIDTH+j*3];
            ary[i*3*WIDTH+j*3+1]= ret[i*3*WIDTH+j*3+1];
            ary[i*3*WIDTH+j*3+2]= ret[i*3*WIDTH+j*3+2];
        }
    }
    delete ret;
}
void Ukiyoe::Gaussian_alpha(unsigned char * ary){
    unsigned char * ret = new unsigned char[WIDTH * HEIGHT * 4];
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            
            for(int m=0;m<4;m++){
                int gouth=0;
                for(int k=-2;k<3;k++){
                    for(int l=-2;l<3;l++){
                        if(i+k>=0&&j+l>=0&&i+k<HEIGHT&&j+l<WIDTH)gouth += (int)ary[(i+k)*4*WIDTH+(j+l)*4+m]*gouth_denom[k+2][l+2]/gouth_numer[k+2][l+2];
                    }
                }
                if(gouth<=255)ret[i*4*WIDTH+j*4+m]= gouth;
                else ret[i*4*WIDTH+j*4+m]=255;
            }
        }
    }
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            ary[i*4*WIDTH+j*4]= ret[i*4*WIDTH+j*4];
            ary[i*4*WIDTH+j*4+1]= ret[i*4*WIDTH+j*4+1];
            ary[i*4*WIDTH+j*4+2]= ret[i*4*WIDTH+j*4+2];
            ary[i*4*WIDTH+j*4+3]= ret[i*4*WIDTH+j*4+3];
        }
    }
    delete ret;
}
void Ukiyoe::Posterize_trans(unsigned char & num){
    unsigned char r;
    r = num;
    for(int i=0;i<LEVEL;i++){
        if(num<=border[LEVEL-1-i]){
            r = post[LEVEL-1-i];
        }
    }
    num = r;
}
void Ukiyoe::Posterize(unsigned char * ary){
     unsigned char * ret = new unsigned char[WIDTH * HEIGHT * 3];
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            ret[i*3*WIDTH+j*3]= ary[i*3*WIDTH+j*3];
            ret[i*3*WIDTH+j*3+1]= ary[i*3*WIDTH+j*3+1];
            ret[i*3*WIDTH+j*3+2]= ary[i*3*WIDTH+j*3+2];
        }
    }
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            Posterize_trans(ret[i*3*WIDTH+j*3]);
            Posterize_trans(ret[i*3*WIDTH+j*3+1]);
            Posterize_trans(ret[i*3*WIDTH+j*3+2]);
        }
    }
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            ary[i*3*WIDTH+j*3]= ret[i*3*WIDTH+j*3];
            ary[i*3*WIDTH+j*3+1]= ret[i*3*WIDTH+j*3+1];
            ary[i*3*WIDTH+j*3+2]= ret[i*3*WIDTH+j*3+2];
        }
    }
    delete ret;
}
void Ukiyoe::Mirror_alpha(unsigned char * ary){
    unsigned char * ret = new unsigned char[WIDTH * HEIGHT * 4];
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            for(int k=0;k<4;k++)ret[i*4*WIDTH+j*4+k]= ary[i*4*WIDTH+(WIDTH-1-j)*4+k];
            
        }
    }
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            for(int k=0;k<4;k++)ary[i*4*WIDTH+j*4+k]= ret[i*4*WIDTH+j*4+k];
        }
    }
    delete ret;
    
}
unsigned char * Ukiyoe::Return(){
    return img.getPixels();
}