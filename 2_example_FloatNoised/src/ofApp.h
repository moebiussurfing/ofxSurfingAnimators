#pragma once

#include "ofMain.h"

#include "FloatAnimator.h"
#include "NoiseAnimator.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	FloatAnimator animatorFloat;
	NoiseAnimator animatorNoise;

	void startTween();
	void stopTween();

};
