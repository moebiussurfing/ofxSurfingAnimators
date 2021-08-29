#pragma once

#include "ofMain.h"

/*

	EXAMPLE OVERVIEW

	To test 2 animators from float and noise classes.
	1. Trigs a float to animate the radius of a circle.
	2. Trigs an enveloped noise point to modify the circle position.

*/


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
