#pragma once

#include "ofMain.h"

#include "ColorAnimator.h"
#include "PositionAnimator.h"

#include "ofxSurfing_ofxGui.h"//to customize gui only

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
    void draw();
    void keyPressed(int key);

	ColorAnimator colorAnim;
	PositionAnimator posAnim;

	void startTween();
	void stopTween();
};
