//
//  equations.cpp
//  ArchimedeanSpiral
//
//  Created by kevin ambrosia on 3/20/17.
//
//

#include "ArchimedeanSpiralEquation.hpp"

void ArchimedeanSpiral::update(){
    float dt = 1/ofGetFrameRate();
    orientation.update(dt);
    turning.update(dt);
    radiusMin.update(dt);
    radiusMax.update(dt);
}

ofPoint ArchimedeanSpiral::valueForPoint(float u,float v){
    ofVec4f combined = ofVec4f(0, 0, 0, 0);

    //need epsilon to prevent division by 0
    float s = max(knots[0][0] + u * (knots[0][knots[0].size() - 1] - knots[0][0]), EPSILON);
    float t = max(knots[1][0] + v * (knots[1][knots[1].size() - 1] - knots[1][0]), EPSILON);

    //get the starting index for knots and control points
    int sSpan = span(s,degrees[0], &knots[0]);
    int tSpan = span(t,degrees[1], &knots[1]);

    //get basis values for each degree

    basis(sSpan, s, degrees[0], &basisS, &knots[0]);
    basis(tSpan, t, degrees[1], &basisT, &knots[1]);

    //iterate through degree of points
    for (int i = 0; i < degrees[1]; i++) {
        ofVec4f temp = ofVec4f(0,0,0,0);
        ofVec3f tempOffset = ofVec3f(0,0,0);
        for (int j = 0; j < degrees[0]; j++) {

            //convert indices to current starting index
            int row = sSpan - degrees[1] + j;
            int col = tSpan - degrees[0] + i;
            ofVec4f point = spiralPoints[row][col];
            //merge with the basis values
            temp += point * basisS[j];
        }
        combined += temp * basisT[i];
    }
    //
    float totalWeight = combined[3];

    //average to a weight of 1
    combined = combined * (1.0/totalWeight);

    clearBasis(degrees[0], &basisS);
    clearBasis(degrees[1], &basisT);
    
    return ofPoint(combined.x,combined.y,combined.z);
}


ofVec2f ArchimedeanSpiral::texCoordForPoint(float u,float v,ofPoint value){
    return ofVec2f(u,v);
}

void ArchimedeanSpiral::setBaseOrientation(float newBaseOrientation) {
    baseOrientation = newBaseOrientation;
    generateSpiralPoints();
}

void ArchimedeanSpiral::setBaseRadii(ofVec2f newRadii) {
    baseRadiusMin = newRadii[0];
    baseRadiusMax = newRadii[1];
    generateSpiralPoints();
}

void ArchimedeanSpiral::setBaseTurning(float newBaseTurning) {
    baseTurning = newBaseTurning;
    generateSpiralPoints();
}

void ArchimedeanSpiral::setStart(ofVec3f & newStart) {
    start = newStart;
    generateSpiralPoints();
}

void ArchimedeanSpiral::setEnd(ofVec3f & newEnd) {
    end = newEnd;
    generateSpiralPoints();
}

void ArchimedeanSpiral::setFlipped(bool newFlipped){
    flipped = newFlipped;
    generateSpiralPoints();
}


void ArchimedeanSpiral::generateSpiralPoints() {
    spiralPoints.clear();
    
    // determines the overall structure of the plane
    // as well as the amount each point is influenced by the passive movement
    float xPercent = 0.0;
    float yPercent = 0.0;
    float theta = 0.0;
    
    //spiral
    float spiralU = 0.0;
    float spiralV = 0.0;
    float flipYPercent = 0.0;
    float normYPercent = 0.0;
    float minRad = 0.0;
    float maxRad = 0.0;
    ofVec3f spiral = ofVec3f(0);
    
    //final
    ofVec3f returned = ofVec3f(0);
    
    for (int i = 0; i< dimensions[0]; i++){
        xPercent = (float)i * (1.0 / (float)(dimensions[0] - 1));
        spiralU = xPercent;
        vector<ofVec4f> points;
        
        for (int j = 0; j< dimensions[1]; j++){
            spiral = ofVec3f(0.0);
            returned = ofVec3f(0.0);
            
            yPercent = (float)j * (1.0 / (float)(dimensions[1] - 1));
            theta = ((2 * M_PI) * baseTurning) * yPercent;
            flipYPercent = flipped ? yPercent : (1-yPercent);
            normYPercent = flipped ? (1-yPercent) : yPercent;
            
            minRad = (baseRadiusMin * flipYPercent);
            maxRad = (baseRadiusMax * normYPercent);
            
            spiral = ofVec3f((minRad + maxRad) * cos(theta + baseOrientation),
                             (minRad + maxRad) * sin(theta + baseOrientation),
                             (spiralU * (start.y - end.y)) + end.y);

            returned = spiral;
            points.push_back(ofVec4f(returned.x, returned.y, returned.z, 1.0));
        }
        
        spiralPoints.push_back(points);
    }
}

void ArchimedeanSpiral::drawSpiralPoints(){
    for (int i = 0; i< dimensions[0]; i++){
        for (int j = 0; j< dimensions[1]; j++){
            ofPushMatrix();
            ofFill();
            ofTranslate(spiralPoints[i][j]);
            ofSetColor(0,255,255);
            ofDrawSphere(0,0,0, 1);
            ofPopMatrix();
        }
    }
}

int ArchimedeanSpiral::span(float u, int degree, vector<float> * currentKnotVector){

    int n = (*currentKnotVector).size() - degree;

    if (u >= (*currentKnotVector)[n]) return n - 1;
    if (u <= (*currentKnotVector)[degree]) return degree;

    int low = degree;
    int high = n;
    int mid = floor((low + high) / 2);

    while (u < (*currentKnotVector)[mid] || u >= (*currentKnotVector)[mid + 1]) {
        if (u < (*currentKnotVector)[mid]) {
            high = mid;
        } else {
            low = mid;
        }

        mid = floor((low + high) / 2);

    }
    return mid;
}

void ArchimedeanSpiral::basis(int span, float u, int degree, vector<float> * basisValues, vector<float> * currentKnotVector){
    _leftBasis.resize(degree, 0.0);
    _rightBasis.resize(degree, 0.0);

    for (int i = 1; i <= degree; i++) {
        _leftBasis[i] = u - (*currentKnotVector)[span + 1 - i];
        _rightBasis[i] = (*currentKnotVector)[span + i] - u;

        float saved = 0.0;

        for (int j = 0; j < i; j++) {
            float rv = _rightBasis[j + 1];
            float lv = _leftBasis[i - j];
            float midv = (*basisValues)[j] / (rv + lv);
            (*basisValues)[j] = saved + rv * midv;
            saved = lv * midv;
        }

        (*basisValues)[i] = saved;
    }

    _leftBasis.clear();
    _rightBasis.clear();

}

void ArchimedeanSpiral::clearBasis(int degree, vector<float>* basisValues){
    (*basisValues).clear();
    (*basisValues).resize(degree, 1.0);
}
void ArchimedeanSpiral::generateUniformKnotVector() {
    int totalDimensions = dimensions.size();
    knots.clear();
    
    for(int i = 0; i< totalDimensions; i++) {
        int size = degrees[i] + dimensions[i] + 1;
        float degreeIndex = degrees[i] - 1;
        float highest = dimensions[i] - degreeIndex + 1;
        float lowest = 0;
        float low = 0;
        float high = 0;
        vector<float> newKnotVector;
        
        for(int j = 0; j< size; j++){
            low = max(j-degreeIndex, lowest);
            high = min(low, highest)/highest;
            newKnotVector.push_back(high * dimensions[i]);
        }
        knots.push_back(newKnotVector);
    }
}
