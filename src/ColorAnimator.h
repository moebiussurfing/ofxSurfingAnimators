#pragma once

#include "ofMain.h"
#include "ofxAnimatableOfColor.h"
#include "ofxGui.h"
//#include "ofxSurfingHelpers.h"

class ColorAnimator
{

public:

    ColorAnimator();
    ~ColorAnimator();

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
    void setColor_TARGET(ofColor &c)
    {
        color_BACK = &c;
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
    void doReset()
    {
        reset = true;
    }

    //--------------------------------------------------------------
    void setEnabled(bool b)
    {
        ENABLE_ColorAnims = b;
    }

    //--------------------------------------------------------------
    void setLooped(bool b)
    {
        anim_loop = b;
    }

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
        return colorAnim.isAnimating();
    }

    //--------------------------------------------------------------
    bool isEnabled()
    {
        return ENABLE_ColorAnims;
    }

    //--------------------------------------------------------------
    void setColor_Start()
    {
        if (color_BACK != nullptr)
        {
            if (ENABLE_ColorAnims) color_BACK->set(colorStart);
        }
    }

    //--------------------------------------------------------------
    void setColor_End()
    {
        if (color_BACK != nullptr)
        {
            if (ENABLE_ColorAnims)
            {
                color_BACK->set(colorEnd);
                colorAnim.setColor(colorEnd);
            }
        }
    }

    //--------------------------------------------------------------
    void refresh_Labels()
    {
        repeatName = AnimRepeat_ToStr(repeatMode.get());
        curveName = colorAnim.getCurveName(AnimCurve(curveType.get()));
    }

    //-

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
	void setNameLabel(string s)
	{
		label = s;
	}

    ofParameterGroup params;
	string label = "COLOR";

    //TODO
    //--------------------------------------------------------------
    ofParameterGroup getParameterGroup()
    {
        return params;
    }

    bool autoSettings = false;

    //--------------------------------------------------------------
    void AutoSettings(bool b)
    {
        autoSettings = b;
    }

    //bool SHOW_Plot = true;

    //--------------------------------------------------------------
    void setVisible_Plot(bool b)
    {
        SHOW_Plot = b;
    }

    ////--------------------------------------------------------------
    //void setGlobalTimeScale(float timeScale)
    //{
    //    globalTimeScale = timeScale;
    //}

    //tricky workaround to check if object class is created
    bool isInstantiated()
    {
        return doneInstantiated;
    }

    ofParameter<bool> ENABLE_ColorAnims;

    //---

    ofParameter<ofColor> colorCurrent;
    ofParameter<ofColor> colorStart;
    ofParameter<ofColor> colorEnd;
	
	ofParameter<bool> SHOW_Plot{ "Show Plots", true };

private:

    ofColor *color_BACK;
    ofxAnimatableOfColor colorAnim;

    //tricky workaround to check if object class is created
    bool doneInstantiated = false;

    //float globalTimeScale = 1.0f;

    void load_GroupSettings(ofParameterGroup &g, string path);
    void save_GroupSettings(ofParameterGroup &g, string path);

    ofxPanel gui;
    void Changed_params(ofAbstractParameter &e);

    ofParameter<bool> anim_loop;
    int repeatMode_anim_loop_PRE = 2;
    
	ofParameter<float> duration;
    ofParameter<float> animDelay;

	//-

	//bpm engine
public:
	void setBpm(float _bpm) {
		bpmSpeed = _bpm;
		//if (!bpmMode) bpmMode = true;
	}
	ofParameter<float> bpmSpeed;
private:
	ofParameterGroup params_Bpm;
	ofParameter<bool> bpmMode;
	ofParameter<int> bpmBeatDuration;
	ofParameter<int> bpmBeatDelay;

	//-

    ofParameter<int> repeatMode;
    ofParameter<string> repeatName;
    ofParameter<int> repeatTimes;
    ofParameter<int> curveType;
    ofParameter<string> curveName;
    ofParameter<int> animProgress;
    ofParameter<bool> reset;

    void onAnim_Done(ofxAnimatable::AnimationEvent &);

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

    float dt;
    ofParameter<bool> SHOW_gui{"SHOW_gui", true};
    glm::vec2 guiPos;
    string path;
};
