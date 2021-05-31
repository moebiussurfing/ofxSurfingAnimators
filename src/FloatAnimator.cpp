#include "FloatAnimator.h"

//--------------------------------------------------------------
FloatAnimator::FloatAnimator()
{
	path_GLOBAL_Folder = "FloatAnimator";
	path_Settings = "settings_FloatAnimator.xml";
	autoSettings = true;
	//autoSettings = false;

	//ofSetLogLevel(OF_LOG_NOTICE);
	//ofSetLogLevel(OF_LOG_SILENT);

	doneInstantiated = true;
	setFps(60);
	SHOW_Gui = true;
	//guiPos = glm::vec2(500, 500);

	ofAddListener(ofEvents().update, this, &FloatAnimator::update);
	ofAddListener(ofEvents().draw, this, &FloatAnimator::draw, OF_EVENT_ORDER_AFTER_APP);
}

//--------------------------------------------------------------
void FloatAnimator::Changed_AnimatorDone(ofxAnimatable::AnimationEvent &)
{
	ofLogNotice(__FUNCTION__);

	// workflow
	if (repeatMode == 2 || repeatMode == 3 || repeatMode == 5)//with a back mode. force to start
	{
		if (value != valueStart) value = valueStart;
		animProgress = 0;
	}
	else {//not a back mode. force to end
		if (value != valueEnd) value = valueEnd;
		animProgress = 100;
	}
}

//--------------------------------------------------------------
void FloatAnimator::nextCurve()
{
	curveType++;
	curveType = curveType % NUM_ANIM_CURVES;

	if (ModeBrowse) {
		start();
	}
}

//--------------------------------------------------------------
void FloatAnimator::previousCurve()
{
	curveType--;
	if (curveType < 0) curveType = 0;
	//if (curveType < 0) curveType = NUM_ANIM_CURVES - 1;

	if (ModeBrowse) {
		start();
	}
}

//--------------------------------------------------------------
void FloatAnimator::setup()
{
	//	ofxSurfingHelpers::setThemeDark_ofxGui;
	//	//ofxSurfingHelpers::setTheme_ofxGui("assets/fonts/iAWriterDuospace-Bold.ttf");

	ENABLE_valueAnim.set("Enable Animator", true);

	value.set("Value", 0.f, 0.f, 1.f);
	valueStart.set("Value Start", 0.f, 0.f, 1.f);
	valueEnd.set("Value End", 1.f, 0.f, 1.f);

	//anim_loop.set("Loop", false);
	duration.set("Duration", 1.f, 0.f, 5.f);
	animDelay.set("PreDelay", 0.f, 0.f, 5.f);
	curveType.set("Curve Type", 16, 0, NUM_ANIM_CURVES - 1);
	repeatMode.set("Repeat Mode", 0, 0, 5);
	curveName.set("", "");
	repeatName.set("", "");
	repeatName = AnimRepeat_ToStr(repeatMode.get());
	repeatTimes.set("Times", 1, 1, 10);
	reset.set("Reset", false);
	animProgress.set("%", 0, 0, 100);

	//exclude from settings
	value.setSerializable(false);
	curveName.setSerializable(false);
	repeatName.setSerializable(false);
	animProgress.setSerializable(false);
	reset.setSerializable(false);
	//anim_loop.setSerializable(false);
	//curveShow.setSerializable(false);
	//SHOW_Plot.setSerializable(false);

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

	////TODO:
	////this will be the bpm group for noise animator too. 
	////so should move out->to ofxSphere
	//_params_Bpm.add(bpmMode);
	//_params_Bpm.add(bpmSpeed);
	//_params_Bpm.add(bpmBeatDuration);
	////_params_Bpm.add(bpmBeatDelay);

	//controls
	params_Control.setName("Animator Controls");
	//params_Control.setName(label);
	//params_Control.add(_params_Bpm);
	////params_Control.setName(label + " CONTROLS");
	params_Control.add(ENABLE_valueAnim);
	//params_Control.add(bpmMode);
	//params_Control.add(bpmSpeed);
	////params_Control.add(_params_Bpm);
	////params_Control.add(duration);

	params_Helpers.setName("Animator Helpers");
	params_Helpers.add(ModeBrowse.set("Mode Browser", true));
	params_Helpers.add(SHOW_Plot);

	//--

	//settings
	//params.setName("Float Animator");
	params.setName(label);
	params.add(ENABLE_valueAnim);
	params.add(value);
	params.add(valueStart);
	params.add(valueEnd);

	params_Time.setName("Time Engine");
	params_Time.add(duration);
	params_Time.add(animDelay);
	params.add(params_Time);

	//params.add(duration);
	//params.add(animDelay);

	params.add(params_Bpm);

	params.add(curveType);
	params.add(curveName);
	params.add(repeatMode);
	params.add(repeatName);
	params.add(repeatTimes);
	params.add(reset);
	params.add(animProgress);
	//params.add(anim_loop);

	ofAddListener(params.parameterChangedE(), this, &FloatAnimator::Changed_params);
	ofAddListener(params_Bpm.parameterChangedE(), this, &FloatAnimator::Changed_params);

	//-

	//// gui
	//gui.setup(label);
	//gui.setPosition(guiPos.x, guiPos.y - 20);
	//gui.add(params);
	//gui.add(SHOW_Plot);

	//-

#ifndef USE_RANDOMIZE_IMGUI_EXTERNAL
	// gui
	//guiManager.setImGuiAutodraw(false);//? TODO: improve multicontext mode..
	guiManager.setup();//initiate ImGui
	//guiManager.setUseAdvancedSubPanel(true);
#endif

	//-

	//autoSettings = true;
	if (autoSettings) ofxSurfingHelpers::loadGroup(params, path_GLOBAL_Folder + "/" + path_Settings);

	ofAddListener(floatAnimator.animFinished, this, &FloatAnimator::Changed_AnimatorDone);

	//-

	//startup
	repeatName = AnimRepeat_ToStr(repeatMode.get());
	curveName = floatAnimator.getCurveName(AnimCurve(curveType.get()));
}

