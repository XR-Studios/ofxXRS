#include "ofxXRSSimpleButton.h"

//--------------------------------------------------------------
/*public */ofxXRSSimpleButton::ofxXRSSimpleButton()
:useName(false)
,useValue(false)
,bRender(false)
,bAppear(false)
,fixPos(true)
,bSelect(false)
,bToggle(false)
,bDoubleTab(false)
,bAnimation(false)
,bang(false)
,startAni(false)
,aniTime(0)
,changeTime(500)
,alpha(255)
,b_c(ofColor::white)
,b_t_c(ofColor::yellow)
,b_img_extension(".png")
{}

//--------------------------------------------------------------
/*public */ofxXRSSimpleButton::~ofxXRSSimpleButton(){}

//--------------------------------------------------------------
/*public */bool ofxXRSSimpleButton::setup(float x, float y, float w, float h, bool useEvent){
    
    setType(TYPE_BUTTON); //default type is button
    setShape(BUTTON_RECT); //defalut shape is rectangle
    
    b_rect.set(x, y, w, h); //set rectangle position and size
    
    if (w > 0 && h > 0 && !bAppear) {
        setAppear(true);
        setRender(true);
        if (useEvent) {
            if (ofGetTargetPlatform() == OF_TARGET_IOS || ofGetTargetPlatform() == OF_TARGET_ANDROID) {
                ofAddListener(ofEvents().touchDown, this, &ofxXRSSimpleButton::touchDown);
                ofAddListener(ofEvents().touchUp, this, &ofxXRSSimpleButton::touchUp);
                ofAddListener(ofEvents().touchMoved, this, &ofxXRSSimpleButton::touchMoved);
                ofAddListener(ofEvents().touchDoubleTap, this, &ofxXRSSimpleButton::touchDoubleTap);
            }else{
                ofAddListener(ofEvents().mousePressed , this, &ofxXRSSimpleButton::mousePressed);
                ofAddListener(ofEvents().mouseReleased, this, &ofxXRSSimpleButton::mouseReleased);
                ofAddListener(ofEvents().mouseMoved, this, &ofxXRSSimpleButton::mouseMoved);
                ofAddListener(ofEvents().mouseDragged, this, &ofxXRSSimpleButton::mouseDragged);
            }
            ofAddListener(ofEvents().draw, this, &ofxXRSSimpleButton::render);
        }
        return bAppear;
    }
    ofLog(OF_LOG_ERROR, "Button's size is not correct. Button couldn't setup.");
    return bAppear;
}

//--------------------------------------------------------------
/*public */bool ofxXRSSimpleButton::setup(float x, float y, float w, float h, bool useEvent, bool manualRender, TYPE_BUTTONS type, BUTTON_SHAPES shape, const ofColor &c)
{
    setType(type);
    setShape(shape);
    
    b_rect.set(x, y, w, h);
    
    if (w > 0 && h > 0 && !bAppear) {
        setAppear(true);
        b_c = c;
        if (useEvent) {
            if (ofGetTargetPlatform() == OF_TARGET_IOS || ofGetTargetPlatform() == OF_TARGET_ANDROID) {
                ofAddListener(ofEvents().touchDown, this, &ofxXRSSimpleButton::touchDown);
                ofAddListener(ofEvents().touchMoved, this, &ofxXRSSimpleButton::touchMoved);
                ofAddListener(ofEvents().touchDoubleTap, this, &ofxXRSSimpleButton::touchDoubleTap);
                ofAddListener(ofEvents().touchUp, this, &ofxXRSSimpleButton::touchUp);
            }else{
                ofAddListener(ofEvents().mousePressed , this, &ofxXRSSimpleButton::mousePressed);
                ofAddListener(ofEvents().mouseReleased, this, &ofxXRSSimpleButton::mouseReleased);
                ofAddListener(ofEvents().mouseMoved, this, &ofxXRSSimpleButton::mouseMoved);
                ofAddListener(ofEvents().mouseDragged, this, &ofxXRSSimpleButton::mouseDragged);
            }
        }

        if (!manualRender) {
			setRender(true);
            ofAddListener(ofEvents().draw, this, &ofxXRSSimpleButton::render);
        }
        
        return bAppear;
    }
    
    ofLog(OF_LOG_ERROR, "Button's size is not correct. Button couldn't setup.");
    return bAppear;
}

