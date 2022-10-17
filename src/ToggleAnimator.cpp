#include "ToggleAnimator.h"

//--------------------------------------------------------------
void ToggleAnimator::onAnimQueueDone(ofxAnimatableQueue::EventArg &)
{
	ofLogNotice(__FUNCTION__) << "ToggleAnimator FINISHED";

	faderValue = faderMin.get();
	if (float_BACK != nullptr)
	{
		(*float_BACK) = faderValue;
	}

	animProgress = 100;

	if (faderLoop)
	{
		stop();
		start();
	}
}

//--------------------------------------------------------------
ToggleAnimator::ToggleAnimator()
{
	//ofSetLogLevel(OF_LOG_SILENT);
	doneInstantiated = true;
	setFps(60);
	SHOW_gui = true;

#ifndef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
	guiPos = glm::vec2(20, 100);
#endif

	bitThreshold = 0.5f;
}

//--------------------------------------------------------------
void ToggleAnimator::setup()
{
	ENABLE_ToggleModulator.set("ENABLE TOGGLER", true);
	faderLoop.set("LOOP", false);
	STATE_ToggleTarget.set("TARGET TOGGLE", false);

	faderValue.set("STATE VALUE", 0, 0, 1);
	faderMin.set("Min", 0, 0, 1);
	faderMax.set("Max", 1, 0, 1);

	faderDelay.set("PreDelay", 0.25f, 0, 2);
	faderAttack.set("Attack", 0.5f, 0, 2);
	faderSustain.set("Sustain", 1.f, 0, 2);
	faderRelease.set("Release", 0.5f, 0, 2);

	// bpm engine
	bpmMode.set("BPM Mode", true);
	bpmSpeed.set("BPM", 120.f, 10.f, 400.f);
	bpmBeatDelay.set("PreDelay Beat", 1, 0, 8);
	bpmBeatAttack.set("Attack Beat", 2, 0, 8);
	bpmBeatSustain.set("Sustain Beat", 4, 0, 8);
	bpmBeatRelease.set("Release Beat", 2, 0, 8);

	//-

	reset.set("Reset", false);
	curveType.set("Curve", AnimCurve(16), 0, NUM_ANIM_CURVES - 1);

	// 3 selected curves that blink on/off to avoid "float curved" ones
	blinkCurvesList[0] = 11;
	blinkCurvesList[1] = 10;
	blinkCurvesList[2] = 9;
	curveBlinkerType.set("Curve Blink", 0, 0, 2);

	curveName.set("", "");
	curveShow.set("Show Curve", true);
	animProgress.set("%", 0, 0, 100);

	// disable for xml serialize (not required)
	STATE_ToggleTarget.setSerializable(false);
	faderLoop.setSerializable(false);
	curveType.setSerializable(false);
	curveName.setSerializable(false);
	animProgress.setSerializable(false);
	reset.setSerializable(false);
	curveShow.setSerializable(false);
	faderValue.setSerializable(false);

	//-

	params.setName(label);
	//params.setName("FX MODULATOR");
	params.add(ENABLE_ToggleModulator);
	//params.add(faderLoop);
	params.add(STATE_ToggleTarget);
	params.add(faderValue);
	//params.add(faderMax);
	//params.add(faderMin);

	//params_Modulator.setName("MODULATOR");
	params_Timers.setName("TIMERS");
	params_Timers.add(faderDelay);
	params_Timers.add(faderAttack);
	params_Timers.add(faderSustain);
	params_Timers.add(faderRelease);
	params.add(params_Timers);
	//params_Modulator.add(params_Timers);

	params_Bpm.setName("BPM ENGINE");
	params_Bpm.add(bpmMode);
	params_Bpm.add(bpmSpeed);
	params_Bpm.add(bpmBeatDelay);
	params_Bpm.add(bpmBeatAttack);
	params_Bpm.add(bpmBeatSustain);
	params_Bpm.add(bpmBeatRelease);
	params.add(params_Bpm);
	//params_Modulator.add(params_Bpm);

	params.add(curveBlinkerType);
	params.add(curveType);
	params.add(curveName);
	//params.add(curveShow);
	params.add(reset);
	params.add(animProgress);

	//-

	// gui

#ifndef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
	gui.setup(label);
	//gui.setup("5 TOGGLE FX");
	gui.setPosition(guiPos.x, guiPos.y);
	gui.add(params);
	gui.add(SHOW_Plot);
#endif
	//-

#ifdef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
	ui.setImGuiAutodraw(true);//? TODO: improve multicontext mode..
	ui.setup(); // initiate ImGui

	//ui.setUseAdvancedSubPanel(true);
	//ui.bAutoResize = false;
#endif


	//-

	ofAddListener(params.parameterChangedE(), this, &ToggleAnimator::Changed_params);

	//-

	path = "settings_ToggleAnimator.xml";
	if (autoSettings)
		load_GroupSettings(params, path);

	curveName = ofxAnimatable::getCurveName(AnimCurve(curveType.get()));
	AnimCurve curve = (AnimCurve)(curveType.get());
	curvePlotable.setCurve(curve);

	setupFader();
	ofAddListener(queue.eventQueueDone, this, &ToggleAnimator::onAnimQueueDone);

	setupPlot();

	//--

#ifdef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
	int size = 92;
	ofFbo::Settings fboSettings2;
	fboSettings2.width = size + 10 + size;
	fboSettings2.height = size;
	fboSettings2.internalformat = GL_RGBA;
	fboSettings2.textureTarget = GL_TEXTURE_2D;
	fboPlot.allocate(fboSettings2);

	fboPlot.begin();
	ofClear(0, 0);
	fboPlot.end();
#endif

	//-

	// startup
	stop();
}

