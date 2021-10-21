#pragma once

#include "ofMain.h"

#include "FloatAnimator.h"


/*
	OVERVIEW

	This example shows how to use two animators for float variables.
	One of the animators is linked to an ofParameter.
	
	SCENE

	We will control the y coord for the center and the radius of a circle.

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

	FloatAnimator animator1_Float;
	FloatAnimator animator2_FloatParam;

	ofParameter<float>value_2 {"value_2", 0.5f, 0, 1};
};
