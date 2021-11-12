#pragma once
#include "ofxXRSTextInput.h"

class ofxXRSColorPicker : public ofxXRSTextInput {

    public:
    
        ofxXRSColorPicker(std::string label, ofColor color=ofColor::black) : ofxXRSTextInput(label, "XXXXXX")
        {
            mColor = color;
            mShowPicker = false;
            mType = ofxXRSType::COLOR_PICKER;
            setTheme(ofxXRSComponent::getTheme());
            
        // center the text input field //
            mInput.setTextInputFieldType(ofxXRSInputType::COLORPICKER);
            setTextFieldInputColor();
            
        // setup the vbo that draws the main gradient //
            gPoints.push_back(ofVec2f(0, 0));
            gPoints.push_back(ofVec2f(0, 0));
            gPoints.push_back(ofVec2f(0, 0));
            gPoints.push_back(ofVec2f(0, 0));
            gPoints.push_back(ofVec2f(0, 0));
            gPoints.push_back(ofVec2f(0, 0));
            
        // center point of the gradient is 1/2 way between mColor & black //
            ofColor center = ofColor(mColor.r/2, mColor.g/2, mColor.b/2);
            
        // draw main gradient as a six point triangle fan //
            gColors.push_back(center);          // center
            gColors.push_back(ofColor::white);  // top-left
            gColors.push_back(mColor);          // top-right
            gColors.push_back(ofColor::black);  // btm-right
            gColors.push_back(ofColor::black);  // btm-left
            gColors.push_back(ofColor::white);  // top-left
            vbo.setColorData(&gColors[0], 6, GL_DYNAMIC_DRAW );
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            ofxXRSTextInput::setTheme(theme);
            mStyle.stripe.color = theme->stripe.colorPicker;
            pickerRect = ofRectangle(0, 0, mInput.getWidth(), (mStyle.height + mStyle.padding) * 3);
            rainbow.image = theme->icon.rainbow;
            rainbow.rect = ofRectangle(0, 0, theme->layout.colorPicker.rainbowWidth, pickerRect.height - (mStyle.padding * 2));
            gradientRect = ofRectangle(0, 0, pickerRect.width - rainbow.rect.width - (mStyle.padding * 3), rainbow.rect.height);
            pickerBorder = theme->color.colorPicker.border;
            setTextFieldInputColor();
        }
    
        void setColor(ofColor color)
        {
            mColor = color;
            setTextFieldInputColor();
        }
    
        void setColor(int hex)
        {
            mColor = ofColor::fromHex(hex);
            setTextFieldInputColor();
        }
    
        void setColor(int r, int g, int b, int a = 255)
        {
            mColor = ofColor(r, g, b, a);
            setTextFieldInputColor();
        }
    
        ofColor getColor()
        {
            return mColor;
        }
    
        void draw()
        {
            if (!mVisible) return;
            ofPushStyle();
                ofxXRSTextInput::draw();
                if (mShowPicker) {
                    pickerRect.x = this->x + mLabel.width;
                    pickerRect.y = this->y + mStyle.padding + mInput.getHeight();
                    pickerRect.width = mInput.getWidth();
                    rainbow.rect.x = pickerRect.x + pickerRect.width - rainbow.rect.width - mStyle.padding;
                    rainbow.rect.y = pickerRect.y + mStyle.padding;
                    gradientRect.x = pickerRect.x + mStyle.padding;
                    gradientRect.y = pickerRect.y + mStyle.padding;
                    gradientRect.width = pickerRect.width - rainbow.rect.width - (mStyle.padding * 3);
                    gPoints[0] = ofVec2f(gradientRect.x+ gradientRect.width/2, gradientRect.y + gradientRect.height/2);
                    gPoints[1] = ofVec2f(gradientRect.x, gradientRect.y);
                    gPoints[2] = ofVec2f(gradientRect.x+ gradientRect.width, gradientRect.y);
                    gPoints[3] = ofVec2f(gradientRect.x+ gradientRect.width, gradientRect.y + gradientRect.height);
                    gPoints[4] = ofVec2f(gradientRect.x, gradientRect.y+gradientRect.height);
                    gPoints[5] = ofVec2f(gradientRect.x, gradientRect.y);
                    vbo.setVertexData(&gPoints[0], 6, GL_DYNAMIC_DRAW );
                    ofSetColor(pickerBorder);
                    ofDrawRectangle(pickerRect);
                    ofSetColor(ofColor::white);
                    rainbow.image->draw(rainbow.rect);
                    vbo.draw( GL_TRIANGLE_FAN, 0, 6 );
                }
            ofPopStyle();
        }
    
