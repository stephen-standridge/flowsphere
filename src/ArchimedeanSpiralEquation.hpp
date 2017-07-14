#pragma once
#include "ofMain.h"
#include "ofxMathMesh.h"
#include "ofxAnimatableFloat.h"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

class ArchimedeanSpiral:public ofxParametricSurface{
public:
    ArchimedeanSpiral():ofxParametricSurface(){
        
        orientation.setCurve(LINEAR);
        orientation.setRepeatType(LOOP);
        orientation.setDuration(8.0);
        orientation.animateFromTo(0, 2 * M_PI);
        
        radiusMin.setCurve(LINEAR);
        radiusMin.setRepeatType(LOOP);
        radiusMin.setDuration(8.0);
        radiusMin.animateFromTo(0.0, 1.0);
        
        radiusMax.setCurve(LINEAR);
        radiusMax.setRepeatType(LOOP);
        radiusMax.setDuration(8.0);
        radiusMax.animateFromTo(1.0, 10.0);
        
        turning.setCurve(LINEAR);
        turning.setRepeatType(LOOP);
        turning.setDuration(8.0);
        turning.animateFromTo(0, 10.0);
        
        clearBasis(degrees[0], &basisS);
        clearBasis(degrees[1], &basisT);
        generateUniformKnotVector();
        generateSpiralPoints();
        
        setup(0, 1, 0, 1, .025, .025);
    };
    
    void update();
    
    //class override methods
    ofPoint valueForPoint(float u,float v);
    ofVec2f texCoordForPoint(float u,float v,ofPoint value);
    
    //setters
    void setDimension(int index, int newDegree);
    void setBaseOrientation(float newBaseOrientation);
    void setBaseTurning(float newTurning);
    void setRangeY(ofVec2f & newRangeY);
    void setFlipped(bool newFlipped);
    void setBaseRadii(ofVec2f newRadii);
    void setStart(ofVec3f & newStart);
    void setEnd(ofVec3f & newEnd);

    void generateSpiralPoints();
    void drawSpiralPoints();
    
    float drawRadius = 0.5;
    const float EPSILON = 0.0001;
    
    
private:
    vector<vector<ofVec4f>> spiralPoints;

    // nurbs
    vector<int> degrees = {3,3};
    vector<int> dimensions = {30,30};
    vector<vector<float>> knots = {};
    vector<float> basisS = {};
    vector<float> basisT = {};
    vector<float> _leftBasis = {};
    vector<float> _rightBasis = {};
    
    int span(float u, int degree, vector<float> * currentKnotVector);
    void basis(int span, float u, int degree, vector<float> * basisValues, vector<float> * currentKnotVector);
    void clearBasis(int degree, vector<float> *basisValues);
    void generateUniformKnotVector();

    // shape
    ofVec3f start = ofVec3f(0.0, 1.0, 0.0);
    ofVec3f end = ofVec3f(0.0, -1.0, 0.0);
    ofVec2f range = ofVec2f(-M_PI/2, M_PI/2);
    
    float baseOrientation = 0.0;
    ofxAnimatableFloat orientation;
    
    float baseRadiusMin = 1.0;
    float baseRadiusMax = 10.0;
    ofxAnimatableFloat radiusMin;
    ofxAnimatableFloat radiusMax;
    
    float baseTurning = 0.0;
    ofxAnimatableFloat turning;
    
    bool flipped = false;
};







