#pragma once

#include "ofMain.h"

#include "ColorAnimator.h"
#include "ofxWindowApp.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
    void draw();
    void keyPressed(int key);

	ColorAnimator colorAnim;

	void startTween();
	void stopTween();

	void randomColors();

	ofxWindowApp windowApp;

	ofColor color2;
};
