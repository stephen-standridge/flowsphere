#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    spiral.setup("spiral");
    ofSetVerticalSync(false);
    ofSetLogLevel(OF_LOG_NOTICE);
    
    drawWidth = 1280;
    drawHeight = 720;
    // process all but the density on 16th resolution
    flowWidth = drawWidth / 4;
    flowHeight = drawHeight / 4;
    
    // FLOW & MASK
    opticalFlow.setup(flowWidth, flowHeight);
    
    // FLUID & PARTICLES
    fluidSimulation.setup(flowWidth, flowHeight, drawWidth, drawHeight);
    
    // VISUALIZATION
    displayScalar.setup(flowWidth, flowHeight);
    velocityField.setup(flowWidth / 4, flowHeight / 4);
    temperatureField.setup(flowWidth / 4, flowHeight / 4);
    pressureField.setup(flowWidth / 4, flowHeight / 4);
    velocityTemperatureField.setup(flowWidth / 4, flowHeight / 4);
    
    // MOUSE DRAW
    mouseForces.setup(flowWidth, flowHeight, drawWidth, drawHeight);
    
    // CAMERA
    simpleCam.setup(640, 480, true);
    opticalFBO.allocate(640, 480);
    opticalFBO.black();
    
    // GUI
    setupGui();
    
    lastTime = ofGetElapsedTimef();
    
}

