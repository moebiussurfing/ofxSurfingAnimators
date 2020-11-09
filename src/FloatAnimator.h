#pragma once

#include "ofMain.h"

#include "ofxAnimatableFloat.h"
#include "ofxGui.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"

class FloatAnimator
{
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

public:
	bool bCustomPositionPlot = false;
	glm::vec2 positionPlot{ 50, 50 };

	FloatAnimator();
	~FloatAnimator();

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

	float sizeCurvePlot = 100;
	void drawCurve(glm::vec2 &p);


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

	//--------------------------------------------------------------
	void setMinimized(bool b)
	{
		if (b)
		{
			gui.minimizeAll();
		}
		else
		{
			gui.maximizeAll();
		}
	}

	//--------------------------------------------------------------
	bool isMinimized()
	{
		return gui.isMinimized();
	}

	//--------------------------------------------------------------
	void disableHeader()
	{
		gui.disableHeader();
	}

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
		return value.get();
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

	//--------------------------------------------------------------
	glm::vec2 getGuiShape()
	{
		ofRectangle r = gui.getShape();
		glm::vec2 _shape = glm::vec2(r.getWidth(), r.getHeight() + sizeCurvePlot + pad + 15);// lastone is text line height 
		return _shape;
	}
	public:
		//bool bCustomPositionPlot = false;
		//glm::vec2 positionPlot{ 50, 50 };
		float pad = 15;
private:
	string label = "Float Animator";

public:
	//--------------------------------------------------------------
	void setNameLabel(string s)//to label gui panel
	{
		label = s;
		path_Settings = label + ".xml";
	}

public:
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

public:
	ofParameter<float> valueStart;
	ofParameter<float> valueEnd;
	ofParameter<float> value;

	//-

private:

	//tricky workaround to check if object class is created
	bool doneInstantiated = false;

	//float globalTimeScale = 1.0f;

	float *valueBack;

	ofxAnimatableFloat floatAnimator;

	ofxPanel gui;
	void Changed_params(ofAbstractParameter &e);

	//-

	//bpm engine
public:
	void setDelayBeatMax(int maxBeats) {
		bpmBeatDelay.setMax(maxBeats);
	}
	void setDurationBeatMax(int maxBeats) {
		bpmBeatDuration.setMax(maxBeats);
	}

public:
	void setBpm(float _bpm) {
		bpmSpeed = _bpm;
		//if (!bpmMode) bpmMode = true;
	}
	ofParameter<float> bpmSpeed;

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

	ofParameter<bool> SHOW_Gui{ "SHOW_Gui", true };
	glm::vec2 guiPos;
	//string path;
	float dt;
};
