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

//--------------------------------------------------------------
void FloatAnimator::setup()
{
	//TODO:
	//guiManager.bAutoResize = false;

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
	bpmSlow.set("Slow", false);
	bpmSpeed.set("BPM", 120.f, 10.f, 400.f);
	bpmBeatDuration.set("Beat Duration", 4, 1, 8);
	bpmBeatDelay.set("Beat Delay", 0, 0, 8);
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
	params_Control.add(ENABLE_valueAnim);
	//params_Control.add(bpmMode);
	//params_Control.add(bpmSpeed);
	////params_Control.add(_params_Bpm);
	////params_Control.add(duration);

	params_Helpers.setName("Animator Helpers");
	params_Helpers.add(ModeBrowse.set("Mode Browser", true));
	params_Helpers.add(SHOW_Plot);

	//--

	// settings
	params.setName(label);
	//params.setName(label + " ANIM");
	//params.setName("ANIMATOR");
	//params.setName("Float Animator");
	//params.setName(label);

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
	params.add(SHOW_Gui);
	//params.add(anim_loop);

	ofAddListener(params.parameterChangedE(), this, &FloatAnimator::Changed_Params);
	ofAddListener(params_Bpm.parameterChangedE(), this, &FloatAnimator::Changed_Params);

	//-

	//// gui
	//gui.setup(label);
	//gui.setPosition(guiPos.x, guiPos.y - 20);
	//gui.add(params);
	//gui.add(SHOW_Plot);

	//-

	// gui
#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	//guiManager.setImGuiAutodraw(false);//? TODO: improve multicontext mode..
	guiManager.setup(); // initiate ImGui
	//guiManager.setUseAdvancedSubPanel(true);
#endif

	//-

	//autoSettings = true;
	if (autoSettings) ofxSurfingHelpers::loadGroup(params, path_GLOBAL_Folder + "/" + path_Settings);

	ofAddListener(floatAnimator.animFinished, this, &FloatAnimator::Changed_AnimatorDone);

	//-

	// startup
	repeatName = AnimRepeat_ToStr(repeatMode.get());
	curveName = floatAnimator.getCurveName(AnimCurve(curveType.get()));

	curveNamesList = getAllCurveNames();

	//--

	// plot
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
}