//--------------------------------------------------------------
/*public */bool ofxXRSSimpleButton::setup(float x, float y, std::string imgRoute, bool useAnimation, bool useEvent, bool manualRender, TYPE_BUTTONS type)
{
    setType(type);
    setShape(BUTTON_IMAGE);
    b_img_fbo.clear();
    
    checkExtension(imgRoute);
    
    ofImage b;
    
    if(b.load(b_img_route + b_img_extension)) {
        
        ofFbo img_fbo;
        img_fbo.allocate(b.getWidth(), b.getHeight());
        
        img_fbo.begin();
        ofClear(255,0);
        img_fbo.end();
        
        img_fbo.begin();
        b.draw(0, 0);
        img_fbo.end();
        
        b_img_fbo.push_back(img_fbo);
    }else {
        ofLog(OF_LOG_ERROR, "couldn't load image");
        return false;
    }
    
    
    if(useAnimation){
        if(b.load(b_img_route + "_" + b_img_extension)){
            
            ofFbo img_fbo;
            img_fbo.allocate(b.getWidth(), b.getHeight());
            img_fbo.begin();
            ofClear(255,0);
            img_fbo.end();
            
            img_fbo.begin();
            b.draw(0, 0);
            img_fbo.end();
            
            b_img_fbo.push_back(img_fbo);
        }
    }
    
    
    
    if (b_img_fbo.size() > 0 && !bAppear) {
        setAppear(true);
        float w = b_img_fbo[0].getWidth();
        float h = b_img_fbo[0].getHeight();
        b_rect.set(x, y, w, h);
        
        if (useEvent) {
            if (ofGetTargetPlatform() == OF_TARGET_IOS || ofGetTargetPlatform() == OF_TARGET_ANDROID) {
                ofAddListener(ofEvents().touchDown, this, &ofxXRSSimpleButton::touchDown);
                ofAddListener(ofEvents().touchMoved, this, &ofxXRSSimpleButton::touchMoved);
                ofAddListener(ofEvents().touchDoubleTap, this, &ofxXRSSimpleButton::touchDoubleTap);
                ofAddListener(ofEvents().touchUp, this, &ofxXRSSimpleButton::touchUp);
            }else{
                ofAddListener(ofEvents().mousePressed , this, &ofxXRSSimpleButton::mousePressed);
                ofAddListener(ofEvents().mouseReleased, this, &ofxXRSSimpleButton::mouseReleased);
                ofAddListener(ofEvents().mouseMoved, this, &ofxXRSSimpleButton::mouseMoved);
                ofAddListener(ofEvents().mouseDragged, this, &ofxXRSSimpleButton::mouseDragged);
            }
        }
        
        if (!manualRender) {
            setRender(true);
            ofAddListener(ofEvents().draw, this, &ofxXRSSimpleButton::render);
        }
        
        return bAppear;
    }
    
    ofLog(OF_LOG_ERROR, "Button couldn't setup.");
    return bAppear;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setAppear(bool show)
{
    bAppear = show;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setRender(bool magic)
{
    bRender = magic;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setType(TYPE_BUTTONS type)
{
    b_type = type;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setShape(BUTTON_SHAPES shape)
{
    b_shape = shape;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setColor(const ofColor &c)
{
    b_c = c;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setToggleColor(const ofColor &c)
{
    b_t_c = c;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setAlpha(int a){
    alpha = a;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setAlpha(float a){
    int result = (int)ofMap(a, 0.0, 1.0, 0., 255.);
    setAlpha(result);
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setPos(const ofPoint &p)
{
    setPos(p.x, p.y);
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setPos(float x, float y){
    b_rect.x = x;
    b_rect.y = y;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setName(std::string n, float n_x, float n_y){
    if (!bAppear) {
        ofLog(OF_LOG_ERROR, "Button is not initialized. First you need to setup.");
        return;
    }
    b_info_text = n;
    useName = true;
    n_pos.set(n_x, n_y);
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setValue(std::string v, float v_x, float v_y){
    if (!bAppear) {
        ofLog(OF_LOG_ERROR, "Button is not initialized. First you need to setup.");
        return;
    }
    b_val_text = v;
    useValue = true;
    v_pos.set(v_x, v_y);
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setFixPos(bool fix)
{
    fixPos = fix;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::setAsAnimationButton(int time){
    
    ofImage a;
    if(!a.load(b_img_route+"__"+b_img_extension)) return;
    
    a_img_fbo.allocate(a.getWidth(), a.getHeight());
    
    a_img_fbo.begin();
    ofClear(255, 0);
    a_img_fbo.end();
    
    a_img_fbo.begin();
    a.draw(0, 0);
    a_img_fbo.end();
    
    bAnimation = true;
    ofAddListener(ofEvents().update, this, &ofxXRSSimpleButton::update);
    
    aniTime = ofGetElapsedTimeMillis();
    changeTime = time;
    
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::resetToggle()
{
    bToggle = false;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::render(){
    
    if (bRender) {
        
        ofPushMatrix();
        ofTranslate(b_rect.x, b_rect.y);
        
        if (b_img_fbo.size() > 0 && b_shape == BUTTON_IMAGE) {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255, alpha);
           
            if (b_img_fbo.size() == 2 && bToggle) { b_img_fbo[1].draw(0, 0); }
            else if (bAnimation && bang) { a_img_fbo.draw(0, 0); }
            else{ b_img_fbo[0].draw(0, 0); }
        
            ofDisableAlphaBlending();
            ofPopStyle();
        }else {
            ofPushStyle();
            ofFill();
            updateButtonColor();
            if (b_shape == BUTTON_CIRCLE) {
                ofDrawEllipse(b_rect.width/2, b_rect.height/2, b_rect.width, b_rect.height);
                //ofEllipse(b_rect.width/2, b_rect.height/2, b_rect.width, b_rect.height);
            }else if (b_shape == BUTTON_RECT){
                ofDrawRectangle(0, 0, b_rect.width, b_rect.height);
                //ofRect(0, 0, b_rect.width, b_rect.height);
            }else if (b_shape == BUTTON_CUSTOM){
                //using vertex?
            }
            ofPopStyle();
        }
        
        if (useName) {
            ofPushStyle();
            ofSetColor(b_c, alpha);
            ofDrawBitmapString(b_info_text, n_pos);
            ofPopStyle();
        }
        
        if (useValue) {
            ofPushStyle();
            ofSetColor(b_c, alpha);
            ofDrawBitmapString(b_val_text, v_pos);
            ofPopStyle();
        }
        ofPopMatrix();
    }
    
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::touchDown(float x, float y)
{
    if (isInside(x, y)) {
        buttonAction(true);
    }
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::touchUp(){
    if (b_type == TYPE_BUTTON) {
        bSelect = bToggle = false;
    }else if (b_type == TYPE_TOGGLE) {
        bSelect = false;
    }
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::touchMoved(float x, float y){
    if (!fixPos && bSelect) {
        setPos(x - b_rect.width/2, y - b_rect.height/2);
    }
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::touchDoubleTap(float x, float y){
    if (isInside(x, y)) { bDoubleTab = !bDoubleTab; }
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::mousePressed(float x, float y)
{
    if (isInside(x, y)) {
		ofxXRSButtonEvent e = ofxXRSButtonEvent(this);
		ofNotifyEvent(mousePressedEvent, this);
        buttonAction(true);
		if (buttonEventCallback != nullptr) {
			ofxXRSButtonEvent e(this);
			buttonEventCallback(e);
		} else {
			std::cout << "ofxXRSSimpleButton::butonEventCallback is null" << std::endl;
		}
    }
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::mouseReleased(){
    if (b_type == TYPE_BUTTON) { bSelect = bToggle = false; }
    else if (b_type == TYPE_TOGGLE) { bSelect = false; }
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::mouseMoved(float x, float y){
    if (!fixPos && bSelect) {
        setPos(x, y);
    }
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::mouseDragged(float x, float y){
    if (!fixPos && bSelect) {
        setPos(x, y);
    }
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::toggleShow(){
    bAppear = !bAppear;
    bRender = !bRender;
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::hide()
{
    bAppear = bRender = false;
    if (bAnimation) { resetAniTime(bAppear); }
}

//--------------------------------------------------------------
/*public */void ofxXRSSimpleButton::show()
{
    bAppear = bRender = true;
    if (bAnimation) { resetAniTime(bAppear); }
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::resetAniTime(bool start){
    startAni = start;
    bang = false;
    aniTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::update(ofEventArgs &event){
    if (startAni) {
        if (ofGetElapsedTimeMillis() - aniTime >= changeTime) {
            bang =! bang;
            aniTime = ofGetElapsedTimeMillis();
        }
    }
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::render(ofEventArgs &event)
{
    render();
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::touchDown(ofTouchEventArgs &touch)
{
    if (touch.id == 0 && isInside(touch.x, touch.y)) {
        buttonAction(true);
    }
    
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::touchUp(ofTouchEventArgs &touch)
{
    if (b_type == TYPE_BUTTON) {
        bSelect = bToggle = false;
    }else if (b_type == TYPE_TOGGLE) {
        bSelect = false;
    }
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::touchMoved(ofTouchEventArgs &touch){
    if (!fixPos && bSelect) {
        setPos(touch.x - b_rect.width/2, touch.y - b_rect.height/2);
    }
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::touchDoubleTap(ofTouchEventArgs &touch){
    if (isInside(touch.x, touch.y)) { bDoubleTab = !bDoubleTab; }
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::mousePressed(ofMouseEventArgs &mouse)
{
    mousePressed(mouse.x, mouse.y);
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::mouseReleased(ofMouseEventArgs &mouse)
{
    if (b_type == TYPE_BUTTON) { bSelect = bToggle = false; }
    else if (b_type == TYPE_TOGGLE) { bSelect = false; }
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::mouseMoved(ofMouseEventArgs &mouse){
    if (!fixPos && bSelect) {
        setPos(mouse.x, mouse.y);
    }
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::mouseDragged(ofMouseEventArgs &mouse){
    if (!fixPos && bSelect) {
        setPos(mouse.x, mouse.y);
    }
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::updateButtonColor(){

    if (bToggle) { ofSetColor(b_t_c, alpha); }
    else { ofSetColor(b_c, alpha); }
}

//--------------------------------------------------------------
/*protected */void ofxXRSSimpleButton::buttonAction(bool b){
    
    if (b_type == TYPE_BUTTON) {
        bToggle = true;
    }else if (b_type == TYPE_TOGGLE) {
        bToggle = !bToggle;
    }
}

//--------------------------------------------------------------
/*protected */int ofxXRSSimpleButton::checkExtension(std::string n){
    int result;
    int extensionFinder[3];
    
    std::string extensionType[] = { ".png", ".jpg", ".jpeg" };
    
    for(int i = 0; i < 3; i++){
        extensionFinder[i] = n.find(extensionType[i]);
        if(extensionFinder[i] != std::string::npos){
            result = i;
            b_img_extension = extensionType[i];
            b_img_route.append(n,0, extensionFinder[i]);
        }
    }
    return result;
}

//--------------------------------------------------------------
/*protected */bool ofxXRSSimpleButton::isInside(float x, float y)
{
    if (!bAppear) return false;
    return  bSelect = x>= b_rect.x && x <= b_rect.x + b_rect.width && y >= b_rect.y && y <= b_rect.y + b_rect.height ;
}
