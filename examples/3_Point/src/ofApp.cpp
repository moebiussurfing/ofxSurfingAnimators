#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	float fps = 60;
	ofSetFrameRate(fps);
	ofSetCircleResolution(200);

	posAnim.setup();
	posAnim.setPositionStart(glm::vec2(200, 200));
	posAnim.setPositionEnd(glm::vec2(600, 600));

	startTween();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {

	//get position animator
	float x = posAnim.getCurrentPosition().x;
	float y = posAnim.getCurrentPosition().y;

	//scene
	{
		ofPushStyle();

		//get color animator
		ofSetColor(255);
		ofFill();

		//big circle
		ofDrawCircle(x, y, 100);

		//border
		ofSetColor(0);
		ofNoFill();
		ofDrawCircle(x, y, 100);

		//start/end points connected
		ofSetColor(255, 0, 0);
		ofFill();
		ofDrawCircle(posAnim.getPositionStart().x, posAnim.getPositionStart().y, 5);
		ofDrawCircle(posAnim.getPositionEnd().x, posAnim.getPositionEnd().y, 5);
		ofDrawLine(posAnim.getPositionStart(), posAnim.getPositionEnd());

		ofPopStyle();
	}

	//-

	//help
	string s = "PRESS SPACE TO TRIG ANIMATOR. \nRETURN TO RANDOMIZE POSITIONS";
	ofDrawBitmapStringHighlight(s, 10, 20);
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
	cout << "key: " << key << endl;

	if (key == ' ') startTween();
	
	// randomize start/end
	if (key == OF_KEY_RETURN)
	{
		posAnim.setPositionStart(glm::vec2(ofRandom(200, 200), ofRandom(200, ofGetHeight() - 200)));
		posAnim.setPositionEnd(glm::vec2(ofRandom(ofGetWidth() - 200, ofGetWidth() - 400), ofRandom(200, ofGetHeight() - 200)));
		posAnim.start();
	}
}

