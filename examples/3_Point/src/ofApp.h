#pragma once

#include "ofMain.h"

#include "PositionAnimator.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
    void draw();
    void keyPressed(int key);

	PositionAnimator posAnim;

	void startTween();
	void stopTween();
};