//--------------------------------------------------------------
void ToggleAnimator::setupFader()
{
	faderAnim = AnimCurve(curveType.get());

	queue.clearQueue();
	queue.setInitialValue(faderMin.get());
	queue.addTransition(faderMin.get(), faderDelay.get(), LINEAR);
	queue.addTransition(faderMax.get(), faderAttack.get(), faderAnim);
	queue.addTransition(faderMax.get(), faderSustain.get(), LINEAR);
	queue.addTransition(faderMin.get(), faderRelease.get(), faderAnim);

	totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
	//cout << "totalTime: " << totalTime << endl;

	//cout << "setupFader()" << endl;
	//cout << "totalTime   : " << totalTime << endl;
	//cout << "faderDelay  : " << faderDelay << endl;
	//cout << "faderAttack : " << faderAttack << endl;
	//cout << "faderSustain: " << faderSustain << endl;
	//cout << "faderRelease: " << faderRelease << endl;
}

//--------------------------------------------------------------
void ToggleAnimator::setupPlot()
{
	plot = new ofxHistoryPlot(NULL, "toggler", 100, false);
	plot->setBackgroundColor(ofColor(0, 255));
	plot->setShowNumericalInfo(false);
	plot->setRange(0, 1);
	plot->setRespectBorders(true);
	plot->setLineWidth(2);
	plot->setCropToRect(true);
	plot->setDrawGrid(false);
	plot->setShowSmoothedCurve(false);
	plot->setColor(255);
	plot->setPrecision(1);
	plot->setDrawTitle(false);
	//plot->setGridUnit(16);
	//plot->setGridColor(ofColor(22, 255));
}

//--------------------------------------------------------------
void ToggleAnimator::update()
{
	queue.update(dt);
	if (queue.isPlaying())
	{
		uint64_t time = ofGetElapsedTimeMillis() - lastStart;
		faderValue = queue.getCurrentValue();
		animProgress = ofMap(time / 1000.f, 0, totalTime / globalTimeScale, 0, 100, true);
		//cout << "faderValue:" << faderValue << endl;

		//---

		if (faderValue < 0.25f)
		{
			STATE_ToggleTarget = false;
		}
		else if (faderValue >= 0.25f)
		{
			STATE_ToggleTarget = true;
		}
	}

	plot->update(faderValue.get());
}

