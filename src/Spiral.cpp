#include "Spiral.hpp"

//--------------------------------------------------------------
void Spiral::setup(string name){
//    ofDisableArbTex();
    
    spiralGroup.setName(name);
    
    start.addListener(this, &Spiral::setStart);
    end.addListener(this, &Spiral::setEnd);
    turning.addListener(this, &Spiral::setTurning);
    orientation.addListener(this, &Spiral::setOrientation);
    radii.addListener(this, &Spiral::setRadii);
    flipped.addListener(this, &Spiral::setFlipped);
    //    control point options
    //surface options
    surfaceGroup.setName("surface");
    //draw options
    surfaceGroup.add(shouldDraw.set("draw",true));
    surfaceGroup.add(drawSurface.set("surface",false));
    surfaceGroup.add(drawOrigin.set("origin",true));
    surfaceGroup.add(drawNormals.set("normals",false));
    surfaceGroup.add(drawSpiralPoints.set("points",true));
    
    shapingGroup.setName("Shaping Settings");
    spiralShapeGroup.setName("spiral");

    //spiral
    spiralShapeGroup.add(start.set("start", ofVec3f(-3,3), ofVec3f(-500), ofVec3f(500)));
    spiralShapeGroup.add(end.set("end", ofVec3f(-3,3), ofVec3f(-500), ofVec3f(500)));
    
    spiralShapeGroup.add(radii.set("radii", ofVec2f(0.1, 1.0), ofVec2f(1.0, 100.0), ofVec2f(100.0, 200.0)));
    spiralShapeGroup.add(turning.set("turning", 1.0, 0.0, 10.0));
    spiralShapeGroup.add(orientation.set("orientation", 0.0, 0.0, 2 * M_PI));
    spiralShapeGroup.add(flipped.set("flip", false));

    //position
    shapingGroup.add(pos.set("pos", ofVec3f(0.0), ofVec3f(-200.0), ofVec3f(200.0)));
    shapingGroup.add(rot.set("rot", ofVec3f(0.0), ofVec3f(-180.0), ofVec3f(180.0)));
    shapingGroup.add(spiralShapeGroup);
    

    spiralGroup.add(surfaceGroup);
    spiralGroup.add(shapingGroup);
    
#ifdef TARGET_OPENGLES
    cout << "using es2 shaders" << endl;
    shader.load("shaders/shadersES2/spiral.vert", "shaders/shadersES2/spiral.frag");
#else
    if(ofIsGLProgrammableRenderer()){
        cout << "using gl3 shaders" << endl;
        shader.load("shaders/shadersGL3/spiral.vert", "shaders/shadersGL3/spiral.frag");
    }else{
        cout << "using gl2 shaders" << endl;
        shader.load("shaders/shadersGL2/spiral.vert", "shaders/shadersGL2/spiral.frag");
    }
#endif
    surface.enableFlatColors();
}

void Spiral::update(){
    if(shouldDraw){
        surface.update();
        surface.reload();
    }
}

void Spiral::setStart(ofVec3f & newStart){
    surface.setStart(newStart);
}
void Spiral::setEnd(ofVec3f & newEnd){
    surface.setEnd(newEnd);
}
void Spiral::setTurning(float & newTurning){
    surface.setBaseTurning(newTurning);
}
void Spiral::setFlipped(bool & newFlipped){
    surface.setFlipped(newFlipped);
}
void Spiral::setRadii(ofVec2f & newRadii){
    surface.setBaseRadii(newRadii);
}
void Spiral::setOrientation(float & newOrientation){
    surface.setBaseOrientation(newOrientation);
}

//--------------------------------------------------------------
void Spiral::draw(){
    if (shouldDraw) {
        ofPushMatrix();
        ofTranslate(pos);
        ofRotateX(rot -> x);
        ofRotateY(rot -> y);
        ofRotateZ(rot -> z);
//        if (drawOrigin){
//            ofPushMatrix();
//            ofFill();
//            ofSetColor(0,255,255);
//            ofDrawSphere(0,0,0, 0.1);
//            ofPopMatrix();
//        }
//        if (drawNormals) {
//            ofSetColor(ofColor::yellow);
//            surface.drawNormals(10);
//        }
//        if (drawSurface) {
        
//            shader.begin();
            surface.draw(false, true);
//            shader.end();
//        }
//        if (drawWireFrame){
//            surface.drawFrontFaces(true, false);
//            surface.drawBackFaces(false, false);
//        }
//        if (drawSpiralPoints){
//            surface.drawSpiralPoints();
//        }
        ofPopMatrix();
    }
}

