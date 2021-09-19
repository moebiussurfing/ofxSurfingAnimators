#pragma once

#include "ofMain.h"

//-

#define INCLUDE_FILTER	// Filter the point to avoid abrupt changes

#define INCLUDE_PLOTS		// Plotting can be disabled without affecting the functionality

//-


#include "ofxAnimatableFloat.h"
#include "ofxAnimatableQueue.h"
#ifdef INCLUDE_PLOTS
#include "ofxHistoryPlot.h"
#endif
#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"

#ifdef INCLUDE_FILTER
#include "ofxBiquadFilter.h"//TODO change to smoother bc could be more performant
#endif

//#include "ofxGui.h"
//#include "ofxSurfing_ofxGui.h"

#define USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
//#define USE_RANDOMIZE_IMGUI_EXTERNAL // must be commented

#define PLOT_BOXES_SIZES 80

#define DEFAUL_POWER 2


class NoiseAnimator : public ofBaseApp
{

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
public:
	ofxSurfing_ImGui_Manager guiManager;
#endif

private:
	bool bOpened = false;
	//bool bParams = true;

public:
	void drawImGuiWidgets();

	float widthGuiLayout;
	float heightGuiLayout;
	ofParameter<glm::vec2> positionGuiLayout{ "Gui PLot Position",
	glm::vec2(ofGetWidth() / 2,ofGetHeight() / 2),//center
		glm::vec2(0,0),
		glm::vec2(ofGetWidth(), ofGetHeight())
	};

private:
	ofFbo fboPlot;
	ofFbo fboPlot2;
	ImVec2 plotShape;
	void drawPlot();

	bool bRestoreTrue = false;

	//-

public:
	NoiseAnimator();
	~NoiseAnimator();

public:
	void refreshStyles();

public:
	void setup();
	void startup();

	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);
	//void update();
	//void draw();

//private:
	//void update(ofEventArgs & args);
	//void draw(ofEventArgs & args);

private:
	std::string path_GLOBAL_Folder;//top parent folder for all other subfolders
	std::string path_Settings;

	int rSeed;

public:
	//--------------------------------------------------------------
	void setPath_GlobalFolder(string folder)
	{
		ofLogNotice(__FUNCTION__) << folder;
		path_GLOBAL_Folder = folder;
		ofxSurfingHelpers::CheckFolder(folder);
	}

#ifdef INCLUDE_FILTER
public:
	ofParameter<bool> ENABLE_NoiseModulatorFilter;
	ofParameter<bool> ENABLE_NoisePointFilter;

	ofParameterGroup params_filters{ "LOW PASS FILTERS" };

private:

	ofxBiquadFilter1f LPFmodulator;//we filter the modulator envelope to avoid abrupt jumps
	ofxBiquadFilter3f LPFpoint;//we filter the modulator point to avoid abrupt jumps
	//ofxBiquadFilter2f LPFpoint;//we filter the modulator point to avoid abrupt jumps
	//float fc;
	ofParameter<float> fc;
	ofParameter<float> fcPoint;
#endif

private:
	bool bCustomPositionPlot = false;
	glm::vec2 positionPlot{ 200, 50 };

	//bool bDisabled = false;//TODO: whats is doing this?

public:
	//void update(float _dt)
	//{
	//	dt = _dt;
	//	update();
	//}

	//void draw();
	//void draw(bool disabled);

public:
	void exit();

	void restart();
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
	glm::vec3 getNoisePoint()
	{
		//flip y
		auto p = glm::vec3(noisePos.x, -noisePos.y, noisePos.z);
		return p;

		//return noisePos;
		//return glm::vec2(noiseX, noiseY);
	}

	//--------------------------------------------------------------
	void point_TARGET(glm::vec3 &p)
	{
		point_BACK = &p;
	}

private:
	float fps;

public:
	//--------------------------------------------------------------
	void setFps(float _fps)
	{
		dt = 1.0f / _fps;
		fps = _fps;
	}

	//--------------------------------------------------------------
	void setVisible(bool b)
	{
		bGui = b;
	}

	//--------------------------------------------------------------
	void setEnabled(bool b)
	{
		ENABLE_Modulator = b;
	}

	//--------------------------------------------------------------
	void setAutoSaveLoad(bool b)
	{
		autoSettings = b;
	}

	//--------------------------------------------------------------
	void doReset()
	{
		Reset_Modulator = true;
		Reset_Noise = true;
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

	////--------------------------------------------------------------
	//void disableHeader()
	//{
	//	gui.disableHeader();
	//}

	////--------------------------------------------------------------
	//void setMinimized(bool b)
	//{
	//	if (!b)
	//	{
	//		gui.maximizeAll();
	//	}
	//	else
	//	{
	//		gui.minimizeAll();
	//	}
	//}

	////--------------------------------------------------------------
	//bool isMinimized()
	//{
	//	return gui.isMinimized();
	//}

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
		return faderDelay;
		//return (faderDelay / globalTimeScale);
	}

	//--------------------------------------------------------------
	float getDuration()
	{
		return totalTime;
		//return (totalTime / globalTimeScale);
	}

	//--------------------------------------------------------------
	float getAttack()
	{
		return faderAttack;
		//return (faderAttack / globalTimeScale);
	}

	//--------------------------------------------------------------
	float getSustain()
	{
		return faderSustain;
		//return (faderSustain / globalTimeScale);
	}

	//--------------------------------------------------------------
	float getRelease()
	{
		return faderRelease;
		//return (faderRelease / globalTimeScale);
	}

	//-

	////--------------------------------------------------------------
	//void setGuiPosition(glm::vec2 _p)
	//{
	//	guiPos = _p;
	//	gui.setPosition(guiPos.x, guiPos.y);
	//}

	////--------------------------------------------------------------
	//glm::vec2 getGuiPosition()
	//{
	//	return guiPos;
	//}

