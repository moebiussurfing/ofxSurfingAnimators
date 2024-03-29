#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    float fps = 60;
	ofSetFrameRate(fps);
	ofSetCircleResolution(200);
	
    envelope.setPath_GlobalFolder("Animators_Settings/");
    envelope.setNameLabel("EnvelopeAnim");
    envelope.setAutoSaveLoad(true);
    envelope.setFps(fps);
    envelope.setup();

	startTween();
}

//--------------------------------------------------------------
void ofApp::update() {
	envelope.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetBackgroundColor(32);

	//guis
	envelope.draw();

	//scene
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);

    //get animator value
    ofScale(MAX(0.01, envelope.getValue()));//clamp min scale to 0.1

	ofSetColor(0);
	ofFill();
	ofDrawCircle(0, 0, 100);
	ofSetColor(255);
	ofNoFill();
	ofDrawCircle(0, 0, 100);
	ofPopStyle();
	ofPopMatrix();

    //help
	string s = "PRESS SPACE TO TRIG ENVELOPE ANIMATOR";
	ofDrawBitmapStringHighlight(s, 15, 15);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	cout << "key: " << key << endl;

	if (key == ' ') startTween();

	if (key == OF_KEY_RETURN) stopTween();
}

//--------------------------------------------------------------
void ofApp::startTween() {
	envelope.start();
}

//--------------------------------------------------------------
void ofApp::stopTween() {
	envelope.stop();
}