        void drawColorPicker()
        {
            if (mVisible && mShowPicker){
                ofPushStyle();
                    ofSetColor(pickerBorder);
                    ofDrawRectangle(pickerRect);
                    ofSetColor(ofColor::white);
                    rainbow.image->draw(rainbow.rect);
                    vbo.draw( GL_TRIANGLE_FAN, 0, 6 );
                ofPopStyle();
            }
        }
    
        bool hitTest(ofPoint m)
        {
            if (mInput.hitTest(m)){
                return true;
            }   else if (mShowPicker && pickerRect.inside(m)){
                unsigned char p[3];
                int y = (ofGetMouseY()-ofGetHeight())*-1;
                glReadPixels(ofGetMouseX(), y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &p);
                gColor.r = int(p[0]);
                gColor.g = int(p[1]);
                gColor.b = int(p[2]);
                if (rainbow.rect.inside(m) && mMouseDown){
                    gColors[2] = gColor;
                    gColors[0] = ofColor(gColor.r/2, gColor.g/2, gColor.b/2);
                    vbo.setColorData(&gColors[0], 6, GL_DYNAMIC_DRAW );
                }   else if (gradientRect.inside(m) && mMouseDown){
                    mColor = gColor;
                // dispatch event out to main application //
                    dispatchEvent();
                    setTextFieldInputColor();
                }
                return true;
            }   else{
                return false;
            }
        }
    
        void dispatchEvent()
        {
            if (colorPickerEventCallback != nullptr) {
                ofxXRSColorPickerEvent e(this, mColor);
                colorPickerEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        static ofxXRSColorPicker* getInstance() { return new ofxXRSColorPicker("X"); }
    
    protected:
    
        void onMouseEnter(ofPoint mouse)
        {
            mShowPicker = true;
            ofxXRSComponent::onFocus();
            ofxXRSComponent::onMouseEnter(mouse);
        }
    
        void onMouseLeave(ofPoint mouse)
        {
            mShowPicker = false;
            ofxXRSTextInput::onMouseLeave(mouse);
            if (!mInput.hasFocus()) ofxXRSComponent::onFocusLost();
        }
    
        void onMousePress(ofPoint mouse)
        {
            ofxXRSComponent::onMousePress(mouse);
            if (mInput.hitTest(mouse)) mInput.onFocus();
        }
    
        void onInputChanged(ofxXRSInternalEvent e)
        {
            mColor = ofColor::fromHex(ofHexToInt(mInput.getText()));
        // set the input field text & background colors //
            updateTextFieldColors();
        // update the gradient picker //
            gColors[2] = mColor;
            gColors[0] = ofColor(mColor.r/2, mColor.g/2, mColor.b/2);
            vbo.setColorData(&gColors[0], 6, GL_DYNAMIC_DRAW );
        // dispatch event out to main application //
            dispatchEvent();
        }

        inline void setTextFieldInputColor()
        {
        // convert color value to a six character hex string //
            std::stringstream ss;
            ss<< std::hex << mColor.getHex();
            std::string res ( ss.str() );
            while(res.size() < 6) res+="0";
            mInput.setText(ofToUpper(res));
            updateTextFieldColors();
        }
    
    private:

        ofColor mColor;
        ofColor gColor;
    
        struct {
	        std::shared_ptr<ofImage> image;
            ofRectangle rect;
        } rainbow;
    
        bool mShowPicker;
        ofColor pickerBorder;
        ofRectangle pickerRect;
        ofRectangle gradientRect;
    
        ofVbo vbo;
        std::vector<ofVec2f> gPoints;
        std::vector<ofFloatColor> gColors;
    
        void updateTextFieldColors()
        {
            mInput.setBackgroundColor(mColor);
        
            //Counting the perceptive luminance - human eye favors green color...
            double a = 1 - ( 0.299 * mColor.r + 0.587 * mColor.g + 0.114 * mColor.b)/255;
            mInput.setTextInactiveColor(a < 0.5 ? ofColor::black : ofColor::white);
        }

};

