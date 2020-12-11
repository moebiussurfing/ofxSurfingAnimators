#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetCircleResolution(200);
	
	ofxSurfingHelpers::setThemeDark_ofxGui();

	animatorFloat.setNameLabel("Float Animator");
	//animatorFloat.setFps(60);
	//animatorFloat.setAutoSaveLoad(true);
	animatorFloat.setup();
	animatorFloat.valueStart = 1.f;
	animatorFloat.valueEnd = 2.f;
	//animatorFloat.setMinimized(true);
	//animatorFloat.setVisible_Plot(true);

	animatorNoise.setNameLabel("Noise Animator");
	//animatorNoise.setFps(60);
	//animatorNoise.setAutoSaveLoad(true);
	animatorNoise.setup();
	//animatorNoise.setMinimized(true);
	//animatorNoise.setVisible_Plot(true);

	animatorFloat.setGuiPosition(glm::vec2(5 + 0 * 205, 5));
	animatorNoise.setGuiPosition(glm::vec2(5 + 1 * 205, 5));

	startTween();
}

//--------------------------------------------------------------
void ofApp::update() {
	animatorFloat.update();
	animatorNoise.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetBackgroundColor(16);

	//guis
	animatorFloat.draw();
	animatorNoise.draw();

	//scene
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

	string s;
	s = "PRESS SPACE TO TRIG ALL ANIMATORS";
	ofDrawBitmapStringHighlight(s, 25 + 2 * 205, 30);
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
	cout << "key: " << key << endl;

	if (key == ' ') startTween();
	if (key == OF_KEY_RETURN) stopTween();
}

//-------------------   -------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
