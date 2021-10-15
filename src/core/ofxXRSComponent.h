#pragma once
#include "ofxXRSIntObject.h"

class ofxXRSComponent : public ofxXRSInteractiveObject
{
    public:
    
        ofxXRSComponent(string label);
        virtual ~ofxXRSComponent();
    
        int     getX();
        int     getY();
        void    setIndex(int index);
        int     getIndex();
        void    setName(string name);
        string  getName();
        bool    is(string name);
    
        void    setLabel(string label);
        string  getLabel();
        void    setLabelColor(ofColor color);
        ofColor getLabelColor();
        void    setLabelUpperCase(bool toUpper);
        bool    getLabelUpperCase();
    
        void    setBackgroundColor(ofColor color);
        void    setBackgroundColorOnMouseOver(ofColor color);
        void    setBackgroundColorOnMouseDown(ofColor color);
        void    setBackgroundColors(ofColor bkgd, ofColor mOver, ofColor mDown);
    
        void    setStripe(ofColor color, int width);
        void    setStripeWidth(int width);
        void    setStripeColor(ofColor color);
        void    setStripeVisible(bool visible);
    
        void    setBorder(ofColor color, int width);
        void    setBorderVisible(bool visible);
    
        void    setMask(const ofRectangle &mask);
        void    setAnchor(ofxXRSPanelAnchor anchor);
        void    setEnabled(bool visible);
        bool    getEnabled();
        void    setVisible(bool visible);
        bool    getVisible();
        void    setFocused(bool focused);
        bool    getFocused();
        void    setOpacity(float opacity);
        bool    getMouseDown();
        ofxXRSType getType();
    
        vector<ofxXRSComponent*> children;
    
        virtual void draw();
        virtual void update(bool acceptEvents = true);
        virtual bool hitTest(ofPoint m);

        virtual void setPosition(int x, int y);
        virtual void setTheme(const ofxXRSTheme* theme) = 0;
        virtual void setWidth(int width, float labelWidth);
        virtual void setLabelAlignment(ofxXRSAlignment align);
    
        virtual int  getWidth();
        virtual int  getHeight();
        virtual bool getIsExpanded();
        virtual void drawColorPicker();

        virtual void onFocus();    
        virtual void onFocusLost();
        virtual void onWindowResized();
        virtual void onKeyPressed(int key);
        virtual void onMouseEnter(ofPoint m);
        virtual void onMousePress(ofPoint m);
        virtual void onMouseDrag(ofPoint m);
        virtual void onMouseLeave(ofPoint m);
        virtual void onMouseRelease(ofPoint m);
        void onKeyPressed(ofKeyEventArgs &e);
        void onWindowResized(ofResizeEventArgs &e);

        static const ofxXRSTheme* getTheme();
    
    protected:
    
        int x;
        int y;
        int mIndex;
        string mName;
        bool mFocused;
        bool mVisible;
        bool mEnabled;
        bool mMouseOver;
        bool mMouseDown;
        ofRectangle mMask;
        ofxXRSType mType;
        ofxXRSPanelAnchor mAnchor;
        shared_ptr<ofxSmartFont> mFont;
    
        struct{
            float width;
            float height;
            float padding;
            float vMargin;
            float opacity;
            struct{
                ofColor inputArea;
                ofColor background;
                ofColor onMouseOver;
                ofColor onMouseDown;
            } color;
            struct{
                int width;
                bool visible;
                ofColor color;
            } border;
            struct{
                int width;
                bool visible;
                ofColor color;
            } stripe;
            ofColor guiBackground;
        } mStyle;
    
        struct{
            int x;
            string text;
            string rendered;
            bool visible;
            ofColor color;
            float width;
            int margin;
            int rightAlignedXpos;
            ofRectangle rect;
            bool forceUpperCase;
            ofxXRSAlignment alignment;
        } mLabel;
    
        struct {
            int x;
            int y;
            int size;
            ofColor color;
        } mIcon;
    
        void drawLabel();
        void drawBorder();
        void drawStripe();
        void drawBackground();
        void positionLabel();
        void setComponentStyle(const ofxXRSTheme* t);
    
    private:
    
        static unique_ptr<ofxXRSTheme> theme;
    
};

