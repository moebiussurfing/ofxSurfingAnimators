#include "EnvelopeAnimator.h"

//--------------------------------------------------------------
void EnvelopeAnimator::onAnimQueueDone(ofxAnimatableQueue::EventArg &)
{
	//cout << "EnvelopeAnimator FINISHED" << endl;

	faderValue = faderMin.get();
	if (float_BACK != nullptr)
	{
		(*float_BACK) = faderValue;
	}
	//animProgress = 0;

	if (faderLoop)
	{
		stop();
		start();
	}
}

//--------------------------------------------------------------
EnvelopeAnimator::EnvelopeAnimator()
{
	ofSetLogLevel(OF_LOG_SILENT);

	doneInstantiated = true;
	setFps(60);
	SHOW_gui = true;
	guiPos = glm::vec2(700, 500);
	label = "Envelope_Modulator";
	path_GLOBAL_Folder = "/";
	path = path_GLOBAL_Folder + label + ".xml";
}

//--------------------------------------------------------------
void EnvelopeAnimator::setup()
{
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

	curveTypeIn.set("Curve In", AnimCurve(16), 0, NUM_ANIM_CURVES - 1);
	curveNameIn.set("", "");
	curveTypeOut.set("Curve Out", AnimCurve(16), 0, NUM_ANIM_CURVES - 1);
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

	gui.setup(label);
	gui.add(params);
	gui.add(SHOW_Plot);
	gui.setPosition(guiPos.x, guiPos.y);

	ofAddListener(params.parameterChangedE(), this, &EnvelopeAnimator::Changed_params);

	ofAddListener(queue.eventQueueDone, this, &EnvelopeAnimator::onAnimQueueDone);

	setupPlot();

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
void EnvelopeAnimator::setupAnimator()
{
	faderAnimIn = AnimCurve(curveTypeIn.get());
	faderAnimOut = AnimCurve(curveTypeOut.get());

	queue.clearQueue();
	queue.setInitialValue(faderMin.get());
	queue.addTransition(faderMin.get(), faderDelay.get(), LINEAR);
	queue.addTransition(faderMax.get(), faderAttack.get(), faderAnimIn);
	queue.addTransition(faderMax.get(), faderSustain.get(), LINEAR);
	queue.addTransition(faderMin.get(), faderRelease.get(), faderAnimOut);
	//queue.addTransition(faderMin.get(), faderRelease.get(), faderAnimIn);

	totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();
}

//--------------------------------------------------------------
void EnvelopeAnimator::setupPlot()
{
	plot = new ofxHistoryPlot(NULL, "fader", 100, false);
	plot->setBackgroundColor(ofColor(0, 230));
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
void EnvelopeAnimator::update()
{
	//if (!bPaused) 
	{
		queue.update(dt);
		if (queue.isPlaying())
		{
			uint64_t time = ofGetElapsedTimeMillis() - lastStart;
			faderValue = queue.getCurrentValue();

			totalTime = faderDelay.get() + faderAttack.get() + faderSustain.get() + faderRelease.get();

			animProgress = ofMap(time / 1000.f, 0, totalTime, 0, 100, true);
		}
		plot->update(faderValue.get());
	}
}

//--------------------------------------------------------------
void EnvelopeAnimator::draw()
{
	if (SHOW_gui)
	{
		if (SHOW_Plot)
		{
			ofPushStyle();

			int x, y, size, px;
			size = 92;
			int pad = 2;
			bool stateColor;
			string str;
			int wplot = gui.getShape().getWidth() - 18;
			int padPlots = 5;
			//int padPlots = 20;//when using curve labels

			x = gui.getPosition().x + pad;
			y = gui.getPosition().y + gui.getHeight() + 15;

			//-

			//1. two curves
			//hide curve plot when attack/release are 0
			if (faderRelease != 0 || faderAttack != 0)
			{
				//curve type
				if (curveShow)
				{
					curvePlotableIn.drawCurve(x + pad, y, size, true, ofColor(255), false);
					curvePlotableOut.drawCurve(x + size + 12, y, size, true, ofColor(255), false);
				}
			}

			//-

			//2. live value enveloped plot
			plot->draw(x + pad, y + size + pad + padPlots, wplot, size);//left
			//plot->draw(x + size + pad, y + size + pad + padPlots, size, size);//right

			int rx, ry, rw, rh;
			rh = 8;
			rx = x + pad;//left
			//rx = x + size + pad;//right
			ry = y + 2 * size - rh + padPlots;
			//ry = y + size - rh;

			if (isAnimating())
			{
				//2.1 progress bar
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
			////vertical line time
			//float h;//display delay wait progress
			//if (floatAnimator.isWaitingForAnimationToStart()) h = floatAnimator.waitTimeLeftPercent() * size;
			//else h = size;
			//px = ofMap(floatAnimator.getPercentDone(), 0, 1, x, x + size, true);
			//ofSetColor(ofColor::red, 200);
			//ofSetLineWidth(2.0);
			//ofDrawLine(px, y + size, px, y + size - h);

			//-

			//2.2 vertical red bar value
			float vb = ofMap(faderValue.get(), faderMin.get(), faderMax.get(), 0.f, 1.f, true);
			int w = 10;
			rx += wplot + 3;
			ry += rh + pad;
			ofFill();

			//bg
			ofSetColor(0, 255);
			pad = 2;//make black outspace
			ofRectangle r2;
			r2 = ofRectangle(rx, ry, w, -size);
			ofDrawRectangle(r2);

			//bar
			ofSetColor(ofColor::red, 225);
			r2 = ofRectangle(rx + 0.5*pad, ry - 0.5*pad, w - pad, pad - MAX(vb*size, 1));
			ofDrawRectangle(r2);

			//-

			////label
			////stateColor = queue.isPlaying();
			//stateColor = queue.isPlaying() && (faderValue > 0);
			//str = label;
			////str = "2 ALPHA";
			//ofDrawBitmapStringHighlight(str, rx + 4, y - 10,
			//	stateColor ? ofColor::white : ofColor::black,
			//	!stateColor ? ofColor::white : ofColor::black);

			ofPopStyle();
		}

		//-

		gui.draw();
	}
}

//--------------------------------------------------------------
void EnvelopeAnimator::start()
{
	if (ENABLE_Envelope)
	{
		//mark
		//plot->update(1);

		//TODO: 
		//rebuild every start
		setupAnimator();
		//stop();

		lastStart = ofGetElapsedTimeMillis();
		queue.setInitialValue(faderMin.get());
		queue.startPlaying();//start the animation
	}
	else
	{
		faderValue = faderMax;
	}
}

//--------------------------------------------------------------
void EnvelopeAnimator::startOn()
{
	if (ENABLE_Envelope)
	{
		setupAnimatorOn();

		lastStart = ofGetElapsedTimeMillis();
		queue.setInitialValue(faderMin.get());
		queue.startPlaying();//start the animation
	}
	else
	{
		faderValue = faderMax;
	}
}

//--------------------------------------------------------------
void EnvelopeAnimator::startOff()
{
	if (ENABLE_Envelope)
	{
		setupAnimatorOff();

		lastStart = ofGetElapsedTimeMillis();
		//faderValue = faderMax.get();
		queue.setInitialValue(faderMax.get());
		queue.startPlaying();//start the animation
	}
	else
	{
		faderValue = faderMax;
	}
}

////--------------------------------------------------------------
//void EnvelopeAnimator::pause()
//{
//	queue.pausePlayback();
//}

//--------------------------------------------------------------
void EnvelopeAnimator::stop()
{
	//cout << "stop()" << endl;
	if (ENABLE_Envelope)
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
EnvelopeAnimator::~EnvelopeAnimator()
{
	ofRemoveListener(queue.eventQueueDone, this, &EnvelopeAnimator::onAnimQueueDone);

	exit();
}

//--------------------------------------------------------------
void EnvelopeAnimator::exit()
{
	/*if (autoSettings)*/
	ofxSurfingHelpers::saveGroup(params, path);
}

//--------------------------------------------------------------
void EnvelopeAnimator::nextCurve()
{
	curveTypeIn++;
	curveTypeIn = curveTypeIn % NUM_ANIM_CURVES;
}

//--------------------------------------------------------------
void EnvelopeAnimator::previousCurve()
{
	curveTypeIn--;
	if (curveTypeIn < 0)
		curveTypeIn = NUM_ANIM_CURVES - 1;
}

//--------------------------------------------------------------
void EnvelopeAnimator::Changed_params(ofAbstractParameter &e)
{
	string name = e.getName();
	if (name != "%" &&
		name != "Value")
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

	if (false) {}

	//bpm engine
	else if (
		name == bpmMode.getName() ||
		name == bpmSpeed.getName() ||
		name == bpmBeatDelay.getName() ||
		name == bpmBeatAttack.getName() ||
		name == bpmBeatSustain.getName() ||
		name == bpmBeatRelease.getName())
	{
		if (bpmMode)
		{
			float _bar = 60.f / bpmSpeed.get();//one bar duration in seconds to this bpm speed
			float _ratio = 2.0f;
			faderDelay = (_bar / _ratio) * (float)bpmBeatDelay;
			faderAttack = (_bar / _ratio) * (float)bpmBeatAttack;
			faderSustain = (_bar / _ratio) * (float)bpmBeatSustain;
			faderRelease = (_bar / _ratio) * (float)bpmBeatRelease;
		}

		if (name == bpmMode.getName())
		{
			//workflow
			refreshGui();
		}
	}

	else if (name == curveTypeIn.getName())
	{
		curveNameIn = ofxAnimatable::getCurveName(AnimCurve(curveTypeIn.get()));
		AnimCurve curve = (AnimCurve)(curveTypeIn.get());
		curvePlotableIn.setCurve(curve);
	}

	else if (name == curveTypeOut.getName())
	{
		curveNameOut = ofxAnimatable::getCurveName(AnimCurve(curveTypeOut.get()));
		AnimCurve curve = (AnimCurve)(curveTypeOut.get());
		curvePlotableOut.setCurve(curve);
	}

	else if (name == "Reset")
	{
		if (reset)
		{
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

	else if (name == "Value")
	{
		if (float_BACK != nullptr)
		{
			(*float_BACK) = faderValue;
		}
	}
	else if (name == "Enable Envelope")
	{
		if (!ENABLE_Envelope)
		{
			faderValue = faderMax;
		}
	}
}
