#pragma once
#include "ofxXRSComponent.h"

class ofxXRSButton : public ofxXRSComponent {

    public:
    
        ofxXRSButton(string label) : ofxXRSComponent(label)
        {
            mType = ofxXRSType::BUTTON;
            setTheme(ofxXRSComponent::getTheme());
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mStyle.stripe.color = theme->stripe.button;
            setWidth(theme->layout.width, theme->layout.labelWidth);
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
            if (mVisible) {
            // anything that extends ofxXRSButton has the same rollover effect //
                ofPushStyle();
                    if (mStyle.border.visible) drawBorder();
                    ofFill();
                    if (mFocused && mMouseDown){
                        ofSetColor(mStyle.color.onMouseDown, mStyle.opacity);
                    }   else if (mMouseOver){
                        ofSetColor(mStyle.color.onMouseOver, mStyle.opacity);
                    }   else{
                        ofSetColor(mStyle.color.background, mStyle.opacity);
                    }
                    ofDrawRectangle(x, y, mStyle.width, mStyle.height);
                    drawLabel();
                    if (mStyle.stripe.visible) drawStripe();
                ofPopStyle();
            }
        }

        void dispatchEvent()
        {
            if (buttonEventCallback != nullptr) {
                ofxXRSButtonEvent e(this);
                buttonEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        static ofxXRSButton* getInstance() { return new ofxXRSButton("X"); }
    
    protected:
    
        void onMouseRelease(ofPoint m)
        {
            ofxXRSComponent::onFocusLost();
            ofxXRSComponent::onMouseRelease(m);
            dispatchEvent();
        }
    
};

class ofxXRSToggle : public ofxXRSButton {
    
    public:
    
        ofxXRSToggle(string label, bool checked = false) : ofxXRSButton(label)
        {
            mChecked = checked;
            mType = ofxXRSType::TOGGLE;
            setTheme(ofxXRSComponent::getTheme());
        }

        ofxXRSToggle(ofParameter<bool>& b) : ofxXRSToggle(b.getName(), b.get()) {
            mParamB = &b;
            mParamB->addListener(this, &ofxXRSToggle::onParamB);
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            radioOn = theme->icon.radioOn;
            radioOff = theme->icon.radioOff;
            mStyle.stripe.color = theme->stripe.toggle;
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }
    
        void setWidth(int width, float labelWidth = 1)
        {
            ofxXRSComponent::setWidth(width, labelWidth);
            mLabel.width = mStyle.width;
            mLabel.rightAlignedXpos = mIcon.x - mLabel.margin;
            ofxXRSComponent::positionLabel();
        }
    
        void toggle()
        {
            mChecked = !mChecked;
        }
    
        void setChecked(bool check)
        {
            mChecked = check;
        }
    
        bool getChecked()
        {
            return mChecked;
        }

        void draw()
        {
            if (mVisible) {
                ofPushStyle();
                ofxXRSButton::draw();
                ofSetColor(mIcon.color);
                if (mChecked == true){
                    radioOn->draw(x+mIcon.x, y+mIcon.y, mIcon.size, mIcon.size);
                }   else{
                    radioOff->draw(x+mIcon.x, y+mIcon.y, mIcon.size, mIcon.size);
                }
                ofPopStyle();
            }
        }

        void dispatchToggleEvent() {
            if(mParamB != nullptr) {
                mParamB->set(mChecked);
            }
            dispatchEvent();
        }
    
        void dispatchEvent()
        {
            if (toggleEventCallback == nullptr) {
                ofxXRSButton::dispatchEvent();
            }   else {
                toggleEventCallback(ofxXRSToggleEvent(this, mChecked));
            }
        }
    
        static ofxXRSToggle* getInstance() { return new ofxXRSToggle("X"); }
    
    protected:
    
        void onMouseRelease(ofPoint m)
        {
            mChecked = !mChecked;
            ofxXRSComponent::onFocusLost();
            ofxXRSComponent::onMouseRelease(m);
            dispatchToggleEvent();
        }
    
    private:
        bool mChecked;
        shared_ptr<ofImage> radioOn;
        shared_ptr<ofImage> radioOff;
        ofParameter<bool>* mParamB = nullptr;

        void onParamB(bool& b) {
            setChecked(b);
        }

};



