#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	float fps = 60;
	ofSetFrameRate(fps);
	ofSetCircleResolution(200);

	colorAnim.setPath_GlobalFolder("myAnimator"); // -> optional path settings customization
	colorAnim.setNameLabel("animColor"); // -> optional name customization. Useful when using multiple instances
	colorAnim.setup();

	colorAnim.setColorPtr(color2);

	startTween();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {

	// scene
	{
		ofPushStyle();

		int x = ofGetWidth() / 2;
		int y = ofGetHeight() / 2;
		int r = 250;

		// get color animator
		ofSetColor(colorAnim.getColorCurrent());
		ofFill();

		// big circle
		ofDrawCircle(x, y, r);

		// border
		ofSetColor(0);
		ofNoFill();
		ofDrawCircle(x, y, r);

		//-

		// color pointer auto-updated
		ofSetColor(color2);
		ofFill();
		ofRectangle rect(ofGetWidth() - 25, 5, 20, 20);
		ofDrawRectRounded(rect, 3);

		ofPopStyle();
	}

	//-

	// help
	string s = "PRESS KEYS \n";
	s += "SPACE       :  TRIG ANIMATOR \n";
	s += "BACK SPACE  :  STOP ANIMATOR \n";
	s += "F1          :  RANDOMIZE COLORS";
	ofDrawBitmapStringHighlight(s, 10, 20);

	colorAnim.draw();
}

//--------------------------------------------------------------
void ofApp::startTween() {
	colorAnim.start();
}

//--------------------------------------------------------------
void ofApp::stopTween() {
	colorAnim.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	ofLogNotice(__FUNCTION__) << key;

	if (key == ' ') startTween();
	if (key == OF_KEY_BACKSPACE) stopTween();
	if (key == OF_KEY_F1) { randomColors();}
}

//--------------------------------------------------------------
void ofApp::randomColors() {
	ofColor c1;
	ofColor c2;
	int hue = ofRandom(255);
	c1.setHsb(hue, 255, 255);
	c2.setHsb(255-hue, 255, 255);

	colorAnim.setColorStart(c1);
	colorAnim.setColorEnd(c2);
	colorAnim.start();
}