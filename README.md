# ofxSurfingAnimators

**openFrameworks** helper addon for ```ofxAnimatable``` from **@armadillu** (https://github.com/armadillu/ofxAnimatable),
with several classes to faster integration into OF Apps:  
*ColorAnimator, FadeAnimator, FloatAnimator, NoiseAnimator, PositionAnimator and ToggleAnimator*.

## Screenshot
![image](/readme_images/Capture.PNG?raw=true "image")

## Overview
**openFrameworks** helper addon for ```ofxAnimatable```:
* Classes:
   * FloatAnimator.h
   * FadeAnimator.h (ADSR envelope)
   * PositionAnimator.h
   * NoiseAnimator.h (Several noise settings + filgers)
   * ColorAnimator.h
   * ToggleAnimator.h
* Added GUI.
* Handle Settings.
* Bpm based time scale.
* Plotting curves.
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

## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**
- **macOS High Sierra** / **Xcode 9/10** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
*MIT License.*