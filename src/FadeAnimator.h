#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableQueue.h"
#include "ofxHistoryPlot.h"
#include "ofxGui.h"
#include "ofxSurfingHelpers.h"

//adsr envelope class

class FadeAnimator
{

public:
	FadeAnimator();
	~FadeAnimator();

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

	bool bPaused = false;
	void setPause(bool b) {
		bPaused = b;
		if(b) queue.pausePlayback();
		else queue.resumePlayback();
	}

	//on/off mode
	//void pause();
	void startOn();
	void startOff();
	ofParameter<bool> MODE_NoteOff;
	void setModeOff(bool b) {
		MODE_NoteOff = b;
	}
	void setupAnimatorOn();
	void setupAnimatorOff();

	void nextCurve();
	void previousCurve();

	//--------------------------------------------------------------
	void setValueTarget(float &f)
	{
		float_BACK = &f;
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

	//--------------------------------------------------------------
	void setEnabled(bool b)
	{
		ENABLE_FaderMOD = b;
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
	bool isEnabled()
	{
		return ENABLE_FaderMOD;
	}

	//--------------------------------------------------------------
	void AutoSettings(bool b)
	{
		autoSettings = b;
	}

    //--------------------------------------------------------------
    void setAutoSaveLoad(bool b)
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

	//TODO
	ofParameterGroup params;

	//---

public:
    //--------------------------------------------------------------
    void setNameLabel(string s)//to label gui panel
    {
        label = s;
//        path = path_GLOBAL_Folder + label + ".xml";
        path = path_GLOBAL_Folder + label + ".xml";
    }
private:
    std::string path_GLOBAL_Folder;//top parent folder for all other subfolders
//    std::string path_Settings;
public:
    //--------------------------------------------------------------
    void setPath_GlobalFolder(string folder)
    {
        ofLogNotice(__FUNCTION__) << folder;
        path_GLOBAL_Folder = folder;
        ofxSurfingHelpers::CheckFolder(folder);
    }

private:
	
	string label;

	//tricky workaround to check if object class is created
	bool doneInstantiated = false;

	//float globalTimeScale = 1.0;

	//workflow
	//to disable alpha and show full opacity
	bool debug;

	ofParameter<bool> SHOW_Plot{ "Show Plot", true };

	bool autoSettings = true;

	ofxHistoryPlot *plot;

	ofxAnimatableQueue queue;
	void onAnimQueueDone(ofxAnimatableQueue::EventArg &);

	ofxPanel gui;
	glm::vec2 guiPos;
	void Changed_params(ofAbstractParameter &e);

public:
	ofParameter<bool> ENABLE_FaderMOD;
	float getValue() {
		return faderValue.get();
	}

private:
	ofParameterGroup params_Modulator;

	ofParameter<bool> faderLoop;

	ofParameter<float> faderValue;
	ofParameter<float> faderMin;
	ofParameter<float> faderMax;

	//bool faderLoop_PRE;
	
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
	ofParameter<bool> reset;

	float totalTime;
	uint64_t lastStart;

	float dt;
	ofParameter<bool> SHOW_gui{ "SHOW GUI", true };
	string path;

	AnimCurve faderAnim;
	ofxAnimatableFloat curvePlotable;

	float *float_BACK;

	void setupAnimator();

	void setupPlot();

    void refreshGui(){

        auto &g1 = gui.getGroup(label);//1st level
        auto &g2 = g1.getGroup("ENVELOPE MOD");//2nd level
        auto &g3 = g2.getGroup(params_Timers.getName());//3nd level
        auto &g4 = g2.getGroup(params_Bpm.getName());//3nd level
        g3.minimize();
        g4.minimize();
        if (bpmMode.get())
        {
//            g3.minimize();
            g4.maximize();
        }
        else {
            g3.maximize();
//            g4.minimize();
        }
    }
};
