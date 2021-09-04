#pragma once

#include "ofMain.h"

#include "FloatAnimator.h"


/*
	OVERVIEW

	This example shows how to use two animators for float variables.
	That will be applied to the y coord for the center and the radius of a circle.
	One of the animators is linked to an ofParameter.

	USAGE
	
	+ Press space key to trig start both animators.
	+ Press start button of each panel to trig start each animator.

*/


class ofApp : public ofBaseApp 
{
public:
	void setup();
	void draw();
	void keyPressed(int key);

	void startTween();
	void stopTween();

	FloatAnimator animatorFloat1;

	FloatAnimator animatorFloat2;
	ofParameter<float>myParamFloat2 {"myParamFloat2", 0.5f, 0, 1};
};
