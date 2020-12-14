#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    float fps = 60;
	ofSetFrameRate(fps);
	ofSetCircleResolution(200);
	
    ofxSurfingHelpers::setThemeDark_ofxGui();

    colorAnim.setPath_GlobalFolder("Animators_Settings/");
    colorAnim.setNameLabel("myAnimColor");
    colorAnim.setAutoSaveLoad(true);
    colorAnim.setFps(fps);
    colorAnim.setup();
	colorAnim.setGuiPosition(glm::vec2(5 + 0 * 205, 5));

	posAnim.setPath_GlobalFolder("Animators_Settings/");
	posAnim.setNameLabel("myAnimPos");
	posAnim.setAutoSaveLoad(true);
	posAnim.setFps(fps);
	posAnim.setup();
	posAnim.setGuiPosition(glm::vec2(5 + 1 * 205, 5));

	startTween();
}

//--------------------------------------------------------------
void ofApp::update() {
	colorAnim.update();
	posAnim.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetBackgroundColor(16);

	//guis
	colorAnim.draw();
	posAnim.draw();

	//scene
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);

	//get color animator
	ofSetColor(colorAnim.getColorCurrent());
	ofFill();

	//get position animator
	ofPushMatrix();
	ofTranslate(posAnim.getCurrentPosition().x, posAnim.getCurrentPosition().y);
	//big circle
	ofDrawCircle(0, 0, 100);
	//border
	ofSetColor(255);
	ofNoFill();
	ofDrawCircle(0, 0, 100);
	ofPopMatrix();
	
	//start/end points connected
	ofSetColor(255);
	ofFill();
	ofDrawCircle(posAnim.getPositionStart().x, posAnim.getPositionStart().y, 5);
	ofDrawCircle(posAnim.getPositionEnd().x, posAnim.getPositionEnd().y, 5);
	ofDrawLine(posAnim.getPositionStart(), posAnim.getPositionEnd());

	ofPopStyle();
	ofPopMatrix();

    //help
	string s = "PRESS SPACE TO TRIG ANIMATORS";
	ofDrawBitmapStringHighlight(s, 25 + 2 * 205, 30);
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
	cout << "key: " << key << endl;

	if (key == ' ') startTween();
	if (key == OF_KEY_RETURN) stopTween();
}

