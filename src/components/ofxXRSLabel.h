#pragma once
#include "ofxXRSComponent.h"

class ofxXRSBreak : public ofxXRSComponent{

    public:
    
        ofxXRSBreak() : ofxXRSComponent("break")
        {
            setTheme(ofxXRSComponent::getTheme());
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mStyle.height = theme->layout.breakHeight;
        }
    
        void setHeight(float height)
        {
            mStyle.height = height;
        }
    
        void draw()
        {
            if (!mVisible) return;
            ofPushStyle();
                ofFill();
                ofSetColor(mStyle.color.background, mStyle.opacity);
                ofDrawRectangle(x, y, mStyle.width, mStyle.height);
            ofPopStyle();
        }
    
        int getHeight()
        {
            return mStyle.height;
        }
    
};

class ofxXRSLabel : public ofxXRSComponent{

    public:
        
        ofxXRSLabel(std::string label) : ofxXRSComponent(label)
        {
            mType = ofxXRSType::LABEL;
            setTheme(ofxXRSComponent::getTheme());
        }

        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mStyle.stripe.color = theme->stripe.label;
        }
    
        void setWidth(int width, float labelWidth = 1)
        {
            ofxXRSComponent::setWidth(width, labelWidth);
            mLabel.width = mStyle.width;
            mLabel.rightAlignedXpos = mLabel.width - mLabel.margin;
            ofxXRSComponent::positionLabel();
        }
    
        void draw()
        {
            ofxXRSComponent::draw();
        }
    
        static ofxXRSLabel* getInstance() { return new ofxXRSLabel("X"); }
};