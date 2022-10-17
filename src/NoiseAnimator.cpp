#include "NoiseAnimator.h"
//--------------------------------------------------------------
void NoiseAnimator::drawPlot() {

	// workaround bc polot is not being drawed well..
	if (ENABLE_Modulator)
	{
		fboPlot2.begin();
		{
			ofClear(0, 0);
			plot->draw(0, 0, PLOT_BOXES_SIZES, PLOT_BOXES_SIZES);
		}
		fboPlot2.end();
	}

	//--

	{
		ofPushStyle();
		ofPushMatrix();
		ofFill();

		int x, y, size, px, pad;

		size = PLOT_BOXES_SIZES;//boxes sizes
		pad = 4;//space between plot boxes

		bool stateColor;
		string str;
		ofColor c;

		//// a.
		//bCustomPositionPlot = !bGui;
		//if (bCustomPositionPlot) {
		//	x = positionPlot.x;
		//	y = positionPlot.y;
		//}
		//else {
		//	//x = gui.getPosition().x + 16;
		//	//y = gui.getPosition().y + gui.getHeight() + 15;

		//	x = positionGuiLayout.get().x + widthGuiLayout / 2 - (size + 19) / 2;
		//	y = positionGuiLayout.get().y + heightGuiLayout + pad;
		//}

		// b.
		x = 0;
		//y = 0;
		y = 20;

		//--

		int xPos;
		int yPos;

		// define color state

		if (!ENABLE_Noise) {//ENABLE_Noise = false
			stateColor = false;
		}
		else {//ENABLE_Noise = true
			if (ENABLE_Modulator)
			{
				//ofLogNotice(_FUNCTION__) <<ofToString(noisePos);
				if (abs(noisePos.x) > 0.001 || abs(noisePos.y) > 0.001) stateColor = true;
				else stateColor = false;

				//ofLogNotice(_FUNCTION__) <<ofToString(LPFpoint.value());
				//if ((LPFpoint.value() != ofVec2f(0, 0)))

				//if ((faderValue > 0)) {
				//	stateColor = true;
				//}
				//else if (ENABLE_NoiseModulatorFilter || ENABLE_NoisePointFilter) {
				//	if (queue.isPlaying() || (LPFmodulator.value() > 0) || (LPFpoint.value() != ofVec2f(0, 0)))
				//		stateColor = true;
				//	else
				//		stateColor = false;
				//}
				//else {
				//	stateColor = queue.isPlaying() && (faderValue > 0);
				//}
			}
			else { //ENABLE_Noise = true & ENABLE_Modulator = false
				stateColor = true;
			}
		}

		//--

//#ifdef INCLUDE_PLOTS
		//// 1. mod ASR plot
		//if (ENABLE_Modulator)
		//{
		//	//live ASR value plot
		//	plot->draw(x + 1 * (size + pad), y, size, size);
		//	//plot->draw(x + 3 * (size + pad), y, size, size);
		//}
//#endif

		if (stateColor) c = ofColor(255, 255);
		else c = ofColor(255, 64);

		//--

		// moved bc black
		// 2. boxes noise x/y plots
		{
			xPos = x + 2 * (size + pad);
			yPos = y;//upper line


#ifdef INCLUDE_PLOTS
			plot_NoiseX->setColor(c);
			plot_NoiseY->setColor(c);
#endif
			////TODO: try to flip..
			//ofPushMatrix();
			//ofTranslate(x, yPos);
			//ofRotateZDeg(90);
			//plot_NoiseX->draw(0,0, size, size);
			//ofPopMatrix();

#ifdef INCLUDE_PLOTS
			//ofPushStyle();
			//ofSetColor(c);
			plot_NoiseX->draw(x, yPos, size, size);
			plot_NoiseY->draw(x + (size + pad), yPos, size, size);
			//ofPopStyle();
#endif
			////labels
			//ofSetColor(255);
			//int labPad = 20;
			//string strLab;
			//strLab = "noiseX:"+ofToString(noiseX);
			//ofDrawBitmapString(strLab, glm::vec2(xPos, yPos-2*labPad));
			//strLab = "noiseY:"+ofToString(noiseY);
			//ofDrawBitmapString(strLab, glm::vec2(xPos, yPos-labPad));
		}

		//-

		// 3. box point circle plot
		{
			ofPushStyle();
			ofFill();
			ofSetColor(ofColor(0, 255));//black

			xPos = x;
#ifdef INCLUDE_PLOTS
			yPos = y + size + pad;
#else
			yPos = y + pad;
#endif
			// 3.1. bg box
			ofDrawRectangle(ofRectangle(xPos, yPos, size, size));

			//string strDebug;
			//strDebug += "x:" + ofToString(noisePos.x);
			//strDebug += "\ny:" + ofToString(noisePos.y);
			//ofSetColor(255);
			//ofDrawBitmapString(strDebug, glm::vec2(xPos, yPos - 30));

			float halfSize = size * 0.5f;
			float ratio_PlotSize_noisePosX, ratio_PlotSize_noisePosY;

			// 3.2. axis
			ofSetColor(ofColor(255, 32));
			ofDrawLine(xPos + halfSize, yPos, xPos + halfSize, yPos + size);//vertical
			ofDrawLine(xPos, yPos + halfSize, xPos + size, yPos + halfSize);//horizontal

			// A. scaling to final movement
			//ratio_PlotSize_noisePosX = halfSize / (noiseSizeMax * 5);
			//ratio_PlotSize_noisePosY = halfSize / (noiseSizeMax * 5);

			// B. scaling from raw modifier not with power aplied
			ratio_PlotSize_noisePosX = halfSize / (noisePowerX * 5);
			ratio_PlotSize_noisePosY = halfSize / (noisePowerY * 5);
			float nx, ny;

			//nx = noisePos.x * ratio_PlotSize_noisePosX;
			// hflip
			nx = noisePos.x * ratio_PlotSize_noisePosX;
			//ny = noisePos.y * ratio_PlotSize_noisePosY;
			// vflip
			ny = -noisePos.y * ratio_PlotSize_noisePosY;

			//ofLogNotice(_FUNCTION__) <<"nx:" << ofToString(nx);
			//ofLogNotice(_FUNCTION__) <<" ny:" << ofToString(ny);
			//strDebug += "nx:" + ofToString(nx);
			//strDebug += "\";
			//strDebug += "ny:" + ofToString(ny);
			//ofSetColor(255);
			//ofDrawBitmapString(strDebug, glm::vec2(xPos, yPos - 70));

			// 3.3 plot circle point
			// clamp plot into box
			float pxx, pyy;
			pxx = ofClamp(nx + xPos + halfSize, nx + xPos - halfSize, nx + xPos + halfSize);
			pyy = ofClamp(ny + yPos + halfSize, ny + yPos - halfSize, ny + yPos + halfSize);

			//draw point
			if (stateColor) c = ofColor(255, 0, 0, 225);
			else c = ofColor(255, 64);
			ofSetColor(c);

#define MAX_RADIUS 15
			float nz = abs(noisePos.z);
			float r = ofMap(nz, 0, 100, 1, MAX_RADIUS, true);
			ofDrawCircle(pxx, pyy, r);
			//ofDrawCircle(pxx, pyy, 3);
			//ofDrawCircle(nx + xPos + halfSize, ny + yPos + halfSize, 3);
			ofPopStyle();
		}

		//-

		//if (ENABLE_Modulator && faderValue == faderMax) 
		{

			//-

			// 4. progress bar
			if (ENABLE_Modulator && isAnimating())
			{
				int rx, ry, rw, rh;
				rh = 10;
#ifdef INCLUDE_PLOTS
				rx = x + size + pad;
#else
				rx = x;
#endif
				ry = yPos + size - rh;
				rw = ofMap(animProgress, 0, 100, 0, size, true);
				ofRectangle r(rx, ry, rw, rh);

				ofFill();
				ofSetColor(255, 255);
				ofDrawRectangle(r);
				//ofDrawLine(px, y, px, y + size);
			}
		}

		//--

		// 4. curve type
		// hide curve plot when attack/release are 0
		if(0)//broken
		if ((faderRelease != 0 || faderAttack != 0) && ENABLE_Modulator)
		{
			if (curveShow)
			{
				xPos = x + size + pad;
#ifdef INCLUDE_PLOTS
				yPos += size + pad;
#endif
				curvePlotable.drawCurve(xPos, yPos, size, true, ofColor(255));

				//curvePlotable.drawCurve(x + 2 * (size + pad), y - (size + pad) - 20, size, true, ofColor(255));

				//-

				/*
				//TODO
				//add monitor red line in attack/release
				//SOLUTION: can measure frames nums not times!
				//if (queue.isPlaying())
				//{
				//    //fps = 1/dt;
				//
				//    float tt = (ofGetElapsedTimeMillis() - lastStart) / 1000.f;
				//    float ttPrc = ofMap(tt, 0, totalTime, 0, 100);
				//    ofLogNotice(_FUNCTION__) <<"ttPrc:" << ttPrc << " dt:"<<dt<<endl;
				//}
				//totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
				//queue.
				//lastStart = ofGetElapsedTimeMillis();
				//
				////vertical line
				//px = ofMap(colorAnim.getPercentDone(), 0, 1, x, x + size);
				//ofSetColor(ofColor::red, 200);
				//ofSetLineWidth(2.0);
				//ofDrawLine(px, y, px, y + size);
				*/
			}
		}

		// 1. modulator envelope plot
		xPos = x + size + pad;
		if (stateColor && ENABLE_Modulator) c = ofColor(255, 255);
		else c = ofColor(255, 64);
		plot->setColor(c);

		if (ENABLE_Modulator)
		{
			fboPlot2.draw(xPos, yPos, PLOT_BOXES_SIZES, PLOT_BOXES_SIZES);
		}

		ofPopStyle();
		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void NoiseAnimator::Changed_AnimatorQueueDone(ofxAnimatableQueue::EventArg &)
{
	ofLogNotice(__FUNCTION__);

	faderValue = faderMin.get();

	if (float_BACK != nullptr)
	{
		(*float_BACK) = faderValue.get();
	}

	//animProgress = 0;

	if (faderLoop)
	{
		stop();
		start();
	}
}

//--------------------------------------------------------------
NoiseAnimator::NoiseAnimator()
{
	float_BACK = NULL;
	point_BACK = NULL;

	//ofSetLogLevel(OF_LOG_SILENT);

	path_GLOBAL_Folder = "NoiseAnimator";
	path_Settings = "settings_NoiseAnimator.xml";
	autoSettings = true;

	doneInstantiated = true;
	fps = 60;
	setFps(fps);
	bGui = true;
	//guiPos = glm::vec2(700, 500);

	ofAddListener(ofEvents().update, this, &NoiseAnimator::update);
	
#ifdef SURFING_ANIMATOR_ENABLE_AUTO_DRAW 
	ofAddListener(ofEvents().draw, this, &NoiseAnimator::draw);
#endif
}

//--------------------------------------------------------------
void NoiseAnimator::startup()
{
	if (autoSettings) ofxSurfingHelpers::loadGroup(params, path_GLOBAL_Folder + "/" + path_Settings);

	curveName = ofxAnimatable::getCurveName(AnimCurve(curveType.get()));
	AnimCurve curve = (AnimCurve)(curveType.get());
	curvePlotable.setCurve(curve);

	setupFader();
	ofAddListener(queue.eventQueueDone, this, &NoiseAnimator::Changed_AnimatorQueueDone);

	setupPlot();
	setupPlot_Noise();

	//-

	if (ENABLE_Noise) bRestoreTrue = true;

	//seed engine
	rSeed = (int)ofRandom(0, fps / 4);
	ENABLE_Noise = false;
}

//--------------------------------------------------------------
void NoiseAnimator::refreshStyles()
{
	ui.clearStyles();

	//-

	// A. Hardcoded Styles

	// groups
	//ui.AddStyleGroup(params, OFX_IM_GROUP_HIDDEN_HEADER);
#ifdef INCLUDE_FILTER
	ui.AddStyleGroup(params_filters, OFX_IM_GROUP_COLLAPSED);
#endif
	ui.AddStyleGroup(params_NoiseZ, OFX_IM_GROUP_COLLAPSED);

	// params
	ui.AddStyle(ENABLE_Noise, OFX_IM_TOGGLE_BIG);
	ui.AddStyle(ENABLE_Modulator, OFX_IM_TOGGLE_BIG);
	ui.AddStyle(Reset_Noise, OFX_IM_BUTTON_SMALL);
#ifdef INCLUDE_FILTER
	ui.AddStyle(ENABLE_NoisePointFilter, OFX_IM_TOGGLE_SMALL, false, 1);
	ui.AddStyle(ENABLE_NoiseModulatorFilter, OFX_IM_TOGGLE_SMALL, false, 1);
#endif
	ui.AddStyle(bpmMode, OFX_IM_TOGGLE_SMALL);
	ui.AddStyle(Reset_Modulator, OFX_IM_BUTTON_SMALL);
	ui.AddStyle(animProgress, OFX_IM_INACTIVE);

	//-

	// B. Mode dependant Styles

	// Modulator
	if (!ENABLE_Modulator || (ui.bMinimize)) {
		ui.AddStyleGroup(params_Modulator, OFX_IM_GROUP_HIDDEN);
		ui.AddStyleGroup(params_Timers, OFX_IM_GROUP_HIDDEN);//-> affected bc recursive
	}
	else {
		ui.AddStyleGroup(params_Modulator, OFX_IM_GROUP_COLLAPSED);
		ui.AddStyleGroup(params_Timers, OFX_IM_GROUP_COLLAPSED);
	}

	// workaround 
	//to fix recursive groups / api
	ui.AddStyleGroup(params_Point, OFX_IM_GROUP_DEFAULT);

	// Minimize
	if (ui.bMinimize) {
		//ui.AddStyle(curveShow, OFX_IM_HIDDEN);
		
		if (bpmMode) {
			ui.AddStyleGroup(params_Bpm, OFX_IM_GROUP_DEFAULT);
			ui.AddStyleGroup(params_Timers, OFX_IM_GROUP_HIDDEN);
		}
		else {
			ui.AddStyleGroup(params_Bpm, OFX_IM_GROUP_HIDDEN);
			ui.AddStyleGroup(params_Timers, OFX_IM_GROUP_DEFAULT);
		}

		ui.AddStyle(noiseSpeedX, OFX_IM_HIDDEN);
		ui.AddStyle(noiseSpeedY, OFX_IM_HIDDEN);
		ui.AddStyle(noiseSpeedZ, OFX_IM_HIDDEN);

		ui.AddStyle(noiseDeepX, OFX_IM_HIDDEN);
		ui.AddStyle(noiseDeepY, OFX_IM_HIDDEN);
		ui.AddStyle(noiseDeepZ, OFX_IM_HIDDEN);
	}
	else {
		//ui.AddStyle(curveShow, OFX_IM_DEFAULT);
		
		ui.AddStyleGroup(params_Bpm, OFX_IM_GROUP_HIDDEN);
		ui.AddStyleGroup(params_Timers, OFX_IM_GROUP_HIDDEN);

		//ui.AddStyle(noiseSpeedX, OFX_IM_DEFAULT);
		//ui.AddStyle(noiseSpeedY, OFX_IM_DEFAULT);
		//ui.AddStyle(noiseSpeedZ, OFX_IM_DEFAULT);

		//ui.AddStyle(noiseDeepX, OFX_IM_DEFAULT);
		//ui.AddStyle(noiseDeepY, OFX_IM_DEFAULT);
		//ui.AddStyle(noiseDeepZ, OFX_IM_DEFAULT);

		// force

		if (ENABLE_NoiseX) {
			ui.AddStyle(noisePowerX, OFX_IM_STEPPER);
			ui.AddStyle(noiseSpeedX, OFX_IM_DEFAULT);
			ui.AddStyle(noiseDeepX, OFX_IM_DEFAULT);
		}
		else
		{
			ui.AddStyle(noisePowerX, OFX_IM_HIDDEN);
			ui.AddStyle(noiseSpeedX, OFX_IM_HIDDEN);
			ui.AddStyle(noiseDeepX, OFX_IM_HIDDEN);
		}

		if (ENABLE_NoiseY) {
			ui.AddStyle(noisePowerY, OFX_IM_STEPPER);
			ui.AddStyle(noiseSpeedY, OFX_IM_DEFAULT);
			ui.AddStyle(noiseDeepY, OFX_IM_DEFAULT);
		}
		else
		{
			ui.AddStyle(noisePowerY, OFX_IM_HIDDEN);
			ui.AddStyle(noiseSpeedY, OFX_IM_HIDDEN);
			ui.AddStyle(noiseDeepY, OFX_IM_HIDDEN);
		}

		if (ENABLE_NoiseZ) {
			ui.AddStyle(noisePowerZ, OFX_IM_STEPPER);
			ui.AddStyle(noiseSpeedZ, OFX_IM_DEFAULT);
			ui.AddStyle(noiseDeepZ, OFX_IM_DEFAULT);
		}
		else
		{
			ui.AddStyle(noisePowerZ, OFX_IM_HIDDEN);
			ui.AddStyle(noiseSpeedZ, OFX_IM_HIDDEN);
			ui.AddStyle(noiseDeepZ, OFX_IM_HIDDEN);
		}
	}

	//ui.AddStyle(noisePowerX, OFX_IM_STEPPER);
	//ui.AddStyle(noisePowerY, OFX_IM_STEPPER);
	//ui.AddStyle(noisePowerZ, OFX_IM_STEPPER);

	ui.AddStyle(ENABLE_NoiseX, OFX_IM_TOGGLE_SMALL);
	ui.AddStyle(ENABLE_NoiseY, OFX_IM_TOGGLE_SMALL);
	ui.AddStyle(ENABLE_NoiseZ, OFX_IM_TOGGLE_SMALL);
}

//--------------------------------------------------------------
void NoiseAnimator::setup()
{
	ENABLE_Noise.set("Enable Noise", true);

	ENABLE_NoiseX.set("ENABLE X", true);
	ENABLE_NoiseY.set("ENABLE Y", true);
	ENABLE_NoiseZ.set("ENABLE Z", true);

	Reset_Noise.set("Reset Noise", false);

	noiseSizeMax = 100;//max noise displacement. plot box sizes are 100
	params_NoiseX.setName("X");
	noisePowerX.set("PowerX", DEFAUL_POWER, 0, noiseSizeMax);
	noiseSpeedX.set("SpeedX", 0.5, 0.1, 1);
	noiseDeepX.set("DeepX", 1, 1, 3);
	params_NoiseX.add(ENABLE_NoiseX);
	params_NoiseX.add(noisePowerX);
	params_NoiseX.add(noiseSpeedX);
	params_NoiseX.add(noiseDeepX);

	params_NoiseY.setName("Y");
	noisePowerY.set("PowerY", DEFAUL_POWER, 0, noiseSizeMax);
	noiseSpeedY.set("SpeedY", 0.5, 0.1, 1);
	noiseDeepY.set("DeepY", 1, 1, 3);
	params_NoiseY.add(ENABLE_NoiseY);
	params_NoiseY.add(noisePowerY);
	params_NoiseY.add(noiseSpeedY);
	params_NoiseY.add(noiseDeepY);

	params_NoiseZ.setName("Z");
	noisePowerZ.set("PowerZ", DEFAUL_POWER, 0, noiseSizeMax);
	noiseSpeedZ.set("SpeedZ", 0.5, 0.1, 1);
	noiseDeepZ.set("DeepZ", 1, 1, 3);
	params_NoiseZ.add(ENABLE_NoiseZ);
	params_NoiseZ.add(noisePowerZ);
	params_NoiseZ.add(noiseSpeedZ);
	params_NoiseZ.add(noiseDeepZ);

	//-

	ENABLE_Modulator.set("Modulator", false);

	bEnableAnimator.set("Enable Animator", true);
	bEnableAnimator.setSerializable(false);

	// filters
#ifdef INCLUDE_FILTER
	ENABLE_NoiseModulatorFilter.set("FILTER MODULATOR", true);
	fc.set("LPF Modulator", 0.5f, 0.f, 1.f);
	ENABLE_NoisePointFilter.set("FILTER POINT", true);
	fcPoint.set("LPF Point", 0.5f, 0.f, 1.f);
#endif

	faderValue.set("VALUE", 0, 0, 1.f);
	faderMin.set("Min", 0, 0, 1.f);
	faderMax.set("Max", 1.f, 0, 1.f);
	faderLoop.set("LOOP", false);//required false

	faderDelay.set("PreDelay", 0.25f, 0, 2);
	faderAttack.set("Attack", 0.5f, 0, 2);
	faderSustain.set("Sustain", 1.f, 0, 2);
	faderRelease.set("Release", 0.5f, 0, 2);

	Reset_Modulator.set("Reset Modulator", false);
	curveType.set("Curve Type", AnimCurve(16), 0, NUM_ANIM_CURVES - 1);
	curveName.set("", "");
	curveShow.set("Show Curve", false);
	animProgress.set("%", 0, 0, 100);

	// disable for xml serialize (not required)
	faderLoop.setSerializable(false);
	curveName.setSerializable(false);
	animProgress.setSerializable(false);
	Reset_Modulator.setSerializable(false);
	curveShow.setSerializable(false);
	faderValue.setSerializable(false);
	Reset_Noise.setSerializable(false);

	params.setName(label);
	//params.setName("Noise Animator");
	params.add(ENABLE_Noise);
	params.add(ENABLE_Modulator);

#ifdef INCLUDE_FILTER
	// filters
	params_filters.add(ENABLE_NoisePointFilter);
	params_filters.add(fcPoint);
	params_filters.add(ENABLE_NoiseModulatorFilter);
	params_filters.add(fc);
	params.add(params_filters);
#endif

	params_Modulator.setName("MODULATOR");
	params_Modulator.add(faderValue);
	params_Modulator.add(faderMax);
	params_Modulator.add(faderMin);

	params_Timers.setName("TIMERS");
	params_Timers.add(faderDelay);
	params_Timers.add(faderAttack);
	params_Timers.add(faderSustain);
	params_Timers.add(faderRelease);
	params_Modulator.add(params_Timers);

	// bpm engine
	bpmMode.set("BPM Mode", true);
	bpmSpeed.set("BPM", 120.f, 10.f, 400.f);
	bpmBeatDelay.set("PreDelay Beat", 1, 0, 8);
	bpmBeatAttack.set("Attack Beat", 2, 0, 8);
	bpmBeatSustain.set("Sustain Beat", 4, 0, 8);
	bpmBeatRelease.set("Release Beat", 2, 0, 8);

	params_Bpm.setName("BPM ENGINE");
	params_Bpm.add(bpmMode);
	params_Bpm.add(bpmSpeed);
	params_Bpm.add(bpmBeatDelay);
	params_Bpm.add(bpmBeatAttack);
	params_Bpm.add(bpmBeatSustain);
	params_Bpm.add(bpmBeatRelease);
	//ofAddListener(params_Bpm.parameterChangedE(), this, &NoiseAnimator::Changed_params_Bpm);

	params_Modulator.add(params_Bpm);
	params_Modulator.add(curveType);
	params_Modulator.add(curveName);
	//params_Modulator.add(curveShow);
	params_Modulator.add(animProgress);
	params_Modulator.add(Reset_Modulator);
	params.add(params_Modulator);

	params_Point.setName("POINT");
	params_Point.add(params_NoiseX);
	params_Point.add(params_NoiseY);
	params_Point.add(params_NoiseZ);
	params.add(params_Point);

	params.add(Reset_Noise);

	ofAddListener(params.parameterChangedE(), this, &NoiseAnimator::Changed_params);

	//-

	// Controls
	params_Control.setName("Noise Controls");
	params_Control.add(bEnableAnimator);
	params_Control.add(ENABLE_Noise);
	params_Control.add(ENABLE_Modulator);

	//params_Control.add(ENABLE_NoiseX);
	//params_Control.add(ENABLE_NoiseY);
	//params_Control.add(ENABLE_NoiseZ);
	//ofAddListener(params_Control.parameterChangedE(), this, &NoiseAnimator::Changed_params);

	// Helpers
	params_Helpers.setName("Noise Helpers");
	params_Helpers.add(bGui);
	//params_Helpers.setName(label);
	//params_Helpers.setName(label + " CONTROLS");
	//params_Helpers.add(ENABLE_Noise);
	//params_Helpers.add(ENABLE_Modulator);
	//params_Helpers.add(SHOW_Plot);

	//--

	// Plot
	//plotShape = ImVec2(210, 315);
	plotShape = ImVec2(210, 200);
	ofFbo::Settings fboSettings;
	fboSettings.width = plotShape.x;
	fboSettings.height = plotShape.y;
	fboSettings.internalformat = GL_RGBA;
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fboPlot.allocate(fboSettings);

	fboSettings.width = PLOT_BOXES_SIZES;
	fboSettings.height = PLOT_BOXES_SIZES;
	fboSettings.internalformat = GL_RGBA;
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fboPlot2.allocate(fboSettings);

	fboPlot.begin();
	ofClear(0, 0);
	fboPlot.end();

	fboPlot2.begin();
	ofClear(0, 0);
	fboPlot2.end();

	//-

	// Gui
#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	ui.setName("NoiseAnimator");
	//ui.setSettingsPathLabel("NoiseAnimator");
	//ui.setAutoSaveSettings(true);
	ui.setup(IM_GUI_MODE_INSTANTIATED);
#endif

	//-

	//--------------------------------------------------------------
	listenerMinimize = ui.bMinimize.newListener([this](bool &b) {
		refreshStyles();
	});

	//-

	// Styles

	refreshStyles();

	//-

	startup();
}

//--------------------------------------------------------------
void NoiseAnimator::setupFader()
{
	faderAnim = AnimCurve(curveType.get());

	queue.clearQueue();
	queue.setInitialValue(faderMin.get());
	queue.addTransition(faderMin.get(), faderDelay.get(), LINEAR);
	queue.addTransition(faderMax.get(), faderAttack.get(), faderAnim);
	queue.addTransition(faderMax.get(), faderSustain.get(), LINEAR);
	queue.addTransition(faderMin.get(), faderRelease.get(), faderAnim);

	totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
	//ofLogNotice(_FUNCTION__) <<"totalTime: " << totalTime;

	//TODO
	//totalFrames = totalTime * dt;

	//ofLogNotice(_FUNCTION__) <<"setupFader()";
	//ofLogNotice(_FUNCTION__) <<"totalTime   : " << totalTime;
	//ofLogNotice(_FUNCTION__) <<"faderDelay  : " << faderDelay;
	//ofLogNotice(_FUNCTION__) <<"faderAttack : " << faderAttack;
	//ofLogNotice(_FUNCTION__) <<"faderSustain: " << faderSustain;
	//ofLogNotice(_FUNCTION__) <<"faderRelease: " << faderRelease;
}

//--------------------------------------------------------------
void NoiseAnimator::setupPlot()
{
#ifdef INCLUDE_PLOTS
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
#endif
}

//--------------------------------------------------------------
void NoiseAnimator::setupPlot_Noise()
{
#ifdef INCLUDE_PLOTS
	plot_NoiseX = new ofxHistoryPlot(NULL, "x:", 100, false);
	plot_NoiseX->setBackgroundColor(ofColor(0));
	plot_NoiseX->setShowNumericalInfo(false);
	//plot_NoiseX->setDrawGuideValues(true);
	plot_NoiseX->setRange(-1, 1);
	plot_NoiseX->setRespectBorders(true);
	plot_NoiseX->setLineWidth(1);
	plot_NoiseX->setCropToRect(0);
	plot_NoiseX->setDrawGrid(false);
	plot_NoiseX->setShowSmoothedCurve(false);
	plot_NoiseX->setColor(255);
	plot_NoiseX->setPrecision(1);
	plot_NoiseX->setDrawTitle(true);
	plot_NoiseX->setGridUnit(.50);
	plot_NoiseX->setGridColor(ofColor(255, 255));

	plot_NoiseY = new ofxHistoryPlot(NULL, "y:", 100, false);
	plot_NoiseY->setBackgroundColor(ofColor(0));
	plot_NoiseY->setShowNumericalInfo(false);
	plot_NoiseY->setRange(-1, 1);
	plot_NoiseY->setRespectBorders(true);
	plot_NoiseY->setLineWidth(1);
	plot_NoiseY->setCropToRect(0);
	plot_NoiseY->setDrawGrid(false);
	plot_NoiseY->setShowSmoothedCurve(false);
	plot_NoiseY->setColor(255);
	plot_NoiseY->setPrecision(1);
	plot_NoiseY->setDrawTitle(true);
	plot_NoiseY->setGridUnit(.50);
	plot_NoiseY->setGridColor(ofColor(255, 255));
#endif
}

//--------------------------------------------------------------
//void NoiseAnimator::update()
void NoiseAnimator::update(ofEventArgs & args)
{
	if (!bEnableAnimator) return;

	//workaround: to variate a the random seed
	//TODO:
	//if (0)
	if (bRestoreTrue)
		if (ofGetFrameNum() == rSeed)
		{
			ENABLE_Noise = true;
		}

	//--

	//TODO
	//could improve noise system dimensions...
	//maybe can we use msaPerlinNoise classes...
	//ofLogNotice(_FUNCTION__) <<"noiseCountX:"<<noiseCountX<<endl;
	//ofLogNotice(_FUNCTION__) <<"noiseCountY:"<<noiseCountY<<endl;

	//-

	if (SHOW_Plot && bGui)
	{
		fboPlot.begin();
		{
			ofClear(0, 0);
			drawPlot();
		}
		fboPlot.end();
	}

	//--

	//1. Pre calcultate point x,y noise

	if (ENABLE_Noise)
	{
		//-

		if (ENABLE_NoiseX)
		{
			noiseCountX += dt * noiseSpeedX;
			switch (noiseDeepX)
			{
			case 1:
				noiseX = (ofSignedNoise(noiseCountX));//slower 1dim
				break;
			case 2:
				noiseX = (ofSignedNoise(noiseCountX, noiseCountX));//medium 2dim
				break;
			case 3:
				noiseX = (ofSignedNoise(2 * noiseCountX, noiseCountX, 2 * noiseCountX));//faster 3dim
				break;
			default:
				break;
			}
			noiseX = ofClamp(noiseX, -1, 1);
		}
		else
		{
			noiseX = 0;
		}

		//-

		if (ENABLE_NoiseY)
		{
			noiseCountY += dt * noiseSpeedY;
			switch (noiseDeepY)
			{
			case 1:
				noiseY = (ofSignedNoise(noiseCountY));//slower 1dim
				break;
			case 2:
				noiseY = (ofSignedNoise(noiseCountY, noiseCountY));//medium 2dim
				break;
			case 3:
				noiseY = (ofSignedNoise(2 * noiseCountY, noiseCountY, 2 * noiseCountY));//faster 3dim
				break;
			default:
				break;
			}
			noiseY = ofClamp(noiseY, -1, 1);
		}
		else
		{
			noiseY = 0;
		}

		//-

		if (ENABLE_NoiseZ)
		{
			noiseCountZ += dt * noiseSpeedZ;
			switch (noiseDeepZ)
			{
			case 1:
				noiseZ = (ofSignedNoise(noiseCountZ));//slower 1dim
				break;
			case 2:
				noiseZ = (ofSignedNoise(noiseCountZ, noiseCountZ));//medium 2dim
				break;
			case 3:
				noiseZ = (ofSignedNoise(2 * noiseCountZ, noiseCountZ, 2 * noiseCountZ));//faster 3dim
				break;
			default:
				break;
			}
			noiseZ = ofClamp(noiseZ, -1, 1);
		}
		else
		{
			noiseZ = 0;
		}
	}
	else
	{
		noiseX = 0;
		noiseY = 0;
		noiseZ = 0;
	}

	//--

	// 2. filter point

#ifdef INCLUDE_FILTER
	if (ENABLE_NoisePointFilter)
	{
		LPFpoint.setFc(ofMap(fcPoint, 1.f, 0.f, 0.01f, 0.05f, true));
		LPFpoint.update(ofVec3f(noiseX, noiseY, noiseZ));
		//LPFpoint.update(ofVec2f(noiseX, noiseY));

#ifdef INCLUDE_PLOTS
		plot_NoiseX->update(-LPFpoint.value().x);
		plot_NoiseY->update(LPFpoint.value().y);
#endif
	}
	else
#endif
	{
#ifdef INCLUDE_PLOTS
		plot_NoiseX->update(-noiseX);
		plot_NoiseY->update(noiseY);
#endif
	}

	//--

	// 3. Process modulator envelope 

	if (ENABLE_Modulator)
	{
		queue.update(dt);
		if (queue.isPlaying())
		{
			faderValue = queue.getCurrentValue();
			uint64_t time = ofGetElapsedTimeMillis() - lastStart;
			totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
			animProgress = ofMap(time / 1000.f, 0, totalTime, 0, 100, true);
			//animProgress = ofMap(time / 1000.f, 0, totalTime / globalTimeScale, 0, 100, true);
			//ofLogNotice(_FUNCTION__) <<"faderValue:" << faderValue;
		}
	}
	if (!ENABLE_Modulator)
	{
		faderValue = faderMax.get();
	}

	//--

	// 4. Filter

	// Modulator

#ifdef INCLUDE_FILTER
	if (ENABLE_NoiseModulatorFilter)
	{
		LPFmodulator.setFc(ofMap(fc, 1.f, 0.f, 0.01f, 0.5f, true));
		//LPFmodulator.setFc(ofMap(fc, 1.f, 0.f, 0.005f, 0.15f, true));
		LPFmodulator.update(faderValue);
	}
#endif

	//--

	// Modulator envelope plot

#ifdef INCLUDE_PLOTS
#ifdef INCLUDE_FILTER
	if (ENABLE_NoiseModulatorFilter) plot->update(LPFmodulator.value());
	else
#endif
	{
		plot->update(faderValue.get());
	}
#endif

	//--

	// 5. Final apply

#ifdef INCLUDE_FILTER
	if (ENABLE_NoiseModulatorFilter)
	{
		if (!ENABLE_NoisePointFilter)
		{
			noisePos = glm::vec3(
				(LPFmodulator.value() * noiseX) * (noisePowerX * 5),
				(LPFmodulator.value() * noiseY) * (noisePowerY * 5),
				(LPFmodulator.value() * noiseZ) * (noisePowerZ * 5));

			//noisePos = glm::vec2(
			//	(LPFmodulator.value() * noiseX) * (noisePowerX * 5),
			//	(LPFmodulator.value() * noiseY) * (noisePowerY * 5));
		}
		else
		{
			noisePos = glm::vec3(
				(LPFmodulator.value() * LPFpoint.value().x) * (noisePowerX * 5),
				(LPFmodulator.value() * LPFpoint.value().y) * (noisePowerY * 5),
				(LPFmodulator.value() * LPFpoint.value().z) * (noisePowerZ * 5));

			//noisePos = glm::vec2(
			//	(LPFmodulator.value() * LPFpoint.value().x) * (noisePowerX * 5),
			//	(LPFmodulator.value() * LPFpoint.value().y) * (noisePowerY * 5));
		}
	}
	else
#endif
	{
#ifdef INCLUDE_FILTER
		if (ENABLE_NoisePointFilter)
		{
			noisePos = glm::vec3(
				(faderValue * LPFpoint.value().x) * (noisePowerX * 5),
				(faderValue * LPFpoint.value().y) * (noisePowerY * 5),
				(faderValue * LPFpoint.value().z) * (noisePowerZ * 5));

			//noisePos = glm::vec2(
			//	(faderValue * LPFpoint.value().x) * (noisePowerX * 5),
			//	(faderValue * LPFpoint.value().y) * (noisePowerY * 5));
		}
		else
#endif
		{
			noisePos = glm::vec3(
				(faderValue * noiseX) * (noisePowerX * 5),
				(faderValue * noiseY) * (noisePowerY * 5),
				(faderValue * noiseZ) * (noisePowerZ * 5));

			//noisePos = glm::vec2(
			//	(faderValue * noiseX) * (noisePowerX * 5),
			//	(faderValue * noiseY) * (noisePowerY * 5));
		}
	}

	//--

	//ofLogNotice(_FUNCTION__) <<"noisePos:" << ofToString(noisePos);

	//TODO:
	// pointer back
	if (point_BACK != nullptr)
	{
		(*point_BACK) = noisePos;
	}
}

////--------------------------------------------------------------
//void NoiseAnimator::draw(bool b) {
//	//bDisabled = b;//?
//	draw();
//}

//--------------------------------------------------------------
#ifndef SURFING_ANIMATOR_ENABLE_AUTO_DRAW 
void NoiseAnimator::draw()
#endif
#ifdef SURFING_ANIMATOR_ENABLE_AUTO_DRAW 
void NoiseAnimator::draw(ofEventArgs & args)
#endif
{
	if (!bGui.get()) return;

	//--

	{
		//gui.draw();

		//-

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
		ui.Begin();
		{
			drawImGuiWidgets();
		}
		ui.End();
#endif

	}

	//-

	//if (SHOW_Plot)// && ENABLE_Noise)
		//drawPlot();
}

//--------------------------------------------------------------
void NoiseAnimator::drawImGuiWidgets() {
	if (!bGui.get()) return;
	{
		string name;

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
		ImGuiWindowFlags _flagsw = ImGuiWindowFlags_None;
		if (ui.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		{
			name = "PANEL " + label;

			bOpened = ui.BeginWindow(bGui);
			//bOpened = ui.BeginWindow(name, (bool*)&bGui.get(), _flagsw);

			if (bOpened)
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
				ui.refreshLayout();

				ui.Add(bEnableAnimator, OFX_IM_TOGGLE_SMALL);

				if (ENABLE_Modulator)
					if (ImGui::Button("START", ImVec2(_w100, 3 * _h))) {
						start();
					}

				ofxImGuiSurfing::AddToggleRoundedButton(ui.bMinimize);
				//ui.Add(ui.bMinimize);

				//-

				ImGui::Spacing();

				// All the parameters
				ui.AddGroup(params);

				if (!ui.bMinimize) {

					//ui.Add(SHOW_Plot);
					ofxImGuiSurfing::AddBigToggle(SHOW_Plot, _w100, _h, false);

					//-

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
					ui.DrawAdvancedBundle();
#endif
				}

				//--

				// Plot fbo
				if (!ui.bMinimize) {
					if (SHOW_Plot)
					{
						ImTextureID textureID = (ImTextureID)(uintptr_t)fboPlot.getTexture().getTextureData().textureID;
						ImGui::Image(textureID, plotShape);
					}
				}
			}

			//--

			if (bOpened)
			{
				ui.EndWindow();
			}
		}
#endif
	}
}

//--------------------------------------------------------------
void NoiseAnimator::restart()
{
	stop();
	start();
}

//--------------------------------------------------------------
void NoiseAnimator::start()
{
	if (!bEnableAnimator) return;

	//ofLogNotice(_FUNCTION__) <<"start()";
	if (ENABLE_Modulator)
	{
		//ofLogNotice(_FUNCTION__) <<"NoiseAnimator START";

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
		faderValue = faderMax.get();
	}
}

//--------------------------------------------------------------
void NoiseAnimator::stop()
{
	//ofLogNotice(_FUNCTION__) <<"stop()";
	if (ENABLE_Modulator)
	{
		faderValue = faderMin.get();
		queue.setInitialValue(faderMin.get());
		queue.pausePlayback(); //start the animation
		animProgress = 0;
	}
	else
	{
		faderValue = faderMax.get();
	}
}

//--------------------------------------------------------------
NoiseAnimator::~NoiseAnimator()
{
	ofRemoveListener(queue.eventQueueDone, this, &NoiseAnimator::Changed_AnimatorQueueDone);

	ofRemoveListener(ofEvents().update, this, &NoiseAnimator::update);

	#ifdef SURFING_ANIMATOR_ENABLE_AUTO_DRAW 
	ofRemoveListener(ofEvents().draw, this, &NoiseAnimator::draw);
#endif
	
	ofRemoveListener(params.parameterChangedE(), this, &NoiseAnimator::Changed_params);
	//ofRemoveListener(params_Control.parameterChangedE(), this, &NoiseAnimator::Changed_params);

	exit();
}

//--------------------------------------------------------------
void NoiseAnimator::exit()
{
	if (autoSettings) ofxSurfingHelpers::saveGroup(params, path_GLOBAL_Folder + "/" + path_Settings);
}

//--------------------------------------------------------------
void NoiseAnimator::nextCurve()
{
	curveType++;
	curveType = curveType % NUM_ANIM_CURVES;
}

//--------------------------------------------------------------
void NoiseAnimator::previousCurve()
{
	curveType--;
	if (curveType < 0) curveType = NUM_ANIM_CURVES - 1;
}

//--------------------------------------------------------------
void NoiseAnimator::Changed_params(ofAbstractParameter &e)
{
	string name = e.getName();
	if (name != "%" && name != "VALUE")
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	//ofLogVerbose(__FUNCTION__) << name << " : " << e;

//-

	if (false) {}

	//if (name == "Min" ||
	//	name == "Max" ||
	//	name == "PreDelay" ||
	//	name == "Attack" ||
	//	name == "Sustain" ||
	//	name == "Release")
	//{
	//	//totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
	//	//setupFader();
	//	//stop();
	//}

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
			//// workflow
			//auto &g1 = gui.getGroup(label);//1st level
			//auto &g2 = g1.getGroup("MODULATOR");//2nd level
			//auto &g3 = g2.getGroup(params_Timers.getName());//3nd level
			//auto &g4 = g2.getGroup(params_Bpm.getName());//3nd level
			//g3.minimize();
			//g4.minimize();
			//if (bpmMode)
			//{
			//	g3.minimize();
			//	g4.maximize();
			//}
			//else {
			//	g3.maximize();
			//	g4.minimize();
			//}
		}
	}

	else if (name == curveType.getName())
	{
		curveName = ofxAnimatable::getCurveName(AnimCurve(curveType.get()));
		AnimCurve curve = (AnimCurve)(curveType.get());
		curvePlotable.setCurve(curve);
	}

	else if (name == Reset_Noise.getName())
	{
		if (Reset_Noise)
		{
			Reset_Noise = false;

			ENABLE_Noise = true;
			ENABLE_NoiseX = true;
			ENABLE_NoiseY = true;
			ENABLE_NoiseZ = false;
			noisePowerX = DEFAUL_POWER;
			noisePowerY = DEFAUL_POWER;
			noisePowerZ = DEFAUL_POWER;
			noiseDeepX = 1.0f;
			noiseDeepY = 1.0f;
			noiseDeepZ = 1.0f;
			noiseSpeedX = 0.5f;
			noiseSpeedY = 0.5f;
			noiseSpeedZ = 0.5f;

			// workflow
			Reset_Modulator = true;
			ENABLE_Modulator = false;
		}
	}

	else if (name == Reset_Modulator.getName())
	{
		if (Reset_Modulator)
		{
			Reset_Modulator = false;

			//ENABLE_Modulator = false;

			faderValue = faderMin;

#ifdef INCLUDE_FILTER
			ENABLE_NoisePointFilter = false;
			ENABLE_NoiseModulatorFilter = false;
			fc = 0.5f;
			fcPoint = 0.5f;
#endif

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

			curveType = 3;

			faderMin = 0.f;
			faderMax = 1.0f;
		}
	}

	else if (name == "VALUE")
	{
		if (float_BACK != nullptr)
		{
			(*float_BACK) = faderValue.get();
		}
	}

	//-

	// Main enablers

	else if (name == ENABLE_Noise.getName())
	{
		// workflow
		if (!ENABLE_Noise)
		{
			//ENABLE_Modulator = false;
			stop();
		}
	}

	else if (name == bpmMode.getName())
	{
		refreshStyles();
	}

	else if (name == ENABLE_Modulator.getName())
	{
		refreshStyles();

		// workflow
		if (!ENABLE_Modulator)
		{
			faderValue = faderMax.get();
		}
		else
		{
			if (!ENABLE_Noise) ENABLE_Noise = true;
			stop();
			//faderValue = faderMin;
		}
	}

	else if (name == ENABLE_NoiseX.getName())
	{
		refreshStyles();
	}
	else if (name == ENABLE_NoiseY.getName())
	{
		refreshStyles();
	}
	else if (name == ENABLE_NoiseZ.getName())
	{
		refreshStyles();
	}
}
