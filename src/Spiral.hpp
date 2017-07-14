//
//  Spiral.hpp
//  Spirals
//
//  Created by kevin ambrosia on 4/3/17.
//
//

#ifndef Spiral_hpp
#define Spiral_hpp

#pragma once

#include "ofMain.h"
#include "ArchimedeanSpiralEquation.hpp"

class Spiral : public ofNode{
    
public:
    void setup(string name);
    void update();
    void draw();
    
    void setRangeY(ofVec2f & newRangeY);
    void setTurning(float & newTurning);
    void setRadii(ofVec2f & newRadii);
    void setFlipped(bool & newFlipped);
    void setOrientation(float & newOrientation);
    void setEnd(ofVec3f & newEnd);
    void setStart(ofVec3f & newStart);


    ofParameterGroup        spiralGroup;
    
    ofParameterGroup        surfaceGroup;
    ofParameter<bool>       shouldDraw;
    ofParameter<bool>       drawWireFrame;
    ofParameter<bool>       drawSurface;
    ofParameter<bool>       drawSpiralPoints;
    ofParameter<bool>       drawNormals;
    ofParameter<bool>       drawOrigin;
    
    ofParameterGroup        shapingGroup;
    ofParameter<ofVec3f>    start;
    ofParameter<ofVec3f>    end;
    ofParameter<ofVec3f>    pos;
    
    ofParameter<ofVec3f>    rot;
    ofParameterGroup        spiralShapeGroup;
    ofParameter<ofVec2f>    rangeY;
    ofParameter<ofVec2f>    radii;
    ofParameter<float>      turning;
    ofParameter<float>      orientation;
    ofParameter<bool>       flipped;

    ArchimedeanSpiral surface;
    ofShader shader;
    
    ofImage colorImage;
    ofImage noiseImage;
    ofImage reflectionUp;
    ofImage reflectionDown;
    ofImage reflectionLeft;
    ofImage reflectionRight;
    ofImage reflectionFront;
    ofImage reflectionBack;
};

#endif /* Spiral_hpp */

