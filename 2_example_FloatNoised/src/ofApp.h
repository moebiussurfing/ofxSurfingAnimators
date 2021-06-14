#pragma once

#include "ofMain.h"

/*

	EXAMPLE OVERVIEW
	Trigs a float to change a circle radius adding an enveloped noise point that modifies the circle position.

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
