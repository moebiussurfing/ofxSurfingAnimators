
#include "ColorAnimator.h"

//--------------------------------------------------------------
void ColorAnimator::setup() {
	ofLogNotice(__FUNCTION__);

	// from base class
	FloatAnimator::setup();

	ofAddListener(ofEvents().update, this, &ColorAnimator::update);

	colorCurrent.set("Color", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
	colorStart.set("Start", ofColor(55, 55, 55, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
	colorEnd.set("End", ofColor(200, 200, 200, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));

	//----

	setupExtra();
}

//--------------------------------------------------------------
void ColorAnimator::setupExtra()
{
	ofLogNotice(__FUNCTION__);

	/*

	Overrides FloatAnimator
	You can define this into your heritated sub class to add parameters to the settings
	that we want to handle and serialize to json.

	*/

	params_Colors.add(colorStart);
	params_Colors.add(colorEnd);
	params.add(params_Colors);

	//--

	// from base class
	// we call again to load settings
	//FloatAnimator::setupExtra();

	FloatAnimator::startup();
}

//--------------------------------------------------------------
void ColorAnimator::update(ofEventArgs & args) {
	FloatAnimator::update(args);

	float r = ofMap(getValue(), 0, 1, colorStart.get().r, colorEnd.get().r, true);
	float g = ofMap(getValue(), 0, 1, colorStart.get().g, colorEnd.get().g, true);
	float b = ofMap(getValue(), 0, 1, colorStart.get().b, colorEnd.get().b, true);
	float a = ofMap(getValue(), 0, 1, colorStart.get().a, colorEnd.get().a, true);

	colorCurrent.set(ofColor(r, g, b, a));

	if (isAnimating())
		if (color_BACK != nullptr)
		{
			color_BACK->set(colorCurrent);
		}
}

//--------------------------------------------------------------
void ColorAnimator::drawImGuiWidgetsExtra() {

	float _w100 = ofxImGuiSurfing::getWidgetsWidth(1);
	float _h = ofxImGuiSurfing::getWidgetsHeightUnit();

	//ofxImGuiSurfing::AddParameter(colorCurrent);
	ImGuiColorEditFlags flags = ImGuiColorEditFlags_None;
	flags |= ImGuiColorEditFlags_NoInputs;
	flags |= ImGuiColorEditFlags_NoLabel;
	flags |= ImGuiColorEditFlags_NoTooltip;
	//ImGui::PushItemWidth(_w100);
	ImGui::ColorButton("", colorCurrent.get(), flags, ImVec2(_w100, _h));
	//ImGui::PopItemWidth();

	ImGui::Spacing();
	//ImGui::Dummy(ImVec2(0, 5));
	ofxImGuiSurfing::AddParameter(colorStart);
	ofxImGuiSurfing::AddParameter(colorEnd);
}

//--------------------------------------------------------------
void ColorAnimator::exit() {
	ofRemoveListener(ofEvents().update, this, &ColorAnimator::update);
	//ofRemoveListener(ofEvents().draw, this, &ColorAnimator::draw);
}