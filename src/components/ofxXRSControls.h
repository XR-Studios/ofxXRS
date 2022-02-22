#pragma once
#include "ofxXRSButton.h"

class ofxXRSHeader : public ofxXRSButton {

    public:

        ofxXRSHeader(std::string label, bool draggable = true) : ofxXRSButton(label)
        {
            mDraggable = draggable;
            setTheme(ofxXRSComponent::getTheme());
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mLabel.width = mStyle.width;
            mStyle.stripe.visible = theme->stripe.visible;
            mStyle.height = mStyle.height * .8;
            setLabelAlignment(ofxXRSAlignment::CENTER);
        }
    
        void setDraggable(bool draggable)
        {
            mDraggable = draggable;
        }
    
        bool getDraggable()
        {
            return mDraggable;
        }
    
        ofPoint getDragOffset()
        {
            return mDragOffset;
        }
    
    protected:

        void onMouseEnter(ofPoint m)
        {
        // disable hover state if we're not draggable //
            if (mDraggable) ofxXRSComponent::onMouseEnter(m);
        }
    
        void onMousePress(ofPoint m)
        {
            mDragOffset = ofPoint(m.x-this->x,m.y-this->y);
            ofxXRSComponent::onMousePress(m);
        }
    
        void onMouseRelease(ofPoint m)
        {
            mDragOffset = m;
            ofxXRSComponent::onFocusLost();
            ofxXRSComponent::onMouseRelease(m);
        }
    
    // allow panel to be dragged around //
        void onFocusLost() {}
    // force header label to always be centered //
        void setLabelAlignment(ofxXRSAlignment align)
        {
            ofxXRSComponent::setLabelAlignment(ofxXRSAlignment::CENTER);
        }
    
    private:
        bool mDraggable;
        ofPoint mDragOffset;

};

class ofxXRSFooter : public ofxXRSButton {

    public:
    
        ofxXRSFooter() : ofxXRSButton("collapse controls")
        {
            mGuiExpanded = true;
            mLabelCollapsed = "expand controls";
            mLabelExpanded = "collapse controls";
            setTheme(ofxXRSComponent::getTheme());
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mLabel.width = mStyle.width;
            mStyle.stripe.visible = false;
            mStyle.height = mStyle.height * .8;
            setLabelAlignment(ofxXRSAlignment::CENTER);
        }
    
        void setLabelWhenExpanded(std::string label)
        {
            mLabelExpanded = label;
            if (mGuiExpanded) setLabel(mLabelExpanded);
        }
    
        void setLabelWhenCollapsed(std::string label)
        {
            mLabelCollapsed = label;
            if (!mGuiExpanded) setLabel(mLabelCollapsed);
        }
    
        void setExpanded(bool expanded)
        {
            mGuiExpanded = expanded;
            if (mGuiExpanded){
                setLabel(mLabelExpanded);
            }   else{
                setLabel(mLabelCollapsed);
            }
        }
    
    protected:
    
        void onMouseRelease(ofPoint m)
        {
            ofxXRSComponent::onMouseRelease(m);
        // dispatch event out to main application //
            ofxXRSInternalEvent e(ofxXRSEventType::GUI_TOGGLED, mIndex);
            internalEventCallback(e);
        }
    
    // force footer label to always be centered //
        void setLabelAlignment(ofxXRSAlignment align)
        {
            ofxXRSComponent::setLabelAlignment(ofxXRSAlignment::CENTER);
        }
    
    private:
        bool mGuiExpanded;
        std::string mLabelExpanded;
        std::string mLabelCollapsed;
    
};
