#pragma once

#include "ofMain.h"
#include "ofxOPC.h"
#include "ofxShadertoy.h"
#include "ofxEasyFft.h"


class ofApp : public ofBaseApp{
private:
    ofxShadertoy shadertoy;
    ofTexture mTexture;
    ofTexture picImage;
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void exit();
    
    ofxEasyFft fft;
    
    unsigned char fftSmoothed[1024];
    
    ofxOPC opcClient;
    ofxNeoPixelRing ring12px;
    ofxNeoPixelRing ring16px;
    ofxNeoPixelRing ring24px;
    ofxNeoPixelRing ring60px;
    ofxNeoPixelJewel jewel;
    ofxNeoPixelRing ring1;
    
    bool hide;
    int w, h;

    ofFbo fbo;

};
