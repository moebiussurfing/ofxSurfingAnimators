#include "ColorAnimator.h"



void ColorAnimator::setup() {
	FloatAnimator::setup();

	ofAddListener(ofEvents().update, this, &ColorAnimator::update);
	//ofAddListener(ofEvents().draw, this, &ColorAnimator::draw);

	colorCurrent.set("Color", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
	colorStart.set("Start", ofColor(55, 55, 55, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
	colorEnd.set("End", ofColor(200, 200, 200, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));

	//-

	// gui

#ifdef USE_IMGUI__COLORANIMATOR
#ifdef USE_IMGUI_LAYOUT_MANAGER__COLORANIMATOR
	guiManager.setImGuiAutodraw(true);
	guiManager.setup(); // initiate ImGui
	//guiManager.setUseAdvancedSubPanel(true);
#endif

#ifndef USE_IMGUI_LAYOUT_MANAGER__COLORANIMATOR
	ImGuiConfigFlags flags = ImGuiConfigFlags_DockingEnable;
	bool bRestore = true;
	bool bMouse = false;
	bool bAutoDraw = false;
	gui.setup(nullptr, bAutoDraw, flags, bRestore, bMouse);
#endif
#endif

}

void ColorAnimator::exit() {
	ofRemoveListener(ofEvents().update, this, &ColorAnimator::update);
	//ofRemoveListener(ofEvents().draw, this, &ColorAnimator::draw);
};

void ColorAnimator::update(ofEventArgs & args) {
	FloatAnimator::update(args);

	float r = ofMap(getValue(), 0, 1, colorStart.get().r, colorEnd.get().r);
	float g = ofMap(getValue(), 0, 1, colorStart.get().g, colorEnd.get().g);
	float b = ofMap(getValue(), 0, 1, colorStart.get().b, colorEnd.get().b);
	float a = ofMap(getValue(), 0, 1, colorStart.get().a, colorEnd.get().a);

	colorCurrent.set(ofColor(r, g, b, a));
}


//void ColorAnimator::draw() 
//{
//	FloatAnimator::draw();
//
//}

//void ColorAnimator::draw(ofEventArgs & args) {
//	FloatAnimator::draw(args);
//
//	//-
//
//#ifdef USE_IMGUI__COLORANIMATOR
//	//TODO:
//	return; // skip
//
//	// insert into same floatanimator window
//#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
//	string name = getNamePanel();
//	ImGuiWindowFlags _flagsw = ImGuiWindowFlags_None;
//	if (guiManager.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
//
//	guiManager.beginWindow(name.c_str(), NULL, _flagsw);
//	{
//		bool bOpen = true;
//		ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
//		_flagt |= ImGuiTreeNodeFlags_Framed;
//		if (ImGui::TreeNodeEx("COLOR", _flagt))
//		{
//			ofxImGuiSurfing::AddParameter(colorCurrent);
//			ImGui::Dummy(ImVec2(0, 5));
//			ofxImGuiSurfing::AddParameter(colorStart);
//			ofxImGuiSurfing::AddParameter(colorEnd);
//			ImGui::TreePop();
//		}
//	}
//	guiManager.endWindow();
//#endif
//
//
//#ifndef USE_IMGUI_LAYOUT_MANAGER__COLORANIMATOR
//	gui.begin();
//	{
//		string name = "COLORS";
//		//string name = getNamePanel(); // -> trying to insert on floatAnim
//		ImGui::Begin(name.c_str());
//		{
//			//bool bOpen = true;
//			//ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
//			//_flagt |= ImGuiTreeNodeFlags_Framed;
//			//if (ImGui::TreeNodeEx("COLOR", _flagt))
//			//{
//			ofxImGuiSurfing::AddParameter(colorCurrent);
//			ImGui::Dummy(ImVec2(0, 5));
//			ofxImGuiSurfing::AddParameter(colorStart);
//			ofxImGuiSurfing::AddParameter(colorEnd);
//			//ImGui::TreePop();
//		//}
//		}
//		ImGui::End();
//	}
//	gui.end();
//#endif
//
//#endif
//	//-
//}


//
////--------------------------------------------------------------
//ColorAnimator::ColorAnimator()
//{
//	//ofSetLogLevel(OF_LOG_NOTICE);
//
//	doneInstantiated = true;
//	setFps(60);
//	SHOW_gui = true;
//	guiPos = glm::vec2(500, 500);
//
//	path_GLOBAL_Folder = "ColorAnimator";
//	path_Settings = "ColorAnimator_Settings.xml";
//	autoSettings = false;
//}
////--------------------------------------------------------------
//void ColorAnimator::start()
//{
//	if (ENABLE_ColorAnims)
//	{
//		colorAnim.setColor(colorStart);
//		if (animDelay != 0) colorAnim.animateToAfterDelay(colorEnd, animDelay);
//		else colorAnim.animateTo(colorEnd);
//	}
//}
//
////--------------------------------------------------------------
//void ColorAnimator::stop()
//{
//	if (ENABLE_ColorAnims)
//	{
//		colorAnim.setColor(colorStart);
//		//colorAnim.reset();
//		animProgress = 0;
//	}
//}
//
////--------------------------------------------------------------
//void ColorAnimator::update()
//{
//	colorAnim.update(dt);
//	colorCurrent = colorAnim.getCurrentColor();
//
//	if (color_BACK != nullptr)
//	{
//		if (ENABLE_ColorAnims) color_BACK->set(colorAnim.getCurrentColor());
//		else color_BACK->set(colorStart);
//	}
//
//	if (colorAnim.isAnimating())
//	{
//		animProgress = colorAnim.getPercentDone() * 100;
//	}
//}