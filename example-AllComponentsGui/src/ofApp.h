#pragma once

#include "ofMain.h"
#include "ofxXRS.h"

class ofApp : public ofBaseApp{

	public:
        void setup();
        void draw();
        void update();
	
        ofxXRSPanel* gui;
        bool mFullscreen;
        void refreshWindow();
        void toggleFullscreen();

        void keyPressed(int key);
        void onButtonEvent(ofxXRSButtonEvent e);
        void onToggleEvent(ofxXRSToggleEvent e);
        void onSliderEvent(ofxXRSSliderEvent e);
        void onTextInputEvent(ofxXRSTextInputEvent e);
        void on2dPadEvent(ofxXRS2dPadEvent e);
        void onDropdownEvent(ofxXRSDropdownEvent e);
        void onColorPickerEvent(ofxXRSColorPickerEvent e);
        void onMatrixEvent(ofxXRSMatrixEvent e);
	
        uint tIndex;
        vector<ofxXRSTheme*> themes;

};
