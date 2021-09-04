#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetCircleResolution(200);

	animatorFloat1.setNameLabel("Radius"); // optional: set a name
	animatorFloat1.setup(); // Default limits are 0 to 1
	//animatorFloat1.setup(1, 2); // But you can be customized too

	myParamFloat2.setName("yPos");
	animatorFloat2.setup(myParamFloat2); // ofParam, autoUpdate

	startTween();
}

//--------------------------------------------------------------
void ofApp::draw() {

	// Get animators values
	float myFloat1 = animatorFloat1.getValue(); // to radius / scale
	myParamFloat2 = animatorFloat2.getValue(); // update param too. not required if initiated with autoUpdate true!

	//-

	// Draw scene
	{
		ofSetBackgroundColor(16);
		ofPushMatrix();
		ofPushStyle();
		int xx = ofGetWidth() * 0.5;
		int yy = (ofGetHeight() * 0.5) - (150 * myParamFloat2.get()); // use the param value
		ofTranslate(xx, yy);
		ofScale(0.5 + 2 * myFloat1);
		ofSetColor(0);
		int r = 100;
		int x = 0;
		int y = 0;
		ofFill();
		ofDrawCircle(x, y, r);
		ofSetColor(255);
		ofNoFill();
		ofDrawCircle(x, y, r);
		ofPopStyle();
		ofPopMatrix();
	}

	// Help
	string s = "PRESS SPACE TO TRIG BOTH ANIMATORS";
	ofDrawBitmapStringHighlight(s, 10, 25);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLogNotice(__FUNCTION__) << "key: " << key;

	if (key == ' ') startTween();

	if (key == OF_KEY_RETURN) stopTween();
}

//--------------------------------------------------------------
void ofApp::startTween() {
	animatorFloat1.start();
	animatorFloat2.start();
}

//--------------------------------------------------------------
void ofApp::stopTween() {
	animatorFloat1.stop();
	animatorFloat2.stop();
}

