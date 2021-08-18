#include "PositionAnimator.h"

void PositionAnimator::setup() {
	//setNameLabel("posAnim");
	//label

	FloatAnimator::setup();

	ofAddListener(ofEvents().update, this, &PositionAnimator::update);

	posStart = glm::vec2(0, 0);
	posEnd = glm::vec2(500, 500);

	//guiManager.setImGuiAutodraw(false);//? TODO: improve multicontext mode..
	guiManager.setup();
}

void PositionAnimator::exit() {
	ofRemoveListener(ofEvents().update, this, &PositionAnimator::update);
};

void PositionAnimator::update(ofEventArgs & args) {
	FloatAnimator::update(args);

	//float x = ofLerp(getValue(), posStart.get().x, posEnd.get().x);
	//float y = ofLerp(getValue(), posStart.get().y, posEnd.get().y);

	float x = ofMap(getValue(), 0, 1, posStart.get().x, posEnd.get().x);
	float y = ofMap(getValue(), 0, 1, posStart.get().y, posEnd.get().y);

	pos = glm::vec2(x, y);

	//cout << "getValue(): " << getValue() << endl;
	//cout << "x,y: " << x << ", " << y << endl;
}

////--------------------------------------------------------------
//void PositionAnimator::draw(ofEventArgs & args)
//{
//	//FloatAnimator::draw(args);
//
//	//TODO. split begin/end imGui? inheritage? derive?
//	//guiManager.begin();
//	{
//		//ImGui::Begin("PANEL posAnim");
//		drawImGuiWidgetsExtra();
//		//ImGui::End();
//	}
//	//guiManager.end();
//}

//--------------------------------------------------------------
void PositionAnimator::drawImGuiWidgetsExtra() {
	float _w100 = ofxImGuiSurfing::getWidgetsWidth(1);
	float _h = ofxImGuiSurfing::getWidgetsHeight();
	ImGui::Button("TEST_ExtraP", ImVec2(_w100, 2 * _h));

	ofxImGuiSurfing::AddParameter(pos);
	ofxImGuiSurfing::AddParameter(posStart);
	ofxImGuiSurfing::AddParameter(posEnd);
}


