#pragma once

#include "ofMain.h"
#include "ofxXRS.h"

class ofxXRSSimpleButton {

public:
typedef enum {
    TYPE_BUTTON = 0,
    TYPE_TOGGLE = 1,
    
}TYPE_BUTTONS;

typedef enum { 
    BUTTON_RECT = 1,
    BUTTON_CIRCLE = 2,
    BUTTON_CUSTOM = 3,
    BUTTON_IMAGE = 4
}BUTTON_SHAPES;

private:
    ofRectangle b_rect;
    ofPoint n_pos;
    ofPoint v_pos;
    
    ofColor b_c;
    ofColor b_t_c;

	std::vector<ofFbo> b_img_fbo;
    ofFbo a_img_fbo;
    
    TYPE_BUTTONS b_type;
    BUTTON_SHAPES b_shape;
    
    std::string b_info_text;
    std::string b_val_text;
    std::string b_img_route;
    std::string b_img_extension;
    
    bool useName;
    bool useValue;
    
    bool bRender;
    bool bAppear;
    bool fixPos;
    
    bool bSelect;
    bool bToggle;
    bool bDoubleTab;
    bool bAnimation;
    
    bool bang;
    bool startAni;
    
    int aniTime;
    int changeTime;
    
    int alpha;
    
protected:
    void resetAniTime(bool start);
    
    void update(ofEventArgs &event);
    void render(ofEventArgs &event);
    
    void touchDown(ofTouchEventArgs &touch);
    void touchUp(ofTouchEventArgs &touch);
    void touchMoved(ofTouchEventArgs &touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    
    void mousePressed(ofMouseEventArgs &mouse);
    void mouseReleased(ofMouseEventArgs &mouse);
    void mouseMoved(ofMouseEventArgs &mouse);
    void mouseDragged(ofMouseEventArgs &mouse);
    
    void updateButtonColor();
    
    void buttonAction(bool b);
    bool isInside(float x, float y);
    
    int checkExtension(std::string n);
public:
    explicit ofxXRSSimpleButton();
    virtual~ofxXRSSimpleButton();
    
    bool setup(float x, float y, float w = 50, float h = 50, bool useEvent = false);
    bool setup(float x, float y, float w, float h, bool useEvent, bool manualRender, TYPE_BUTTONS type = TYPE_BUTTON, BUTTON_SHAPES shape = BUTTON_RECT, const ofColor &c = ofColor::white);
    bool setup(float x, float y, std::string imgRoute, bool useAnimation = false, bool useEvent = true, bool manualRender = false, TYPE_BUTTONS type = TYPE_BUTTON);
    
    void setAppear(bool show);
    void setRender(bool magic);
    void setType(TYPE_BUTTONS type);
    void setShape(BUTTON_SHAPES shape);
    void setColor(const ofColor &c);
    void setToggleColor(const ofColor &c);
    void setAlpha(int a);
    void setAlpha(float a);
    void setPos(const ofPoint & p);
    void setPos(float x, float y);
    void setName(std::string n, float n_x = 0, float n_y = -10);
    void setValue(std::string v, float v_x = 0, float v_y = -10);
    void setFixPos(bool fix);
    void setAsAnimationButton(int time = 500);
    void resetToggle();
    void toggleShow();
    void hide();
    void show();
    
    void render();
    
    void touchDown(float x, float y);
    void touchUp();
    void touchMoved(float x, float y);
    void touchDoubleTap(float x, float y);
    
    void mousePressed(float x, float y);
    void mouseReleased();
    void mouseMoved(float x, float y);
    void mouseDragged(float x, float y);
    
    bool getIsAppear() const { return bAppear; }
    bool getIsRender() const { return bRender; }
    bool getIsSelect() const { return bSelect; }
    bool getIsToggl() const { return bToggle; }
    bool getIsDoubleTab() const { return bDoubleTab; }
    
    float getX() const { return b_rect.x; }
    float getY() const { return b_rect.y; }
    float getWidth() const { return b_rect.width; }
    float getHeight() const {return b_rect.height; }
	ofColor getColor() const { return b_c; }
    std::string getName() const { return b_info_text; }
    

	ofEvent<void> mousePressedEvent;
	typedef std::function<void(ofxXRSButtonEvent)> onButtonEventCallback;
	onButtonEventCallback buttonEventCallback;

	template<typename T, typename args, class ListenerClass>
	void onButtonEvent(T* owner, void (ListenerClass::*listenerMethod)(args)) {
		buttonEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
	}

	void onButtonEvent(onButtonEventCallback callback) {
		buttonEventCallback = callback;
	}
};

class ofxSimpleSlider{
private:
    ofxXRSSimpleButton *sliderButton;
    
    ofxXRSSimpleButton *plusButton;
    ofxXRSSimpleButton *minusButton;
    
    ofRectangle s_rect;
    float max_v;
    float min_v;
    float v;
    float d_v;
    float m_v;
    float s_v;
    
    bool useCountButton;
    bool hideValue;
    ofColor s_c;
    
