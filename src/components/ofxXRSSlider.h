#pragma once
#include "ofxXRSComponent.h"
#include "ofxXRSTextInputField.h"

class ofxXRSSlider : public ofxXRSComponent {

    public:
    
        ofxXRSSlider(std::string label, float min, float max, float val) : ofxXRSComponent(label)
        {
            mMin = min;
            mMax = max;
            setPrecision(2);
            mType = ofxXRSType::SLIDER;
            mInput = new ofxXRSTextInputField();
            mInput->setTextInputFieldType(ofxXRSInputType::NUMERIC);
            mInput->onInternalEvent(this, &ofxXRSSlider::onInputChanged);
            setTheme(ofxXRSComponent::getTheme());
            setValue(val, false); // don't dispatch a change event when component is constructed //
        }
    
        ofxXRSSlider(std::string label, float min, float max) : ofxXRSSlider(label, min, max, (max+min)/2) {}

        ofxXRSSlider(ofParameter<int> & p) : ofxXRSSlider(p.getName(), p.getMin(), p.getMax(), p.get()) {
            mParamI = &p;
            setPrecision(0);
            calculateScale();
            mParamI->addListener(this, &ofxXRSSlider::onParamI);
        }
        
        ofxXRSSlider(ofParameter<float> & p) : ofxXRSSlider(p.getName(), p.getMin(), p.getMax(), p.get()) {
            mParamF = &p;
            setPrecision(2);
            calculateScale();
            mParamF->addListener(this, &ofxXRSSlider::onParamF);
        }
    
        ~ofxXRSSlider()
        {
            delete mInput;
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mSliderFill = theme->color.slider.fill;
            mBackgroundFill = theme->color.inputAreaBackground;
            mStyle.stripe.color = theme->stripe.slider;
            mInput->setTheme(theme);
            mInput->setTextInactiveColor(theme->color.slider.text);
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }
    
        void setWidth(int width, float labelWidth)
        {
            ofxXRSComponent::setWidth(width, labelWidth);
            float totalWidth = mStyle.width - mLabel.width;
            mSliderWidth = totalWidth * .7;
            mInputX = mLabel.width + mSliderWidth + mStyle.padding;
            mInputWidth = totalWidth - mSliderWidth - (mStyle.padding * 2);
            mInput->setWidth(mInputWidth);
            mInput->setPosition(x + mInputX, y + mStyle.padding);
        }
    
        void setPosition(int x, int y)
        {
            ofxXRSComponent::setPosition(x, y);
            mInput->setPosition(x + mInputX, y + mStyle.padding);
        }
    
        void setPrecision(int precision)
        {
            mPrecision = precision;
            if (mPrecision > MAX_PRECISION) mPrecision = MAX_PRECISION;
        }
    
        void setMin(float min)
        {
            mMin = min;
            if (mMin < mMax){
                calculateScale();
            }   else{
                onInvalidMinMaxValues();
            }
        }
    
        void setMax(float max)
        {
            mMax = max;
            if (mMax > mMin){
                calculateScale();
            }   else{
                onInvalidMinMaxValues();
            }
        }
    
        void setValue(float value, bool dispatchEvent = true)
        {
            value = round(value, mPrecision);
            if (value != mValue){
                mValue = value;
                if (mValue > mMax){
                    mValue = mMax;
                }   else if (mValue < mMin){
                    mValue = mMin;
                }
                calculateScale();
                if (dispatchEvent) dispatchSliderChangedEvent();
            }
        }
    
        float getValue()
        {
            return mValue;
        }
    
        void setScale(float scale)
        {
            mScale = scale;
            if (mScale < 0 || mScale > 1){
                ofLogError() << "row #" << mIndex << " : scale must be between 0 & 1" << " [setting to 50%]";
                mScale = 0.5f;
            }
            mValue = ((mMax-mMin) * mScale) + mMin;
        }
    
        float getScale()
        {
            return mScale;
        }

    /*
        variable binding methods
    */
    
        void bind(int &val)
        {
            mBoundi = &val;
            mBoundf = nullptr;
        }
    
        void bind(float &val)
        {
            mBoundf = &val;
            mBoundi = nullptr;
        }

        void bind(int &val, int min, int max)
        {
            mMin = min;
            mMax = max;
            mBoundi = &val;
            mBoundf = nullptr;
        }
    
        void bind(float &val, float min, float max)
        {
            mMin = min;
            mMax = max;
            mBoundf = &val;
            mBoundi = nullptr;
        }
    
        void update(bool acceptEvents = true)
        {
            ofxXRSComponent::update(acceptEvents);
        // check for variable bindings //
            if (mBoundf != nullptr && !mInput->hasFocus()) {
                setValue(*mBoundf);
            }   else if (mBoundi != nullptr && !mInput->hasFocus()){
                setValue(*mBoundi);
            }
        }

