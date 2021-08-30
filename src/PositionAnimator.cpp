#include "PositionAnimator.h"

//--------------------------------------------------------------
void PositionAnimator::setup() {
	ofLogNotice(__FUNCTION__);

	//setNameLabel("posAnim");

	FloatAnimator::setup();

	ofAddListener(ofEvents().update, this, &PositionAnimator::update);

	pos.set("Position", glm::vec2(0), glm::vec2(0, 0), glm::vec2(1920, 1080));
	posStart.set("From", glm::vec2(0), glm::vec2(0, 0), glm::vec2(1920, 1080));
	posEnd.set("To", glm::vec2(0), glm::vec2(0, 0), glm::vec2(1920, 1080));

	posStart = glm::vec2(0, 0);
	posEnd = glm::vec2(500, 500);

	//----

	setupExtra();
}

//--------------------------------------------------------------
void PositionAnimator::setupExtra()
{
	ofLogNotice(__FUNCTION__);

	/*

	Overrides FloatAnimator
	You can define this into your heritated sub class to add parameters to the settings
	that we want to handle and serialize to json.

	*/

	pos.setSerializable(false);

	params_Positions.add(pos);
	params_Positions.add(posStart);
	params_Positions.add(posEnd);
	params.add(params_Positions);

	//--

	// from base class
	// we call again to load settings
	//FloatAnimator::setupExtra();

	FloatAnimator::startup();
}

//--------------------------------------------------------------
void PositionAnimator::exit() {
	ofRemoveListener(ofEvents().update, this, &PositionAnimator::update);
};

void PositionAnimator::update(ofEventArgs & args) {
	FloatAnimator::update(args);

	//float x = ofLerp(getValue(), posStart.get().x, posEnd.get().x);
	//float y = ofLerp(getValue(), posStart.get().y, posEnd.get().y);

	float x = ofMap(getValue(), 0, 1, posStart.get().x, posEnd.get().x);
	float y = ofMap(getValue(), 0, 1, posStart.get().y, posEnd.get().y);

	pos = glm::vec2(x, y);

	//cout << "getValue(): " << getValue() << endl;
	//cout << "x,y: " << x << ", " << y << endl;
}

//--------------------------------------------------------------
void PositionAnimator::drawImGuiWidgetsExtra() {
	float _w100 = ofxImGuiSurfing::getWidgetsWidth(1);
	float _h = ofxImGuiSurfing::getWidgetsHeight();

	//ImGui::Button("TEST_ExtraP", ImVec2(_w100, 2 * _h));

	ofxImGuiSurfing::AddParameter(pos);
	ofxImGuiSurfing::AddParameter(posStart);
	ofxImGuiSurfing::AddParameter(posEnd);
}