////--------------------------------------------------------------
//PositionAnimator::PositionAnimator()
//{
//	path_GLOBAL_Folder = "PositionAnimator";
//	path_Settings = "PositionAnimator_Settings.xml";
//	autoSettings = false;
//
//	//ofSetLogLevel(OF_LOG_NOTICE);
//
//	doneInstantiated = true;
//	setFps(60);
//	SHOW_Gui = true;
//	guiPos = glm::vec2(500, 500);
//}
//
////--------------------------------------------------------------
//void PositionAnimator::Changed_AnimatorDone(ofxAnimatable::AnimationEvent &)
//{
//	ofLogVerbose(__FUNCTION__);
//
//	//workflow
//	//force ending points to avoid offset bc dt resolution?
//	if (repeatMode == 2 || repeatMode == 3 || repeatMode == 5)//with a back mode. force to start
//	{
//		animProgress = 0;
//		pos = posStart;
//	}
//	else {//not a back mode. force to end
//		pos = posEnd;
//		animProgress = 100;
//	}
//
//	bDone = true;
//}
//
////--------------------------------------------------------------
//void PositionAnimator::nextCurve()
//{
//	curveType++;
//	curveType = curveType % NUM_ANIM_CURVES;
//
//	if (ModeBrowse) start();
//}
//
////--------------------------------------------------------------
//void PositionAnimator::previousCurve()
//{
//	curveType--;
//	if (curveType < 0) curveType = 0;
//	//if (curveType < 0) curveType = NUM_ANIM_CURVES - 1;
//
//	if (ModeBrowse) start();
//}
//
////--------------------------------------------------------------
//void PositionAnimator::setup()
//{
//	//ofxSurfingHelpers::setThemeDark_ofxGui;
//
//	ENABLE_valueAnim.set("Enable Position Animator", true);
//
//	pos.set("Position", glm::vec2(0), glm::vec2(-1920, -1080), glm::vec2(1920, 1080));
//	posStart.set("From", glm::vec2(0), glm::vec2(-1920, -1080), glm::vec2(1920, 1080));
//	posEnd.set("To", glm::vec2(0), glm::vec2(-1920, -1080), glm::vec2(1920, 1080));
//
//	duration.set("Duration", 1.f, 0.f, 5.f);
//	animDelay.set("PreDelay", 0.f, 0.f, 5.f);
//	curveType.set("Curve Type", 16, 0, NUM_ANIM_CURVES - 1);
//	repeatMode.set("Repeat Mode", 0, 0, 5);
//	curveName.set("", "");
//	repeatName.set("", "");
//	repeatName = AnimRepeat_ToStr(repeatMode.get());
//	repeatTimes.set("Times", 1, 1, 10);
//	reset.set("Reset", false);
//	animProgress.set("%", 0, 0, 100);
//	//anim_loop.set("Loop", false);
//
//	//exclude from settings
//	//value.setSerializable(false);
//	curveName.setSerializable(false);
//	repeatName.setSerializable(false);
//	reset.setSerializable(false);
//	animProgress.setSerializable(false);
//	//anim_loop.setSerializable(false);
//	//curveShow.setSerializable(false);
//	//SHOW_Plot.setSerializable(false);
//
//	//--
//
//	//bpm engine
//	bpmMode.set("BPM Mode", true);
//	bpmSpeed.set("BPM", 120.f, 10.f, 400.f);
//	bpmBeatDuration.set("Duration Beats", 4, 1, 16);
//	bpmBeatDelay.set("PreDelay Beats", 2, 0, 8);
//	params_Bpm.setName("BPM Engine");
//
//	params_Bpm.add(bpmSpeed);
//	params_Bpm.add(bpmMode);
//	params_Bpm.add(bpmBeatDuration);
//	params_Bpm.add(bpmBeatDelay);
//
//	//-
//
//	//controls
//	//params_Control.setName("Position Animator Controls");
//	params_Control.setName(label);
//	params_Control.add(ENABLE_valueAnim);
//	//params_Control.add(_params_Bpm);
//	////params_Control.setName(label + " CONTROLS");
//	//params_Control.add(duration);
//
//	params_Helpers.setName("Animator Helpers");
//	params_Helpers.add(ModeBrowse.set("Mode Browser", true));
//	params_Helpers.add(SHOW_Plot);
//
//	//--
//
//	//settings
//	params.setName(label);
//	params.add(ENABLE_valueAnim);
//
//	params.add(pos);
//	params.add(posStart);
//	params.add(posEnd);
//
//	params_Time.setName("Time Engine");
//	params_Time.add(duration);
//	params_Time.add(animDelay);
//	params.add(params_Time);
//
//	params.add(params_Bpm);
//	params.add(curveType);
//	params.add(curveName);
//	params.add(repeatMode);
//	params.add(repeatName);
//	params.add(repeatTimes);
//	params.add(reset);
//	params.add(animProgress);
//
//	ofAddListener(params.parameterChangedE(), this, &PositionAnimator::Changed_params);
//	ofAddListener(params_Bpm.parameterChangedE(), this, &PositionAnimator::Changed_params);
//
//	//-
//
//	//gui
//	gui.setup(label);
//	gui.setPosition(guiPos.x, guiPos.y - 20);
//	gui.add(params);
//	gui.add(SHOW_Plot);
//
//	//-
//
//	//autoSettings = true;
//	if (autoSettings) ofxSurfingHelpers::loadGroup(params, path_GLOBAL_Folder + "/" + path_Settings);
//
//	ofAddListener(animatorFloat1.animFinished, this, &PositionAnimator::Changed_AnimatorDone);
//
//	//-
//
//	//startup
//	repeatName = AnimRepeat_ToStr(repeatMode.get());
//	curveName = animatorFloat1.getCurveName(AnimCurve(curveType.get()));
//}
//
////--------------------------------------------------------------
//void PositionAnimator::saveSettings()
//{
//	ofLogVerbose(__FUNCTION__);
//	if (autoSettings) ofxSurfingHelpers::saveGroup(params, path_GLOBAL_Folder + "/" + path_Settings);
//}
//
////--------------------------------------------------------------
//void PositionAnimator::loadSettings()
//{
//	ofLogVerbose(__FUNCTION__);
//	if (autoSettings) ofxSurfingHelpers::loadGroup(params, path_GLOBAL_Folder + "/" + path_Settings);
//}
//
////--------------------------------------------------------------
//void PositionAnimator::start()
//{
//	ofLogVerbose(__FUNCTION__);
//
//	if (ENABLE_valueAnim)
//	{
//		animatorFloat1.setDuration(duration);
//		animatorFloat1.setPosition(ofPoint(posStart.get().x, posStart.get().y));
//		//animatorFloat1.animateTo(ofPoint(posEnd.get().x, posEnd.get().y));
//
//		if (animDelay != 0)
//		{
//			ofPoint p{ posEnd.get().x, posEnd.get().y };
//			animatorFloat1.animateToAfterDelay(p, animDelay);
//		}
//		else
//		{
//			animatorFloat1.animateTo(ofPoint(posEnd.get()));
//		}
//
//		//animatorFloat1.reset(valueStart);
//		//if (animDelay != 0.f) animatorFloat1.animateToAfterDelay(valueEnd, animDelay);
//		//else animatorFloat1.animateTo(valueEnd);
//	}
//
//	bDone = false;
//}
//
////--------------------------------------------------------------
//void PositionAnimator::stop()
//{
//	ofLogNotice(__FUNCTION__);
//
//	if (ENABLE_valueAnim)
//	{
//		animatorFloat1.reset();
//		animProgress = 0;
//	}
//}
//
////--------------------------------------------------------------
//void PositionAnimator::update()
//{
//	animatorFloat1.update(dt);
//
//	//if (position_BACK != nullptr)
//	//{
//	//	//TODO: glm pointer?
//	//    if (ENABLE_valueAnim) position_BACK->set(animatorFloat1.getCurrentPosition());
//	//    else position_BACK->(posStart.get());
//	//}
//
//	if (animatorFloat1.isAnimating())
//	{
//		animProgress = MIN(100, animatorFloat1.getPercentDone() * 100);
//		pos = glm::vec2(animatorFloat1.getCurrentPosition().x, animatorFloat1.getCurrentPosition().y);
//	}
//}
//
////--------------------------------------------------------------
//void PositionAnimator::drawCurve(glm::vec2 &p)
//{
//	//float sizeCurvePlot = 100;
//	//animatorFloat1.drawCurve(p.x, p.y, sizeCurvePlot, true, ofColor(255));
//
//	if (SHOW_Plot)
//	{
//		ofPushStyle();
//		ofFill();
//
//		float x, y, px, w;
//		bool stateColor;
//		string str;
//
//		x = p.x;
//		y = p.y;
//
//		//curve type plot
//
//		//bCustomPositionPlot = !SHOW_Gui;
//		//if (bCustomPositionPlot) {
//		//	x = positionPlot.x;
//		//	y = positionPlot.y;
//		//}
//		//else {
//		//	x = gui.getPosition().x + 45;
//		//	y = gui.getPosition().y + gui.getHeight() + 15;
//		//}
//
//		animatorFloat1.drawCurve(x, y, sizeCurvePlot, true, ofColor(255));
//
//		//vertical line time
//		float h;//display delay wait progress
//		if (animatorFloat1.isWaitingForAnimationToStart()) h = animatorFloat1.waitTimeLeftPercent() * sizeCurvePlot;
//		else h = sizeCurvePlot;
//		px = ofMap(animatorFloat1.getPercentDone(), 0, 1, x, x + sizeCurvePlot, true);
//		ofSetColor(ofColor::red, 255);
//		ofSetLineWidth(2.0);
//		ofDrawLine(px, y + sizeCurvePlot, px, y + sizeCurvePlot - h);
//
//		////vertical bar value
//		//ofRectangle r;
//		//w = 12;
//		//x += sizeCurvePlot + 7;
//		//ofFill();
//		////bg
//		//ofSetColor(0, 200);
//		//r = ofRectangle(x, y + sizeCurvePlot, w, -sizeCurvePlot);
//		//float pad = 2;//make black outpsace
//		////r = ofRectangle(x - pad * 0.5f, y + sizeCurvePlot + pad * 0.5f, w + pad, -sizeCurvePlot - pad);
//		//ofDrawRectangle(r);
//		////bar
//		//ofSetColor(ofColor::red, 200);
//		//float vb = ofMap(value.get(), valueStart, valueEnd, 0.f, 1.f, true);
//		//r = ofRectangle(x + pad * 0.5f, y - pad * 0.5f + sizeCurvePlot, w - pad, pad - MAX(vb*sizeCurvePlot, 1));
//		////r = ofRectangle(x + pad * 0.5f, y - pad * 0.5f + sizeCurvePlot, w - pad, pad - MAX(value.get()*sizeCurvePlot, 1));
//		////r = ofRectangle(x, y + sizeCurvePlot, w, -MAX(value.get()*sizeCurvePlot, 1));
//		//ofDrawRectangle(r);
//
//		//-
//
//		//stateColor = animatorFloat1.isAnimating();
//		//str = label;
//		////str = "4 COLOR";
//		//ofDrawBitmapStringHighlight(str, x + 5, y - 10,
//		//	stateColor ? ofColor::white : ofColor::black,
//		//	!stateColor ? ofColor::white : ofColor::black);
//
//		ofPopStyle();
//	}
//
//}
////--------------------------------------------------------------
//void PositionAnimator::draw()
//{
//	if (SHOW_Gui)
//	{
//		gui.draw();
//	}
//
//	if (SHOW_Plot)
//	{
//		ofPushStyle();
//		ofFill();
//
//		float x, y, size, px, w;
//		size = 100;
//		bool stateColor;
//		string str;
//
//		//curve type plot
//		bCustomPositionPlot = !SHOW_Gui;
//		if (bCustomPositionPlot) {
//			x = positionPlot.x;
//			y = positionPlot.y;
//		}
//		else {
//			x = gui.getPosition().x + 45;
//			y = gui.getPosition().y + gui.getHeight() + pad;
//		}
//
//		animatorFloat1.drawCurve(x, y, size, true, ofColor(255));
//
//		//vertical line time
//		float h;//display delay wait progress
//		if (animatorFloat1.isWaitingForAnimationToStart()) h = animatorFloat1.waitTimeLeftPercent() * size;
//		else h = size;
//		px = ofMap(animatorFloat1.getPercentDone(), 0, 1, x, x + size, true);
//		ofSetColor(ofColor::red, 255);
//		ofSetLineWidth(2.0);
//		ofDrawLine(px, y + size, px, y + size - h);
//
//		////vertical bar value
//		//ofRectangle r;
//		//w = 12;
//		//x += sizeCurvePlot + 7;
//		//ofFill();
//		////bg
//		//ofSetColor(0, 200);
//		//r = ofRectangle(x, y + sizeCurvePlot, w, -sizeCurvePlot);
//		//float pad = 2;//make black outpsace
//		////r = ofRectangle(x - pad * 0.5f, y + sizeCurvePlot + pad * 0.5f, w + pad, -sizeCurvePlot - pad);
//		//ofDrawRectangle(r);
//		////bar
//		//ofSetColor(ofColor::red, 200);
//		//float vb = ofMap(value.get(), valueStart, valueEnd, 0.f, 1.f, true);
//		//r = ofRectangle(x + pad * 0.5f, y - pad * 0.5f + sizeCurvePlot, w - pad, pad - MAX(vb*sizeCurvePlot, 1));
//		////r = ofRectangle(x + pad * 0.5f, y - pad * 0.5f + sizeCurvePlot, w - pad, pad - MAX(value.get()*sizeCurvePlot, 1));
//		////r = ofRectangle(x, y + sizeCurvePlot, w, -MAX(value.get()*sizeCurvePlot, 1));
//		//ofDrawRectangle(r);
//
//		//-
//
//		//stateColor = animatorFloat1.isAnimating();
//		//str = label;
//		////str = "4 COLOR";
//		//ofDrawBitmapStringHighlight(str, x + 5, y - 10,
//		//	stateColor ? ofColor::white : ofColor::black,
//		//	!stateColor ? ofColor::white : ofColor::black);
//
//		ofPopStyle();
//	}
//}
//
////--------------------------------------------------------------
//PositionAnimator::~PositionAnimator()
//{
//	ofRemoveListener(params.parameterChangedE(), this, &PositionAnimator::Changed_params);
//	ofRemoveListener(params_Bpm.parameterChangedE(), this, &PositionAnimator::Changed_params);
//	ofRemoveListener(animatorFloat1.animFinished, this, &PositionAnimator::Changed_AnimatorDone);
//	exit();
//}
//
////--------------------------------------------------------------
//void PositionAnimator::exit()
//{
//	if (autoSettings) ofxSurfingHelpers::saveGroup(params, path_GLOBAL_Folder + "/" + path_Settings);
//}
//
////--------------------------------------------------------------
//void PositionAnimator::Changed_params(ofAbstractParameter &e)
//{
//	string name = e.getName();
//	if (name != "%") ofLogVerbose(__FUNCTION__) << name << " : " << e;
//
//	//-
//
//	if (false) {}
//
//	else if (name == ENABLE_valueAnim.getName())
//	{
//		if (!ENABLE_valueAnim && animatorFloat1.isAnimating()) animatorFloat1.pause();
//		else if (ENABLE_valueAnim && !animatorFloat1.isAnimating()) animatorFloat1.resume();
//	}
//
//	//tween duration
//	else if (name == "Duration")
//	{
//		animatorFloat1.setDuration(duration.get());
//	}
//
//	//bpm engine
//	else if (name == bpmMode.getName())
//	{
//		// exclude bpm or time info depends of time mode
//
//		params_Time.setSerializable(!bpmMode);
//		duration.setSerializable(!bpmMode);
//		animDelay.setSerializable(!bpmMode);
//
//		params_Bpm.setSerializable(bpmMode);
//		bpmSpeed.setSerializable(bpmMode);
//		bpmBeatDuration.setSerializable(bpmMode);
//		bpmBeatDelay.setSerializable(bpmMode);
//		
//		if (bpmMode) {
//			float _bar = 60.f / bpmSpeed.get();//one bar duration in seconds to this bpm speed
//			duration = (_bar / 8.f) * (float)bpmBeatDuration;
//			animDelay = (_bar / 8.f) * (float)bpmBeatDelay;
//		}
//
//		// gui workflow
//		gui.getGroup(params.getName()).getGroup(params_Time.getName()).minimize();
//		gui.getGroup(params.getName()).getGroup(params_Bpm.getName()).minimize();
//		if (!bpmMode) gui.getGroup(params.getName()).getGroup(params_Time.getName()).maximize();
//		else gui.getGroup(params.getName()).getGroup(params_Bpm.getName()).maximize();
//	}
//
//	else if (name == bpmBeatDuration.getName() || name == bpmSpeed.getName() || name == bpmBeatDelay.getName())
//	{
//		if (bpmMode) {
//			float _bar = 60.f / bpmSpeed.get();//one bar duration in seconds to this bpm speed
//			duration = (_bar / 8.f) * (float)bpmBeatDuration;
//			animDelay = (_bar / 8.f) * (float)bpmBeatDelay;
//		}
//	}
//
//	else if (name == "Curve Type")
//	{
//		animatorFloat1.setCurve(AnimCurve(curveType.get()));
//		curveName = animatorFloat1.getCurveName(AnimCurve(curveType.get()));
//	}
//	else if (name == "Repeat Mode")
//	{
//		repeatName = AnimRepeat_ToStr(repeatMode.get());
//		animatorFloat1.setRepeatType(AnimRepeat(repeatMode.get()));
//	}
//	else if (name == "Times")
//	{
//		animatorFloat1.setRepeatTimes(repeatTimes.get());
//	}
//	else if (name == "Reset")
//	{
//		if (reset)
//		{
//			reset = false;
//
//			doReset();
//		}
//	}
//
//	//else if (name == "Loop")
//	//{
//	//	if (anim_loop)
//	//	{
//	//		repeatMode_anim_loop_PRE = repeatMode;
//	//		repeatMode = 2;
//	//		repeatName = AnimRepeat_ToStr(repeatMode.get());
//	//		animatorFloat1.setRepeatType(AnimRepeat(repeatMode.get()));
//	//	}
//	//	else
//	//	{
//	//		repeatMode = repeatMode_anim_loop_PRE;
//	//		repeatName = AnimRepeat_ToStr(repeatMode.get());
//	//		animatorFloat1.setRepeatType(AnimRepeat(repeatMode.get()));
//	//	}
//	//}
//
//	//position
//	else if (name == "Position")
//	{
//	}
//	else if (name == "From")
//	{
//		animatorFloat1.setPosition(posStart.get());
//		//if (ENABLE_valueAnim && animatorFloat1.isAnimating())
//		//{
//		//    start();
//		//}
//	}
//	else if (name == "End")
//	{
//		animatorFloat1.setPosition(posStart.get());
//		//animatorFloat1.setColor(valueEnd);
//		//if (ENABLE_valueAnim && animatorFloat1.isAnimating())
//		//{
//		//    start();
//		//}
//	}
//}
//
////--------------------------------------------------------------
//void PositionAnimator::doReset()
//{
//	repeatMode = 0;
//	curveType = 3;// linear
//	repeatTimes = 1;
//
//	animDelay = 0.f;
//	duration = 1.f;
//
//	bpmMode = true;
//	bpmBeatDuration = 16;
//	bpmBeatDelay = 2;
//	bpmSpeed = 120;
//
//	pos = glm::vec2(0, 0);
//	posStart = glm::vec2(0, 0);
//	posEnd = glm::vec2(0, 0);
//	pos = posStart;
//}