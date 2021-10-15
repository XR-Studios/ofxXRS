#pragma once
#include "ofxXRSTextInput.h"

class ofxXRSFRM : public ofxXRSTextInput {

    public:
    
        ofxXRSFRM(float refresh = 1.0f) : ofxXRSTextInput("framerate", "XX")
        {
            mRefresh = refresh;
            mTime = ofGetElapsedTimef();
            mInput.setText(ofToString(ofGetFrameRate(), 2));
        }
    
        void update(bool ignoreMouseEvents = true)
        {
            if (ofGetElapsedTimef() - mTime > mRefresh){
                mTime = ofGetElapsedTimef();
                mInput.setText(ofToString(ofGetFrameRate(), 2));
            }
        }
        
    private:
    
        float mTime;
        float mRefresh;

};