//--------------------------------------------------------------
void FloatAnimator::start()
{
	ofLogVerbose(__FUNCTION__);

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
	ofLogVerbose(__FUNCTION__);

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

	if (SHOW_Plot && SHOW_Gui)
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
	if (!SHOW_Gui) return;

	{
#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
		guiManager.begin();
		{
			drawImGuiWidgets();
		}
		guiManager.end();
#endif
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
	//bCustomPositionPlot = !SHOW_Gui;
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

	float _w100 = ofxImGuiSurfing::getWidgetsWidth(1);
	float _h = ofxImGuiSurfing::getWidgetsHeightUnit();
	ImGui::Button("TEST_ExtraF", ImVec2(_w100, 2 * _h));
}

//--------------------------------------------------------------
void FloatAnimator::drawImGuiWidgetsBegin() {
	// 1. window parameters
	//static bool bParams = true;
	//static bool bOpen = false;
	if (bParams)
	{
		string name;

		// widgets sizes
		float _w100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;

		ImGuiWindowFlags _flagsw = ImGuiWindowFlags_None;
		if (guiManager.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		//name = "PARAMETERS";
		//name = "ANIMATOR";
		name = "PANEL " + label;
		panelName = name;

		ImGuiCond flagCond = ImGuiCond_FirstUseEver;
		ImGui::SetNextWindowPos(ImVec2(100, 100), flagCond);
		ImGui::SetNextWindowSize(ImVec2(200, 600), flagCond);

		guiManager.beginWindow(name.c_str(), NULL, _flagsw);
		{
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

			static ImGuiTreeNodeFlags flagst;
			flagst = ImGuiTreeNodeFlags_None;
			//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
			flagst |= ImGuiTreeNodeFlags_Framed;

			if (ImGui::Button("START", ImVec2(_w100, 4 * _h))) {
				start();
			}

			//--
		}
	}
}

//--------------------------------------------------------------
void FloatAnimator::drawImGuiWidgetsEnd() {

	//--

	if (bParams)//??
	{
		string name;

		// widgets sizes
		float _w100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;

		ImGuiWindowFlags _flagsw = ImGuiWindowFlags_None;
		if (guiManager.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		static ImGuiTreeNodeFlags flagst;
		flagst = ImGuiTreeNodeFlags_None;
		//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
		flagst |= ImGuiTreeNodeFlags_Framed;

		//-

#ifdef USE_SURFING_PRESETS
				//bool bOpen = true;
				//ImGuiTreeNodeFlags _flagt2 = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				//_flagt2 |= ImGuiTreeNodeFlags_Framed;
				//if (ImGui::TreeNodeEx("PRESETS", _flagt2))
		{
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
			presets.draw_ImGui_Minimal();
			//ImGui::TreePop();
		}
#endif


		//-

		flagst = ImGuiTreeNodeFlags_None;
		flagst |= ImGuiTreeNodeFlags_DefaultOpen;
		flagst |= ImGuiTreeNodeFlags_Framed;

		if (ImGui::CollapsingHeader("DURATION", flagst))
		{
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

			if (!bpmMode)
			{
				guiManager.Add(duration);
				guiManager.Add(animDelay);
				//ofxImGuiSurfing::AddParameter(duration);
				//ofxImGuiSurfing::AddParameter(animDelay);
			}
			else
			{
				ofxImGuiSurfing::AddDragFloatSlider(duration);
				ofxImGuiSurfing::AddDragFloatSlider(animDelay);
			}

			ofxImGuiSurfing::AddBigToggle(bpmMode, _w100, _h);

			if (bpmMode) {

				ofxImGuiSurfing::AddDragFloatSlider(bpmSpeed);
				//float _bpmSpeed = bpmSpeed.get();
				//ImGui::PushID(1);
				//if(ImGui::DragFloat("BPM", &_bpmSpeed)) {
				//	bpmSpeed.set(_bpmSpeed);
				//}
				//ImGui::PopID();

				//ImGui::PushID(2);
				//ofxImGuiSurfing::AddParameter(bpmSpeed);
				//ImGui::PopID();

				if (ImGui::Button("HALF", ImVec2(_w50, _h))) {
					bpmSpeed = bpmSpeed / 2.0f;
				}
				ImGui::SameLine();
				if (ImGui::Button("DOUBLE", ImVec2(_w50, _h))) {
					bpmSpeed = bpmSpeed * 2.0f;
				}

				ofxImGuiSurfing::AddBigToggle(bpmSlow, _w100, _h);
				//ofxImGuiSurfing::AddParameter(bpmSlow);

				guiManager.Add(bpmBeatDuration);
				guiManager.Add(bpmBeatDelay);
				//ofxImGuiSurfing::AddParameter(bpmBeatDuration);
				//ofxImGuiSurfing::AddParameter(bpmBeatDelay);
			}
			//else {
			//	ofxImGuiSurfing::AddParameter(animDelay);
			//	ofxImGuiSurfing::AddParameter(duration);
			//}
			if (ImGui::Button("Reset Time", ImVec2(_w100, _h))) {
				bpmSpeed = 120;
				animDelay = 0.f;
				duration = 1.f;
				bpmMode = true;
				bpmSlow = false;
				bpmBeatDelay = 0;
				bpmBeatDuration = 8;
			}
		}

		//-

		// animator group

		//-

		//ofxImGuiSurfing::AddParameter(bpmSpeed);
		//ofxImGuiSurfing::AddBigToggle(bpmMode, _w100, _h);
		//ofxImGuiSurfing::AddParameter(duration);
		//ofxImGuiSurfing::AddParameter(animDelay);
		//ofxImGuiSurfing::AddParameter(bpmBeatDuration);
		//ofxImGuiSurfing::AddParameter(bpmBeatDelay);

		flagst = ImGuiTreeNodeFlags_None;
		//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
		flagst |= ImGuiTreeNodeFlags_Framed;

		if (ImGui::CollapsingHeader("CURVE", flagst))
		{
			//ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
			//ImGui::Text("CURVE:");
			//ofxImGuiSurfing::AddParameter(curveName);
			//ImGui::Text(curveName.get().c_str());
			ImGui::PushItemWidth(_w100 - 80);
			ofxImGuiSurfing::AddCombo(curveType, curveNamesList);
			ImGui::PopItemWidth();

			if (ImGui::Button("<", ImVec2(_w50, _h))) {
				previousCurve();
			}
			ImGui::SameLine();
			if (ImGui::Button(">", ImVec2(_w50, _h))) {
				nextCurve();
			}
			//ImGui::Dummy(ImVec2(0.0f, 2.0f));

			//-

			guiManager.Add(curveType);
			////ofxImGuiSurfing::AddParameter(curveName);
			//ImGui::Text(curveName.get().c_str());

			guiManager.Add(repeatMode);
			//ofxImGuiSurfing::AddParameter(repeatMode);
			//ofxImGuiSurfing::AddParameter(repeatName);
			ImGui::Text(repeatName.get().c_str());
			if (repeatMode == 4 || repeatMode == 5)
				ofxImGuiSurfing::AddParameter(repeatTimes);
		}

		//-

		flagst = ImGuiTreeNodeFlags_None;
		//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
		flagst |= ImGuiTreeNodeFlags_Framed;

		if (ImGui::CollapsingHeader("RANGE", flagst))
		{
			guiManager.Add(valueStart);
			guiManager.Add(valueEnd);
			//ofxImGuiSurfing::AddParameter(valueStart);
			//ofxImGuiSurfing::AddParameter(valueEnd);
		}

		if (ImGui::CollapsingHeader("MONITOR", flagst))
		{
			guiManager.Add(animProgress);
			guiManager.Add(value);
			//ofxImGuiSurfing::AddParameter(animProgress);
			//ofxImGuiSurfing::AddParameter(value);
			ImGui::Dummy(ImVec2(0.0f, 2.0f));
		}

		//--

		//ofxImGuiSurfing::AddParameter(animProgress);

		ImGui::Dummy(ImVec2(0.0f, 2.0f));
		ofxImGuiSurfing::AddBigToggle(reset, _w100, _h, false);
		//ofxImGuiSurfing::AddParameter(reset);
		//ofxSurfingHelpers::AddBigButton(reset, _w100, _h);

		////bundle
		//ImGui::PushItemWidth(_w100 - WIDGET_PARAM_PADDING);
		//ofxImGuiSurfing::AddGroup(params, flagst);
		//ImGui::PopItemWidth();

		ImGui::Spacing();

		//--

		// plot fbo

		if (SHOW_Plot)
		{
			widthGuiLayout = ImGui::GetWindowWidth();
			float _spacing = widthGuiLayout / 2 - (plotShape.x / 2);
			ImGui::Indent(_spacing);
			ImTextureID textureID = (ImTextureID)(uintptr_t)fboPlot.getTexture().getTextureData().textureID;
			ImGui::Image(textureID, plotShape);
			ImGui::Unindent;
		}

		//-

		flagst = ImGuiTreeNodeFlags_None;
		//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
		flagst |= ImGuiTreeNodeFlags_Framed;

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);
		//if (ImGui::CollapsingHeader("EXTRA", flagst))
		if (guiManager.bExtra)
		{
			ImGui::Indent();

			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

			ofxImGuiSurfing::AddBigToggle(SHOW_Plot, _w100, _h, false);
			ofxImGuiSurfing::AddBigToggle(ModeBrowse, _w100, _h, false);
			//ofxImGuiSurfing::AddBigToggle(reset, _w100, _h, false);

			//-

			ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAdvanced);
			if (guiManager.bExtra) guiManager.drawAdvancedSubPanel();

			ImGui::Unindent();
		}

		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void FloatAnimator::drawImGuiWidgets() {
	{
		drawImGuiWidgetsBegin();

#ifdef TEST_HERITAGE_FUNCTIONS 
		// NOTE:
		// this splitted section is to insert custom widget for different animator types
		// that we will derive from this float class
		// It's an attempt to call methods from parent scope
		// That's to to add other widgets
		drawImGuiWidgetsExtra();
#endif

		drawImGuiWidgetsEnd();
	}
}

/*
//--------------------------------------------------------------
void FloatAnimator::drawImGuiWidgets() {
	{
		string name;

		// widgets sizes
		float _w100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;

		ImGuiWindowFlags _flagsw = ImGuiWindowFlags_None;
		if (guiManager.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		// 1. window parameters
		static bool bParams = true;
		//static bool bOpen = false;
		if (bParams)
		{
			//name = "PARAMETERS";
			//name = "ANIMATOR";
			name = "PANEL " + label;
			panelName = name;
			guiManager.beginWindow(name.c_str(), NULL, _flagsw);
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

				static ImGuiTreeNodeFlags flagst;
				flagst = ImGuiTreeNodeFlags_None;
				//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
				flagst |= ImGuiTreeNodeFlags_Framed;

				if (ImGui::Button("START", ImVec2(_w100, _h))) {
					start();
				}

				//-

#ifdef USE_SURFING_PRESETS
				//bool bOpen = true;
				//ImGuiTreeNodeFlags _flagt2 = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				//_flagt2 |= ImGuiTreeNodeFlags_Framed;
				//if (ImGui::TreeNodeEx("PRESETS", _flagt2))
				{
					ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
					presets.draw_ImGui_Minimal();
					//ImGui::TreePop();
				}
#endif


				//-

				flagst = ImGuiTreeNodeFlags_None;
				flagst |= ImGuiTreeNodeFlags_DefaultOpen;
				flagst |= ImGuiTreeNodeFlags_Framed;

				if (ImGui::CollapsingHeader("DURATION", flagst))
				{
					ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

					ImGui::PushItemWidth(_w100 - WIDGET_PARAM_PADDING);
					if (!bpmMode) {
						ofxImGuiSurfing::AddParameter(duration);
						ofxImGuiSurfing::AddParameter(animDelay);
					}
					else
					{
						ofxImGuiSurfing::AddDragFloatSlider(duration);
						ofxImGuiSurfing::AddDragFloatSlider(animDelay);
					}
					ImGui::PopItemWidth();

					ofxImGuiSurfing::AddBigToggle(bpmMode, _w100, _h);

					if (bpmMode) {

						ofxImGuiSurfing::AddDragFloatSlider(bpmSpeed);
						//float _bpmSpeed = bpmSpeed.get();
						//ImGui::PushID(1);
						//if(ImGui::DragFloat("BPM", &_bpmSpeed)) {
						//	bpmSpeed.set(_bpmSpeed);
						//}
						//ImGui::PopID();

						//ImGui::PushID(2);
						//ofxImGuiSurfing::AddParameter(bpmSpeed);
						//ImGui::PopID();

						if (ImGui::Button("HALF", ImVec2(_w50, _h))) {
							bpmSpeed = bpmSpeed / 2.0f;
						}
						ImGui::SameLine();
						if (ImGui::Button("DOUBLE", ImVec2(_w50, _h))) {
							bpmSpeed = bpmSpeed * 2.0f;
						}

						ofxImGuiSurfing::AddBigToggle(bpmSlow, _w100, _h);
						//ofxImGuiSurfing::AddParameter(bpmSlow);

						ImGui::PushItemWidth(_w100 - WIDGET_PARAM_PADDING);
						ofxImGuiSurfing::AddParameter(bpmBeatDuration);
						ofxImGuiSurfing::AddParameter(bpmBeatDelay);
						ImGui::PopItemWidth();
					}
					//else {
					//	ofxImGuiSurfing::AddParameter(animDelay);
					//	ofxImGuiSurfing::AddParameter(duration);
					//}
					if (ImGui::Button("Reset Time", ImVec2(_w100, _h))) {
						bpmSpeed = 120;
						animDelay = 0.f;
						duration = 1.f;
						bpmMode = true;
						bpmSlow = false;
						bpmBeatDelay = 0;
						bpmBeatDuration = 8;
					}
				}

				//-

				// animator group

				//-

				//ofxImGuiSurfing::AddParameter(bpmSpeed);
				//ofxImGuiSurfing::AddBigToggle(bpmMode, _w100, _h);
				//ofxImGuiSurfing::AddParameter(duration);
				//ofxImGuiSurfing::AddParameter(animDelay);
				//ofxImGuiSurfing::AddParameter(bpmBeatDuration);
				//ofxImGuiSurfing::AddParameter(bpmBeatDelay);

				flagst = ImGuiTreeNodeFlags_None;
				//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
				flagst |= ImGuiTreeNodeFlags_Framed;

				if (ImGui::CollapsingHeader("CURVE", flagst))
				{
					ImGui::PushItemWidth(_w100 - WIDGET_PARAM_PADDING);

					//ImGui::Text("CURVE:");
					//ofxImGuiSurfing::AddParameter(curveName);
					//ImGui::Text(curveName.get().c_str());
					ofxImGuiSurfing::AddCombo(curveType, curveNamesList);

					if (ImGui::Button("<", ImVec2(_w50, _h))) {
						previousCurve();
					}
					ImGui::SameLine();
					if (ImGui::Button(">", ImVec2(_w50, _h))) {
						nextCurve();
					}
					//ImGui::Dummy(ImVec2(0.0f, 2.0f));

					//-

					ofxImGuiSurfing::AddParameter(curveType);
					//ofxImGuiSurfing::AddCombo(curveType, curveNamesList);
					////ofxImGuiSurfing::AddParameter(curveName);
					//ImGui::Text(curveName.get().c_str());

					ofxImGuiSurfing::AddParameter(repeatMode);
					//ofxImGuiSurfing::AddParameter(repeatName);
					ImGui::Text(repeatName.get().c_str());
					if (repeatMode == 4 || repeatMode == 5)
						ofxImGuiSurfing::AddParameter(repeatTimes);
					ImGui::PopItemWidth();
				}

				//-

				flagst = ImGuiTreeNodeFlags_None;
				//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
				flagst |= ImGuiTreeNodeFlags_Framed;

				if (ImGui::CollapsingHeader("RANGE", flagst))
				{
					ImGui::PushItemWidth(_w100 - WIDGET_PARAM_PADDING);
					ofxImGuiSurfing::AddParameter(valueStart);
					ofxImGuiSurfing::AddParameter(valueEnd);
					ImGui::PopItemWidth();
				}

				if (ImGui::CollapsingHeader("MONITOR", flagst))
				{
					//ImGui::PushItemWidth(_w100 - WIDGET_PARAM_PADDING);
					ImGui::PushItemWidth(_w100 - 50);
					ofxImGuiSurfing::AddParameter(animProgress);
					ofxImGuiSurfing::AddParameter(value);
					ImGui::PopItemWidth();
					ImGui::Dummy(ImVec2(0.0f, 2.0f));
				}

				//--

				//ofxImGuiSurfing::AddParameter(animProgress);

				ImGui::Dummy(ImVec2(0.0f, 2.0f));
				ofxImGuiSurfing::AddBigToggle(reset, _w100, _h, false);
				//ofxImGuiSurfing::AddParameter(reset);
				//ofxSurfingHelpers::AddBigButton(reset, _w100, _h);

				////bundle
				//ImGui::PushItemWidth(_w100 - WIDGET_PARAM_PADDING);
				//ofxImGuiSurfing::AddGroup(params, flagst);
				//ImGui::PopItemWidth();

				//-

				flagst = ImGuiTreeNodeFlags_None;
				//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
				flagst |= ImGuiTreeNodeFlags_Framed;

				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);
				if (guiManager.bExtra)
					//if (ImGui::CollapsingHeader("EXTRA", flagst))
				{
					ImGui::Indent();

					ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

					ofxImGuiSurfing::AddBigToggle(SHOW_Plot, _w100, _h, false);
					ofxImGuiSurfing::AddBigToggle(ModeBrowse, _w100, _h, false);
					//ofxImGuiSurfing::AddBigToggle(reset, _w100, _h, false);

					//-

					ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAdvanced);
					if (guiManager.bExtra) guiManager.drawAdvancedSubPanel();

					ImGui::Unindent();
				}

				//--

				// plot fbo

				if (SHOW_Plot)
				{
					widthGuiLayout = ImGui::GetWindowWidth();
					float _spacing = widthGuiLayout / 2 - (plotShape.x / 2);
					ImGui::Indent(_spacing);
					ImTextureID textureID = (ImTextureID)(uintptr_t)fboPlot.getTexture().getTextureData().textureID;
					ImGui::Image(textureID, plotShape);
					ImGui::Unindent;
				}
			}
			guiManager.endWindow();
		}
	}
}
*/

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
void FloatAnimator::Changed_Params(ofAbstractParameter &e)
{
	string name = e.getName();
	if (name != "%") ofLogVerbose(__FUNCTION__) << name << " : " << e;

	//-

	if (false) {}

	else if (name == ENABLE_valueAnim.getName())
	{
		if (!ENABLE_valueAnim && floatAnimator.isAnimating())
			floatAnimator.pause();
		else if (ENABLE_valueAnim && !floatAnimator.isAnimating())
			floatAnimator.resume();
	}

	//tween duration
	else if (name == duration.getName())
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

			if (bpmSlow) {
				_bar *= 2;
				duration *= 2;
				animDelay *= 2;
			}
		}
	}

	//bpm engine
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

	//value
	else if (name == "Value")
	{
	}
	else if (name == valueStart.getName())
	{
		//floatAnimator.setColor(valueStart);
		//if (ENABLE_valueAnim && floatAnimator.isAnimating())
		//{
		if (ModeBrowse) start();
		//}
	}
	else if (name == valueEnd.getName())
	{
		//floatAnimator.setColor(valueEnd);
		//if (ENABLE_valueAnim && floatAnimator.isAnimating())
		//{
		if (ModeBrowse) start();
		//}
	}

	//-

//	else if (name == SHOW_Gui.getName() && !SHOW_Gui)
//	{
//#ifdef USE_SURFING_PRESETS
//		presets.bGui = false;
//#endif
//	}

	//-

	// workflow

#ifdef USE_SURFING_PRESETS
	else if (name == SHOW_Gui.getName() && !SHOW_Gui)
	{
		presets.bGui = false;
	}
#endif
}