//--------------------------------------------------------------
void ToggleAnimator::drawPlot()
{
	if (SHOW_Plot && ENABLE_ToggleModulator)
	{
		ofPushStyle();

		int x, y, size, px;
		size = 90;
		string str;
		int pad = 6;

		bool stateColor;
		ofColor c;
		stateColor = (queue.isPlaying()) && (faderValue > bitThreshold);
		c = (stateColor ? ofColor(255) : ofColor(64));


#ifndef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
		//x = guiPos.x;
		//y = ofGetHeight() - size - 20;
		x = gui.getPosition().x;
		y = gui.getPosition().y + gui.getHeight() + 15;
#endif
		x = 0;
		y = 0;

		//1.1 live alpha plot
		plot->draw(x, y, size, size);

		//1.2 axis
		ofSetColor(ofColor(255, 32));
		ofDrawLine(x, y + size / 2, x + size, y + size / 2);//horizontal

		//2. hide curve plot when attack/release are 0
		if (faderRelease != 0 || faderAttack != 0)
		{
			//curve type for attack/release
			if (curveShow) curvePlotable.drawCurve(x + 1 * (size + pad), y, size, true, c);
		}

		//-

		if (isAnimating()) {
			//3. progress bar
			int rx, ry, rw, rh;
			rh = 10;
			rx = x + 0 * (size + pad);
			ry = y + size - rh;
			px = ofMap(animProgress, 0, 100, 0, size, true);
			rw = px;
			ofRectangle r(rx, ry, rw, rh);
			ofFill();
			//ofSetColor(c);
			ofSetColor(255, 225);
			ofDrawRectangle(r);
			//ofDrawLine(px, y, px, y + size);
		}

		//-

		//4. label
		//str = label;
		//ofDrawBitmapStringHighlight(str, rx + 4, y - 10,
		//          stateColor ? ofColor::white : ofColor::black,
		//          !stateColor ? ofColor::white : ofColor::black);

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ToggleAnimator::draw()
{
	update();

	//-

#ifdef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
	if (SHOW_Plot)
	{
		fboPlot.begin();
		{
			ofClear(0, 0);
			drawPlot();
		}
		fboPlot.end();
	}
#endif

	//--

	if (SHOW_gui)
	{
#ifndef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
		gui.draw();
#endif
		//-

#ifdef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
		ui.Begin();
		{
			drawImGuiWidgets();
		}
		ui.End();
#endif
	}
}

//--------------------------------------------------------------
void ToggleAnimator::start()
{
	ofLogNotice(__FUNCTION__);

	if (ENABLE_ToggleModulator)
	{
		//mark
		//plot->update(1);

		//TODO: rebuild every start
		setupFader();
		//stop();

		lastStart = ofGetElapsedTimeMillis();

		queue.setInitialValue(faderMin.get());
		queue.startPlaying(); //start the animation
	}
	else
	{
		faderValue = faderMax;
	}
}

//--------------------------------------------------------------
void ToggleAnimator::stop()
{
	ofLogNotice(__FUNCTION__);

	if (ENABLE_ToggleModulator)
	{
		faderValue = faderMin.get();
		queue.setInitialValue(faderMin.get());
		queue.pausePlayback(); //start the animation
		animProgress = 0;
	}
	else
	{
		faderValue = faderMax;
	}
}

//--------------------------------------------------------------
ToggleAnimator::~ToggleAnimator()
{
	ofRemoveListener(queue.eventQueueDone, this, &ToggleAnimator::onAnimQueueDone);
}

//--------------------------------------------------------------
void ToggleAnimator::exit()
{
	if (autoSettings)
		save_GroupSettings(params, path);
}

//--------------------------------------------------------------
void ToggleAnimator::load_GroupSettings(ofParameterGroup &g, string path)
{
	ofLogNotice("ToggleAnimator") << "load_GroupSettings " << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ToggleAnimator::save_GroupSettings(ofParameterGroup &g, string path)
{
	ofLogNotice("ToggleAnimator") << "save_GroupSettings " << path;
	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}

//--------------------------------------------------------------
void ToggleAnimator::nextCurve()
{
	curveType++;
	curveType = curveType % NUM_ANIM_CURVES;
}

//--------------------------------------------------------------
void ToggleAnimator::previousCurve()
{
	curveType--;
	if (curveType < 0)
		curveType = NUM_ANIM_CURVES - 1;
}

//--------------------------------------------------------------
void ToggleAnimator::Changed_params(ofAbstractParameter &e)
{
	string name = e.getName();
	if (name != "%"
		//&& name != "ENABLE TOGGLER"
		&& name != "TARGET TOGGLE"
		&& name != "STATE VALUE")
		ofLogNotice(__FUNCTION__) << "> '" << name << "': " << e;

	//-

	if (name == "Min" ||
		name == "Max" ||
		name == "Delay" ||
		name == "Attack" ||
		name == "Sustain" ||
		name == "Release")
	{
		totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
		//setupFader();
		//stop();
	}


	//bpm engine
	else if (name == bpmMode.getName() || name == bpmSpeed.getName() ||
		name == bpmBeatDelay.getName() ||
		name == bpmBeatAttack.getName() || name == bpmBeatSustain.getName() || name == bpmBeatRelease.getName())
	{
		if (bpmMode) {
			float _bar = 60.f / bpmSpeed.get();//one bar duration in seconds to this bpm speed
			float _ratio = 2.0f;
			faderDelay = (_bar / _ratio) * (float)bpmBeatDelay;
			faderAttack = (_bar / _ratio) * (float)bpmBeatAttack;
			faderSustain = (_bar / _ratio) * (float)bpmBeatSustain;
			faderRelease = (_bar / _ratio) * (float)bpmBeatRelease;
		}

		if (name == bpmMode.getName())
		{

#ifndef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
			//workflow
			auto &g2 = gui.getGroup(label);//1st level
			//auto &g2 = g1.getGroup("MODULATOR");//2nd level
			auto &g3 = g2.getGroup(params_Timers.getName());//3nd level
			auto &g4 = g2.getGroup(params_Bpm.getName());//3nd level
			g3.minimize();
			g4.minimize();
			if (bpmMode)
			{
				g3.minimize();
				g4.maximize();
			}
			else {
				g3.maximize();
				g4.minimize();
			}
#endif
	}
}

	else if (name == "Curve Blink")
	{
		int i = curveBlinkerType.get();
		if (i >= 0 && i < 3)
			curveType = blinkCurvesList[i];
	}
	else if (name == "Curve")
	{
		curveName = ofxAnimatable::getCurveName(AnimCurve(curveType.get()));
		AnimCurve curve = (AnimCurve)(curveType.get());
		curvePlotable.setCurve(curve);
	}

	else if (name == "Reset")
	{
		if (reset)
		{
			reset = false;

			faderValue = faderMax;
			STATE_ToggleTarget = false;

			faderDelay = 0.25f;
			faderAttack = 0.5f;
			faderSustain = 1.f;
			faderRelease = 0.5f;

			faderMin = 0;
			faderMax = 1;
			curveType = 3;
			//curveType = 10;//blink 3
		}
	}

	else if (name == "TARGET TOGGLE")
	{
		if (bool_BACK != nullptr) (*bool_BACK) = STATE_ToggleTarget;


		bool stateColor;
		ofColor c;
		stateColor = (queue.isPlaying()) && (faderValue > bitThreshold);
		c = (stateColor ? ofColor(255) : ofColor(64));

		plot->setColor(c);
	}

	else if (name == "STATE VALUE")
	{
		if (float_BACK != nullptr)
		{
			(*float_BACK) = faderValue;
		}

		//if (bool_BACK != nullptr)
		//{
		//    if (faderValue < 0.1)
		//    {
		//        (*bool_BACK) = false;
		//    }
		//    else if (faderValue > 0.9)
		//    {
		//        (*bool_BACK) = true;
		//    }
		//}
	}

	else if (name == "ENABLE TOGGLER")
	{
		if (!ENABLE_ToggleModulator)
		{
			faderValue = faderMax;
		}
	}

}

#ifdef USE_IMGUI_LAYOUT_MANAGER__TOGGLER
//--------------------------------------------------------------
void ToggleAnimator::drawImGuiWidgets()
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
	if (ui.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

	name = "PANEL " + label;
	if (ui.BeginWindow(name.c_str(), NULL, _flagsw))
	{
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

		static ImGuiTreeNodeFlags flagst;
		flagst = ImGuiTreeNodeFlags_None;
		//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
		flagst |= ImGuiTreeNodeFlags_Framed;

		if (ImGui::Button("START", ImVec2(_w100, 4 * _h))) {
			start();
		}

		//TODO: fails
		//ui.AddGroup(params, ImGuiTreeNodeFlags_None, OFX_IM_GROUP_DEFAULT);
		//ui.AddGroup(params, OFX_IM_GROUP_HIDDEN_HEADER);
		//ui.Add(SHOW_Plot);

		ofxImGuiSurfing::AddGroup(params, flagst);
		ofxImGuiSurfing::AddParameter(SHOW_Plot);

		//--

		// plot fbo

		if (SHOW_Plot)
		{
			ImVec2 pos = ImVec2(rectPlot.getWidth() / 2 - fboPlot.getWidth() / 2, 10);
			ImVec2 plotShape = ImVec2(fboPlot.getWidth(), fboPlot.getHeight());
			float _spacing = pos.x;
			ImGui::Dummy(ImVec2(0, pos.y));
			ImGui::Indent(_spacing);
			ImTextureID textureID = (ImTextureID)(uintptr_t)fboPlot.getTexture().getTextureData().textureID;
			ImGui::Image(textureID, plotShape);
			ImGui::Unindent;
		}

		//-

		rectPlot = ofRectangle(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

		ui.EndWindow();
	}
}
#endif