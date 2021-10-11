#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetCircleResolution(200);

	animator1_Float.setNameLabel("Radius"); // Optional: set a name
	animator1_Float.setup(); // Default limits are 0 to 1
	//animator1_Float.setup(1, 2); // But you can be customized too

	value_2.setName("yPos");
	animator2_FloatParam.setup(value_2); // ofParam, autoUpdate

	startTween();
}

//--------------------------------------------------------------
void ofApp::draw() {

	// Get animators values
	float value_1 = animator1_Float.getValue(); // To radius / scale

	//-

	// Draw scene
	{
		ofSetBackgroundColor(16);
		ofPushMatrix();
		ofPushStyle();
		int xx = ofGetWidth() * 0.5;
		int yy = (ofGetHeight() * 0.5) - (150 * value_2.get()); // Use the param value
		ofTranslate(xx, yy);
		ofScale(0.5 + 2 * value_1);
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
	animator1_Float.start();
	animator2_FloatParam.start();
}

//--------------------------------------------------------------
void ofApp::stopTween() {
	animator1_Float.stop();
	animator2_FloatParam.stop();
}

