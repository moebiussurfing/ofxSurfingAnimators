#pragma once


/*

TODO:

+ fbo plots inside ImGui panel
+ combo presetsManager lite version

*/


#include "ofMain.h"

#include "ofxAnimatableFloat.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"

#define USE_SURFING_PRESETS

//#include "ofxGui.h"

#define USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
//#define USE_RANDOMIZE_IMGUI_EXTERNAL // must be commented

#ifdef USE_SURFING_PRESETS
#include "ofxSurfingPresets.h"
#endif

class FloatAnimator
{
	//----

public:
#ifdef USE_SURFING_PRESETS
		ofxSurfingPresets presets;
#endif

public:
	FloatAnimator();
	~FloatAnimator();

	//TODO://use &
	//void setup(ofParameter<float>paramFloat, bool autoUpdate = false) { // only use to set min/max range but not auto update the param value!
	//--------------------------------------------------------------
	void setup(ofParameter<float> &paramFloat) { // only use to set min/max range but not auto update the param value!
		//bAutoUpdate = autoUpdate;
		setNameLabel(paramFloat.getName());
		setup(paramFloat.getMin(), paramFloat.getMax());
	}

	void setup();
	//--------------------------------------------------------------
	void setup(float start, float end) {
		setStart(start);
		setEnd(end);
		setRanges();
		setup();
	}

	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);

	//void update();
	//void draw();
	//void update(float _dt)
	//{
	//	dt = _dt;
	//	update();
	//}

	void exit();
	
	//--

private:
	//TODO: use a pointer to allow autoUpdate..
	//ofParameter<float> * paramRef = NULL;
	//bool bAutoUpdate = false;

	ofParameter<float>paramFloat{ "-1", -1, -1, -1 };

	//-

private:
#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	ofxSurfing_ImGui_Manager guiManager;
#endif

public:
	void drawImGuiWidgets();

private:
	ofFbo fboPlot;
	void drawPlot();
	ImVec2 plotShape;

	//-

	bool bCustomPositionPlot = false;
	glm::vec2 positionPlot{ 50, 50 };

	float widthGuiLayout;
	float heightGuiLayout;
	ofParameter<glm::vec2> positionGuiLayout{ "Gui PLot Position",
	glm::vec2(ofGetWidth() / 2,ofGetHeight() / 2),//center
		glm::vec2(0,0),
		glm::vec2(ofGetWidth(), ofGetHeight())
	};

	//-

private:
	std::string path_GLOBAL_Folder;//top parent folder for all other subfolders
	std::string path_Settings;

public:
	//--------------------------------------------------------------
	void setPath_GlobalFolder(string folder)
	{
		ofLogNotice(__FUNCTION__) << folder;
		path_GLOBAL_Folder = folder;
		ofxSurfingHelpers::CheckFolder(folder);
	}

	//--------------------------------------------------------------
	
	// api

public:
	void start();
	void stop();

	void nextCurve(bool bAutoTrig = true);
	void previousCurve(bool bAutoTrig = true);

private:
	float size = 100;
	void drawCurve(glm::vec2 &p);


