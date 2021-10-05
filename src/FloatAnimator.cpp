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
	bGui = true;
	//guiPos = glm::vec2(500, 500);

	ofAddListener(ofEvents().update, this, &FloatAnimator::update);
	ofAddListener(ofEvents().draw, this, &FloatAnimator::draw, OF_EVENT_ORDER_AFTER_APP);
}

//--------------------------------------------------------------
void FloatAnimator::Changed_AnimatorDone(ofxAnimatable::AnimationEvent &)
{
	ofLogVerbose(__FUNCTION__);

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
void FloatAnimator::nextCurve(bool bAutoTrig)
{
	curveType++;
	curveType = curveType % NUM_ANIM_CURVES;

	if (ModeBrowse && bAutoTrig) {
		start();
	}
}

//--------------------------------------------------------------
void FloatAnimator::previousCurve(bool bAutoTrig)
{
	curveType--;
	if (curveType < 0) curveType = 0;
	//if (curveType < 0) curveType = NUM_ANIM_CURVES - 1;

	if (ModeBrowse && bAutoTrig) {
		start();
	}
}

////--------------------------------------------------------------
//void FloatAnimator::setupExtra()
//{
//	ofLogNotice(__FUNCTION__);
//
//	/*
//	
//	You can define this into your heritated sub class to add parameters to the settings 
//	that we want to handle and serialize to json.
//	
//	*/
//}

//--------------------------------------------------------------
void FloatAnimator::setupGui()
{
	ofLogNotice(__FUNCTION__);

	guiManager.setSettingsPathLabel(label);
	guiManager.setup(IM_GUI_MODE_INSTANTIATED);

	//guiManager.setImGuiAutodraw(true);//TODO: required when only one instance ?
	//guiManager.setup(); // initiate ImGui
	//guiManager.setUseAdvancedSubPanel(true);
}

//--------------------------------------------------------------
void FloatAnimator::setup()
{
	ofLogNotice(__FUNCTION__);

	bEnableAnimator.set("Enable Animator", true);

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
	bpmSlow.set("Slow", false);
	bpmSpeed.set("BPM", 120.f, 10.f, 400.f);
	bpmBeatDuration.set("B Duration", 4, 1, 8);
	bpmBeatDelay.set("B Delay", 0, 0, 8);
	params_Bpm.setName("BPM Engine");
	params_Bpm.add(bpmSpeed);
	params_Bpm.add(bpmMode);
	params_Bpm.add(bpmSlow);
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
	params_Control.add(bEnableAnimator);
	//params_Control.add(bpmMode);
	//params_Control.add(bpmSpeed);
	////params_Control.add(_params_Bpm);
	////params_Control.add(duration);

	params_Helpers.setName("Animator Helpers");
	params_Helpers.add(bGui);
	params_Helpers.add(ModeBrowse.set("Mode Browser", true));
	//params_Helpers.add(SHOW_Plot);

	//--

	// settings
	params.setName(label);
	//params.setName(label + " ANIM");
	//params.setName("ANIMATOR");
	//params.setName("Float Animator");
	//params.setName(label);

	params.add(bEnableAnimator);
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
	params.add(bGui);
	//params.add(anim_loop);

	ofAddListener(params.parameterChangedE(), this, &FloatAnimator::Changed_Params);
	ofAddListener(params_Bpm.parameterChangedE(), this, &FloatAnimator::Changed_Params);

	//--

	//// gui

	//gui.setup(label);
	//gui.setPosition(guiPos.x, guiPos.y - 20);
	//gui.add(params);
	//gui.add(SHOW_Plot);

	//--

	//#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	setupGui();
	//#endif

	//--

	////autoSettings = true;
	//if (autoSettings) ofxSurfingHelpers::loadGroup(params, path_GLOBAL_Folder + "/" + path_Settings);

	ofAddListener(floatAnimator.animFinished, this, &FloatAnimator::Changed_AnimatorDone);

	//-

	// startup
	repeatName = AnimRepeat_ToStr(repeatMode.get());
	curveName = floatAnimator.getCurveName(AnimCurve(curveType.get()));

	curveNamesList = getAllCurveNames();

	//--

	// plot
	//plotShape = ImVec2(210, (float)PLOT_BOXES_SIZES + 20);
	plotShape = ImVec2(210, 140);
	ofFbo::Settings fboSettings;
	fboSettings.width = plotShape.x;
	fboSettings.height = plotShape.y;
	fboSettings.internalformat = GL_RGBA;
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fboPlot.allocate(fboSettings);

	//--

#ifdef USE_SURFING_PRESETS
	presets.setPathPresets(path_GLOBAL_Folder + "/Presets/" + params.getName());
	presets.addGroup(params);
	//presets.addGroup(getParameters());
#endif

	//--

	//setupExtra(); // -> sub classes can add more extra parameters to params here!

	//startup(); // -> load settings
}

//--------------------------------------------------------------
void FloatAnimator::setupExtra()
{
	ofLogNotice(__FUNCTION__);

	/*

	You can define this into your heritated sub class to add parameters to the settings
	that we want to handle and serialize to json.

	*/

	startup();
}

//--------------------------------------------------------------
void FloatAnimator::startup()
{
	ofLogNotice(__FUNCTION__);

	loadSettings();
}

//--------------------------------------------------------------
void FloatAnimator::start()
{
	ofLogVerbose(__FUNCTION__);

	if (bEnableAnimator)
	{
		floatAnimator.reset(valueStart);
		if (animDelay != 0.f) floatAnimator.animateToAfterDelay(valueEnd, animDelay);
		else floatAnimator.animateTo(valueEnd);
	}
}

//--------------------------------------------------------------
void FloatAnimator::stop()
{
	ofLogVerbose(__FUNCTION__);

	if (bEnableAnimator)
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
	//    if (bEnableAnimator)
	//        valueBack->set(floatAnimator.getCurrentColor());
	//    else
	//        valueBack->set(valueStart);
	//}


#ifdef USE_SURFING_PRESETS
	if (presets.isRetrigged())
	{
		start();
	}
#endif

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

	//-

	if (SHOW_Plot && bGui)
	{
		//drawPlot();

		fboPlot.begin();
		{
			ofClear(0, 0);
			drawPlot();
		}
		fboPlot.end();
	}
}

//--------------------------------------------------------------
void FloatAnimator::draw(ofEventArgs & args)
{
	if (!bGui) return;

	{
		//#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
		guiManager.begin();
		{
			drawImGuiWidgets();
		}
		guiManager.end();
		//#endif
	}
}

//--------------------------------------------------------------
void FloatAnimator::drawPlot() {

	// curve type plot

	ofPushStyle();
	ofFill();

	float x, y, size, px, w;
	size = 100;
	bool stateColor;
	string str;

	// a.
	//bCustomPositionPlot = !bGui;
	//if (bCustomPositionPlot) {
	//	x = positionPlot.x;
	//	y = positionPlot.y;
	//}
	//else {
	//	//x = positionGuiLayout.get().x + 45;
	//	x = positionGuiLayout.get().x + widthGuiLayout / 2 - (size + 19) / 2;
	//	y = positionGuiLayout.get().y + heightGuiLayout + pad;
	//	//x = gui.getPosition().x + 45;
	//	//y = gui.getPosition().y + gui.getHeight() + pad;
	//}

	// b.
	x = 0;
	y = 20;

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

//--------------------------------------------------------------
void FloatAnimator::drawImGuiWidgetsExtra() {
	/*

	To be feeded into heritated classes.

	*/

	//float _w100 = ofxImGuiSurfing::getWidgetsWidth(1);
	//float _h = ofxImGuiSurfing::getWidgetsHeightUnit();
	//ImGui::Button("TEST_ExtraF", ImVec2(_w100, 2 * _h));
}

//--------------------------------------------------------------
void FloatAnimator::drawImGuiWidgetsBegin() {
	{
		ImGuiWindowFlags _flagsw = ImGuiWindowFlags_None;
		if (guiManager.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		string name;
		//name = "ANIMATOR";
		name = label;
		//name = "PANEL " + label;
		panelName = name;

		ImGuiCond flagCond = ImGuiCond_FirstUseEver;
		ImGui::SetNextWindowPos(ImVec2(100, 100), flagCond);
		ImGui::SetNextWindowSize(ImVec2(200, 600), flagCond);

		bOpened = guiManager.beginWindow(name.c_str(), (bool*)&bGui.get(), _flagsw);

		//--

		if (bOpened)
		{
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

			//static ImGuiTreeNodeFlags flagst;
			//flagst = ImGuiTreeNodeFlags_None;
			////flagst |= ImGuiTreeNodeFlags_DefaultOpen;
			//flagst |= ImGuiTreeNodeFlags_Framed;

			//AddToggleRoundedButton(guiManager.bMinimize);
			guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

			guiManager.Add(bEnableAnimator, OFX_IM_TOGGLE_SMALL);

			if (ImGui::Button("START", ImVec2(_w100, 3 * _h))) {
				start();
			}

			if (repeatMode == 2 || repeatMode == 3 || repeatMode == 5)//with a back mode. force to start
				if (ImGui::Button("STOP", ImVec2(_w100, _h))) {
					stop();
				}
		}
		else
		{
			guiManager.endWindow();
		}
	}
}

//--------------------------------------------------------------
void FloatAnimator::drawImGuiWidgetsEnd() {
	//if (!bOpened) return;

	//--

	{
		string name;

		ImGuiWindowFlags _flagsw = ImGuiWindowFlags_None;
		if (guiManager.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		static ImGuiTreeNodeFlags flagst;
		flagst = ImGuiTreeNodeFlags_None;
		//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
		flagst |= ImGuiTreeNodeFlags_Framed;

		//-

		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

#ifdef USE_SURFING_PRESETS
		//bool bOpen = true;
		//ImGuiTreeNodeFlags _flagt2 = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
		//_flagt2 |= ImGuiTreeNodeFlags_Framed;
		//if (ImGui::TreeNodeEx("PRESETS", _flagt2))
		{
			presets.draw_ImGui_Minimal();
			//ImGui::TreePop();
		}
#endif


		//-

		flagst = ImGuiTreeNodeFlags_None;
		if (!guiManager.bMinimize) {
			flagst |= ImGuiTreeNodeFlags_DefaultOpen;
		}
		flagst |= ImGuiTreeNodeFlags_Framed;

		if (ImGui::CollapsingHeader("DURATION", flagst))
		{
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

			guiManager.Add(bpmMode, OFX_IM_TOGGLE_BIG);
			//ofxImGuiSurfing::AddBigToggle(bpmMode, _w100, _h);

			if (!bpmMode)
			{
				guiManager.Add(duration);
				guiManager.Add(animDelay);
			}
			else
			{
				//if (!guiManager.bMinimize) {
				//	guiManager.Add(duration, OFX_IM_DRAG);
				//	guiManager.Add(animDelay, OFX_IM_DRAG);
				//}
			}

			if (bpmMode) {

				guiManager.Add(bpmSpeed, OFX_IM_DRAG);

				if (!guiManager.bMinimize) {
					if (ImGui::Button("HALF", ImVec2(_w50, _h))) {
						bpmSpeed = bpmSpeed / 2.0f;
					}
					ImGui::SameLine();
					if (ImGui::Button("DOUBLE", ImVec2(_w50, _h))) {
						bpmSpeed = bpmSpeed * 2.0f;
					}
				}
				guiManager.Add(bpmBeatDuration);
				guiManager.Add(bpmBeatDelay);
				if (!guiManager.bMinimize) guiManager.Add(bpmSlow);
			}

			if (!guiManager.bMinimize) {
				if (ImGui::Button("Reset Time", ImVec2(_w100, 2*_h)))
				{
					bpmSpeed = 120;
					animDelay = 0.f;
					duration = 1.f;
					bpmMode = true;
					bpmSlow = false;
					bpmBeatDelay = 0;
					bpmBeatDuration = 8;
				}
			}
		}

		//-

		// Animator group

		flagst = ImGuiTreeNodeFlags_None;
		flagst |= ImGuiTreeNodeFlags_Framed;
		//flagst |= ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::CollapsingHeader("CURVE", flagst))
		{
			ImGui::PushItemWidth(_w100 - 80);
			ofxImGuiSurfing::AddCombo(curveType, curveNamesList);
			ImGui::PopItemWidth();

			if (!guiManager.bMinimize) {

				if (ImGui::Button("<", ImVec2(_w50, _h))) {
					previousCurve();
				}
				ImGui::SameLine();
				if (ImGui::Button(">", ImVec2(_w50, _h))) {
					nextCurve();
				}

				//-

				guiManager.Add(curveType);
				guiManager.Add(repeatMode);

				ImGui::Text(repeatName.get().c_str());
				if (repeatMode == 4 || repeatMode == 5)
				{
					guiManager.Add(repeatTimes);
				}
			}
		}

		//-

		if (!guiManager.bMinimize) {

			flagst = ImGuiTreeNodeFlags_None;
			flagst |= ImGuiTreeNodeFlags_Framed;
			//flagst |= ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::CollapsingHeader("RANGE", flagst))
			{
				guiManager.Add(valueStart);
				guiManager.Add(valueEnd);
			}

			if (ImGui::CollapsingHeader("MONITOR", flagst))
			{
				guiManager.Add(animProgress, OFX_IM_INACTIVE);
				guiManager.Add(value);
			}
		}

		//--

		ImGui::Dummy(ImVec2(0.0f, 2.0f));

		guiManager.Add(reset, OFX_IM_TOGGLE_BIG);
		//guiManager.Add(reset, OFX_IM_TOGGLE_SMALL);
		//ofxImGuiSurfing::AddBigToggle(reset, _w100, _h, false);

		////bundle
		//ImGui::PushItemWidth(_w100 - WIDGET_PARAM_PADDING);
		//ofxImGuiSurfing::AddGroup(params, flagst);
		//ImGui::PopItemWidth();

		ImGui::Spacing();

		//--

		// plot fbo
		if (!guiManager.bMinimize) 
		{
			if (SHOW_Plot)
			{
				//widthGuiLayout = ImGui::GetWindowWidth();
				//float _spacing = widthGuiLayout / 2 - (plotShape.x / 2);
				//ImGui::Indent(_spacing);
				{
					ImTextureID textureID = (ImTextureID)(uintptr_t)fboPlot.getTexture().getTextureData().textureID;
					ImGui::Image(textureID, plotShape);
				}
				//ImGui::Unindent;
			}
		}

		//-

		if (!guiManager.bMinimize)
		{
			flagst = ImGuiTreeNodeFlags_None;
			//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
			flagst |= ImGuiTreeNodeFlags_Framed;

			//ImGui::Spacing();
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			guiManager.Add(guiManager.bExtra, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
			//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);

			if (guiManager.bExtra)
			{
				ImGui::Indent();
				{
					guiManager.refreshLayout();
					//ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

					guiManager.Add(SHOW_Plot, OFX_IM_TOGGLE_SMALL);
					guiManager.Add(ModeBrowse, OFX_IM_TOGGLE_SMALL);

					//-

					if (guiManager.bExtra) guiManager.drawAdvanced();
				}
				ImGui::Unindent();
			}
		}

		//--

		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void FloatAnimator::drawImGuiWidgets() {
	if (!bGui) return;

	{
		drawImGuiWidgetsBegin();

		if (!bOpened) return;

		// NOTE:
		// this splitted section is to insert custom widget for different animator types
		// that we will derive from this float class
		// It's an attempt to call methods from parent scope
		// That's to to add other widgets

		drawImGuiWidgetsExtra();

		drawImGuiWidgetsEnd();
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

		//bCustomPositionPlot = !bGui;
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
	ofRemoveListener(params.parameterChangedE(), this, &FloatAnimator::Changed_Params);
	ofRemoveListener(params_Bpm.parameterChangedE(), this, &FloatAnimator::Changed_Params);

	ofRemoveListener(floatAnimator.animFinished, this, &FloatAnimator::Changed_AnimatorDone);

	ofRemoveListener(ofEvents().update, this, &FloatAnimator::update);
	ofRemoveListener(ofEvents().draw, this, &FloatAnimator::draw, OF_EVENT_ORDER_AFTER_APP);

	exit();
}

//--------------------------------------------------------------
void FloatAnimator::exit()
{
	ofLogNotice(__FUNCTION__);
	saveSettings();
}

// settings
//--------------------------------------------------------------
void FloatAnimator::saveSettings()
{
	ofLogNotice(__FUNCTION__);
	if (autoSettings) ofxSurfingHelpers::saveGroup(params, path_GLOBAL_Folder + "/" + path_Settings);
}
//--------------------------------------------------------------
void FloatAnimator::loadSettings()
{
	ofLogNotice(__FUNCTION__);
	if (autoSettings) ofxSurfingHelpers::loadGroup(params, path_GLOBAL_Folder + "/" + path_Settings);
}

//--------------------------------------------------------------
void FloatAnimator::Changed_Params(ofAbstractParameter &e)
{
	string name = e.getName();
	if (name != "%") ofLogVerbose(__FUNCTION__) << name << " : " << e;

	//-

	if (false) {}

	else if (name == bEnableAnimator.getName())
	{
		if (!bEnableAnimator && floatAnimator.isAnimating())
			floatAnimator.pause();
		else if (bEnableAnimator && !floatAnimator.isAnimating())
			floatAnimator.resume();
	}

	// tween duration
	else if (name == duration.getName())
	{
		floatAnimator.setDuration(duration.get());
	}

	// bpm engine
	else if (name == bpmBeatDuration.getName() || name == bpmSpeed.getName() || name == bpmBeatDelay.getName())
	{
		if (bpmMode) {
			float _bar = 60.f / bpmSpeed.get();//one bar duration in seconds to this bpm speed
			duration = (_bar / 8.f) * (float)bpmBeatDuration;
			animDelay = (_bar / 8.f) * (float)bpmBeatDelay;

			if (bpmSlow) {
				_bar *= 2;
				duration *= 2;
				animDelay *= 2;
			}
		}
	}

	// bpm engine
	else if (name == bpmSlow.getName())
	{
		if (bpmMode) {
			float _bar = 60.f / bpmSpeed.get();//one bar duration in seconds to this bpm speed
			duration = (_bar / 8.f) * (float)bpmBeatDuration;
			animDelay = (_bar / 8.f) * (float)bpmBeatDelay;

			if (bpmSlow) {
				_bar *= 2;
				duration *= 2;
				animDelay *= 2;
			}
		}
	}

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

			if (bpmSlow) {
				_bar *= 2;
				duration *= 2;
				animDelay *= 2;
			}
		}

		//// gui workflow
		//gui.getGroup(params.getName()).getGroup(params_Time.getName()).minimize();
		//gui.getGroup(params.getName()).getGroup(params_Bpm.getName()).minimize();
		//if (!bpmMode) gui.getGroup(params.getName()).getGroup(params_Time.getName()).maximize();
		//else gui.getGroup(params.getName()).getGroup(params_Bpm.getName()).maximize();
	}

	else if (name == curveType.getName())
	{
		floatAnimator.setCurve(AnimCurve(curveType.get()));
		curveName = floatAnimator.getCurveName(AnimCurve(curveType.get()));

		if (ModeBrowse) {
			start();
		}
	}
	else if (name == repeatMode.getName())
	{
		repeatName = AnimRepeat_ToStr(repeatMode.get());
		floatAnimator.setRepeatType(AnimRepeat(repeatMode.get()));
	}
	else if (name == repeatTimes.getName())
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
	else if (name == reset.getName())
	{
		if (reset)
		{
			reset = false;

			// standard

			//repeatMode = 0;
			repeatMode = 0;// play once
			//repeatMode = 3;// back and forth once
			curveType = 3;
			repeatTimes = 1;

			bpmSpeed = 120;
			animDelay = 0.f;
			duration = 1.f;
			bpmMode = true;
			bpmSlow = false;
			bpmBeatDelay = 0;
			bpmBeatDuration = 8;

			valueStart = 0.f;
			valueEnd = 1.f;
		}
	}

	// value
	else if (name == "Value")
	{
	}
	else if (name == valueStart.getName())
	{
		//floatAnimator.setColor(valueStart);
		//if (bEnableAnimator && floatAnimator.isAnimating())
		//{
		if (ModeBrowse) start();
		//}
	}
	else if (name == valueEnd.getName())
	{
		//floatAnimator.setColor(valueEnd);
		//if (bEnableAnimator && floatAnimator.isAnimating())
		//{
		if (ModeBrowse) start();
		//}
	}

	//-

//	else if (name == bGui.getName() && !bGui)
//	{
//#ifdef USE_SURFING_PRESETS
//		presets.bGui = false;
//#endif
//	}

	//-

	// workflow

#ifdef USE_SURFING_PRESETS
	else if (name == bGui.getName() && !bGui)
	{
		presets.bGui = false;
}
#endif
}