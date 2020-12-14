#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    float fps = 30;
	ofSetFrameRate(fps);
	ofSetCircleResolution(200);
	
    ofxSurfingHelpers::setThemeDark_ofxGui();

    envelope.setPath_GlobalFolder("Animators_Settings/");
    envelope.setNameLabel("Envelope_ADSR");
    envelope.setAutoSaveLoad(true);
    envelope.setFps(fps);
    envelope.setup();

	envelope.setGuiPosition(glm::vec2(5 + 0 * 205, 5));

	startTween();
}

//--------------------------------------------------------------
void ofApp::update() {
	envelope.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetBackgroundColor(16);

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
	string s = "PRESS SPACE TO TRIG ENVELOPE ANIMATOR\n(TO CIRCLE RADIUS)";
	ofDrawBitmapStringHighlight(s, 25 + 2 * 205, 30);
}

//--------------------------------------------------------------
void ofApp::startTween() {
	envelope.start();
}

//--------------------------------------------------------------
void ofApp::stopTween() {
	envelope.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	cout << "key: " << key << endl;

	if (key == ' ') startTween();
	if (key == OF_KEY_RETURN) stopTween();
}

