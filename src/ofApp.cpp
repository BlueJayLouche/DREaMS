#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    //    ofSetVerticalSync(true);
    w = ofGetWidth();
    h = ofGetHeight();
    
    ofDisableArbTex();
    mTexture.allocate(512,2,GL_RED, false);
    fbo.allocate(w, h, GL_RGBA32F);
    mTexture.setTextureWrap(GL_REPEAT, GL_REPEAT);
    
    shadertoy.load("shaders/Shader.glsl");
    shadertoy.setTexture(0, mTexture);
    ofImage pic("images/7.jpg");
    //    pic.mirror(true, false);
    picImage.allocate(pic.getPixels());
    
    picImage.setTextureWrap(GL_REPEAT, GL_REPEAT);
    shadertoy.setTexture(1, picImage);
    
    ofSetFrameRate(60);
    shadertoy.setAdvanceTime(true);
    ofSetWindowShape(w*2, h);
    
    
    // Connect to the fcserver
    opcClient.setup("127.0.0.1", 7890);
    
    // You define the stage size and draw into the stage
    opcClient.setupStage(h, w);
    
    //    defaultEffects.setup(opcClient.getStageCenterX(), opcClient.getStageCenterY(), opcClient.getStageWidth(), opcClient.getStageHeight());
    
    // Setup the rings
    ring12px.setupLedRing(opcClient.getStageCenterX(),opcClient.getStageCenterY(), 12, 12);
    jewel.setupLedJewel(opcClient.getStageCenterX(),opcClient.getStageCenterY(), 28);
    ring16px.setupLedRing(opcClient.getStageCenterX(),opcClient.getStageCenterY(), 16, 48);
    ring24px.setupLedRing(opcClient.getStageCenterX(),opcClient.getStageCenterY(), 24, 70);
    ring60px.setupLedRing(opcClient.getStageCenterX(),opcClient.getStageCenterY(), 64, 80);
    
    //FFT
    fft.setup(16384);
    
    for (int i = 0;i < 1024; i++){
        fftSmoothed[i] = 0;
    }
    
    ofBackground(0, 0, 0);

}
//--------------------------------------------------------------
void ofApp::update()
{
    
    ofSetWindowTitle("DREaMS");
    opcClient.update();
    
    // Now Draw the effects to the stage
    opcClient.beginStage();
    
    // Draw what you want rendered here

    shadertoy.draw(0, 0, w, h);
    
    // For now here are some default effects
    //    defaultEffects.draw(effect);
    
    opcClient.endStage();
    
    // New Get Method
    opcClient.getStagePixels(ring12px.getPixelCoordinates(), ring12px.colors);
    opcClient.getStagePixels(ring16px.getPixelCoordinates(), ring16px.colors);
    opcClient.getStagePixels(ring24px.getPixelCoordinates(), ring24px.colors);
    opcClient.getStagePixels(ring60px.getPixelCoordinates(), ring60px.colors);
    opcClient.getStagePixels(jewel.getPixelCoordinates(), jewel.colors);
    
    // If the client is not connected do not try and send information
    if (!opcClient.isConnected()) {
        // Will continue to try and reconnect to the Pixel Server
        opcClient.tryConnecting();
    }
    else {
        // Write out the first set of data
        opcClient.writeChannelFour(ring12px.colorData());
        opcClient.writeChannelTwo(ring16px.colorData());
        opcClient.writeChannelThree(ring24px.colorData());
        opcClient.writeChannelOne(ring60px.colorData());
        opcClient.writeChannelFive(jewel.colorData());
    }
    
    //FFT
    fft.update();
    
    vector<float>& buffer = fft.getBins();
    
    unsigned char signal[1024];
    for (int i = 0; i < 512; i++) {
        signal[i] = (unsigned char) (buffer.at(i) * 80.0); // FFT
    }
    for (int i = 0; i < 512; i++) {
        float audioSig = fft.getAudio()[i];
        audioSig = (audioSig * 0.5 + 0.5) * 254.0;
        signal[512+i] = (unsigned char) audioSig;
    }
    
    // Time to smoooooth our the FFT singal ;)
    for (int i = 0;i < 1024; i++){
        // let the smoothed calue sink to zero:
        fftSmoothed[i] *= 0.96f;
        
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < signal[i]) fftSmoothed[i] = signal[i];
        
    }
    mTexture.loadData(fftSmoothed, 512, 2, GL_RED);
    
    
}
//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);
    opcClient.drawStage(hide);
    
    // Show the grabber area
    ring12px.drawGrabRegion(hide);
    ring16px.drawGrabRegion(hide);
    ring24px.drawGrabRegion(hide);
    ring60px.drawGrabRegion(hide);
    jewel.drawGrabRegion(hide);
    
    int x = opcClient.getStageWidth()+150;
    int y = 200;
    // Draw the output
    ring60px.draw(x,y);
    ring24px.draw(x,y);
    ring16px.draw(x,y);
    jewel.draw(x,y);
    ring12px.draw(x,y);
    
    // Report Messages
    stringstream ss;
    ss << "FPS: " << ofToString((int)(ofGetFrameRate())) << endl;
    ofDrawBitmapStringHighlight(ss.str(), 5,ofGetHeight()-30);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key) {
        case ' ':
            hide = !hide;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}
//--------------------------------------------------------------
void ofApp::exit()
{
    // Close Connection
    opcClient.close();
}
