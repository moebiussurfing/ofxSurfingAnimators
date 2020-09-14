#include "ColorAnimator.h"

//--------------------------------------------------------------
ColorAnimator::ColorAnimator()
{
	//ofSetLogLevel(OF_LOG_NOTICE);

	doneInstantiated = true;
	setFps(60);
	SHOW_gui = true;
	guiPos = glm::vec2(500, 500);
}

//--------------------------------------------------------------
void ColorAnimator::onAnim_Done(ofxAnimatable::AnimationEvent &)
{
	animProgress = 100;
	//color = 
}

//--------------------------------------------------------------
void ColorAnimator::nextCurve()
{
	curveType++;
	curveType = curveType % NUM_ANIM_CURVES;
}

//--------------------------------------------------------------
void ColorAnimator::previousCurve()
{
	curveType--;
	if (curveType < 0) curveType = NUM_ANIM_CURVES - 1;
}

//--------------------------------------------------------------
void ColorAnimator::setup()
{
	ENABLE_ColorAnims.set("ENABLE COLOR", true);

	colorCurrent.set("Color", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
	colorStart.set("Color Start", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
	colorEnd.set("Color End", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));

	anim_loop.set("LOOP", false);
	duration.set("Duration", 1.f, 0.f, 10.f);
	animDelay.set("Delay", 1.f, 0, 10.f);

	//--

	//bpm engine
	bpmMode.set("BPM Mode", true);
	bpmSpeed.set("BPM", 120.f, 10.f, 400.f);
	bpmBeatDuration.set("Beat", 4, 1, 8);
	bpmBeatDelay.set("Beat Delay", 0, 0, 8);
	params_Bpm.setName("BPM Engine");
	params_Bpm.add(bpmSpeed);
	params_Bpm.add(bpmMode);
	params_Bpm.add(bpmBeatDuration);
	params_Bpm.add(bpmBeatDelay);
	//params_Bpm.add(duration);

	//-

	curveType.set("Curve", 16, 0, NUM_ANIM_CURVES - 1);
	repeatMode.set("Repeat", 0, 0, 5);
	curveName.set("", "");
	repeatName.set("", "");
	repeatName = AnimRepeat_ToStr(repeatMode.get());
	repeatTimes.set("Times", 1, 1, 10);
	reset.set("Reset", false);
	animProgress.set("%", 0, 0, 100);

	//disable for xml serialize (not required)
	anim_loop.setSerializable(false);
	curveName.setSerializable(false);
	repeatName.setSerializable(false);
	animProgress.setSerializable(false);
	reset.setSerializable(false);
	//curveShow.setSerializable(false);

	params.setName(label);
	params.add(ENABLE_ColorAnims);
	params.add(colorCurrent);
	params.add(colorStart);
	params.add(colorEnd);
	params.add(duration);
	params.add(animDelay);
	params.add(params_Bpm);
	//params.add(anim_loop);

	params.add(curveType);
	params.add(repeatMode);
	params.add(repeatTimes);
	params.add(curveName);
	params.add(repeatName);
	params.add(reset);
	params.add(animProgress);

	gui.setup(label);
	gui.setPosition(guiPos.x, guiPos.y - 20);
	gui.add(params);
	gui.add(SHOW_Plot);

	ofAddListener(params.parameterChangedE(), this, &ColorAnimator::Changed_params);

	path = "settings_ColorAnimator.xml";
	if (autoSettings) load_GroupSettings(params, path);

	//-

	repeatName = AnimRepeat_ToStr(repeatMode.get());
	curveName = colorAnim.getCurveName(AnimCurve(curveType.get()));

	ofAddListener(colorAnim.animFinished, this, &ColorAnimator::onAnim_Done);
}

//--------------------------------------------------------------
void ColorAnimator::start()
{
	if (ENABLE_ColorAnims)
	{
		colorAnim.setColor(colorStart);
		if (animDelay != 0) colorAnim.animateToAfterDelay(colorEnd, animDelay);
		else colorAnim.animateTo(colorEnd);
	}
}

//--------------------------------------------------------------
void ColorAnimator::stop()
{
	if (ENABLE_ColorAnims)
	{
		colorAnim.setColor(colorStart);
		//colorAnim.reset();
		animProgress = 0;
	}
}

//--------------------------------------------------------------
void ColorAnimator::update()
{
	colorAnim.update(dt);
	colorCurrent = colorAnim.getCurrentColor();

	if (color_BACK != nullptr)
	{
		if (ENABLE_ColorAnims) color_BACK->set(colorAnim.getCurrentColor());
		else color_BACK->set(colorStart);
	}

	if (colorAnim.isAnimating())
	{
		animProgress = colorAnim.getPercentDone() * 100;
	}
}

//--------------------------------------------------------------
void ColorAnimator::draw()
{
	if (SHOW_gui)
	{
		if (SHOW_Plot)
		{
			ofPushStyle();

			int x, y, size, px;
			size = 100;
			bool stateColor;
			string str;

			//x = guiPos.x;
			//y = ofGetHeight() - size - 20;
			x = gui.getPosition().x + 16;
			y = gui.getPosition().y + gui.getHeight() + 15;

			//curve plot
			colorAnim.drawCurve(x, y, size, true, ofColor(255));

			//vertical line
			px = ofMap(colorAnim.getPercentDone(), 0, 1, x, x + size);
			ofSetColor(ofColor::red, 200);
			ofSetLineWidth(2.0);
			ofDrawLine(px, y, px, y + size);

			//-

			//vertical red bar value
			ofRectangle r;
			int w = 20;
			x += size + 7;
			ofFill();
			//bg
			ofSetColor(0, 200);
			r = ofRectangle(x, y + size, w, -size);
			float pad = 2;//make black outpsace
			ofDrawRectangle(r);
			//bar
			ofSetColor(colorCurrent, 255);
			r = ofRectangle(x + pad * 0.5f, y - pad * 0.5f + size, w - pad, pad - size);
			ofDrawRectangle(r);

			//-

			//label
			//stateColor = colorAnim.isAnimating();
			//str = label;
			////str = "4 COLOR";
			//ofDrawBitmapStringHighlight(str, x+5, y - 10,
			//    stateColor ? ofColor::white : ofColor::black,
			//    !stateColor ? ofColor::white : ofColor::black);

			ofPopStyle();
		}

		//-

		gui.draw();
	}
}

//--------------------------------------------------------------
ColorAnimator::~ColorAnimator()
{
	ofRemoveListener(colorAnim.animFinished, this, &ColorAnimator::onAnim_Done);
}

//--------------------------------------------------------------
void ColorAnimator::exit()
{
	if (autoSettings) save_GroupSettings(params, path);
}

//--------------------------------------------------------------
void ColorAnimator::load_GroupSettings(ofParameterGroup &g, string path)
{
	ofLogNotice(__FUNCTION__) << "load_GroupSettings " << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ColorAnimator::save_GroupSettings(ofParameterGroup &g, string path)
{
	ofLogNotice(__FUNCTION__) << "XML_save_AppSettings " << path;
	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}

//--------------------------------------------------------------
void ColorAnimator::Changed_params(ofAbstractParameter &e)
{
	string name = e.getName();
	if (name != "%") ofLogVerbose(__FUNCTION__) << name << " : " << e;

	//-

	if (name == "Curve")
	{
		colorAnim.setCurve(AnimCurve(curveType.get()));
		curveName = colorAnim.getCurveName(AnimCurve(curveType.get()));
	}
	else if (name == "ENABLE COLOR")
	{
		if (!ENABLE_ColorAnims && colorAnim.isAnimating()) colorAnim.pause();
		else if (ENABLE_ColorAnims && !colorAnim.isAnimating()) colorAnim.resume();
	}
	else if (name == "ENABLE COLOR")//TODO: ??
	{
		if (!ENABLE_ColorAnims) colorAnim.setColor(colorStart);
		else colorAnim.setColor(colorStart);
	}
	else if (name == "Duration")
	{
		colorAnim.setDuration(duration.get());
	}
	else if (name == "Repeat")
	{
		repeatName = AnimRepeat_ToStr(repeatMode.get());
		colorAnim.setRepeatType(AnimRepeat(repeatMode.get()));

	}
	else if (name == "Times")
	{
		colorAnim.setRepeatTimes(repeatTimes.get());
	}
	else if (name == "LOOP")
	{
		if (anim_loop)
		{
			repeatMode_anim_loop_PRE = repeatMode;
			repeatMode = 2;
			repeatName = AnimRepeat_ToStr(repeatMode.get());
			colorAnim.setRepeatType(AnimRepeat(repeatMode.get()));
		}
		else
		{
			repeatMode = repeatMode_anim_loop_PRE;
			repeatName = AnimRepeat_ToStr(repeatMode.get());
			colorAnim.setRepeatType(AnimRepeat(repeatMode.get()));
		}
	}
	else if (name == "color Start")
	{
		colorAnim.setColor(colorStart);
		if (ENABLE_ColorAnims && colorAnim.isAnimating())
		{
			//start();
			colorAnim.setColor(colorStart);
			if (animDelay != 0) colorAnim.animateToAfterDelay(colorEnd, animDelay);
			else colorAnim.animateTo(colorEnd);
		}
	}
	else if (name == "Color End")
	{
		colorAnim.setColor(colorEnd);
		if (ENABLE_ColorAnims && colorAnim.isAnimating())
		{
			//start();
			colorAnim.setColor(colorStart);
			if (animDelay != 0) colorAnim.animateToAfterDelay(colorEnd, animDelay);
			else colorAnim.animateTo(colorEnd);
		}
	}

	//bpm engine
	else if (name == bpmBeatDuration.getName() || name == bpmSpeed.getName() || name == bpmMode.getName() || name == bpmBeatDelay.getName())
	{
		if (bpmMode) {
			float _ratio = 1.f;
			float _bar = (_ratio * 60.f) / bpmSpeed.get();//one bar duration in seconds to this bpm speed
			duration = (_bar) * (float)bpmBeatDuration;
			animDelay = (_bar) * (float)bpmBeatDelay;
		}
	}

	else if (name == "Reset")
	{
		if (reset)
		{
			reset = false;

			repeatMode = 0;
			curveType = 3;
			repeatTimes = 1;

			bpmBeatDelay = 0;
			duration = 1.f;
			bpmMode = true;
			bpmBeatDuration = 4;
			animDelay = 0.f;

			colorStart.set(ofColor::yellow);
			colorEnd.set(ofColor(255));
			colorCurrent.set(colorStart);
		}
	}
}