public:
	//--------------------------------------------------------------
	void setValueTarget(float &v)
	{
		valueBack = &v;
	}

	//--------------------------------------------------------------
	void setFps(float _fps)
	{
		dt = 1.0f / _fps;
	}

	//--------------------------------------------------------------
	void setVisible(bool b)
	{
		SHOW_Gui = b;
	}

	//--------------------------------------------------------------
	void doReset()
	{
		reset = true;
	}

	//--------------------------------------------------------------
	void setEnabled(bool b)
	{
		ENABLE_valueAnim = b;
	}

	//--------------------------------------------------------------
	void setAutoSaveLoad(bool b)
	{
		autoSettings = b;
	}
	//--------------------------------------------------------------
	void saveSettings();
	void loadSettings();

	////--------------------------------------------------------------
	//void setLooped(bool b)
	//{
	//    anim_loop = b;
	//}

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

	////--------------------------------------------------------------
	//glm::vec2 getGuiShape()
	//{
	//	ofRectangle r = gui.getShape();
	//	glm::vec2 _shape = glm::vec2(r.getWidth(), r.getHeight() + size + pad + 15);// lastone is text line height 
	//	return _shape;
	//}

	////--------------------------------------------------------------
	//void setMinimized(bool b)
	//{
	//	if (b)
	//	{
	//		gui.minimizeAll();
	//	}
	//	else
	//	{
	//		gui.maximizeAll();
	//	}
	//}

	////--------------------------------------------------------------
	//bool isMinimized()
	//{
	//	return gui.isMinimized();
	//}

	////--------------------------------------------------------------
	//void disableHeader()
	//{
	//	gui.disableHeader();
	//}

	//--------------------------------------------------------------
	bool isAnimating()
	{
		return floatAnimator.isAnimating();
	}

	//--------------------------------------------------------------
	bool isEnabled()
	{
		return ENABLE_valueAnim;
	}

	//--------------------------------------------------------------
	void setStartRange(float v)
	{
		//valueStart = v;
		value.setMin(v);
		valueStart.setMin(v);
		valueEnd.setMin(v);
	}

	//--------------------------------------------------------------
	void setEndRange(float v)
	{
		//valueEnd = v;
		value.setMax(v);
		valueStart.setMax(v);
		valueEnd.setMax(v);
	}

	//--------------------------------------------------------------
	void setStart(float v)
	{
		valueStart = v;
	}

	//--------------------------------------------------------------
	void setEnd(float v)
	{
		valueEnd = v;
	}

	//--------------------------------------------------------------
	void setRanges() {//set params limits
		float a, b;
		a = valueStart;
		b = valueEnd;

		setStartRange(a);
		setEndRange(b);
	}

	////--------------------------------------------------------------
	//void setValue_Start()
	//{
	//	if (valueBack != nullptr)
	//	{
	//		if (ENABLE_valueAnim)
	//		{
	//			//valueBack->set(valueStart);
	//		}
	//	}
	//}

	////--------------------------------------------------------------
	//void setValue_End()
	//{
	//	if (valueBack != nullptr)
	//	{
	//		if (ENABLE_valueAnim)
	//		{
	//			//valueBack->set(valueEnd);
	//			//floatAnimator.setColor(valueEnd);
	//			//valueBack = (*float) valueEnd.get();
	//		}
	//	}
	//}

	//--------------------------------------------------------------
	void refresh_Labels()
	{
		repeatName = AnimRepeat_ToStr(repeatMode.get());
		curveName = floatAnimator.getCurveName(AnimCurve(curveType.get()));
	}

	//-

	//--------------------------------------------------------------
	float getPercentDone()
	{
		return floatAnimator.getPercentDone();
	}

	//--------------------------------------------------------------
	float getValue()
	{
		if (floatAnimator.isWaitingForAnimationToStart()) return valueStart;
		else return value.get();
	}

	//--------------------------------------------------------------
	float getDelay()
	{
		return animDelay;
		//return (animDelay / globalTimeScale);
	}

	//--------------------------------------------------------------
	float getDuration()
	{
		return duration;
		//return (duration / globalTimeScale);
	}

	//-

public:
	//bool bCustomPositionPlot = false;
	//glm::vec2 positionPlot{ 50, 50 };
	float pad = 15;
private:
	string label = "Float Animator";
	
	std::vector<std::string> curveNamesList;

public:
	//--------------------------------------------------------------
	void setNameLabel(string s)//to label gui panel
	{
		label = s;
		path_Settings = label + ".xml";

		//TODO: allow more than one instance.
		// must use different name and paths
		//params.setName(label + "_ANIM");
	}

