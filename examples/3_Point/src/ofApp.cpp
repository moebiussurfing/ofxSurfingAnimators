#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	float fps = 60;
	ofSetFrameRate(fps);
	ofSetCircleResolution(200);
	
	posAnim.setPath_GlobalFolder("myAnimator"); // -> optional path settings customization
	posAnim.setNameLabel("posAnim"); // -> optional name customization. Useful when using multiple instances
	posAnim.setup();

	posAnim.setVisible(true);

	startTween();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {

	// scene
	{
		// get position animator
		float x = posAnim.getCurrentPosition().x;
		float y = posAnim.getCurrentPosition().y;

		ofPushStyle();

		// start/end points connected
		ofSetColor(255, 0, 0);
		ofFill();
		ofDrawCircle(posAnim.getPositionStart().x, posAnim.getPositionStart().y, 5);
		ofDrawCircle(posAnim.getPositionEnd().x, posAnim.getPositionEnd().y, 5);
		ofDrawLine(posAnim.getPositionStart(), posAnim.getPositionEnd());

		// get color animator
		ofSetColor(255);
		ofFill();
		// big circle
		ofDrawCircle(x, y, 100);
		// border
		ofSetColor(0);
		ofNoFill();
		ofDrawCircle(x, y, 100);

		ofPopStyle();
	}

	//-

	// help
	string s = "PRESS SPACE TO TRIG ANIMATOR. \nRETURN TO RANDOMIZE POSITIONS";
	ofDrawBitmapStringHighlight(s, 10, 20);

	// Gui
	posAnim.draw();
}

//--------------------------------------------------------------
void ofApp::startTween() {
	posAnim.start();
}

//--------------------------------------------------------------
void ofApp::stopTween() {
	posAnim.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	ofLogNotice() << "key: " << key;

	if (key == 'g') posAnim.setToggleVisible();

	if (key == ' ') startTween();

	if (key == OF_KEY_BACKSPACE) stopTween();

	// randomize start/end
	if (key == OF_KEY_RETURN)
	{
		posAnim.setPositionStart(glm::vec2(ofRandom(200, 200), ofRandom(200, ofGetHeight() - 200)));
		posAnim.setPositionEnd(glm::vec2(ofRandom(ofGetWidth() - 200, ofGetWidth() - 400), ofRandom(200, ofGetHeight() - 200)));
		posAnim.start();
	}
}
