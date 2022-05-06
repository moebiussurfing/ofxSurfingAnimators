#pragma once

#include "ofMain.h"

//----

#include "ofxAnimatableOfColor.h"
#include "ofxSurfingHelpers.h"
#include "FloatAnimator.h"

#include "ofxSurfingImGui.h"

class ColorAnimator : public FloatAnimator
{

public:

	void setup() override;
	void setupExtra() override;
	void update(ofEventArgs & args) override;
	void drawImGuiWidgetsExtra() override;

	void exit();

	//--

public:

	//--------------------------------------------------------------
	ofColor getColorCurrent() {
		return colorCurrent;
	}

	
public:
	ofParameter<ofColor> colorCurrent;

	ofParameter<ofColor> colorStart;
	ofParameter<ofColor> colorEnd;
	
private:
	ofColor *color_BACK;

public:

	//--------------------------------------------------------------
	void setColor_TARGET(ofColor &c)//legacy api
	{
		color_BACK = &c;
	}

	//--------------------------------------------------------------
	void setColorPtr(ofColor &c)
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

private:
	ofParameterGroup params_Colors{ "Colors" };
};
