# ofxSurfingAnimators

**openFrameworks** helper addon for ```ofxAnimatable``` from **@armadillu**  
(https://github.com/armadillu/ofxAnimatable)  
Some typical classes to faster integration into your OF Apps:  
*ColorAnimator, FadeAnimator, FloatAnimator, NoiseAnimator, PositionAnimator* and *ToggleAnimator*.

## Screenshot
/example_Basic (FloatAnimator + NoiseAnimator)
![image](/readme_images/Capture.PNG?raw=true "image")  
/example_Envelope (FadeAnimator ADSR)
![image](/readme_images/Capture2.PNG?raw=true "image")

## Overview
**openFrameworks** helper addon for ```ofxAnimatable```:
* Classes:
   * FloatAnimator.h
   * FadeAnimator.h (ADSR envelope for a Float)
   * PositionAnimator.h
   * NoiseAnimator.h (Many Noise settings + Filters)
   * ColorAnimator.h
   * ToggleAnimator.h
* Added GUI:  
Setters for start/end, time duration and pre pause, curve types, loop modes, repeats...etc. 
* Handle Settings. Auto store-recall.
* Musical BPM based time or absolute scale.
* Plotting realtime value curves.
* Curve Tweens drawing.

## Dependencies
ofxAnimatable (https://github.com/armadillu/ofxAnimatable)  
ofxHistoryPlot (https://github.com/moebiussurfing/ofxHistoryPlot)  
ofxSurfingHelpers (https://github.com/moebiussurfing/ofxSurfingHelpers)  
ofxGui  

* Bundled into ```OF_ADDON/libs```. To Filter Noise point:  
ofxBiquadFilter  
ofxOneEuroFilter  

## Usage
- Look the example.

### ofApp.h
```.c++
#include "FloatAnimator.h"
FloatAnimator animatorFloat;
```

### ofApp.cpp
```.c++
animatorFloat.setup();
animatorFloat.update();
animatorFloat.draw();//gui

float value = animatorFloat.getValue();

animatorFloat.start();
animatorFloat.stop();
```

## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**
- **macOS High Sierra** / **Xcode 9/10** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
*MIT License.*