#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetCircleResolution(200);
	
	animatorFloat.setNameLabel("myFloat");
	animatorFloat.setup(1, 2);

	animatorNoise.setNameLabel("myNoise");
	animatorNoise.setup();


	startTween();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {

	//scene
	ofSetBackgroundColor(16);
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
	ofTranslate(animatorNoise.getNoisePoint());
	ofScale(animatorFloat.getValue());
	ofSetColor(0);
	ofFill();
	ofDrawCircle(0, 0, 100);
	ofSetColor(255);
	ofNoFill();
	ofDrawCircle(0, 0, 100);
	ofPopStyle();
	ofPopMatrix();

	//help
	string s = "PRESS SPACE TO TRIG ANIMATORS";
	ofDrawBitmapStringHighlight(s, 10, 25);
}

//--------------------------------------------------------------
void ofApp::startTween() {
	animatorFloat.start();
	animatorNoise.start();
}

//--------------------------------------------------------------
void ofApp::stopTween() {
	animatorFloat.stop();
	animatorNoise.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLogNotice(__FUNCTION__) << "key: " << key;

	if (key == ' ') startTween();
	if (key == OF_KEY_RETURN) stopTween();
}