//--------------------------------------------------------------
void ofApp::setupGui() {
    
    gui.setup("settings");
    gui.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
    gui.setDefaultFillColor(ofColor(160, 160, 160, 160));
    gui.add(guiFPS.set("average FPS", 0, 0, 60));
    gui.add(guiMinFPS.set("minimum FPS", 0, 0, 60));
    gui.add(doFullScreen.set("fullscreen (F)", false));
    doFullScreen.addListener(this, &ofApp::setFullScreen);
    gui.add(toggleGuiDraw.set("show gui (G)", false));
    gui.add(doDrawCamBackground.set("draw camera (C)", true));
    gui.add(drawMode.set("draw mode", DRAW_COMPOSITE, DRAW_COMPOSITE, DRAW_MOUSE));
    drawMode.addListener(this, &ofApp::drawModeSetName);
    gui.add(drawName.set("MODE", "draw name"));
    
    
    int guiColorSwitch = 0;
    ofColor guiHeaderColor[2];
    guiHeaderColor[0].set(160, 160, 80, 200);
    guiHeaderColor[1].set(80, 160, 160, 200);
    ofColor guiFillColor[2];
    guiFillColor[0].set(160, 160, 80, 200);
    guiFillColor[1].set(80, 160, 160, 200);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(opticalFlow.parameters);

    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(fluidSimulation.parameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(mouseForces.leftButtonParameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(mouseForces.rightButtonParameters);
    
    visualizeParameters.setName("visualizers");
    visualizeParameters.add(showScalar.set("show scalar", true));
    visualizeParameters.add(showField.set("show field", true));
    visualizeParameters.add(displayScalarScale.set("scalar scale", 0.15, 0.05, 1.0));
    displayScalarScale.addListener(this, &ofApp::setDisplayScalarScale);
    visualizeParameters.add(velocityFieldScale.set("velocity scale", 0.1, 0.0, 0.5));
    velocityFieldScale.addListener(this, &ofApp::setVelocityFieldScale);
    visualizeParameters.add(temperatureFieldScale.set("temperature scale", 0.1, 0.0, 0.5));
    temperatureFieldScale.addListener(this, &ofApp::setTemperatureFieldScale);
    visualizeParameters.add(pressureFieldScale.set("pressure scale", 0.02, 0.0, 0.5));
    pressureFieldScale.addListener(this, &ofApp::setPressureFieldScale);
    visualizeParameters.add(velocityLineSmooth.set("line smooth", false));
    velocityLineSmooth.addListener(this, &ofApp::setVelocityLineSmooth);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(visualizeParameters);
    gui.add(spiral.spiralGroup);
    
    // if the settings file is not present the parameters will not be set during this setup
    if (!ofFile("settings.xml"))
        gui.saveToFile("settings.xml");
    
    gui.loadFromFile("settings.xml");
    
    gui.minimizeAll();
    toggleGuiDraw = true;
    
}

void ofApp::drawFBO() {

}

//--------------------------------------------------------------
void ofApp::update(){
    spiral.update();
    deltaTime = ofGetElapsedTimef() - lastTime;
    lastTime = ofGetElapsedTimef();
    
    simpleCam.update();
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    opticalFBO.begin();
    ofSetColor(0,0,0);
//    ofDrawRectangle(0, 0, opticalFBO.getWidth(), opticalFBO.getHeight());
        simpleCam.draw(0, 0, opticalFBO.getWidth(), opticalFBO.getHeight());
    opticalFBO.end();
    ofPopStyle();
    
    opticalFlow.setSource(opticalFBO.getTexture());
    
    // opticalFlow.update(deltaTime);
    // use internal deltatime instead
    opticalFlow.update();
    
    fluidSimulation.addVelocity(opticalFlow.getOpticalFlowDecay());
    
    mouseForces.update(deltaTime);
    
    for (int i=0; i<mouseForces.getNumForces(); i++) {
        if (mouseForces.didChange(i)) {
            switch (mouseForces.getType(i)) {
                case FT_DENSITY:
                    fluidSimulation.addDensity(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
                    break;
                case FT_VELOCITY:
                    fluidSimulation.addVelocity(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
                    break;
                case FT_TEMPERATURE:
                    fluidSimulation.addTemperature(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
                    break;
                case FT_PRESSURE:
                    fluidSimulation.addPressure(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
                    break;
                case FT_OBSTACLE:
                    fluidSimulation.addTempObstacle(mouseForces.getTextureReference(i));
                default:
                    break;
            }
        }
    }
    
    fluidSimulation.update();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'G':
        case 'g': toggleGuiDraw = !toggleGuiDraw; break;
        case 'f':
        case 'F': doFullScreen.set(!doFullScreen.get()); break;
        case 'c':
        case 'C': doDrawCamBackground.set(!doDrawCamBackground.get()); break;
            
        case '1': drawMode.set(DRAW_COMPOSITE); break;
        case '2': drawMode.set(DRAW_FLUID_FIELDS); break;
        case '3': drawMode.set(DRAW_FLUID_VELOCITY); break;
        case '4': drawMode.set(DRAW_FLUID_PRESSURE); break;
        case '5': drawMode.set(DRAW_OPTICAL_FLOW); break;
        case '6': drawMode.set(DRAW_MOUSE); break;
        case '7': drawMode.set(DRAW_SOURCE); break;

        case 'r':
        case 'R':
            fluidSimulation.reset();
            mouseForces.reset();
            break;
            
        default: break;
    }
}

//--------------------------------------------------------------
void ofApp::drawModeSetName(int &_value) {
    switch(_value) {
        case DRAW_COMPOSITE:		drawName.set("Composite      (1)"); break;
        case DRAW_FLUID_FIELDS:		drawName.set("Fluid Fields   (2)"); break;
        case DRAW_FLUID_VELOCITY:	drawName.set("Fluid Velocity (3)"); break;
        case DRAW_FLUID_PRESSURE:	drawName.set("Fluid Pressure (4)"); break;
        case DRAW_OPTICAL_FLOW:		drawName.set("Optical Flow   (5)"); break;
        case DRAW_MOUSE:			drawName.set("Left Mouse     (6)"); break;
        case DRAW_SOURCE:			drawName.set("Source         (7)"); break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0,0);
    if (doDrawCamBackground.get())
        drawSource();
    
    
    if (!toggleGuiDraw) {
        ofHideCursor();
        drawComposite();
    }
    else {
        ofShowCursor();
        switch(drawMode.get()) {
            case DRAW_COMPOSITE: drawComposite(); break;
            case DRAW_FLUID_FIELDS: drawFluidFields(); break;
            case DRAW_FLUID_VELOCITY: drawFluidVelocity(); break;
            case DRAW_FLUID_PRESSURE: drawFluidPressure(); break;
            case DRAW_OPTICAL_FLOW: drawOpticalFlow(); break;
            case DRAW_SOURCE: drawSource(); break;
            case DRAW_MOUSE: drawMouseForces(); break;
        }
        drawGui();
    }
    cam.begin();
    spiral.draw();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::drawComposite(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    fluidSimulation.draw(_x, _y, _width, _height);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawFluidFields(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    pressureField.setPressure(fluidSimulation.getPressure());
    pressureField.draw(_x, _y, _width, _height);
    velocityTemperatureField.setVelocity(fluidSimulation.getVelocity());
    velocityTemperatureField.setTemperature(fluidSimulation.getTemperature());
    velocityTemperatureField.draw(_x, _y, _width, _height);
    temperatureField.setTemperature(fluidSimulation.getTemperature());
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawFluidVelocity(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    if (showScalar.get()) {
        ofClear(0,0);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        //	ofEnableBlendMode(OF_BLENDMODE_DISABLED); // altenate mode
        displayScalar.setSource(fluidSimulation.getVelocity());
        displayScalar.draw(_x, _y, _width, _height);
    }
    if (showField.get()) {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        velocityField.setVelocity(fluidSimulation.getVelocity());
        velocityField.draw(_x, _y, _width, _height);
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawFluidPressure(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    ofClear(128);
    if (showScalar.get()) {
        ofEnableBlendMode(OF_BLENDMODE_DISABLED);
        displayScalar.setSource(fluidSimulation.getPressure());
        displayScalar.draw(_x, _y, _width, _height);
    }
    if (showField.get()) {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        pressureField.setPressure(fluidSimulation.getPressure());
        pressureField.draw(_x, _y, _width, _height);
    }
    ofPopStyle();
}


//--------------------------------------------------------------
void ofApp::drawOpticalFlow(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    if (showScalar.get()) {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        displayScalar.setSource(opticalFlow.getOpticalFlowDecay());
        displayScalar.draw(0, 0, _width, _height);
    }
    if (showField.get()) {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        velocityField.setVelocity(opticalFlow.getOpticalFlowDecay());
        velocityField.draw(0, 0, _width, _height);
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawSource(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    opticalFBO.draw(_x, _y, _width, _height);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawMouseForces(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    ofClear(0,0);
    
    for(int i=0; i<mouseForces.getNumForces(); i++) {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        if (mouseForces.didChange(i)) {
            ftDrawForceType dfType = mouseForces.getType(i);
            if (dfType == FT_DENSITY)
                mouseForces.getTextureReference(i).draw(_x, _y, _width, _height);
        }
    }
    
    for(int i=0; i<mouseForces.getNumForces(); i++) {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        if (mouseForces.didChange(i)) {
            switch (mouseForces.getType(i)) {
                case FT_VELOCITY:
                    velocityField.setVelocity(mouseForces.getTextureReference(i));
                    velocityField.draw(_x, _y, _width, _height);
                    break;
                case FT_TEMPERATURE:
                    temperatureField.setTemperature(mouseForces.getTextureReference(i));
                    temperatureField.draw(_x, _y, _width, _height);
                    break;
                case FT_PRESSURE:
                    pressureField.setPressure(mouseForces.getTextureReference(i));
                    pressureField.draw(_x, _y, _width, _height);
                    break;
                default:
                    break;
            }
        }
    }
    
    ofPopStyle();
}


//--------------------------------------------------------------
void ofApp::drawGui() {
    guiFPS = (int)(ofGetFrameRate() + 0.5);
    
    // calculate minimum fps
    deltaTimeDeque.push_back(deltaTime);
    
    while (deltaTimeDeque.size() > guiFPS.get())
        deltaTimeDeque.pop_front();
    
    float longestTime = 0;
    for (int i=0; i<deltaTimeDeque.size(); i++){
        if (deltaTimeDeque[i] > longestTime)
            longestTime = deltaTimeDeque[i];
    }
    
    guiMinFPS.set(1.0 / longestTime);
    
    
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gui.draw();
    
    // HACK TO COMPENSATE FOR DISSAPEARING MOUSE
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), ofGetWindowWidth() / 300.0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), ofGetWindowWidth() / 600.0);
    ofPopStyle();
}
