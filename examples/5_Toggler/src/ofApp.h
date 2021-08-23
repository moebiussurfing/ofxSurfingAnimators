#pragma once

#include "ofMain.h"

#include "ToggleAnimator.h"

/*
	OVERVIEW
	+ shows the toggler animator

	USAGE
	+ press space key to trig start 

*/

class ofApp : public ofBaseApp 
{

public:

	void setup();
	void draw();
	void keyPressed(int key);

	void startTween();
	void stopTween();

	ToggleAnimator animatorToggle;

	bool bState;
};