    std::string s_name;
    
public:
    explicit ofxSimpleSlider():min_v(0.0), max_v(1.0),d_v(0.5),s_v(100.0),useCountButton(false),hideValue(false),s_c(ofColor::white){}
    ~ofxSimpleSlider() {
        if(useCountButton){
            delete plusButton;
            delete minusButton;
        }
        delete sliderButton;
    }
    void setup(std::string name, float x, float y, float w, float h, float min, float max, float defalut){
        s_name = name;
        s_rect.set(x, y, w, h);
        
        sliderButton = new ofxXRSSimpleButton();
        sliderButton->setup(x, y, h, h, true, true);
        sliderButton->show();
        
        min_v = min;
        max_v = max;
        if (defalut < min_v || defalut > max_v) { d_v = (max_v - min_v)/2.0; }
        else { d_v = defalut; }
        m_v = (max_v - min_v) / s_v;
        v = ofMap(d_v, min_v, max_v, s_rect.x, s_rect.x + s_rect.getWidth() - sliderButton->getWidth());
        sliderButton->setPos(v, sliderButton->getY());
        
        if (ofGetTargetPlatform() == OF_TARGET_IOS) {
            ofAddListener(ofEvents().touchDown, this, &ofxSimpleSlider::touchDown);
            ofAddListener(ofEvents().touchMoved, this, &ofxSimpleSlider::touchMoved);
            ofAddListener(ofEvents().touchDoubleTap, this, &ofxSimpleSlider::touchDoubleTap);
            ofAddListener(ofEvents().touchUp, this, &ofxSimpleSlider::touchUp);
        }else{
            ofAddListener(ofEvents().mousePressed , this, &ofxSimpleSlider::mousePressed);
            ofAddListener(ofEvents().mouseReleased, this, &ofxSimpleSlider::mouseReleased);
            ofAddListener(ofEvents().mouseMoved, this, &ofxSimpleSlider::mouseMoved);
            ofAddListener(ofEvents().mouseDragged, this, &ofxSimpleSlider::mouseDragged);
        }
        
        ofAddListener(ofEvents().draw, this, &ofxSimpleSlider::render);
    }
    
    void setDivide(float s) { m_v = (max_v - min_v) / s; }
    void setAmount(float a) { m_v = a; }
    void setColor(const ofColor &c) {
        s_c = c;
        sliderButton->setColor(s_c);
    }
    
    void showButtons() {
        if(!useCountButton){
            useCountButton = true;
            minusButton = new ofxXRSSimpleButton();
            plusButton = new ofxXRSSimpleButton();
            minusButton->setup(s_rect.x + s_rect.getWidth() + 10, s_rect.y, sliderButton->getWidth(), sliderButton->getHeight(), false, true);
            
            plusButton->setup(minusButton->getX() + minusButton->getWidth() + 10, s_rect.y, sliderButton->getWidth(), sliderButton->getHeight(), false, true);
            minusButton->setName("-", minusButton->getWidth()/2 - 3, -3);
            plusButton->setName("+", plusButton->getWidth()/2 - 3, -3);
        }
        plusButton->show();
        minusButton->show();
        
        plusButton->setColor(s_c);
        minusButton->setColor(s_c);
    }
    
    void update(){ }
    
    void hideButtons() {
        plusButton->hide();
        minusButton->hide();
    }
    
    void mouseDragged(int x, int y){
        setButtonPos(x, y);
    }
    
    void mousePressed(int x, int y){
        setButtonPos(x, y);
        
        if (useCountButton) {
            minusButton->mousePressed(x, y);
            plusButton->mousePressed(x, y);
            
            if (minusButton->getIsSelect()) {
                d_v -= m_v;
                if (d_v <= min_v) d_v = min_v;
                v = ofMap(d_v, min_v, max_v, s_rect.x, s_rect.x + s_rect.getWidth() - sliderButton->getWidth());
                sliderButton->setPos(v, sliderButton->getY());
            }else if (plusButton->getIsSelect()){
                d_v += m_v;
                if (d_v >= max_v) d_v = max_v;
                v = ofMap(d_v, min_v, max_v, s_rect.x, s_rect.x + s_rect.getWidth() - sliderButton->getWidth());
                sliderButton->setPos(v, sliderButton->getY());
            }
        }
    }
    
    void mouseReleased(int x, int y){
        
    }
    
    void mouseMoved(int x, int y){
        
    }
    
    void setButtonPos(int x, int y){
        if (y >= s_rect.y && y <= s_rect.y + s_rect.getHeight() && x >= s_rect.x && x <= s_rect.x + s_rect.getWidth()) {
            v = x - sliderButton->getWidth()/2;
            if (v <= s_rect.x) v = s_rect.x;
            if (v >= s_rect.x + s_rect.getWidth() - sliderButton->getWidth()) v = s_rect.x + s_rect.getWidth() - sliderButton->getWidth();
            d_v = ofMap(v, s_rect.x, s_rect.x + s_rect.getWidth() - sliderButton->getWidth(), min_v, max_v);
            sliderButton->setPos(v, sliderButton->getY());
        }
    }
    
    void render(ofEventArgs &event) { render(); }
    void mousePressed(ofMouseEventArgs &mouse) { mousePressed(mouse.x, mouse.y); }
    void mouseDragged(ofMouseEventArgs &mouse) { mouseDragged(mouse.x, mouse.y); }
    void mouseReleased(ofMouseEventArgs &mouse) { mouseReleased(mouse.x, mouse.y); }
    void mouseMoved(ofMouseEventArgs &mouse) { mouseMoved(mouse.x, mouse.y); }
    
    void touchDown(ofTouchEventArgs &touch) { }
    void touchUp(ofTouchEventArgs &touch) { }
    void touchMoved(ofTouchEventArgs &touch) { }
    void touchDoubleTap(ofTouchEventArgs &touch) { }
    
    void render(){
        
        sliderButton->render();
        
        ofPushStyle();
        ofSetColor(s_c);
		ofFill();
        //ofRect(s_rect);
        ofDrawRectangle(s_rect);
        ofPopStyle();
        
        if(useCountButton){
            plusButton->render();
            minusButton->render();
        }
        if (!hideValue) {
            ofPushStyle();
            ofSetColor(s_c);
            ofDrawBitmapString(s_name + " "+ofToString(d_v), s_rect.x, s_rect.y - 2);
            ofPopStyle();
        }
    }
    
    float getValue() { return d_v; }
    
};

