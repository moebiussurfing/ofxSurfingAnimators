#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetCircleResolution(200);

	//animatorFloat1.setNameLabel("myFloat");//set a name
	animatorFloat1.setup();//default is 0 to 1
	//animatorFloat1.setup(1, 2);//customize

	animatorFloat2.setup(myParamFloat);//ofParam, autoUpdate

	startTween();
}

//--------------------------------------------------------------
void ofApp::draw() {

	// scene
	float value = animatorFloat1.getValue();
	myParamFloat = animatorFloat2.getValue();//update param too. not required if initiated with autoUpdate true!

	ofSetBackgroundColor(16);
	ofPushMatrix();
	ofPushStyle();
	int xx = ofGetWidth() * 0.5;
	int yy = (ofGetHeight() * 0.5) - (150 * myParamFloat.get());//get the param value
	ofTranslate(xx, yy);
	ofScale(0.5 + 2 * value);
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

	// help
	string s = "PRESS SPACE TO TRIG ANIMATORS";
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

