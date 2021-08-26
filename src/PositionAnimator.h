#pragma once

#include "ofMain.h"

#include "FloatAnimator.h"

class PositionAnimator : public FloatAnimator {

public:
	
	void setup() override;
	void setupExtra() override;
	void update(ofEventArgs & args);
	void drawImGuiWidgetsExtra() override;
	
	void exit();

	ofParameter<glm::vec2> posStart, posEnd, pos;

	ofParameterGroup params_Positions{ "Positions" };

	//--------------------------------------------------------------
	void setPosition(glm::vec2 p) {
		pos = p;
	}
	//--------------------------------------------------------------
	void setPositionStart(glm::vec2 p) {
		posStart = p;
	}
	//--------------------------------------------------------------
	void setPositionEnd(glm::vec2 p) {
		posEnd = p;
	}
	//--------------------------------------------------------------
	void setToPositionEnd(glm::vec2 p) {
		setPositionStart(pos);
		posEnd = p;
	}
	//--------------------------------------------------------------
	void goToPosition(glm::vec2 p) {
		setToPositionEnd(p);
		start();
	}
	//--------------------------------------------------------------
	glm::vec2 getCurrentPosition() {
		return pos;
	}
	//--------------------------------------------------------------
	glm::vec2 getPositionStart() {
		return posStart;
	}
	//--------------------------------------------------------------
	glm::vec2 getPositionEnd() {
		return posEnd;
	}
};
