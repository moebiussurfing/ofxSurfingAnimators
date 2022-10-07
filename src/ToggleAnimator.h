#pragma once

#include "ofMain.h"

//--


#define USE_IMGUI_LAYOUT_MANAGER__TOGGLER


//--


#include "ofxAnimatableFloat.h"
#include "ofxAnimatableQueue.h"
#include "ofxHistoryPlot.h"


#ifndef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
#include "ofxGui.h"
#endif

#ifdef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
#include "ofxSurfingImGui.h"
#endif

class ToggleAnimator
{

public:

	ToggleAnimator();
	~ToggleAnimator();

	void setup();
	void update();

	void update(float _dt)
	{
		dt = _dt;
		update();
	}

	void draw();
	void exit();

	void start();
	void stop();

	void nextCurve();
	void previousCurve();

	//--------------------------------------------------------------
	void float_TARGET(float &f)
	{
		float_BACK = &f;
	}

	//--------------------------------------------------------------
	void bool_TARGET(bool &b)
	{
		bool_BACK = &b;
	}

	//--------------------------------------------------------------
	bool getState() {
		return STATE_ToggleTarget.get();
	}

	//--------------------------------------------------------------
	void setFps(float _fps)
	{
		dt = 1.0f / _fps;
	}

	//--------------------------------------------------------------
	void setVisible(bool b)
	{
		SHOW_gui = b;
	}

#ifndef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
	//--------------------------------------------------------------
	void setPosition(glm::vec2 p)
	{
		guiPos = glm::vec2(p.x, p.y);
		gui.setPosition(guiPos.x, guiPos.y);
	}
#endif

	//--------------------------------------------------------------
	void setEnabled(bool b)
	{
		ENABLE_ToggleModulator = b;
	}

	//--------------------------------------------------------------
	void doReset()
	{
		reset = true;
	}

	//--------------------------------------------------------------
	float getTotalTime()
	{
		return totalTime;
	}

	//--------------------------------------------------------------
	void setLooped(bool b)
	{
		faderLoop = b;
	}

#ifndef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
	//--------------------------------------------------------------
	void disableHeader()
	{
		gui.disableHeader();
	}

	//--------------------------------------------------------------
	void setMinimized(bool b)
	{
		if (!b)
		{
			gui.maximizeAll();
		}
		else
		{
			gui.minimizeAll();
		}
	}

	//--------------------------------------------------------------
	bool isMinimized()
	{
		return gui.isMinimized();
	}
#endif

	//--------------------------------------------------------------
	void setDebug(bool b)
	{
		debug = b;
		if (debug)
			faderValue = faderMax.get();
	}

	//--------------------------------------------------------------
	bool isAnimating()
	{
		return queue.isPlaying();
	}

	//-

	//--------------------------------------------------------------
	float getDelay()
	{
		//return faderDelay;
		return (faderDelay / globalTimeScale);
	}

	//--------------------------------------------------------------
	float getDuration()
	{
		//return totalTime;
		return (totalTime / globalTimeScale);
	}

	//--------------------------------------------------------------
	float getAttack()
	{
		//return faderAttack;
		return (faderAttack / globalTimeScale);
	}

	//--------------------------------------------------------------
	float getSustain()
	{
		//return faderSustain;
		return (faderSustain / globalTimeScale);
	}

	//--------------------------------------------------------------
	float getRelease()
	{
		//return faderRelease;
		return (faderRelease / globalTimeScale);
	}

	//-

#ifndef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
	//--------------------------------------------------------------
	void setGuiPosition(glm::vec2 _p)
	{
		guiPos = _p;
		gui.setPosition(guiPos.x, guiPos.y);
	}
	//--------------------------------------------------------------
	glm::vec2 getGuiPosition()
	{
		return guiPos;
	}

#endif

	string label = "ToggleAnimator";
	//--------------------------------------------------------------
	void setNameLabel(string s)
	{
		label = s;
	}

