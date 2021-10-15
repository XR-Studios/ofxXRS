#pragma once

class ofxXRSButton;
class ofxXRSToggle;
class ofxXRSSlider;
class ofxXRSDropdown;
class ofxXRSTextInput;
class ofxXRS2dPad;
class ofxXRSColorPicker;
class ofxXRSMatrix;
class ofxXRSScrollView;
class ofxXRSScrollViewItem;

enum ofxXRSEventType
{
    GUI_TOGGLED = 0,
    BUTTON_CLICKED,
    BUTTON_TOGGLED,
    INPUT_CHANGED,
    COLOR_CHANGED,
    SLIDER_CHANGED,
    OPTION_SELECTED,
    GROUP_TOGGLED,
    VISIBILITY_CHANGED,
    MATRIX_BUTTON_TOGGLED
};

class ofxXRSInternalEvent{
    
    public:
        ofxXRSInternalEvent(int eType, int eIndex){
            type = eType;
            index = eIndex;
        };
        int type;
        int index;
};

class ofxXRSButtonEvent{

    public:
        ofxXRSButtonEvent(ofxXRSButton* t)
        {
            target = t;
        }
    ofxXRSButton* target;
};

class ofxXRSToggleEvent{

    public:
        ofxXRSToggleEvent(ofxXRSToggle* t, bool e = false)
        {
            target = t;
            checked = e;
        }
    bool checked;
    ofxXRSToggle* target;
};

class ofxXRSSliderEvent{

    public:
        ofxXRSSliderEvent(ofxXRSSlider* t, float v, float s)
        {
            value = v;
            scale = s;
            target = t;
        }
    float value;
    float scale;
    ofxXRSSlider* target;
};

class ofxXRSTextInputEvent{

    public:
        ofxXRSTextInputEvent(ofxXRSTextInput* t, std::string s)
        {
            text = s;
            target = t;
        }
    std::string text;
    ofxXRSTextInput* target;
};

class ofxXRSColorPickerEvent{
    
    public:
        ofxXRSColorPickerEvent(ofxXRSColorPicker* t, ofColor c)
        {
            color = c;
            target = t;
        }
    ofColor color;
    ofxXRSColorPicker* target;
};

class ofxXRSDropdownEvent{

    public:
        ofxXRSDropdownEvent(ofxXRSDropdown* t, int p, int c)
        {
            child = c;
            parent = p;
            target = t;
        }
    int child;
    int parent;
    ofxXRSDropdown* target;
};

class ofxXRSScrollViewEvent{

    public:
        ofxXRSScrollViewEvent(ofxXRSScrollView* p, ofxXRSScrollViewItem* b)
        {
            parent = p;
            target = b;
        }
    ofxXRSScrollView* parent;
    ofxXRSScrollViewItem* target;
};

class ofxXRS2dPadEvent{

    public:
        ofxXRS2dPadEvent(ofxXRS2dPad* t, float xp, float yp)
        {
            x = xp;
            y = yp;
            target = t;
        }
    float x;
    float y;
    ofxXRS2dPad* target;
};

class ofxXRSMatrixEvent{

    public:
        ofxXRSMatrixEvent(ofxXRSMatrix* t, int i, bool e)
        {
            child = i;
            target = t;
            enabled = e;
        }
    int child;
    bool enabled;
    ofxXRSMatrix* target;
};



