
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
void ColorAnimator::exit() {
	ofRemoveListener(ofEvents().update, this, &ColorAnimator::update);
	//ofRemoveListener(ofEvents().draw, this, &ColorAnimator::draw);
};

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
	//ImGui::Button("TEST_ColorAnimator_ExtraF", ImVec2(_w100, 2 * _h));

	ofxImGuiSurfing::AddParameter(colorCurrent);
	ImGui::Spacing();
	//ImGui::Dummy(ImVec2(0, 5));
	ofxImGuiSurfing::AddParameter(colorStart);
	ofxImGuiSurfing::AddParameter(colorEnd);
}



////--------------------------------------------------------------
//ColorAnimator::ColorAnimator()
//{
//	//ofSetLogLevel(OF_LOG_NOTICE);
//
//	doneInstantiated = true;
//	setFps(60);
//	SHOW_gui = true;
//	guiPos = glm::vec2(500, 500);
//
//	path_GLOBAL_Folder = "ColorAnimator";
//	path_Settings = "ColorAnimator_Settings.xml";
//	autoSettings = false;
//}
//
////--------------------------------------------------------------
//void ColorAnimator::start()
//{
//	if (ENABLE_ColorAnims)
//	{
//		colorAnim.setColor(colorStart);
//		if (animDelay != 0) colorAnim.animateToAfterDelay(colorEnd, animDelay);
//		else colorAnim.animateTo(colorEnd);
//	}
//}
//
////--------------------------------------------------------------
//void ColorAnimator::stop()
//{
//	if (ENABLE_ColorAnims)
//	{
//		colorAnim.setColor(colorStart);
//		//colorAnim.reset();
//		animProgress = 0;
//	}
//}
//
////--------------------------------------------------------------
//void ColorAnimator::update()
//{
//	colorAnim.update(dt);
//	colorCurrent = colorAnim.getCurrentColor();
//
//	if (color_BACK != nullptr)
//	{
//		if (ENABLE_ColorAnims) color_BACK->set(colorAnim.getCurrentColor());
//		else color_BACK->set(colorStart);
//	}
//
//	if (colorAnim.isAnimating())
//	{
//		animProgress = colorAnim.getPercentDone() * 100;
//	}
//}