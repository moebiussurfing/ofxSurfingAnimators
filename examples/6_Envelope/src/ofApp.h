#pragma once

//TODO: WIP

#include "ofMain.h"

#include "EnvelopeAnimator.h"
#include "ofxWindowApp.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
    void draw();
    void keyPressed(int key);

	EnvelopeAnimator envelope;

	void startTween();
	void stopTween();

	ofxWindowApp windowApp;
};
