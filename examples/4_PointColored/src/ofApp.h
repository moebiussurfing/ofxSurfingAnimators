#pragma once

#include "ofMain.h"

#include "ColorAnimator.h"
#include "PositionAnimator.h"

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

	void randomPositions();
	void randomColors();
};
