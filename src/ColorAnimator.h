#pragma once

#include "ofMain.h"


//----

#include "ofxAnimatableOfColor.h"
#include "ofxSurfingHelpers.h"
#include "FloatAnimator.h"

#include "ofxSurfingImGui.h"

class ColorAnimator : public FloatAnimator
{
	//--

public:

	void setup() override;
	void setupExtra() override;
	void update(ofEventArgs & args) override;
	void drawImGuiWidgetsExtra() override;

	void exit();

	//--

	//--------------------------------------------------------------
	ofColor getColorCurrent() {
		return colorCurrent;
	}
	ofColor *color_BACK;

	ofParameter<ofColor> colorCurrent;

	ofParameter<ofColor> colorStart;
	ofParameter<ofColor> colorEnd;

	//--------------------------------------------------------------
	void setColor_TARGET(ofColor &c)
	{
		color_BACK = &c;
	}

	//--------------------------------------------------------------
	void setColorStart(ofColor c)
	{
		colorStart.set(c);
	}

	//--------------------------------------------------------------
	void setColorEnd(ofColor c)
	{
		colorEnd.set(c);
	}

	//--

	ofParameterGroup params_Colors{ "Colors" };




	//ofParameterGroup params;
	//string label = "Color Animator";

	////--------------------------------------------------------------
	//ofParameterGroup getParameterGroup()
	//{
	//	return params;
	//}

	////--------------------------------------------------------------
	//void setColor_Start()
	//{
	//	if (color_BACK != nullptr)
	//	{
	//		//if (ENABLE_ColorAnims) color_BACK->set(colorStart);
	//		color_BACK->set(colorStart);
	//	}
	//}

	////--------------------------------------------------------------
	//void setColor_End()
	//{
	//	if (color_BACK != nullptr)
	//	{
	//		if (ENABLE_ColorAnims)
	//		{
	//			color_BACK->set(colorEnd);
	//			colorAnim.setColor(colorEnd);
	//		}
	//	}
	//}

	////--------------------------------------------------------------
	//void refresh_Labels()
	//{
	//	repeatName = AnimRepeat_ToStr(repeatMode.get());
	//	curveName = colorAnim.getCurveName(AnimCurve(curveType.get()));
	//}

	////--------------------------------------------------------------
	//void setNameLabel(string s)
	//{
	//	label = s;
	//	path_Settings = label + ".xml";
	//}

	//---
};
