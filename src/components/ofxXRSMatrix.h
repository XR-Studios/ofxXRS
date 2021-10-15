#pragma once
#include "ofxXRSComponent.h"

class ofxXRSMatrixButton : public ofxXRSInteractiveObject {

    public:
    
        ofxXRSMatrixButton(int size, int index, bool showLabels)
        {
            mIndex = index;
            mSelected = false;
            mShowLabels = showLabels;
            mRect = ofRectangle(0, 0, size, size);
        }
    
        void setPosition(float x, float y)
        {
            origin.x = x;
            origin.y = y;
        }
    
        void draw(int x, int y)
        {
            mRect.x = x + origin.x;
            mRect.y = y + origin.y;
            ofPushStyle();
                ofFill();
                ofSetColor(mBkgdColor);
                ofDrawRectangle(mRect);
                if (mShowLabels) {
                    ofSetColor(mLabelColor);
                    mFont->draw(ofToString(mIndex+1), mRect.x + mRect.width/2 - mFontRect.width/2, mRect.y + mRect.height/2 + mFontRect.height/2);
                }
            ofPopStyle();
        }
    
        void hitTest(ofPoint m, bool mouseDown)
        {
            if (mRect.inside(m) && !mSelected){
                if (mouseDown){
                    mBkgdColor = colors.selected.button;
                    mLabelColor = colors.selected.label;
                }   else{
                    mBkgdColor = colors.hover.button;
                    mLabelColor = colors.hover.label;
                }
            }   else{
                onMouseOut();
            }
        }
    
        int getIndex()
        {
            return mIndex;
        }
    
        void setSelected(bool selected)
        {
            mSelected = selected;
        }
    
        bool getSelected()
        {
            return mSelected;
        }
    
        void onMouseOut()
        {
            if (mSelected){
                mBkgdColor = colors.selected.button;
                mLabelColor = colors.selected.label;
            }   else{
                mBkgdColor = colors.normal.button;
                mLabelColor = colors.normal.label;
            }
        }
    
        void onMouseRelease(ofPoint m)
        {
            if (mRect.inside(m)) {
                mSelected = !mSelected;
                ofxXRSInternalEvent e(ofxXRSEventType::MATRIX_BUTTON_TOGGLED, mIndex);
                internalEventCallback(e);
            }
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            mFont = theme->font.ptr;
            mFontRect = mFont->rect(ofToString(mIndex+1));
            mBkgdColor = theme->color.matrix.normal.button;
            mLabelColor = theme->color.matrix.normal.label;
            colors.normal.label = theme->color.matrix.normal.label;
            colors.normal.button = theme->color.matrix.normal.button;
            colors.hover.label = theme->color.matrix.hover.label;
            colors.hover.button = theme->color.matrix.hover.button;
            colors.selected.label = theme->color.matrix.selected.label;
            colors.selected.button = theme->color.matrix.selected.button;
        }
    
    private:
        int x;
        int y;
        int mIndex;
        ofPoint origin;
        ofRectangle mRect;
        ofColor mBkgdColor;
        ofColor mLabelColor;
        bool mSelected;
        bool mShowLabels;
        ofRectangle mFontRect;
        shared_ptr<ofxSmartFont> mFont;
        struct {
            struct{
                ofColor label;
                ofColor button;
            } normal;
            struct{
                ofColor label;
                ofColor button;
            } hover;
            struct{
                ofColor label;
                ofColor button;
            } selected;
        } colors;
};

class ofxXRSMatrix : public ofxXRSComponent {

    public:
    
        ofxXRSMatrix(string label, int numButtons, bool showLabels = false) : ofxXRSComponent(label)
        {
            mRadioMode = false;
            mNumButtons = numButtons;
            mShowLabels = showLabels;
            mType = ofxXRSType::MATRIX;
            setTheme(ofxXRSComponent::getTheme());
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mFillColor = theme->color.inputAreaBackground;
            mButtonSize = theme->layout.matrix.buttonSize;
            mButtonPadding = theme->layout.matrix.buttonPadding;
            mStyle.stripe.color = theme->stripe.matrix;
            attachButtons(theme);
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }
    