private:
	string label = "Noise Animator";

public:
	//--------------------------------------------------------------
	void setNameLabel(string s)
	{
		label = s;

		//TODO:
		bGui.setName(s);
	}

	//--------------------------------------------------------------
	bool isEnabled()
	{
		return ENABLE_Modulator;
	}

	//--------------------------------------------------------------
	void AutoSettings(bool b)
	{
		autoSettings = b;
	}

	//--------------------------------------------------------------
	ofParameterGroup getParameterGroup()
	{
		return params;
	}

private:
	ofParameterGroup params_Control;
	ofParameterGroup params_Helpers;

public:
	//--------------------------------------------------------------
	ofParameterGroup getControls()
	{
		return params_Control;
	}
	//--------------------------------------------------------------
	ofParameterGroup getHelpers()
	{
		return params_Helpers;
	}

public:
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

	////--------------------------------------------------------------
	//void setGlobalTimeScale(float timeScale)
	//{
	//	globalTimeScale = timeScale;
	//}

	//tricky workaround to check if object class is created
	bool isInstantiated()
	{
		return doneInstantiated;
	}

	//--

public:
	ofParameterGroup params;
	ofParameterGroup params_Point;
	ofParameter<bool> bEnableAnimator;

	ofParameter<bool> ENABLE_Noise;
	ofParameter<bool> ENABLE_Modulator;

	//---

	glm::vec3 noisePos;//the main point that we will use
	//glm::vec2 noisePos;

	//---

	// noise

private:
	ofParameter<bool> ENABLE_NoiseX;
	ofParameter<bool> ENABLE_NoiseY;
	ofParameter<bool> ENABLE_NoiseZ;
	ofParameter<bool> Reset_Noise;

	ofParameterGroup params_NoiseX;
	float noiseCountX;
	ofParameter<float> noiseSpeedX;
	ofParameter<int> noisePowerX;
	ofParameter<int> noiseDeepX;
	float noiseX;

	ofParameterGroup params_NoiseY;
	float noiseCountY;
	ofParameter<float> noiseSpeedY;
	ofParameter<int> noisePowerY;
	ofParameter<int> noiseDeepY;
	float noiseY;

	ofParameterGroup params_NoiseZ;
	float noiseCountZ;
	ofParameter<float> noiseSpeedZ;
	ofParameter<int> noisePowerZ;
	ofParameter<int> noiseDeepZ;
	float noiseZ;

	//-

	//tricky workaround to check if object class is created
	bool doneInstantiated = false;

	//float globalTimeScale = 1.0;

	//WORKFLOW
	//to disable alpha and show full opacity
	bool debug;

	//bool SHOW_Plot = true;
	ofParameter<bool> SHOW_Plot{ "Show Plots", true };
	bool autoSettings;

	int noiseSizeMax;

#ifdef INCLUDE_PLOTS
	ofxHistoryPlot *plot;//envelope
	ofxHistoryPlot *plot_NoiseX;
	ofxHistoryPlot *plot_NoiseY;
#endif

private:
	ofxAnimatableQueue queue;
	void Changed_AnimatorQueueDone(ofxAnimatableQueue::EventArg &);
	
	ofEventListener listenerMinimize;

	//ofxPanel gui;
	//glm::vec2 guiPos;

	void Changed_params(ofAbstractParameter &e);

	ofParameterGroup params_Modulator;

	ofParameter<bool> faderLoop;
	//bool faderLoop_PRE;

	ofParameter<float> faderValue;
	ofParameter<float> faderMin;
	ofParameter<float> faderMax;

	ofParameter<float> faderDelay;
	ofParameter<float> faderAttack;
	ofParameter<float> faderSustain;
	ofParameter<float> faderRelease;

	//-

	//bpm engine
public:
	void setBpm(float _bpm) {
		bpmSpeed = _bpm;
		//if (!bpmMode) bpmMode = true;
	}
	ofParameter<float> bpmSpeed;

private:
	ofParameterGroup params_Timers;
	ofParameterGroup params_Bpm;
	//void Changed_params_Bpm(ofAbstractParameter &e);
	ofParameter<bool> bpmMode;
	ofParameter<int> bpmBeatDelay;
	ofParameter<int> bpmBeatAttack;
	ofParameter<int> bpmBeatSustain;
	ofParameter<int> bpmBeatRelease;

	//-

	ofParameter<int> animProgress;
	ofParameter<int> curveType;
	ofParameter<string> curveName;
	ofParameter<bool> curveShow;
	ofParameter<bool> Reset_Modulator;

	float totalTime;
	uint64_t lastStart;

	float dt;
	string path;

	AnimCurve faderAnim;
	ofxAnimatableFloat curvePlotable;

	float *float_BACK;
	glm::vec3 *point_BACK;
	//glm::vec2 *point_BACK;

	void setupFader();
	void setupPlot();

	void setupPlot_Noise();

	int lastFrame;
	//int totalFrames;

public:
	ofParameter<bool> bGui{ "Show Animator", true };

};
