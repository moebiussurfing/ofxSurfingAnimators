# ofxSurfingAnimators

**openFrameworks** helper addon for ```ofxAnimatable``` from **@armadillu**  
(https://github.com/armadillu/ofxAnimatable)  
Some classes to faster integration into your OF Apps/Addons:  
*ColorAnimator, EnvelopeAnimator, FloatAnimator, NoiseAnimator, PositionAnimator* and *ToggleAnimator*.

## ScreenshotS
/2_example_FloatNoised (FloatAnimator + NoiseAnimator)
![image](/readme_images/2_example_FloatNoised.PNG?raw=true "image")  
/3_example_PointColored (EnvelopeAnimator)
![image](/readme_images/3_example_PointColored.PNG?raw=true "image")
/4_example_Envelope (Point + Color animators)
![image](/readme_images/4_example_Envelope.PNG?raw=true "image")
 	
## Overview
**openFrameworks** helper addon for ```ofxAnimatable```:
* Classes:
   * FloatAnimator.h
   * EnvelopeAnimator.h (ASR envelope for a Float. Double independent curve types for Input/Output.)
   * PositionAnimator.h
   * NoiseAnimator.h (Envelope Modulated 2D Noise point + Filters)
   * ColorAnimator.h
   * ToggleAnimator.h
* Added GUI:  
Setters for start/end, time mode, duration and pre pause, curve tween types, loop modes, repeats ...etc. 
* Handle Settings: Auto store-recall.
* Musical BPM based or absolute time scale.
* Plotting realtime value curves.
* Curve Tweens preview drawing.

## Dependencies
* [ofxAnimatable](https://github.com/armadillu/ofxAnimatable)  
* [ofxHistoryPlot](https://github.com/moebiussurfing/ofxHistoryPlot)  
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxImGui [**FORK**]](https://github.com/Daandelange/ofxImGui/tree/ofParameters-Helpers-Test)
* ofxGui  [ **OF** ]

Bundled into ```OF_ADDON/libs```. No need to add to the ```Project Generator```!  
(To Filter Noise point)  
**ofxBiquadFilter**  (https://github.com/dzlonline/ofxBiquadFilter)  
**ofxOneEuroFilter**  (https://github.com/i-n-g-o/ofxOneEuroFilter)

## Usage
- Look the examples.

### ofApp.h
```.c++
#include "FloatAnimator.h"
FloatAnimator animatorFloat;
```

### ofApp.cpp
```.c++
animatorFloat.setup(0, 1);

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