        void setWidth(int width, float labelWidth)
        {
            ofxXRSComponent::setWidth(width, labelWidth);
            mMatrixRect.x = x + mLabel.width;
            mMatrixRect.y = y + mStyle.padding;
            mMatrixRect.width = mStyle.width - mStyle.padding - mLabel.width;
            int nCols = floor(mMatrixRect.width / (mButtonSize + mButtonPadding));
            int nRows = ceil(btns.size() / float(nCols));
            float padding = (mMatrixRect.width - (mButtonSize * nCols)) / (nCols - 1);
            for(size_t i=0; i<btns.size(); i++){
                float bx = (mButtonSize + padding) * (i % nCols);
                float by = (mButtonSize + padding) * (floor(i/nCols));
                btns[i].setPosition(bx, by + mStyle.padding);
            }
            mStyle.height = (mStyle.padding*2) + ((mButtonSize + padding) * (nRows - 1)) + mButtonSize;
            mMatrixRect.height = mStyle.height - (mStyle.padding * 2);
        }
    
        void setPosition(int x, int y)
        {
            ofxXRSComponent::setPosition(x, y);
            mMatrixRect.x = x + mLabel.width;
            mMatrixRect.y = y + mStyle.padding;
        }
    
        void setRadioMode(bool enabled)
        {
            mRadioMode = enabled;
        }
    
        bool hitTest(ofPoint m)
        {
            if (mMatrixRect.inside(m)){
                for(size_t i=0; i<btns.size(); i++) btns[i].hitTest(m, mMouseDown);
                return true;
            }   else{
                for(size_t i=0; i<btns.size(); i++) btns[i].onMouseOut();
                return false;
            }
        }
    
        void draw()
        {
            if (!mVisible) return;
            ofPushStyle();
                ofxXRSComponent::draw();
                ofSetColor(mFillColor);
                ofDrawRectangle(mMatrixRect);
                for(size_t i=0; i<btns.size(); i++) btns[i].draw(x+mLabel.width, y);
            ofPopStyle();
        }
    
        void clear()
        {
            for (size_t i=0; i<btns.size(); i++) btns[i].setSelected(false);
        }
    
        void setSelected(vector<int> v)
        {
            clear();
            for (size_t i=0; i<v.size(); i++) btns[v[i]].setSelected(true);
            mLastItemSelected = &btns[v.back()];
        }
    
        vector<int> getSelected()
        {
            vector<int> selected;
            for(size_t i=0; i<btns.size(); i++) if (btns[i].getSelected()) selected.push_back(i);
            return selected;
        }
    
        ofxXRSMatrixButton* getButtonAtIndex(int index)
        {
            return &btns[index];
        }
    
        void dispatchEvent()
        {
            if (matrixEventCallback != nullptr) {
                if (btns.size() != 0){
                    if (mLastItemSelected == nullptr){
                        mLastItemSelected = &btns.back();
                    }
                    ofxXRSMatrixEvent e(this, mLastItemSelected->getIndex(), mLastItemSelected->getSelected());
                    matrixEventCallback(e);
                }   else{
                    ofxXRSLog::write(ofxXRSMsg::MATRIX_EMPTY);
                }
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        static ofxXRSMatrix* getInstance() { return new ofxXRSMatrix("X", 0); }
    
    protected:
    
        void onMouseRelease(ofPoint m)
        {
            ofxXRSComponent::onFocusLost();
            ofxXRSComponent::onMouseRelease(m);
            for(size_t i=0; i<btns.size(); i++) btns[i].onMouseRelease(m);
        }
    
        void onButtonSelected(ofxXRSInternalEvent e)
        {
            if (mRadioMode) {
        // deselect all buttons save the one that was selected //
                for(size_t i=0; i<btns.size(); i++) btns[i].setSelected(e.index == i);
            }
            mLastItemSelected = &btns[e.index];
            dispatchEvent();
        }
    
        void attachButtons(const ofxXRSTheme* theme)
        {
            btns.clear();
            for(int i=0; i < mNumButtons; i++) {
                ofxXRSMatrixButton btn(mButtonSize, i, mShowLabels);
                btn.setTheme(theme);
                btn.onInternalEvent(this, &ofxXRSMatrix::onButtonSelected);
                btns.push_back(btn);
            }
        }
    
    private:
    
        int mButtonSize;
        int mNumButtons;
        int mButtonPadding;
        bool mRadioMode;
        bool mShowLabels;
        ofColor mFillColor;
        ofRectangle mMatrixRect;
        vector<ofxXRSMatrixButton> btns;
        ofxXRSMatrixButton* mLastItemSelected;
};



