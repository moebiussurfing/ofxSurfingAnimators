#pragma once

#include "ofMain.h"

#include "FloatAnimator.h"

class ofApp : public ofBaseApp 
{
public:
	void setup();
	void draw();
	void keyPressed(int key);

	FloatAnimator animatorFloat1;
	FloatAnimator animatorFloat2;

	void startTween();
	void stopTween();

	ofParameter<float>myParamFloat {"myParamFloat", 0.5, 0, 1};

};
