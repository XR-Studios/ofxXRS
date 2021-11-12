#pragma once
#include "ofxXRSComponent.h"
#include "ofxXRSTextInputField.h"

class ofxXRSTextInput : public ofxXRSComponent {

    public:
    
        ofxXRSTextInput(std::string label, std::string text = "") : ofxXRSComponent(label)
        {
            mInput.setText(text);
            mInput.onInternalEvent(this, &ofxXRSTextInput::onInputChanged);
            mType = ofxXRSType::TEXT_INPUT;
            setTheme(ofxXRSComponent::getTheme());
        }

        ofxXRSTextInput(ofParameter<std::string>& str) : ofxXRSTextInput(str.getName(), str.get()) {
            mParamStr = &str;
            mParamStr->addListener(this, &ofxXRSTextInput::onParamStr);
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mStyle.stripe.color = theme->stripe.textInput;
            mInput.setTheme(theme);
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }
    
        void setWidth(int width, float labelWidth)
        {
            ofxXRSComponent::setWidth(width, labelWidth);
            mInput.setPosition(x + mLabel.width, y + mStyle.padding);
            mInput.setWidth(mStyle.width - mStyle.padding - mLabel.width);
        }
    
        void setPosition(int x, int y)
        {
            ofxXRSComponent::setPosition(x, y);
            mInput.setPosition(x + mLabel.width, y + mStyle.padding);
        }
    
        void setText(std::string text)
        {
            mInput.setText(text);
        }

        std::string getText()
        {
            return mInput.getText();
        }
    
        void setTextUpperCase(bool toUpper)
        {
            mInput.setTextUpperCase(toUpper);
        }
    
        bool getTextUpperCase()
        {
            return mInput.getTextUpperCase();
        }
    
        void setInputType(ofxXRSInputType type)
        {
            mInput.setTextInputFieldType(type);
        }
    
        void draw()
        {
            if (mVisible){
                ofxXRSComponent::draw();
                mInput.draw();
            }
        }
    
        bool hitTest(ofPoint m)
        {
            return mInput.hitTest(m);
        }

        void dispatchTextInputEvent() {
            if(mParamStr != nullptr) {
                mParamStr->set(mInput.getText());
            }
            dispatchEvent();
        }
    
        void dispatchEvent()
        {
            if (textInputEventCallback != nullptr) {
                ofxXRSTextInputEvent e(this, mInput.getText());
                textInputEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        static ofxXRSTextInput* getInstance(){ return new ofxXRSTextInput("X"); }
    
    protected:

        void onFocus()
        {
            mInput.onFocus();
            ofxXRSComponent::onFocus();
        }
    
        void onFocusLost()
        {
            mInput.onFocusLost();
            ofxXRSComponent::onFocusLost();
        }
    
        void onKeyPressed(int key)
        {
            if (key != OF_KEY_UP &&
                key != OF_KEY_DOWN)
            mInput.onKeyPressed(key);
        }
    
        virtual void onInputChanged(ofxXRSInternalEvent e)
        {
        //  dispatch event out to main application //
            dispatchTextInputEvent();
        }

        void onParamStr(std::string& str) {
            setText(str);
        }
    
        ofxXRSTextInputField mInput;
        ofParameter<std::string>* mParamStr;
    
};