	//--------------------------------------------------------------
	bool isEnabled()
	{
		return ENABLE_ToggleModulator;
	}

	//--------------------------------------------------------------
	void AutoSettings(bool b)//call before setup()!
	{
		autoSettings = b;
	}

	//--------------------------------------------------------------
	ofParameterGroup getParameterGroup()
	{
		return params;
	}

	//--------------------------------------------------------------
	void refresh_Labels()
	{
		curveName = ofxAnimatable::getCurveName(AnimCurve(curveType.get()));
		AnimCurve curve = (AnimCurve)(curveType.get());
		curvePlotable.setCurve(curve);
	}

	//--------------------------------------------------------------
	void setVisible_Plot(bool b)
	{
		SHOW_Plot = b;
	}

	//--------------------------------------------------------------
	void setGlobalTimeScale(float timeScale)
	{
		globalTimeScale = timeScale;
	}

	//tricky workaround to check if object class is created
	bool isInstantiated()
	{
		return doneInstantiated;
	}

	//TODO
	ofParameterGroup params;

	ofParameter<bool> ENABLE_ToggleModulator;

	//---

private:

	//tricky workaround to check if object class is created
	bool doneInstantiated = false;

	float globalTimeScale = 1.0f;

	//workflow
	//to disable alpha and show full opacity
	bool debug;

	ofParameter<bool> SHOW_Plot{ "Show Plots", true };

	bool autoSettings = false;

	ofxHistoryPlot *plot;

	ofxAnimatableQueue queue;
	void onAnimQueueDone(ofxAnimatableQueue::EventArg &);

#ifdef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
	ofxSurfingGui guiManager;
	void drawImGuiWidgets();
#endif

#ifndef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
	ofxPanel gui;
	glm::vec2 guiPos;
#endif

	void Changed_params(ofAbstractParameter &e);

	ofParameter<bool> faderLoop;
	//bool faderLoop_PRE;

public:
	ofParameter<bool> STATE_ToggleTarget;

private:
	ofParameter<float> faderValue;
	
	ofParameter<float> faderDelay;
	ofParameter<float> faderAttack;
	ofParameter<float> faderSustain;
	ofParameter<float> faderRelease;

	//-

	// bpm engine

public:
	void setBpm(float _bpm) {
		bpmSpeed = _bpm;
		//if (!bpmMode) bpmMode = true;
	}
	ofParameter<float> bpmSpeed;
private:
	//ofParameterGroup params_Modulator;
	ofParameterGroup params_Timers;
	ofParameterGroup params_Bpm;
	//void Changed_params_Bpm(ofAbstractParameter &e);
	ofParameter<bool> bpmMode;
	ofParameter<int> bpmBeatDelay;
	ofParameter<int> bpmBeatAttack;
	ofParameter<int> bpmBeatSustain;
	ofParameter<int> bpmBeatRelease;

	//-
		
	ofParameter<float> faderMin;
	ofParameter<float> faderMax;

	ofParameter<int> animProgress;
	ofParameter<int> curveType;
	ofParameter<int> curveBlinkerType;

	//vector<int> blinkCurvesList{9,10,11};
	int blinkCurvesList[3];
	ofParameter<string> curveName;
	ofParameter<bool> curveShow;
	ofParameter<bool> reset;

	float totalTime;
	uint64_t lastStart;

	float dt;
	ofParameter<bool> SHOW_gui{ "SHOW_gui", true };
	string path;

	void load_GroupSettings(ofParameterGroup &g, string path);
	void save_GroupSettings(ofParameterGroup &g, string path);

	AnimCurve faderAnim;
	ofxAnimatableFloat curvePlotable;

	float *float_BACK;
	bool *bool_BACK;

	void setupFader();
	void setupPlot();

	float bitThreshold;

	void drawPlot();
	ofRectangle rectPlot;
	ofFbo fboPlot;

};
