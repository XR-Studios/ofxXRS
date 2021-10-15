#pragma once
#include "ofxXRSGroups.h"
#include "ofxXRSControls.h"
#include "ofxXRSSimpleButton.h"

class ofxXRSPanel : public ofxXRSInteractiveObject
{
    
    public:
    
        ofxXRSPanel(int x, int y);
        ofxXRSPanel(ofxXRSPanelAnchor anchor = ofxXRSPanelAnchor::TOP_LEFT);
        ~ofxXRSPanel();
    
        void draw();
        void update();
        void focus();
        void expand();
        void toggle();
        void collapse();

		void setHeight(int height, float labelHeight = 0.35f);
        void setWidth(int width, float labelWidth = 0.35f);
        void setVisible(bool visible);
        void setEnabled(bool enabled);
        void setOpacity(float opacity);
        void setPosition(int x, int y);
        void setPosition(ofxXRSPanelAnchor anchor);
        void setTheme(ofxXRSTheme* t, bool applyImmediately = false);
        void setAutoDraw(bool autodraw, int priority = 0);
        void setLabelAlignment(ofxXRSAlignment align);
        static void setAssetPath(string path);
        static string getAssetPath();
    
        int getWidth();
        int getHeight();
        bool getFocused();
        bool getVisible();
        bool getAutoDraw();
        bool getMouseDown();
        ofPoint getPosition();
    
        ofxXRSHeader* addHeader(string label = "", bool draggable = true);
        ofxXRSFooter* addFooter();
        ofxXRSLabel* addLabel(string label);
        ofxXRSButton* addButton(string label);
        ofxXRSToggle* addToggle(string label, bool state = false);
        ofxXRSSlider* addSlider(string label, float min, float max);
        ofxXRSSlider* addSlider(string label, float min, float max, float val);
        ofxXRSSlider* addSlider(ofParameter<int> & p);
        ofxXRSSlider* addSlider(ofParameter<float> & p);
        ofxXRSTextInput* addTextInput(string label, string value = "");
        ofxXRSDropdown* addDropdown(string label, vector<string> options);
        ofxXRSFRM* addFRM(float refresh = 1.0f);
        ofxXRSBreak* addBreak();
        ofxXRS2dPad* add2dPad(string label);
        ofxXRS2dPad* add2dPad(string label, ofRectangle bounds);
        ofxXRSWaveMonitor* addWaveMonitor(string label, float min, float max);
        ofxXRSValuePlotter* addValuePlotter(string label, float min, float max);
        ofxXRSColorPicker* addColorPicker(string label, ofColor color = ofColor::black);
        ofxXRSMatrix* addMatrix(string label, int numButtons, bool showLabels = false);
        ofxXRSFolder* addFolder(string label, ofColor color = ofColor::white);
        ofxXRSFolder* addFolder(ofxXRSFolder* folder);
    
        ofxXRSHeader* getHeader();
        ofxXRSFooter* getFooter();
        ofxXRSLabel* getLabel(string label, string folder = "");
        ofxXRSButton* getButton(string label, string folder = "");
        ofxXRSToggle* getToggle(string label, string folder = "");
        ofxXRSSlider* getSlider(string label, string folder = "");
        ofxXRS2dPad* get2dPad(string label, string folder = "");
        ofxXRSTextInput* getTextInput(string label, string folder = "");
        ofxXRSColorPicker* getColorPicker(string label, string folder = "");
        ofxXRSMatrix* getMatrix(string label, string folder = "");
        ofxXRSWaveMonitor* getWaveMonitor(string label, string folder = "");
        ofxXRSValuePlotter* getValuePlotter(string label, string folder = "");
        ofxXRSFolder* getFolder(string label);
        ofxXRSDropdown* getDropdown(string label);
    
    public:
    
        int mIndex;
        int mWidth;
        int mHeight;
        int mRowSpacing;
        float mAlpha;
        float mLabelWidth;
        bool mMoving;
        bool mVisible;
        bool mEnabled;
        bool mExpanded;
        bool mAutoDraw;
        bool mMouseDown;
        bool mAlphaChanged;
        bool mWidthChanged;
        bool mThemeChanged;
        bool mAlignmentChanged;
        ofColor mGuiBackground;
    
        ofPoint mPosition;
        ofRectangle mGuiBounds;
        ofxXRSPanelAnchor mAnchor;
        ofxXRSHeader* mGuiHeader;
        ofxXRSFooter* mGuiFooter;
        ofxXRSTheme* mTheme;
        ofxXRSAlignment mAlignment;
        vector<ofxXRSComponent*> items;
        vector<ofxXRSComponent*> trash;
        static ofxXRSPanel* mActiveGui;
        static vector<ofxXRSPanel*> mGuis;
        static unique_ptr<ofxXRSTheme> theme;
    
        void init();
        void layoutGui();
    	void positionGui();
        void moveGui(ofPoint pt);
        bool hitTest(ofPoint pt);
        void attachItem(ofxXRSComponent* item);
    
        void onDraw(ofEventArgs &e);
        void onUpdate(ofEventArgs &e);
        void onWindowResized(ofResizeEventArgs &e);
    
        ofxXRSComponent* getComponent(string key);
        ofxXRSComponent* getComponent(ofxXRSType type, string label);
        void onInternalEventCallback(ofxXRSInternalEvent e);
        void onButtonEventCallback(ofxXRSButtonEvent e);
        void onToggleEventCallback(ofxXRSToggleEvent e);
        void onSliderEventCallback(ofxXRSSliderEvent e);
        void onTextInputEventCallback(ofxXRSTextInputEvent e);
        void onDropdownEventCallback(ofxXRSDropdownEvent e);
        void on2dPadEventCallback(ofxXRS2dPadEvent e);
        void onColorPickerEventCallback(ofxXRSColorPickerEvent e);
        void onMatrixEventCallback(ofxXRSMatrixEvent e);

};
