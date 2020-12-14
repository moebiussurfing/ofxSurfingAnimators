#pragma once

#include "ofMain.h"

#include "FadeAnimator.h"

#include "ofxSurfing_ofxGui.h"//to customize gui only

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
    void draw();
    void keyPressed(int key);

	FadeAnimator envelope;

	void startTween();
	void stopTween();
};