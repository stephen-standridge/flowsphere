#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFlowTools.h"
#include "Spiral.hpp"

//#define USE_PROGRAMMABLE_GL

using namespace flowTools;

enum drawModeEnum{
    DRAW_COMPOSITE = 0,
    DRAW_FLUID_FIELDS,
    DRAW_FLUID_VELOCITY,
    DRAW_FLUID_PRESSURE,
    DRAW_OPTICAL_FLOW,
    DRAW_MOUSE,
    DRAW_SOURCE
};

class ofApp : public ofBaseApp{
public:
    void	setup();
    void	update();
    void	draw();
    void    drawFBO();
    
    Spiral spiral;
    // Camera
    ofEasyCam cam;
    ofVideoGrabber		simpleCam;
    ftFbo				opticalFBO;
    
    // Time
    float				lastTime;
    float				deltaTime;
    
    // FlowTools
    int					flowWidth;
    int					flowHeight;
    int					drawWidth;
    int					drawHeight;
    
    ftOpticalFlow		opticalFlow;
    ftFluidSimulation	fluidSimulation;
    
    // MouseDraw
    ftDrawMouseForces	mouseForces;
    
    // Visualisations
    ofParameterGroup	visualizeParameters;
    ftDisplayScalar		displayScalar;
    ftVelocityField		velocityField;
    ftTemperatureField	temperatureField;
    ftPressureField		pressureField;
    ftVTField			velocityTemperatureField;
    
    ofParameter<bool>	showScalar;
    ofParameter<bool>	showField;
    ofParameter<float>	displayScalarScale;
    void				setDisplayScalarScale(float& _value) { displayScalar.setScale(_value); }
    ofParameter<float>	velocityFieldScale;
    void				setVelocityFieldScale(float& _value) { velocityField.setVelocityScale(_value); velocityTemperatureField.setVelocityScale(_value); }
    ofParameter<float>	temperatureFieldScale;
    void				setTemperatureFieldScale(float& _value) { temperatureField.setTemperatureScale(_value); velocityTemperatureField.setTemperatureScale(_value); }
    ofParameter<float>	pressureFieldScale;
    void				setPressureFieldScale(float& _value) { pressureField.setPressureScale(_value); }
    ofParameter<bool>	velocityLineSmooth;
    void				setVelocityLineSmooth(bool& _value) { velocityField.setLineSmooth(_value); velocityTemperatureField.setLineSmooth(_value);  }
    
    // GUI
    ofxPanel			gui;
    void				setupGui();
    void				keyPressed(int key);
    void				drawGui();
    ofParameter<bool>	toggleGuiDraw;
    ofParameter<float>	guiFPS;
    ofParameter<float>	guiMinFPS;
    deque<float>		deltaTimeDeque;
    ofParameter<bool>	doFullScreen;
    void				setFullScreen(bool& _value) { ofSetFullscreen(_value);}
    
    // DRAW
    ofParameter<bool>	doDrawCamBackground;
    
    ofParameter<int>	drawMode;
    void				drawModeSetName(int& _value) ;
    ofParameter<string> drawName;
    
    void				drawComposite()			{ drawComposite(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void				drawComposite(int _x, int _y, int _width, int _height);
    void				drawFluidFields()		{ drawFluidFields(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void				drawFluidFields(int _x, int _y, int _width, int _height);
    void				drawFluidVelocity()		{ drawFluidVelocity(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void				drawFluidVelocity(int _x, int _y, int _width, int _height);
    void				drawFluidPressure()		{ drawFluidPressure(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void				drawFluidPressure(int _x, int _y, int _width, int _height);
    void				drawOpticalFlow()		{ drawOpticalFlow(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void				drawOpticalFlow(int _x, int _y, int _width, int _height);
    void				drawSource()			{ drawSource(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void				drawSource(int _x, int _y, int _width, int _height);
    void				drawMouseForces()		{ drawMouseForces(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void				drawMouseForces(int _x, int _y, int _width, int _height);
};
