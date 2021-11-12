//
//  ofxXRSClickFader.cpp
//  
//
//  Created by Sheep on 12/06/21.
//  Copyright (c) 2012 Sheep. All rights reserved.
//

#include "ofxXRSClickFader.h"

ofxXRSClickFader::ofxXRSClickFader(float* val,ofPoint pos){
	phase = FADER_VIEW;
	frame = 0;
	valueP = val;
	value = *val * 256;
	defaultV = *val;
	fValue = *val;
	
	window_pos = pos+ofPoint(1,0);
	window_size = ofPoint(256,60);
	window_y = 0;
	fader_name = "*Float Fader*";
	okc = "Cancel   OK";
	hilight.setHsb(235, 255, 255,180);
	
	ofRegisterKeyEvents(this);
	ofRegisterMouseEvents(this);
	viewValue = *val;
}

ofxXRSClickFader::~ofxXRSClickFader(){
	ofUnregisterKeyEvents(this);
	ofUnregisterMouseEvents(this);
}

void ofxXRSClickFader::draw(){
	if (fValue < 0.02) {
		fValue = 0.0;
		viewValue = 0.0;
	}
	if (fValue > 0.98) {
		fValue = 1.0;
		viewValue = 1.0;
	}
	
	if (phase == FADER_VIEW){
		frame += 1.0f * 90.0f / ofGetFrameRate();;
		window_y += (window_size.y - window_y) / 5.0f * 120.0f / ofGetFrameRate();;
		
		ofSetColor(255, 255, 255,140);
		ofNoFill();
		ofDrawRectangle(window_pos,window_size.x,window_y);
		ofFill();
		ofSetColor(0, 0, 0,100);
		ofDrawRectangle(window_pos,window_size.x,window_y);

		if (frame > 7){
			viewValue += (fValue - viewValue) / 2.5f  * 60.0f / ofGetFrameRate();;

			ofSetColor(255,255,255,100);
			std::string mes;
			
			ofDrawLine(window_pos.x		, window_pos.y+20, 
				   window_pos.x+window_size.x*(1-(powf((MIN(1.0,MAX(0.0,frame-7)/15.0))-1.0,4.0))),
				   window_pos.y+20);
			ofDrawLine(window_pos.x+256		, window_pos.y+40, 
				   window_pos.x+256-window_size.x*(1-(powf((MIN(1.0,MAX(0.0,frame-9)/15.0))-1.0,4.0))),
				   window_pos.y+40);
			ofSetColor(255,255,255);
			mes += fader_name.substr(0,MIN(fader_name.length(),(frame)/2));
			
			for (int j = 0;j < MAX(0,(float)fader_name.length()-MAX(0,(frame)/2));j++){
				mes += ofToString(static_cast<char>(ofRandom(33,120)));
			}

			std::string mes2;
			
			mes2 += okc.substr(0,MIN(okc.length(),(frame)/2));
			
			for (int j = 0;j < MAX(0,(float)okc.length()-MAX(0,(frame)/2));j++){
				mes2 += ofToString(static_cast<char>(ofRandom(33,120)));
			}
			
			ofDrawBitmapString(ofToString(viewValue), window_pos+ofPoint(4,53));
			ofDrawBitmapString(mes, window_pos.x+4,window_pos.y+13);
			ofDrawBitmapString(mes2, window_pos.x+165,window_pos.y+53);
		}
		if (frame > 10){
			ofSetColor(hilight);
			ofEnableBlendMode(OF_BLENDMODE_ADD);
			ofDrawRectangle(window_pos.x, window_pos.y+20, 256*viewValue, 20);
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			*valueP = viewValue;
		}
	}
	if (phase == FADER_CLOSE){
		frame += 1 * 90.0f / ofGetFrameRate();;
		
		ofSetHexColor(0xFFFFFF);
		if (frame > 7) ofSetColor(255*(ofGetFrameNum()%2));

		ofDrawBitmapString(fader_name, window_pos.x+4,window_pos.y+13);
		ofDrawBitmapString(ofToString(viewValue), window_pos+ofPoint(4,53));
		ofDrawBitmapString(okc, window_pos.x+165,window_pos.y+53);

		ofEnableBlendMode(OF_BLENDMODE_ADD);
		if (frame > 7) ofSetColor(hilight.r,hilight.g,hilight.b,255*ofGetFrameNum()%2);
		else		   ofSetColor(hilight);
		
		ofDrawRectangle(window_pos.x,window_pos.y+30, window_size.x, 10.0/(frame));
		if (frame > 15){
			//parent->setHasFaderChild(false);
			parent->haveFChild = false;
			//delete this;
			return;
		}

	}
}

void ofxXRSClickFader::mousePressed(ofMouseEventArgs &mouse){
	if ((window_pos.x    < mouse.x)&&(mouse.x < window_pos.x+256)&&
		(window_pos.y+20 < mouse.y)&&(mouse.y < window_pos.y+40)){
		fValue = (mouse.x - window_pos.x) / 256.;
		value = (mouse.x - window_pos.x);
	}
	if ((window_pos.x+156< mouse.x)&&(mouse.x < window_pos.x+256)&&
		(window_pos.y+40 < mouse.y)&&(mouse.y < window_pos.y+60)){
		if ((mouse.x - window_pos.x) < 221){
			okc = "Cancel   ";
			*valueP = defaultV;
			viewValue = defaultV;
		}else if (parent != nullptr){
			okc = "         OK";
			parent->update();
		}
		frame = 0;
		phase = FADER_CLOSE;
	}
}

void ofxXRSClickFader::mouseDragged(ofMouseEventArgs &mouse){
	if ((window_pos.x    < mouse.x)&&(mouse.x < window_pos.x+256)&&
		(window_pos.y+20 < mouse.y)&&(mouse.y < window_pos.y+40)){
		fValue = (mouse.x - window_pos.x) / 256.;
		value = (mouse.x - window_pos.x);
	}
}

void ofxXRSClickFader::mouseMoved(ofMouseEventArgs &mouse){
	
}

void ofxXRSClickFader::mouseReleased(ofMouseEventArgs &mouse){
	
}

void ofxXRSClickFader::keyPressed(ofKeyEventArgs &key){
	
}

void ofxXRSClickFader::keyReleased(ofKeyEventArgs &key){
	
}

void ofxXRSClickFader::mouseScrolled(ofMouseEventArgs& mouse) {
	
}

void ofxXRSClickFader::mouseEntered(ofMouseEventArgs& mouse) {

}

void ofxXRSClickFader::mouseExited(ofMouseEventArgs& mouse) {

}