//--------------------------------------------------------------
void FloatAnimator::start()
{
	ofLogNotice(__FUNCTION__);

	if (ENABLE_valueAnim)
	{
		floatAnimator.reset(valueStart);
		if (animDelay != 0.f) floatAnimator.animateToAfterDelay(valueEnd, animDelay);
		else floatAnimator.animateTo(valueEnd);
	}
}

//--------------------------------------------------------------
void FloatAnimator::stop()
{
	ofLogNotice(__FUNCTION__);

	if (ENABLE_valueAnim)
	{
		floatAnimator.reset(valueStart);
		animProgress = 0;
	}
}
//--------------------------------------------------------------
void FloatAnimator::update(ofEventArgs & args)
{
	floatAnimator.update(dt);

	//if (valueBack != nullptr)
	//{
	//    if (ENABLE_valueAnim)
	//        valueBack->set(floatAnimator.getCurrentColor());
	//    else
	//        valueBack->set(valueStart);
	//}

	if (floatAnimator.isAnimating())
	{
		animProgress = floatAnimator.getPercentDone() * 100;
		value = floatAnimator.val();

		//TODO: autoUpdate param
		//if (bAutoUpdate) {
		//	if (paramFloat.getName() != "-1")
		//		paramFloat = value;
		//}
	}
}

