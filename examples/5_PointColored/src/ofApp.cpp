#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	float fps = 60;
	ofSetFrameRate(fps);
	ofSetCircleResolution(200);

	colorAnim.setNameLabel("animColor");
	colorAnim.setup();

	posAnim.setNameLabel("animPos");
	posAnim.setup();
	posAnim.setPositionStart(glm::vec2(0.2f, 0.2f));
	posAnim.setPositionEnd(glm::vec2(0.6f, 0.6f));

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

		// 1. get position animator
		float x = posAnim.getCurrentPosition().x * ofGetWidth();
		float y = posAnim.getCurrentPosition().y * ofGetHeight();

		// 2. get color animator
		ofSetColor(colorAnim.getColorCurrent());
		ofFill();

		// big circle
		ofDrawCircle(x, y, 100);

		// border
		ofSetColor(0);
		ofNoFill();
		ofDrawCircle(x, y, 100);

		// start/end points connected
		ofSetColor(255, 0, 0);
		ofFill();
		ofDrawCircle(posAnim.getPositionStart().x * ofGetWidth(), posAnim.getPositionStart().y * ofGetHeight(), 5);
		ofDrawCircle(posAnim.getPositionEnd().x * ofGetWidth(), posAnim.getPositionEnd().y * ofGetHeight(), 5);
		ofDrawLine(posAnim.getPositionStart() * glm::vec2(ofGetWidth(), ofGetHeight()), posAnim.getPositionEnd() * glm::vec2(ofGetWidth(), ofGetHeight()));

		ofPopStyle();
	}

	//-

	colorAnim.draw();
	posAnim.draw();

	//-

	// help
	string s = "PRESS KEYS \n";
	s += "SPACE       :  TRIG ANIMATORS \n";
	s += "BACK SPACE  :  STOP ANIMATORS \n";
	s += "F1          :  RANDOMIZE POSITIONS \n";
	s += "F2          :  RANDOMIZE COLORS \n";
	s += "F3          :  RANDOMIZE BOTH";
	ofDrawBitmapStringHighlight(s, 10, 20);
}

//--------------------------------------------------------------
void ofApp::startTween() {
	colorAnim.start();
	posAnim.start();
}

//--------------------------------------------------------------
void ofApp::stopTween() {
	colorAnim.stop();
	posAnim.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	ofLogNotice(__FUNCTION__) << key;

	if (key == ' ') startTween();

	if (key == OF_KEY_BACKSPACE) stopTween();

	if (key == OF_KEY_F1) // randomize start/end position
	{
		randomPositions();
	}

	if (key == OF_KEY_F2) // randomize start/end colors
	{
		randomColors();
	}

	if (key == OF_KEY_F3) { // randomize both

		randomPositions();
		randomColors();
	}
}

//--------------------------------------------------------------
void ofApp::randomPositions() {
	posAnim.setPositionStart(glm::vec2(ofRandom(1), ofRandom(1)));
	posAnim.setPositionEnd(glm::vec2(ofRandom(1), ofRandom(1)));
	posAnim.start();
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
