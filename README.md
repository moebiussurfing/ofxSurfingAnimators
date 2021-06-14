# ofxSurfingAnimators

**openFrameworks** helper add-on for [ofxAnimatable]((https://github.com/armadillu/ofxAnimatable) from **@armadillu**  
 
Some classes to faster integration into your OF Apps/Add-ons:  
*ColorAnimator, EnvelopeAnimator, FloatAnimator, NoiseAnimator, PositionAnimator* and *ToggleAnimator*.

## Screenshots

#### 1_example_Float (2 x FloatAnimator)
![gif](/readme_images/1_example_Float.gif?raw=true "gif")  

#### 2_example_FloatNoised (FloatAnimator + NoiseAnimator)
![gif](/readme_images/2_example_FloatNoised.gif?raw=true "gif")  

#### 3_example_PointColored (EnvelopeAnimator)
![image](/readme_images/3_example_PointColored.PNG?raw=true "image")

#### 4_example_Envelope (Point + Color animators)
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
* Added **ImGui** based **GUI**:  
Setters for Start/End, time mode, duration and pre pause, curve tween types, loop modes, repeats ...etc. 
* Handle Settings: Auto store-recall.
* Musical **BPM** based or absolute **time scale**.
* **Plotting** realtime value curves.
* **Curves preview** drawing.

## Dependencies
* [ofxAnimatable](https://github.com/armadillu/ofxAnimatable)  
* [ofxHistoryPlot](https://github.com/moebiussurfing/ofxHistoryPlot)  
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)  
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxImGui [ **FORK** ]](https://github.com/Daandelange/ofxImGui/tree/ofParameters-Helpers-Test)
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
setup() {
   animatorFloat.setup(0.0f, 1.0f);
}

udate() {
   float value = animatorFloat.getValue();
}

void keyPressed(int key){
   if (key == ' '){
      animatorFloat.start();
      animatorFloat.stop();
   }
}
```

## Tested Systems
- **Windows10** / **VS2017** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
MIT License
