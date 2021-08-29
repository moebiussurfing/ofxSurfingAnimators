#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetCircleResolution(200);

	animatorToggle.setNameLabel("ToggleState"); // optional: set a name
	animatorToggle.setup();

	startTween();
}

//--------------------------------------------------------------
void ofApp::draw() {

	// get animators values
	bState = animatorToggle.getState(); // update param too. not required if initiated with autoUpdate true!

	// draw scene
	ofClear(128);
	ofPushStyle();
	float r = 100;
	int xx = ofGetWidth() * 0.5;
	int yy = ofGetHeight() * 0.5;
	if (!animatorToggle.isAnimating()) ofSetColor(128);
	else {
		if (!bState) ofSetColor(0);
		else ofSetColor(255);
	}
	ofFill();
	ofDrawCircle(xx, yy, r);
	ofNoFill();
	if (bState) ofSetColor(0);
	else ofSetColor(255);
	ofDrawCircle(xx, yy, r);
	ofPopStyle();

	// help
	string s = "PRESS SPACE TO TRIG TOGGLE ANIMATOR";
	ofDrawBitmapStringHighlight(s, 10, 25);

	animatorToggle.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLogNotice(__FUNCTION__) << "key: " << key;

	if (key == ' ') startTween();
	if (key == OF_KEY_RETURN) stopTween();
}

//--------------------------------------------------------------
void ofApp::startTween() {
	animatorToggle.start();
}

//--------------------------------------------------------------
void ofApp::stopTween() {
	ofLogNotice(__FUNCTION__);
}