        void draw()
        {
            if (!mVisible) return;
            ofPushStyle();
                ofxXRSComponent::draw();
            // slider bkgd //
                ofSetColor(mBackgroundFill);
                ofDrawRectangle(x+mLabel.width, y+mStyle.padding, mSliderWidth, mStyle.height-(mStyle.padding*2));
            // slider fill //
                if (mScale > 0){
                    ofSetColor(mSliderFill);
                    ofDrawRectangle(x+mLabel.width, y+mStyle.padding, mSliderWidth*mScale, mStyle.height-(mStyle.padding*2));
                }
            // numeric input field //
                mInput->draw();
            ofPopStyle();
        }
    
        bool hitTest(ofPoint m)
        {
            if (!mEnabled || !mVisible){
                return false;
            }   else if (m.x>=x+mLabel.width && m.x<= x+mLabel.width+mSliderWidth && m.y>=y+mStyle.padding && m.y<= y+mStyle.height-mStyle.padding){
                return true;
            }   else if (mInput->hitTest(m)){
                return true;
            }   else{
                return false;
            }
        }
    
        void dispatchEvent()
        {
            if (sliderEventCallback != nullptr) {
                ofxXRSSliderEvent e(this, mValue, mScale);
                sliderEventCallback(e);
            }   else{
                ofxXRSLog::write("ofxXRSSlider", ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        static ofxXRSSlider* getInstance() { return new ofxXRSSlider("X", 0, 100); }
    
    protected:
    
        void onMousePress(ofPoint m)
        {
            ofxXRSComponent::onMousePress(m);
            if (mInput->hitTest(m)){
                mInput->onFocus();
            }   else if (mInput->hasFocus()){
                mInput->onFocusLost();
            }
        }
    
        void onMouseDrag(ofPoint m)
        {
            if (mFocused && mInput->hasFocus() == false){
                float s = (m.x-x-mLabel.width)/mSliderWidth;
                if (s > .999) s = 1;
                if (s < .001) s = 0;
        // don't dispatch an event if scale hasn't changed //
                if (s == mScale) return;
                mScale = s;
                setValue(((mMax-mMin) * mScale) + mMin);
            }
        }
    
        void onMouseRelease(ofPoint m)
        {
            ofxXRSComponent::onMouseRelease(m);
            if (mInput->hitTest(m) == false) onFocusLost();
        }
    
        void onFocusLost()
        {
            ofxXRSComponent::onFocusLost();
            if (mInput->hasFocus()) mInput->onFocusLost();
        }
    
        void onKeyPressed(int key)
        {
            if (mInput->hasFocus()) mInput->onKeyPressed(key);
        }
    
        void onInputChanged(ofxXRSInternalEvent e)
        {
            setValue(ofToFloat(mInput->getText()));
        }
    
        void dispatchSliderChangedEvent()
        {
            if (mBoundf != nullptr) {
                *mBoundf = mValue;
            }   else if (mBoundi != nullptr) {
                *mBoundi = mValue;
            }   else if (mParamI != nullptr) {
                mParamI->set(mValue);
            }   else if (mParamF != nullptr) {
                mParamF->set(mValue);
            }
            dispatchEvent();
        }

    private:
    
        float   mMin;
        float   mMax;
        float  	mValue;
        float  	mScale;
        int     mPrecision;
        int     mInputX;
        int     mInputWidth;
        int     mSliderWidth;
        ofColor mSliderFill;
        ofColor mBackgroundFill;
        ofxXRSTextInputField* mInput;
    
        static const int MAX_PRECISION = 4;
    
        int*    mBoundi = nullptr;
        float*  mBoundf = nullptr;
        ofParameter<int>* mParamI = nullptr;
        ofParameter<float>* mParamF = nullptr;
        void onParamI(int& n) { setValue(n); }
        void onParamF(float& n) { setValue(n); }
    
        void calculateScale()
        {
            mScale = ofxXRSScale(mValue, mMin, mMax);
            setTextInput();
        }
    
        void setTextInput()
        {
	        std::string v = ofToString(round(mValue, mPrecision));
            if (mValue != mMin && mValue != mMax){
                size_t p = v.find('.');
                if (p == -1 && mPrecision != 0){
                    v+='.';
                    p = v.find('.');
                }
                while(static_cast<int>(v.length() - p) < (mPrecision + 1)) v+='0';
            }
            mInput->setText(v);
        }
    
        float round(float num, int precision)
        {
            return roundf(num * pow(10, precision)) / pow(10, precision);
        }
    
        void onInvalidMinMaxValues()
        {
            ofLogError() << "row #" << mIndex << " : invalid min & max values" << " [setting to 50%]";
            mMin = 0;
            mMax = 100;
            mScale = 0.5f;
            mValue = (mMax+mMin) * mScale;
        }
        
};

