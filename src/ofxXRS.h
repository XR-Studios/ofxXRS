#pragma once
#include "ofxXRSGroups.h"
#include "ofxXRSControls.h"
#include "ofxXRSSimpleButton.h"
#include "ofxXRSClickMenu.h"

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
        static void setAssetPath(std::string path);
        static std::string getAssetPath();
    
        int getWidth();
        int getHeight();
        bool getFocused();
        bool getVisible();
        bool getAutoDraw();
        bool getMouseDown();
        ofPoint getPosition();
		ofRectangle getRect();
    
        ofxXRSHeader* addHeader(std::string label = "", bool draggable = true);
        ofxXRSFooter* addFooter();
        ofxXRSLabel* addLabel(std::string label);
        ofxXRSButton* addButton(std::string label);
        ofxXRSToggle* addToggle(std::string label, bool state = false);
        ofxXRSSlider* addSlider(std::string label, float min, float max);
        ofxXRSSlider* addSlider(std::string label, float min, float max, float val);
        ofxXRSSlider* addSlider(ofParameter<int> & p);
        ofxXRSSlider* addSlider(ofParameter<float> & p);
        ofxXRSTextInput* addTextInput(std::string label, std::string value = "");
        ofxXRSDropdown* addDropdown(std::string label, std::vector<std::string> options);
        ofxXRSFRM* addFRM(float refresh = 1.0f);
        ofxXRSBreak* addBreak();
        ofxXRS2dPad* add2dPad(std::string label);
        ofxXRS2dPad* add2dPad(std::string label, ofRectangle bounds);
        ofxXRSWaveMonitor* addWaveMonitor(std::string label, float min, float max);
        ofxXRSValuePlotter* addValuePlotter(std::string label, float min, float max);
        ofxXRSColorPicker* addColorPicker(std::string label, ofColor color = ofColor::black);
        ofxXRSMatrix* addMatrix(std::string label, int numButtons, bool showLabels = false);
        ofxXRSFolder* addFolder(std::string label, ofColor color = ofColor::white);
        ofxXRSFolder* addFolder(ofxXRSFolder* folder);
    
        ofxXRSHeader* getHeader();
        ofxXRSFooter* getFooter();
        ofxXRSLabel* getLabel(std::string label, std::string folder = "");
        ofxXRSButton* getButton(std::string label, std::string folder = "");
        ofxXRSToggle* getToggle(std::string label, std::string folder = "");
        ofxXRSSlider* getSlider(std::string label, std::string folder = "");
        ofxXRS2dPad* get2dPad(std::string label, std::string folder = "");
        ofxXRSTextInput* getTextInput(std::string label, std::string folder = "");
        ofxXRSColorPicker* getColorPicker(std::string label, std::string folder = "");
        ofxXRSMatrix* getMatrix(std::string label, std::string folder = "");
        ofxXRSWaveMonitor* getWaveMonitor(std::string label, std::string folder = "");
        ofxXRSValuePlotter* getValuePlotter(std::string label, std::string folder = "");
        ofxXRSFolder* getFolder(std::string label);
        ofxXRSDropdown* getDropdown(std::string label);
    
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
        std::vector<ofxXRSComponent*> items;
        std::vector<ofxXRSComponent*> trash;
        static ofxXRSPanel* mActiveGui;
        static std::vector<ofxXRSPanel*> mGuis;
        static std::unique_ptr<ofxXRSTheme> theme;
    
        void init();
        void layoutGui();
    	void positionGui();
        void moveGui(ofPoint pt);
        bool hitTest(ofPoint pt);
        void attachItem(ofxXRSComponent* item);
    
        void onDraw(ofEventArgs &e);
        void onUpdate(ofEventArgs &e);
        void onWindowResized(ofResizeEventArgs &e);
    
        ofxXRSComponent* getComponent(std::string key);
        ofxXRSComponent* getComponent(ofxXRSType type, std::string label);
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