//--------------------------------------------------------------
void FloatAnimator::draw(ofEventArgs & args)
{
	if (SHOW_Gui)
	{
		//gui.draw();

		//-

#ifndef USE_RANDOMIZE_IMGUI_EXTERNAL
		guiManager.begin();
		//#endif
		{
			drawImGuiWidgets();
		}
		//#ifndef USE_RANDOMIZE_IMGUI_EXTERNAL
		guiManager.end();
#endif
	}

	//-

	if (SHOW_Plot)
	{
		ofPushStyle();
		ofFill();

		float x, y, size, px, w;
		size = 100;
		bool stateColor;
		string str;

		//curve type plot
		bCustomPositionPlot = !SHOW_Gui;
		if (bCustomPositionPlot) {
			x = positionPlot.x;
			y = positionPlot.y;
		}
		else {
			//x = positionGuiLayout.get().x + 45;
			x = positionGuiLayout.get().x + widthGuiLayout / 2 - (size + 19) / 2;
			y = positionGuiLayout.get().y + heightGuiLayout + pad;

			//x = gui.getPosition().x + 45;
			//y = gui.getPosition().y + gui.getHeight() + pad;
		}

		//-

		//drawCurve(glm::vec2 (x,y));
		floatAnimator.drawCurve(x, y, size, true, ofColor(255));

		//vertical line time
		float h;//display delay wait progress
		if (floatAnimator.isWaitingForAnimationToStart()) h = floatAnimator.waitTimeLeftPercent() * size;
		else h = size;
		px = ofMap(floatAnimator.getPercentDone(), 0, 1, x, x + size, true);
		ofSetColor(ofColor::red, 200);
		ofSetLineWidth(2.0);
		ofDrawLine(px, y + size, px, y + size - h);

		//-

		//vertical red bar value
		ofRectangle r;
		w = 12;
		x += size + 7;
		ofFill();
		//bg
		ofSetColor(0, 200);
		r = ofRectangle(x, y + size, w, -size);
		float pad = 2;//make black outpsace
		//r = ofRectangle(x - pad * 0.5f, y + size + pad * 0.5f, w + pad, -size - pad);
		ofDrawRectangle(r);
		//bar
		ofSetColor(ofColor::red, 200);
		float vb = ofMap(value.get(), valueStart, valueEnd, 0.f, 1.f, true);
		r = ofRectangle(x + pad * 0.5f, y - pad * 0.5f + size, w - pad, pad - MAX(vb*size, 1));
		//r = ofRectangle(x + pad * 0.5f, y - pad * 0.5f + size, w - pad, pad - MAX(value.get()*size, 1));
		//r = ofRectangle(x, y + size, w, -MAX(value.get()*size, 1));
		ofDrawRectangle(r);

		//-

		//stateColor = floatAnimator.isAnimating();
		//str = label;
		////str = "4 COLOR";
		//ofDrawBitmapStringHighlight(str, x + 5, y - 10,
		//	stateColor ? ofColor::white : ofColor::black,
		//	!stateColor ? ofColor::white : ofColor::black);

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void FloatAnimator::drawImGuiWidgets() {
	{
		auto mainSettings = ofxImGui::Settings();
		ImGuiWindowFlags _flagsw = ImGuiWindowFlags_None;
		string name;

		//bool bOpen;
		//ImGuiColorEditFlags _flagc;

		// widgets sizes
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
		if (guiManager.auto_resize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		// 1. window parameters
		static bool bParams = true;
		//static bool bOpen = false;
		if (bParams)
		{
			//name = "PARAMETERS";
			name = "PANEL " + label;
			//if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagsw, &bOpen))
			if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagsw))
			{
				ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

				static ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
				//flags |= ImGuiTreeNodeFlags_DefaultOpen;
				flags |= ImGuiTreeNodeFlags_Framed;

				if (ImGui::Button("START", ImVec2(_w100, _h))) {
					start();
				}
				ImGui::PushItemWidth(_w100 - WIDGET_PARAM_PADDING);
				ofxImGui::AddParameter(value);
				ImGui::PopItemWidth();

				ImGui::Text("CURVE");
				if (ImGui::Button("-", ImVec2(_w50, _h / 2))) {
					previousCurve();
				}
				ImGui::SameLine();
				if (ImGui::Button("+", ImVec2(_w50, _h / 2))) {
					nextCurve();
				}

				ImGui::Dummy(ImVec2(0.0f, 2.0f));

				ofxSurfingHelpers::AddBigToggle(bpmMode, _w100, _h / 2);

				ImGui::PushItemWidth(_w100 - WIDGET_PARAM_PADDING);
				if (bpmMode) {
					ofxImGui::AddParameter(bpmSpeed);
					ofxImGui::AddParameter(bpmBeatDelay);
					ofxImGui::AddParameter(bpmBeatDuration);
				}
				else {
					ofxImGui::AddParameter(animDelay);
					ofxImGui::AddParameter(duration);
				}
				ImGui::PopItemWidth();

				ofxImGui::AddGroup(params, flags);
				ofxSurfingHelpers::AddBigToggle(SHOW_Plot, _w100, _h / 2, false);

				//-

#ifndef USE_RANDOMIZE_IMGUI_EXTERNAL
				guiManager.drawAdvancedSubPanel();
#endif

				//get window position for advanced layout paired position
				auto posx = ImGui::GetWindowPos().x;
				auto posy = ImGui::GetWindowPos().y;
				widthGuiLayout = ImGui::GetWindowWidth();
				heightGuiLayout = ImGui::GetWindowHeight();
				positionGuiLayout = glm::vec2(posx, posy);
				//positionGuiLayout = glm::vec2(posx + w, posy);
			}
			ofxImGui::EndWindow(mainSettings);
		}
#endif
	}
}

//--------------------------------------------------------------
void FloatAnimator::drawCurve(glm::vec2 &p)
{
	//float size = 100;
	//animatorPosition.drawCurve(p.x, p.y, size, true, ofColor(255));

	if (SHOW_Plot)
	{
		ofPushStyle();
		ofFill();

		float x, y, px, w;
		bool stateColor;
		string str;

		x = p.x;
		y = p.y;

		//curve type plot

		//bCustomPositionPlot = !SHOW_Gui;
		//if (bCustomPositionPlot) {
		//	x = positionPlot.x;
		//	y = positionPlot.y;
		//}
		//else {
		//	x = gui.getPosition().x + 45;
		//	y = gui.getPosition().y + gui.getHeight() + 15;
		//}

		floatAnimator.drawCurve(x, y, size, true, ofColor(255));

		//vertical line time
		float h;//display delay wait progress
		if (floatAnimator.isWaitingForAnimationToStart()) h = floatAnimator.waitTimeLeftPercent() * size;
		else h = size;
		px = ofMap(floatAnimator.getPercentDone(), 0, 1, x, x + size, true);
		ofSetColor(ofColor::red, 255);
		ofSetLineWidth(2.0);
		ofDrawLine(px, y + size, px, y + size - h);

		////vertical bar value
		//ofRectangle r;
		//w = 12;
		//x += size + 7;
		//ofFill();
		////bg
		//ofSetColor(0, 200);
		//r = ofRectangle(x, y + size, w, -size);
		//float pad = 2;//make black outpsace
		////r = ofRectangle(x - pad * 0.5f, y + size + pad * 0.5f, w + pad, -size - pad);
		//ofDrawRectangle(r);
		////bar
		//ofSetColor(ofColor::red, 200);
		//float vb = ofMap(value.get(), valueStart, valueEnd, 0.f, 1.f, true);
		//r = ofRectangle(x + pad * 0.5f, y - pad * 0.5f + size, w - pad, pad - MAX(vb*size, 1));
		////r = ofRectangle(x + pad * 0.5f, y - pad * 0.5f + size, w - pad, pad - MAX(value.get()*size, 1));
		////r = ofRectangle(x, y + size, w, -MAX(value.get()*size, 1));
		//ofDrawRectangle(r);

		//-

		//stateColor = animatorPosition.isAnimating();
		//str = label;
		////str = "4 COLOR";
		//ofDrawBitmapStringHighlight(str, x + 5, y - 10,
		//	stateColor ? ofColor::white : ofColor::black,
		//	!stateColor ? ofColor::white : ofColor::black);

		ofPopStyle();
	}

}
//--------------------------------------------------------------
FloatAnimator::~FloatAnimator()
{
	ofRemoveListener(params.parameterChangedE(), this, &FloatAnimator::Changed_params);
	ofRemoveListener(params_Bpm.parameterChangedE(), this, &FloatAnimator::Changed_params);

	ofRemoveListener(floatAnimator.animFinished, this, &FloatAnimator::Changed_AnimatorDone);

	ofRemoveListener(ofEvents().update, this, &FloatAnimator::update);
	ofRemoveListener(ofEvents().draw, this, &FloatAnimator::draw, OF_EVENT_ORDER_AFTER_APP);

	exit();
}

//--------------------------------------------------------------
void FloatAnimator::exit()
{
	if (autoSettings) ofxSurfingHelpers::saveGroup(params, path_GLOBAL_Folder + "/" + path_Settings);
}

//--------------------------------------------------------------
void FloatAnimator::saveSettings()
{
	ofLogVerbose(__FUNCTION__);
	if (autoSettings) ofxSurfingHelpers::saveGroup(params, path_GLOBAL_Folder + "/" + path_Settings);
}

//--------------------------------------------------------------
void FloatAnimator::loadSettings()
{
	ofLogVerbose(__FUNCTION__);
	if (autoSettings) ofxSurfingHelpers::loadGroup(params, path_GLOBAL_Folder + "/" + path_Settings);
}

//--------------------------------------------------------------
void FloatAnimator::Changed_params(ofAbstractParameter &e)
{
	string name = e.getName();
	if (name != "%") ofLogVerbose(__FUNCTION__) << name << " : " << e;

	//-

	if (false) {}

	else if (name == "Enable Animator")
	{
		if (!ENABLE_valueAnim && floatAnimator.isAnimating())
			floatAnimator.pause();
		else if (ENABLE_valueAnim && !floatAnimator.isAnimating())
			floatAnimator.resume();
	}

	//tween duration
	else if (name == "Duration")
	{
		floatAnimator.setDuration(duration.get());
	}

	//bpm engine
	else if (name == bpmBeatDuration.getName() || name == bpmSpeed.getName() || name == bpmBeatDelay.getName())
	{
		if (bpmMode) {
			float _bar = 60.f / bpmSpeed.get();//one bar duration in seconds to this bpm speed
			duration = (_bar / 8.f) * (float)bpmBeatDuration;
			animDelay = (_bar / 8.f) * (float)bpmBeatDelay;
		}
	}

	//bpm engine
	else if (name == bpmMode.getName())
	{
		// exclude bpm or time info depends of time mode

		params_Time.setSerializable(!bpmMode);
		duration.setSerializable(!bpmMode);
		animDelay.setSerializable(!bpmMode);

		params_Bpm.setSerializable(bpmMode);
		bpmSpeed.setSerializable(bpmMode);
		bpmBeatDuration.setSerializable(bpmMode);
		bpmBeatDelay.setSerializable(bpmMode);

		if (bpmMode) {
			float _bar = 60.f / bpmSpeed.get();//one bar duration in seconds to this bpm speed
			duration = (_bar / 8.f) * (float)bpmBeatDuration;
			animDelay = (_bar / 8.f) * (float)bpmBeatDelay;
		}

		//// gui workflow
		//gui.getGroup(params.getName()).getGroup(params_Time.getName()).minimize();
		//gui.getGroup(params.getName()).getGroup(params_Bpm.getName()).minimize();
		//if (!bpmMode) gui.getGroup(params.getName()).getGroup(params_Time.getName()).maximize();
		//else gui.getGroup(params.getName()).getGroup(params_Bpm.getName()).maximize();
	}

	else if (name == "Curve Type")
	{
		floatAnimator.setCurve(AnimCurve(curveType.get()));
		curveName = floatAnimator.getCurveName(AnimCurve(curveType.get()));
	}
	else if (name == "Repeat Mode")
	{
		repeatName = AnimRepeat_ToStr(repeatMode.get());
		floatAnimator.setRepeatType(AnimRepeat(repeatMode.get()));
	}
	else if (name == "Times")
	{
		floatAnimator.setRepeatTimes(repeatTimes.get());
	}
	//else if (name == "Loop")
	//{
	//	if (anim_loop)
	//	{
	//		repeatMode_anim_loop_PRE = repeatMode;
	//		repeatMode = 2;
	//		repeatName = AnimRepeat_ToStr(repeatMode.get());
	//		floatAnimator.setRepeatType(AnimRepeat(repeatMode.get()));
	//	}
	//	else
	//	{
	//		repeatMode = repeatMode_anim_loop_PRE;
	//		repeatName = AnimRepeat_ToStr(repeatMode.get());
	//		floatAnimator.setRepeatType(AnimRepeat(repeatMode.get()));
	//	}
	//}
	else if (name == "Reset")
	{
		if (reset)
		{
			reset = false;

			// standard
			//bpmBeatDelay = 0;
			//repeatMode = 0;

			// 
			bpmBeatDelay = 2;
			repeatMode = 3;// back and forth once

			curveType = 3;
			repeatTimes = 1;

			animDelay = 0.f;
			duration = 1.f;
			bpmMode = true;
			bpmBeatDuration = 4;

			valueStart = 0.f;
			valueEnd = 1.f;
		}
	}

	//value
	else if (name == "Value")
	{
	}
	else if (name == "Value Start")
	{
		//floatAnimator.setColor(valueStart);
		//if (ENABLE_valueAnim && floatAnimator.isAnimating())
		//{
		start();
		//}
	}
	else if (name == "Value End")
	{
		//floatAnimator.setColor(valueEnd);
		//if (ENABLE_valueAnim && floatAnimator.isAnimating())
		//{
		start();
		//}
	}
}