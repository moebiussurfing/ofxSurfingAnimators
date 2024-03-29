#include "EnvelopeAnimator.h"

//--------------------------------------------------------------
void EnvelopeAnimator::drawImGuiWidgets() {
#ifdef USE_IMGUI_LAYOUT_MANAGER__ENVELOPE

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
	ui.BeginWindow(name.c_str(), NULL, _flagsw);
	{
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

		static ImGuiTreeNodeFlags flagst;
		flagst = ImGuiTreeNodeFlags_None;
		//flagst |= ImGuiTreeNodeFlags_DefaultOpen;
		flagst |= ImGuiTreeNodeFlags_Framed;

		if (ImGui::Button("START", ImVec2(_w100, 4 * _h))) {
			start();
		}

		ofxImGuiSurfing::AddGroup(params, flagst);

	#ifdef INCLUDE_PLOTS
		ofxImGuiSurfing::AddParameter(SHOW_Plot);

		//--

		// Plot fbo
		if (SHOW_Plot) {
			widthGuiLayout = ImGui::GetWindowWidth();
			float _spacing = widthGuiLayout / 2 - (plotShape.x / 2);
			ImGui::Indent(_spacing);
			ImTextureID textureID = (ImTextureID)(uintptr_t)fboPlot.getTexture().getTextureData().textureID;
			ImGui::Image(textureID, plotShape);
			ImGui::Unindent;
		}
	#endif

		//-

		rectPlot = ofRectangle(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
	}
	ui.EndWindow();

#endif
}

//--------------------------------------------------------------
void EnvelopeAnimator::onAnimQueueDone(ofxAnimatableQueue::EventArg &) {
	//cout << "EnvelopeAnimator FINISHED" << endl;

	faderValue = faderMin.get();
	if (float_BACK != nullptr) {
		(*float_BACK) = faderValue;
	}
	//animProgress = 0;

	if (faderLoop) {
		stop();
		start();
	}
}

//--------------------------------------------------------------
EnvelopeAnimator::EnvelopeAnimator() {
	//ofSetLogLevel(OF_LOG_SILENT);

	doneInstantiated = true;
	setFps(60);
	SHOW_gui = true;
	//guiPos = glm::vec2(700, 500);
	label = "Envelope_Modulator";
	path_GLOBAL_Folder = "/";
	path = path_GLOBAL_Folder + label + ".xml";
}

//--------------------------------------------------------------
void EnvelopeAnimator::setup() {
	ENABLE_Envelope.set("Enable Envelope", true);
	faderLoop.set("LOOP", false);
	MODE_NoteOff.set("MODE OFF", false);
	faderValue.set("Value", 0, 0, 1);
	faderMin.set("Min", 0, 0, 1);
	faderMax.set("Max", 1, 0, 1);

	faderDelay.set("Delay", 0.25, 0, 2);
	faderAttack.set("Attack", 0.5, 0, 2);
	faderSustain.set("Sustain", 1, 0, 2);
	faderRelease.set("Release", 0.5, 0, 2);

	reset.set("Reset", false);

	curveTypeIn.set("CurveIn", AnimCurve(16), 0, NUM_ANIM_CURVES - 1);
	curveNameIn.set("", "");
	curveTypeOut.set("CurveOut", AnimCurve(16), 0, NUM_ANIM_CURVES - 1);
	curveNameOut.set("", "");

	curveShow.set("Show Curve", true);
	animProgress.set("%", 0, 0, 100);

	//disable for xml serialize (not required)
	faderLoop.setSerializable(false);
	curveNameIn.setSerializable(false);
	animProgress.setSerializable(false);
	reset.setSerializable(false);
	curveShow.setSerializable(false);
	faderValue.setSerializable(false);

	//--

	//params.setName("Value");
	params.setName(label);
	params.add(ENABLE_Envelope);
	////params.add(faderLoop);
	//params.add(faderValue);
	//params.add(faderMax);
	//params.add(faderMin);
	//params.add(faderDelay);
	//params.add(faderAttack);
	//params.add(faderSustain);
	//params.add(MODE_NoteOff);
	//params.add(faderRelease);

	params_Modulator.setName("VALUES");
	//params_Modulator.add(faderLoop);
	params_Modulator.add(faderValue);
	params_Modulator.add(faderMax);
	params_Modulator.add(faderMin);

	params_Timers.setName("TIMERS");
	params_Timers.add(faderDelay);
	params_Timers.add(faderAttack);
	params_Timers.add(faderSustain);
	//params_Timers.add(MODE_NoteOff);//TODO:
	params_Timers.add(faderRelease);
	params_Modulator.add(params_Timers);

	//bpm engine
	bpmMode.set("BPM Mode", false);
	bpmSpeed.set("BPM", 120.f, 10.f, 400.f);
	bpmBeatDelay.set("PreDelayB", 1, 0, 8);
	bpmBeatAttack.set("AttackB", 2, 0, 8);
	bpmBeatSustain.set("SustainB", 4, 0, 8);
	bpmBeatRelease.set("ReleaseB", 2, 0, 8);

	params_Bpm.setName("BPM ENGINE");
	params_Bpm.add(bpmMode);
	params_Bpm.add(bpmSpeed);
	params_Bpm.add(bpmBeatDelay);
	params_Bpm.add(bpmBeatAttack);
	params_Bpm.add(bpmBeatSustain);
	params_Bpm.add(bpmBeatRelease);

	params_Modulator.add(params_Bpm);
	params.add(params_Modulator);

	//-

	params.add(curveTypeIn);
	params.add(curveNameIn);
	params.add(curveTypeOut);
	params.add(curveNameOut);

	//params.add(curveShow);
	params.add(reset);
	params.add(animProgress);

	//-

	// gui
#ifdef USE_IMGUI_LAYOUT_MANAGER__ENVELOPE
	ui.setup(); // initiate ImGui
	//ui.setUseAdvancedSubPanel(true);
#endif

	//--

	// plot

#ifdef INCLUDE_PLOTS
#ifdef FIX_WORKAROUND_FBO_PLOT
	plotShape = ImVec2(210, 100);
#endif
#ifndef FIX_WORKAROUND_FBO_PLOT
	plotShape = ImVec2(210, 210);
#endif

	ofFbo::Settings fboSettings;
	fboSettings.width = plotShape.x;
	fboSettings.height = plotShape.y;
	fboSettings.internalformat = GL_RGBA;
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fboPlot.allocate(fboSettings);
#endif

	int size = 92;
	ofFbo::Settings fboSettings2;
	fboSettings2.width = size;
	fboSettings2.height = size;
	fboSettings2.internalformat = GL_RGBA;
	fboSettings2.textureTarget = GL_TEXTURE_2D;
	fboCurve.allocate(fboSettings2);

	//--

	//gui.setup(label);
	//gui.add(params);curve
	//gui.add(SHOW_Plot);
	//gui.setPosition(guiPos.x, guiPos.y);

	ofAddListener(params.parameterChangedE(), this, &EnvelopeAnimator::Changed_params);

	ofAddListener(queue.eventQueueDone, this, &EnvelopeAnimator::onAnimQueueDone);

#ifdef INCLUDE_PLOTS
	setupPlot();
#endif

	//-

	if (autoSettings) ofxSurfingHelpers::loadGroup(params, path);
}

//TODO: king of note on/off behaviour
//--------------------------------------------------------------
void EnvelopeAnimator::setupAnimatorOn() {
	faderAnimIn = AnimCurve(curveTypeIn.get());
	faderAnimOut = AnimCurve(curveTypeOut.get());

	queue.clearQueue();
	queue.setInitialValue(faderMin.get());
	queue.addTransition(faderMin.get(), faderDelay.get(), LINEAR);
	queue.addTransition(faderMax.get(), faderAttack.get(), faderAnimIn);
	//queue.addTransition(faderMax.get(), faderSustain.get(), LINEAR);
	//queue.addTransition(faderMin.get(), faderRelease.get(), faderAnimIn);

	totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
}

//--------------------------------------------------------------
void EnvelopeAnimator::setupAnimatorOff() {
	faderAnimIn = AnimCurve(curveTypeIn.get());
	faderAnimOut = AnimCurve(curveTypeOut.get());

	queue.clearQueue();
	queue.setInitialValue(faderMax.get());
	//queue.addTransition(faderMax.get(), faderDelay.get(), LINEAR);
	//queue.addTransition(faderMax.get(), faderSustain.get(), LINEAR);
	//queue.addTransition(faderMin.get(), faderRelease.get(), faderAnimIn);
	queue.addTransition(faderMin.get(), faderRelease.get(), faderAnimOut);

	totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
}

//--------------------------------------------------------------
void EnvelopeAnimator::setupAnimator() {
	faderAnimIn = AnimCurve(curveTypeIn.get());
	faderAnimOut = AnimCurve(curveTypeOut.get());

	//TODO:
	//curve out gets flipped..
	//static bool isCurveInvertable(AnimCurve c); //easeIn becomes easeOut, etc
	//static AnimCurve getInverseCurve(AnimCurve c); //does this curve have an inverse?
	//void setAutoFlipCurve(bool autoF);//when repeat set to back and forth, and autoFlip==true it will auto invert your curve when looping (ease_in > ease_out and so on)
	//faderAnimOut = AnimCurve(curveTypeOut.get()).setAutoFlipCurve(true);

	queue.clearQueue();
	queue.setInitialValue(faderMin.get());
	queue.addTransition(faderMin.get(), faderDelay.get(), LINEAR);
	queue.addTransition(faderMax.get(), faderAttack.get(), faderAnimIn);
	queue.addTransition(faderMax.get(), faderSustain.get(), LINEAR);
	queue.addTransition(faderMin.get(), faderRelease.get(), faderAnimOut);
	//queue.addTransition(faderMin.get(), faderRelease.get(), faderAnimIn);

	totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
}

#ifdef INCLUDE_PLOTS
//--------------------------------------------------------------
void EnvelopeAnimator::setupPlot() {
	plot = new ofxHistoryPlot(NULL, "fader", 100, false);
	plot->setBackgroundColor(ofColor(0));
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
#endif

//--------------------------------------------------------------
void EnvelopeAnimator::update() {
	//if (!bPaused)
	{
		queue.update(dt);

		if (queue.isPlaying()) {
			uint64_t time = ofGetElapsedTimeMillis() - lastStart;
			faderValue = queue.getCurrentValue();

			totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();

			animProgress = ofMap(time / 1000.f, 0, totalTime, 0, 100, true);
		}

#ifdef INCLUDE_PLOTS
		plot->update(faderValue.get());
#endif
	}
}

//--------------------------------------------------------------
void EnvelopeAnimator::drawPlot() {
	ofPushStyle();
	ofPushMatrix();
	ofFill();

	int x, y, size, px;
	size = 92;
	int pad = 2;
	bool stateColor;
	string str;
	int wplot = 200;
	//int wplot = gui.getShape().getWidth() - 18;
	int padPlots = 5;
	//int padPlots = 20;//when using curve labels
	x = y = 0;
	//x = gui.getPosition().x + pad;
	//y = gui.getPosition().y + gui.getHeight() + 15;

	//-

	//1. two curves
	//hide curve plot when attack/release are 0
	if (faderRelease != 0 || faderAttack != 0) {
		//curve type
		if (0) //broken
			if (curveShow) {
				curvePlotableIn.drawCurve(x + pad, y, size, true, ofColor(255), false);
				//curvePlotableOut.drawCurve(x + size + 12, y, size, true, ofColor(255), false);

				float xx = x + size + 12;
				float yy = y;
				fboCurve.begin();
				ofClear(0, 0);
				curvePlotableOut.setAutoFlipCurve(true);
				curvePlotableOut.drawCurve(0, 0, size, true, ofColor(255), false);
				fboCurve.end();
				ofSetColor(255, 255);
				fboCurve.draw(xx, yy);
				//fboCurve.draw(xx + size, yy, -size, size);//flip
				//fboCurve.draw(xx + size, yy + size, -size, -size);//flip
			}
	}

	//-

	////2. live value enveloped plot
	//plot->draw(x + pad, y + size + pad + padPlots, wplot, size);//left
	////plot->draw(x + size + pad, y + size + pad + padPlots, size, size);//right

	int rx, ry, rw, rh;
	rx = x + pad; //left
	//rx = x + size + pad;//right

#ifndef FIX_WORKAROUND_FBO_PLOT
	ry = y + 2 * size - rh + padPlots;
	rh = 8;
#endif
#ifdef FIX_WORKAROUND_FBO_PLOT
	rh = 5;
	ry = y + 1 * size + 0;
#endif

	//ry = y + size - rh;

	if (isAnimating()) {
		// 2.1 progress bar
		px = ofMap(animProgress, 0, 100, 0, wplot, true);
		rw = px;
		ofRectangle r(rx, ry + pad, rw, rh);

		ofFill();
		ofSetColor(ofColor::white, 255);
		ofDrawRectangle(r);
		//ofDrawLine(px, y, px, y + size);
	}

	//-

	//TODO:
	//// vertical line time
	//float h;//display delay wait progress
	//if (floatAnimator.isWaitingForAnimationToStart()) h = floatAnimator.waitTimeLeftPercent() * size;
	//else h = size;
	//px = ofMap(floatAnimator.getPercentDone(), 0, 1, x, x + size, true);
	//ofSetColor(ofColor::red, 200);
	//ofSetLineWidth(2.0);
	//ofDrawLine(px, y + size, px, y + size - h);

	//-

	// 2.2 vertical red bar value
	float vb = ofMap(faderValue.get(), faderMin.get(), faderMax.get(), 0.f, 1.f, true);
	int w = 10;
	rx += wplot + 3;
#ifndef FIX_WORKAROUND_FBO_PLOT
	ry += rh + pad;
#endif
	ofFill();

	// black bg
	ofSetColor(0, 255);
	pad = 2; //make black outspace
	ofRectangle r2;
	r2 = ofRectangle(rx, ry, w, -size);
	ofDrawRectangle(r2);

	//plot->draw(x + pad, y + size + pad + padPlots, wplot, size);//left

	// red vert bar
	ofSetColor(ofColor::red, 225);
	r2 = ofRectangle(rx + 0.5 * pad, ry - 0.5 * pad, w - pad, pad - MAX(vb * size, 1));
	ofDrawRectangle(r2);

	//-

	//// label
	////stateColor = queue.isPlaying();
	//stateColor = queue.isPlaying() && (faderValue > 0);
	//str = label;
	////str = "2 ALPHA";
	//ofDrawBitmapStringHighlight(str, rx + 4, y - 10,
	//	stateColor ? ofColor::white : ofColor::black,
	//	!stateColor ? ofColor::white : ofColor::black);

#ifdef FIX_WORKAROUND_FBO_PLOT
	if (0)
#endif
	{
#ifdef INCLUDE_PLOTS
		int x = 0;
		int y = 2;
		int pad = 5;
		int padPlots = 5;
		int wplot = plotShape.x - 16;
		//int wplot = plotShape.x;
		//int wplot = 500;
		int size = 92;
		ofNoFill();
		ofColor c = ofColor(255, 255);
		//ofSetColor(c);

		//plot->setBackgroundColor(0);
		//plot->setDrawBackground(true);
		plot->setColor(c);
		plot->draw(x, y + size + pad + padPlots, wplot, size); //left
		//plot->draw(x + pad, y + size + pad + padPlots, wplot, size);//left
#endif
	}

	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void EnvelopeAnimator::draw() {
	if (SHOW_gui) 
	{
#ifdef INCLUDE_PLOTS
		if (SHOW_Plot) {
			fboPlot.begin();
			{
				ofClear(0, 0);
				drawPlot();
			}
			fboPlot.end();
		}
#endif

		//-

#ifdef USE_IMGUI_LAYOUT_MANAGER__ENVELOPE
		ui.Begin();
		{
			drawImGuiWidgets();
		}
		ui.End();
#endif

		//gui.draw();
	}

	//-

	//TDOO:
	//extra bc fbo fails going black
#ifdef FIX_WORKAROUND_FBO_PLOT
	#ifdef INCLUDE_PLOTS
	if (SHOW_Plot) {
		int padx = 20;
		int pady = 5;
		int x = rectPlot.getBottomLeft().x + padx;
		int y = rectPlot.getBottomLeft().y + pady;
		int padPlots = 5;
		int wplot = plotShape.x;
		int size = 92;
		plot->draw(x, y, wplot, size); //left
	}
	#endif
#endif
}

//--------------------------------------------------------------
void EnvelopeAnimator::start() {
	if (ENABLE_Envelope) {
		//mark
		//plot->update(1);

		//TODO:
		//rebuild every start
		setupAnimator();
		//stop();

		lastStart = ofGetElapsedTimeMillis();
		queue.setInitialValue(faderMin.get());
		queue.startPlaying(); //start the animation
	} else {
		faderValue = faderMax;
	}
}

//--------------------------------------------------------------
void EnvelopeAnimator::startOn() {
	if (ENABLE_Envelope) {
		setupAnimatorOn();

		lastStart = ofGetElapsedTimeMillis();
		queue.setInitialValue(faderMin.get());
		queue.startPlaying(); //start the animation
	} else {
		faderValue = faderMax;
	}
}

//--------------------------------------------------------------
void EnvelopeAnimator::startOff() {
	if (ENABLE_Envelope) {
		setupAnimatorOff();

		lastStart = ofGetElapsedTimeMillis();
		//faderValue = faderMax.get();
		queue.setInitialValue(faderMax.get());
		queue.startPlaying(); //start the animation
	} else {
		faderValue = faderMax;
	}
}

////--------------------------------------------------------------
//void EnvelopeAnimator::pause()
//{
//	queue.pausePlayback();
//}

//--------------------------------------------------------------
void EnvelopeAnimator::stop() {
	//cout << "stop()" << endl;
	if (ENABLE_Envelope) {
		faderValue = faderMin.get();
		queue.setInitialValue(faderMin.get());
		queue.pausePlayback(); //start the animation
		animProgress = 0;
	} else {
		faderValue = faderMax;
	}
}

//--------------------------------------------------------------
EnvelopeAnimator::~EnvelopeAnimator() {
	ofRemoveListener(queue.eventQueueDone, this, &EnvelopeAnimator::onAnimQueueDone);

	exit();
}

//--------------------------------------------------------------
void EnvelopeAnimator::exit() {
	/*if (autoSettings)*/
	ofxSurfingHelpers::saveGroup(params, path);
}

//--------------------------------------------------------------
void EnvelopeAnimator::nextCurve() {
	curveTypeIn++;
	curveTypeIn = curveTypeIn % NUM_ANIM_CURVES;
}

//--------------------------------------------------------------
void EnvelopeAnimator::previousCurve() {
	curveTypeIn--;
	if (curveTypeIn < 0)
		curveTypeIn = NUM_ANIM_CURVES - 1;
}

//--------------------------------------------------------------
void EnvelopeAnimator::Changed_params(ofAbstractParameter & e) {
	string name = e.getName();
	if (name != "%" && name != "Value")
		ofLogVerbose(__FUNCTION__) << name << " : " << e;

	//-

	//if (name == "Min" ||
	//	name == "Max" ||
	//	name == "Delay" ||
	//	name == "Attack" ||
	//	name == "Sustain" ||
	//	name == "Release")
	//{
	//	totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
	//	//setupAnimator();
	//	//stop();
	//}

	if (false) {
	}

	//bpm engine
	else if (
		name == bpmMode.getName() || name == bpmSpeed.getName() || name == bpmBeatDelay.getName() || name == bpmBeatAttack.getName() || name == bpmBeatSustain.getName() || name == bpmBeatRelease.getName()) {
		if (bpmMode) {
			float _bar = 60.f / bpmSpeed.get(); //one bar duration in seconds to this bpm speed
			float _ratio = 2.0f;
			faderDelay = (_bar / _ratio) * (float)bpmBeatDelay;
			faderAttack = (_bar / _ratio) * (float)bpmBeatAttack;
			faderSustain = (_bar / _ratio) * (float)bpmBeatSustain;
			faderRelease = (_bar / _ratio) * (float)bpmBeatRelease;
		}

		if (name == bpmMode.getName()) {
			//workflow
			refreshGui();
		}
	}

	else if (name == curveTypeIn.getName()) {
		curveNameIn = ofxAnimatable::getCurveName(AnimCurve(curveTypeIn.get()));
		AnimCurve curve = (AnimCurve)(curveTypeIn.get());
		curvePlotableIn.setCurve(curve);
	}

	else if (name == curveTypeOut.getName()) {
		curveNameOut = ofxAnimatable::getCurveName(AnimCurve(curveTypeOut.get()));
		AnimCurve curve = (AnimCurve)(curveTypeOut.get());
		curvePlotableOut.setCurve(curve);
	}

	else if (name == "Reset") {
		if (reset) {
			reset = false;

			faderValue = faderMax;

			faderDelay = 0.25f;
			faderAttack = 0.5f;
			faderSustain = 1.f;
			faderRelease = 0.5f;

			//bpm engine
			bpmMode = true;
			bpmSpeed = 120.f;
			bpmBeatDelay = 1;
			bpmBeatAttack = 2;
			bpmBeatSustain = 4;
			bpmBeatRelease = 2;

			curveTypeIn = 3;
			curveTypeOut = 3;

			faderMin = 0.f;
			faderMax = 1.0f;
		}
	}

	else if (name == "Value") {
		if (float_BACK != nullptr) {
			(*float_BACK) = faderValue;
		}
	} else if (name == "Enable Envelope") {
		if (!ENABLE_Envelope) {
			faderValue = faderMax;
		}
	}
}
