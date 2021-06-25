#pragma once

#include "ofMain.h"

//#define USE_IMGUI__COLORANIMATOR
#define USE_IMGUI_LAYOUT_MANAGER__COLORANIMATOR

//----

#include "ofxAnimatableOfColor.h"
#include "ofxSurfingHelpers.h"
#include "FloatAnimator.h"

#ifdef USE_IMGUI__COLORANIMATOR
#include "ofxSurfingImGui.h"
#include "ofxImGui.h"
#endif


class ColorAnimator : public FloatAnimator
{
	//-

public:

#ifdef USE_IMGUI__COLORANIMATOR
#ifdef USE_IMGUI_LAYOUT_MANAGER__COLORANIMATOR
	ofxSurfing_ImGui_Manager guiManager;
#endif

#ifndef USE_IMGUI_LAYOUT_MANAGER__COLORANIMATOR
	ofxImGui::Gui gui;
#endif
#endif

	//-

	//ColorAnimator();
	//~ColorAnimator();

	void setup();
	void exit();

	virtual void update(ofEventArgs & args);
	//void draw();
	//virtual void draw(ofEventArgs & args);

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

	//   
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
