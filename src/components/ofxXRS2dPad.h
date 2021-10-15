#pragma once
#include "ofxXRSComponent.h"

class ofxXRS2dPad : public ofxXRSComponent {

    public:
    
        ofxXRS2dPad(string label) : ofxXRSComponent(label)
        {
            mPercentX = 0.5f;
            mPercentY = 0.5f;
            mType = ofxXRSType::PAD2D;
            setTheme(ofxXRSComponent::getTheme());
            setBounds(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()), true);
            ofAddListener(ofEvents().windowResized, this, &ofxXRS2dPad::onWindowResized);
        }
    
        ofxXRS2dPad(string label, ofRectangle bounds) : ofxXRSComponent(label)
        {
            mPercentX = 0.5f;
            mPercentY = 0.5f;
            mType = ofxXRSType::PAD2D;
            setTheme(ofxXRSComponent::getTheme());
            setBounds(bounds, false);
            ofAddListener(ofEvents().windowResized, this, &ofxXRS2dPad::onWindowResized);
        }
    
        ~ofxXRS2dPad()
        {
            ofRemoveListener(ofEvents().windowResized, this, &ofxXRS2dPad::onWindowResized);
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mStyle.height = theme->layout.pad2d.height;
            mStyle.stripe.color = theme->stripe.pad2d;
            mColors.line = theme->color.pad2d.line;
            mColors.ball = theme->color.pad2d.ball;
            mColors.fill = theme->color.inputAreaBackground;
            mBallSize = theme->layout.pad2d.ballSize;
            mLineWeight = theme->layout.pad2d.lineWeight;
            mPad = ofRectangle(0, 0, mStyle.width - mStyle.padding - mLabel.width, mStyle.height - (mStyle.padding * 2));
        }
    
        void setPoint(ofPoint pt)
        {
            if (mBounds.inside(pt)){
                mPercentX = (pt.x-mBounds.x) / mBounds.width;
                mPercentY = (pt.y-mBounds.y) / mBounds.height;
                setWorldCoordinates();
            }   else{
        //  the point assigment is outside of the 2d pad's bounds //
            }
        }
    
        ofPoint getPoint()
        {
            return mWorld;
        }
    
        void setBounds(ofRectangle bounds, bool scaleOnResize = false)
        {
            mBounds = bounds;
            mScaleOnResize = scaleOnResize;
            setWorldCoordinates();
        }
    
        ofRectangle getBounds()
        {
            return mBounds;
        }
    
        void reset()
        {
            mPercentX = 0.5f;
            mPercentY = 0.5f;
            setWorldCoordinates();
        }
    
        void draw()
        {
            if (!mVisible) return;
            ofPushStyle();
                mPad.x = x + mLabel.width;
                mPad.y = y + mStyle.padding;
                mPad.width = mStyle.width - mStyle.padding - mLabel.width;
                mLocal.x = mPad.x + mPad.width * mPercentX;
                mLocal.y = mPad.y + mPad.height * mPercentY;
                ofxXRSComponent::draw();
                ofSetColor(mColors.fill);
                ofDrawRectangle(mPad);
                ofSetLineWidth(mLineWeight);
                ofSetColor(mColors.line);
                ofDrawLine(mPad.x, mLocal.y, mPad.x + mPad.width, mLocal.y);
                ofDrawLine(mLocal.x, mPad.y, mLocal.x, mPad.y + mPad.height);
                ofSetColor(mColors.ball);
                ofDrawCircle(mLocal, mBallSize);
            ofPopStyle();
        }
    
        void dispatchEvent()
        {
            if (pad2dEventCallback != nullptr) {
                ofxXRS2dPadEvent e(this, mWorld.x, mWorld.y);
                pad2dEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        static ofxXRS2dPad* getInstance() { return new ofxXRS2dPad("X"); }
    
    protected:
    
        void setWorldCoordinates()
        {
            mWorld.x = mBounds.x + (mBounds.width * mPercentX);
            mWorld.y = mBounds.y + (mBounds.height * mPercentY);
        }
    
        void onMouseDrag(ofPoint m)
        {
            if (mPad.inside(m)){
                mPercentX = (m.x-mPad.x) / mPad.width;
                mPercentY = (m.y-mPad.y) / mPad.height;
                setWorldCoordinates();
                dispatchEvent();
            }
        }
    
        void onWindowResized(ofResizeEventArgs &e)
        {
        // scale the bounds to the resized window //
            if (mScaleOnResize){
                mBounds.width *= (ofGetWidth() / mBounds.width);
                mBounds.height *= (ofGetHeight() / mBounds.height);
                setWorldCoordinates();
            }
        }
    
    private:
        ofPoint mLocal;
        ofPoint mWorld;
        ofRectangle mPad;
        ofRectangle mBounds;
        float mPercentX;
        float mPercentY;
        int mBallSize;
        int mLineWeight;
        bool mScaleOnResize;
        struct{
            ofColor fill;
            ofColor line;
            ofColor ball;
        } mColors;
    
};
    