public:
	std::vector<std::string> getAllCurveNames() {
		return floatAnimator.getAllCurveNames();
	}

	//control settings
	//--------------------------------------------------------------
	ofParameterGroup getControls()
	{
		return params_Control;
	}
private:
	//bpm engine
	ofParameterGroup _params_Bpm{ "BPM ENGINE" };
public:
	//--------------------------------------------------------------
	ofParameterGroup getControlsBpm()
	{
		return _params_Bpm;
	}
	//helpers
	//--------------------------------------------------------------
	ofParameterGroup getHelpers()
	{
		return params_Helpers;
	}
	//params
	//--------------------------------------------------------------
	ofParameterGroup getParameters()
	{
		return params;
	}

	ofParameterGroup params;
	ofParameter<bool> ENABLE_valueAnim;

private:
	ofParameterGroup params_Control;
	ofParameterGroup params_Helpers;
	bool autoSettings;
	ofParameter<bool> SHOW_Plot{ "Show Plot", true };

public:
	//--------------------------------------------------------------
	void AutoSettings(bool b)
	{
		autoSettings = b;
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

	//---

//private:
public:
	ofParameter<float> value;
	ofParameter<float> valueStart;
	ofParameter<float> valueEnd;

	//-

private:

	//tricky workaround to check if object class is created
	bool doneInstantiated = false;

	//float globalTimeScale = 1.0f;

	float *valueBack;

	ofxAnimatableFloat floatAnimator;

	void Changed_Params(ofAbstractParameter &e);

	//public:
	//	ofxPanel gui;

		//-

		// bpm engine
public:
	//--------------------------------------------------------------
	void setDelayBeatMax(int maxBeats) {
		bpmBeatDelay.setMax(maxBeats);
	}
	//--------------------------------------------------------------
	void setDurationBeatMax(int maxBeats) {
		bpmBeatDuration.setMax(maxBeats);
	}

public:
	//--------------------------------------------------------------
	void setBpm(float _bpm) {
		bpmSpeed = _bpm;
		//if (!bpmMode) bpmMode = true;
	}
	ofParameter<float> bpmSpeed;

	//--------------------------------------------------------------
	void setModeBrowse(bool b) {//to autotrig animator when changing curve type
		ModeBrowse = b;
	}

private:
	ofParameterGroup params_Time;
	ofParameterGroup params_Bpm;
	ofParameter<bool> bpmMode;
	ofParameter<int> bpmBeatDuration;
	ofParameter<int> bpmBeatDelay;

	//-

	ofParameter<bool> ModeBrowse;

	ofParameter<float> duration;
	ofParameter<float> animDelay;
	ofParameter<int> repeatMode;
	ofParameter<string> repeatName;
	ofParameter<int> repeatTimes;
	ofParameter<int> curveType;
	ofParameter<string> curveName;
	ofParameter<int> animProgress;
	ofParameter<bool> reset;
	//ofParameter<bool> anim_loop;
	//int repeatMode_anim_loop_PRE = 2;

	void Changed_AnimatorDone(ofxAnimatable::AnimationEvent &);

	//--------------------------------------------------------------
	std::string AnimRepeat_ToStr(int n)
	{
		std::string s("unknown");
		switch (n)
		{
		case 0:
		{
			s = "PLAY ONCE";
		}
		break;
		case 1:
		{
			s = "LOOP";
		}
		break;
		case 2:
		{
			s = "LOOP BACK & FORTH";
		}
		break;
		case 3:
		{
			s = "BACK & FORTH ONCE";
		}
		break;
		case 4:
		{
			s = "PLAY N TIMES";
		}
		break;
		case 5:
		{
			s = "BACK & FORTH N TIMES";
		}
		break;
		}
		return s;
	}

	//glm::vec2 guiPos;
	//string path;
	float dt;

	//--

public:
	ofParameter<bool> SHOW_Gui{ "SHOW ANIMATOR", true };
};
