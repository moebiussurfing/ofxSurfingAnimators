# ofxSurfingAnimators

**openFrameworks** helper add-on for [ofxAnimatable](https://github.com/armadillu/ofxAnimatable) from **@armadillu**  
 
Some classes to faster integration into your OF Apps/Add-ons:  
*ColorAnimator, EnvelopeAnimator, FloatAnimator, NoiseAnimator, PositionAnimator* and *ToggleAnimator*.

## Screenshots

#### 1_Float (2 x FloatAnimator)
![gif](/docs/readme_images/1_example_Float.gif?raw=true "gif")  

#### 2_FloatNoised (FloatAnimator + NoiseAnimator)
![gif](/docs/readme_images/2_example_FloatNoised.gif?raw=true "gif")  

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
* Added **ImGui** based **GUI**:  

Setters for Start/End, time mode, duration and pre pause, curve tween types, loop modes, repeats ...etc. 
* Handle Settings: Auto store-recall.
* Musical **BPM** based or absolute **time scale**.
* **Plotting** realtime value curves.
* **Curves Preview** drawing.

## Dependencies
* [ofxAnimatable](https://github.com/armadillu/ofxAnimatable)  
* [ofxHistoryPlot](https://github.com/moebiussurfing/ofxHistoryPlot)  
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)  
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxImGui](https://github.com/Daandelange/ofxImGui/) FORK
* [ofxSurfingPresets](https://github.com/moebiussurfing/ofxSurfingPresets) [Recommended]  
* ofxGui [ **OF** ]

Bundled into ```OF_ADDON/libs```. No need to add to the **PROJECT GENERATOR**!    
(To Filter Noise point)  
[ofxBiquadFilter](https://github.com/dzlonline/ofxBiquadFilter)  
[ofxOneEuroFilter](https://github.com/i-n-g-o/ofxOneEuroFilter)

## Usage
- Look the examples.

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
