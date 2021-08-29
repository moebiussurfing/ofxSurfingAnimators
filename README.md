# ofxSurfingAnimators

**openFrameworks** helper add-on for the _AWESOME_ [ofxAnimatable](https://github.com/armadillu/ofxAnimatable) from **@armadillu**.  
 
Some classes to faster integration into your _OF Apps / Add-ons_:  

*FloatAnimator, NoiseAnimator, PositionAnimator, ColorAnimator, EnvelopeAnimator* and *ToggleAnimator*.

## Screenshots

#### 1_Float (2 x FloatAnimator)
![gif](/docs/readme_images/1_example_Float.gif?raw=true "gif")  

#### 2_FloatNoised (FloatAnimator + NoiseAnimator)
![gif](/docs/readme_images/2_FloatNoised.gif?raw=true "gif")  

#### 3_Point (PositionAnimator)
![image](/docs/readme_images/3_Point.gif?raw=true "gif")

#### 4_PointColored (PositionAnimator + ColorAnimator)
![image](/docs/readme_images/4_PointColored.gif?raw=true "gif")

#### 5_Envelope (EnvelopeAnimator)
![image](/docs/readme_images/5_Envelope.gif?raw=true "gif")
 	
## Overview
**openFrameworks** helper addon for ```ofxAnimatable```:
* Classes:
   * FloatAnimator.h
   * PositionAnimator.h
   * NoiseAnimator.h (Envelope Modulated 3D Noise point + Filters)
   * ColorAnimator.h
   * EnvelopeAnimator.h (ASR envelope for a Float. Double independent curve types for Input/Output.)
   * ToggleAnimator.h  

* Added **ImGui** based **GUI**.  

* Internal settings handling: Auto **Store-Recall**.
* Musical **BPM** based or absolute **time scale**.
* **Plotting** realtime value curves.
* **Curves Preview** drawing.
* **Setters** for _Start/End_, time mode, duration and pre pause, curve tween types, loop modes, repeats ...etc. 

## Dependencies
* [ofxAnimatable](https://github.com/armadillu/ofxAnimatable)  
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxHistoryPlot](https://github.com/moebiussurfing/ofxHistoryPlot)  
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)  
* [ofxImGui](https://github.com/Daandelange/ofxImGui/) [Fork]
* [ofxSurfingPresets](https://github.com/moebiussurfing/ofxSurfingPresets) [Optional]  

Bundled into ```OF_ADDON/libs```.  
No need to add manually to the **PROJECT GENERATOR**!    
[ofxBiquadFilter](https://github.com/dzlonline/ofxBiquadFilter)[Optional] Used to Filter the Noise Point  

## Usage
- Look the Examples.

### ofApp.h
```.c++
#include "FloatAnimator.h"
FloatAnimator animatorFloat;
```

### ofApp.cpp
```.c++
void setup() {
   animatorFloat.setup(0.0f, 1.0f);
}

void update() {
   float value = animatorFloat.getValue();
}

void keyPressed(int key){
   if (key == ' ') animatorFloat.start();
}
```

## Tested Systems
- **Windows10** / **VS2017** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
MIT License
