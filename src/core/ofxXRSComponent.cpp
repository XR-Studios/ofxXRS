#include "ofxXRSComponent.h"

bool ofxXRSLog::mQuiet = false;
string ofxXRSTheme::AssetPath = "";
std::unique_ptr<ofxXRSTheme> ofxXRSComponent::theme;

ofxXRSComponent::ofxXRSComponent(string label)
{
    mName = label;
    mVisible = true;
    mEnabled = true;
    mFocused = false;
    mMouseOver = false;
    mMouseDown = false;
    mStyle.opacity = 255;
    this->x = 0; this->y = 0;
    mAnchor = ofxXRSPanelAnchor::NO_ANCHOR;
    mLabel.text = label;
    mLabel.alignment = ofxXRSAlignment::LEFT;
}

ofxXRSComponent::~ofxXRSComponent()
{
//  cout << "ofxXRSComponent "<< mName << " destroyed" << endl;
    ofRemoveListener(ofEvents().keyPressed, this, &ofxXRSComponent::onKeyPressed);
    ofRemoveListener(ofEvents().windowResized, this, &ofxXRSComponent::onWindowResized);
}

/*
    instance getters & setters
*/


void ofxXRSComponent::setIndex(int index)
{
    mIndex = index;
}

int ofxXRSComponent::getIndex()
{
    return mIndex;
}

void ofxXRSComponent::setName(string name)
{
    mName = name;
}

string ofxXRSComponent::getName()
{
    return mName;
}

bool ofxXRSComponent::is(string name)
{
    return ofToLower(mName) == ofToLower(name);
}

ofxXRSType ofxXRSComponent::getType()
{
    return mType;
}

const ofxXRSTheme* ofxXRSComponent::getTheme()
{
    if (theme == nullptr) theme = make_unique<ofxXRSTheme>(true);
    return theme.get();
}

void ofxXRSComponent::setComponentStyle(const ofxXRSTheme* theme)
{
    mStyle.height = theme->layout.height;
    mStyle.padding = theme->layout.padding;
    mStyle.vMargin = theme->layout.vMargin;
    mStyle.color.background = theme->color.background;
    mStyle.color.inputArea = theme->color.inputAreaBackground;
    mStyle.color.onMouseOver = theme->color.backgroundOnMouseOver;
    mStyle.color.onMouseDown = theme->color.backgroundOnMouseDown;
    mStyle.stripe.width = theme->stripe.width;
    mStyle.stripe.visible = theme->stripe.visible;
    mStyle.border.width = theme->border.width;
    mStyle.border.color = theme->border.color;
    mStyle.border.visible = theme->border.visible;
    mStyle.guiBackground = theme->color.guiBackground;
    mFont = theme->font.ptr;
    mIcon.y = mStyle.height * .33;
    mIcon.color = theme->color.icons;
    mIcon.size = theme->layout.iconSize;
    mLabel.color = theme->color.label;
    mLabel.margin = theme->layout.labelMargin;
    mLabel.forceUpperCase = theme->layout.upperCaseLabels;
    setLabel(mLabel.text);
    setWidth(theme->layout.width, theme->layout.labelWidth);
    for (size_t i=0; i<children.size(); i++) children[i]->setTheme(theme);
}

void ofxXRSComponent::setWidth(int width, float labelWidth)
{
    mStyle.width = width;
    if (labelWidth > 1){
// we received a pixel value //
        mLabel.width = labelWidth;
    }   else{
// we received a percentage //
        mLabel.width = mStyle.width * labelWidth;
    }
    mIcon.x = mStyle.width - (mStyle.width * .05) - mIcon.size;
    mLabel.rightAlignedXpos = mLabel.width - mLabel.margin;
    for (size_t i=0; i<children.size(); i++) children[i]->setWidth(width, labelWidth);
    positionLabel();
}

int ofxXRSComponent::getWidth()
{
    return mStyle.width;
}

int ofxXRSComponent::getHeight()
{
    return mStyle.height;
}

int ofxXRSComponent::getX()
{
    return this->x;
}

int ofxXRSComponent::getY()
{
    return this->y;
}

void ofxXRSComponent::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
    for(size_t i=0; i<children.size(); i++) children[i]->setPosition(x, this->y + (mStyle.height+mStyle.vMargin)*(i+1));
}

void ofxXRSComponent::setVisible(bool visible)
{
    mVisible = visible;
    if (internalEventCallback != nullptr){
        ofxXRSInternalEvent e(ofxXRSEventType::VISIBILITY_CHANGED, mIndex);
        internalEventCallback(e);
    }
}

bool ofxXRSComponent::getVisible()
{
    return mVisible;
}

void ofxXRSComponent::setOpacity(float opacity)
{
    mStyle.opacity = opacity * 255;
    for (size_t i=0; i<children.size(); i++) children[i]->setOpacity(opacity);
}

void ofxXRSComponent::setEnabled(bool enabled)
{
    mEnabled = enabled;
}

bool ofxXRSComponent::getEnabled()
{
    return mEnabled;
}

void ofxXRSComponent::setFocused(bool focused)
{
    if (focused){
        onFocus();
    }   else{
        onFocusLost();
    }
}

bool ofxXRSComponent::getFocused()
{
    return mFocused;
}

bool ofxXRSComponent::getMouseDown()
{
    return mMouseDown;
}

void ofxXRSComponent::setMask(const ofRectangle &mask)
{
    mMask = mask;
}

void ofxXRSComponent::setAnchor(ofxXRSPanelAnchor anchor)
{
    mAnchor = anchor;
    if (mAnchor != ofxXRSPanelAnchor::NO_ANCHOR){
        ofAddListener(ofEvents().windowResized, this, &ofxXRSComponent::onWindowResized);
    }   else{
        ofRemoveListener(ofEvents().windowResized, this, &ofxXRSComponent::onWindowResized);
    }
    onWindowResized();
}

bool ofxXRSComponent::getIsExpanded()
{
	return false;
}

/*
    component label
*/

void ofxXRSComponent::setLabel(string label)
{
    mLabel.text = label;
    mLabel.rendered = mLabel.forceUpperCase ? ofToUpper(mLabel.text) : mLabel.rendered = mLabel.text;
    mLabel.rect = mFont->rect(mLabel.rendered);
    positionLabel();
}

string ofxXRSComponent::getLabel()
{
    return mLabel.text;
}

void ofxXRSComponent::setLabelColor(ofColor c)
{
    mLabel.color = c;
}

ofColor ofxXRSComponent::getLabelColor()
{
    return mLabel.color;
}

void ofxXRSComponent::setLabelUpperCase(bool toUpper)
{
    mLabel.forceUpperCase = toUpper;
    setLabel(mLabel.text);
}

bool ofxXRSComponent::getLabelUpperCase()
{
    return mLabel.forceUpperCase;
}

void ofxXRSComponent::setLabelAlignment(ofxXRSAlignment align)
{
    mLabel.alignment = align;
    for (size_t i=0; i<children.size(); i++) children[i]->setLabelAlignment(align);
    positionLabel();
}

void ofxXRSComponent::positionLabel()
{
    if (mLabel.alignment == ofxXRSAlignment::LEFT){
        mLabel.x = mLabel.margin;
    }   else if (mLabel.alignment == ofxXRSAlignment::CENTER){
        mLabel.x = (mLabel.width / 2) - (mLabel.rect.width / 2);
    }   else if (mLabel.alignment == ofxXRSAlignment::RIGHT){
        mLabel.x = mLabel.rightAlignedXpos - mLabel.rect.width;
    }
}

/*
    visual customization
*/

void ofxXRSComponent::setBackgroundColor(ofColor color)
{
    mStyle.color.background = color;
}

void ofxXRSComponent::setBackgroundColorOnMouseOver(ofColor color)
{
    mStyle.color.onMouseOver = color;
}

void ofxXRSComponent::setBackgroundColorOnMouseDown(ofColor color)
{
    mStyle.color.onMouseDown = color;
}

void ofxXRSComponent::setBackgroundColors(ofColor c1, ofColor c2, ofColor c3)
{
    mStyle.color.background = c1;
    mStyle.color.onMouseOver = c2;
    mStyle.color.onMouseDown = c3;
}

void ofxXRSComponent::setStripe(ofColor color, int width)
{
    mStyle.stripe.color = color;
    mStyle.stripe.width = width;
}

void ofxXRSComponent::setStripeColor(ofColor color)
{
    mStyle.stripe.color = color;
}

void ofxXRSComponent::setStripeWidth(int width)
{
    mStyle.stripe.width = width;
}

void ofxXRSComponent::setStripeVisible(bool visible)
{
    mStyle.stripe.visible = visible;
}

void ofxXRSComponent::setBorder(ofColor color, int width)
{
    mStyle.border.color = color;
    mStyle.border.width = width;
    mStyle.border.visible = true;
}

void ofxXRSComponent::setBorderVisible(bool visible)
{
    mStyle.border.visible = visible;
}

/*
    draw methods
*/

void ofxXRSComponent::update(bool acceptEvents)
{
    if (acceptEvents && mEnabled && mVisible){
        bool mp = ofGetMousePressed();
        ofPoint mouse = ofPoint(ofGetMouseX() - mMask.x, ofGetMouseY() - mMask.y);
        if (hitTest(mouse)){
            if (!mMouseOver){
                onMouseEnter(mouse);
            }
            if (!mMouseDown && mp){
                onMousePress(mouse);
                if (!mFocused) onFocus();
            }
        }   else{
    // the mouse is not over the component //
            if (mMouseOver){
                onMouseLeave(mouse);
            }
            if (!mMouseDown && mp && mFocused){
                onFocusLost();
            }
        }
        if (mMouseDown) {
            if (mp){
                onMouseDrag(mouse);
            }   else{
                onMouseRelease(mouse);
            }
        }
    }
// don't update children unless they're visible //
    if (this->getIsExpanded()) {
        for(size_t i=0; i<children.size(); i++) {
            children[i]->update(acceptEvents);
            if (children[i]->getFocused()){
                if (acceptEvents == false ) children[i]->setFocused(false);
                acceptEvents = false;
            }
        }
    }
}

void ofxXRSComponent::draw()
{
    ofPushStyle();
        if (mStyle.border.visible) drawBorder();
        drawBackground();
        drawLabel();
        if (mStyle.stripe.visible) drawStripe();
    ofPopStyle();
}

void ofxXRSComponent::drawBackground()
{
    ofFill();
    ofSetColor(mStyle.color.background, mStyle.opacity);
    ofDrawRectangle(x, y, mStyle.width, mStyle.height);
}

void ofxXRSComponent::drawLabel()
{
    ofSetColor(mLabel.color);
    if (mType != ofxXRSType::DROPDOWN_OPTION){
        mFont->draw(mLabel.rendered, x+mLabel.x, y+mStyle.height/2 + mLabel.rect.height/2);
    }   else{
        mFont->draw("* "+mLabel.rendered, x+mLabel.x, y+mStyle.height/2 + mLabel.rect.height/2);
    }
}

void ofxXRSComponent::drawStripe()
{
    ofSetColor(mStyle.stripe.color);
    ofDrawRectangle(x, y, mStyle.stripe.width, mStyle.height);
}

void ofxXRSComponent::drawBorder()
{
    ofFill();
    int w = mStyle.border.width;
    ofSetColor(mStyle.border.color, mStyle.opacity);
    ofDrawRectangle(x-w, y-w, mStyle.width+(w*2), mStyle.height+(w*2));
}

void ofxXRSComponent::drawColorPicker() { }

/*
    events
*/

bool ofxXRSComponent::hitTest(ofPoint m)
{
    if (mMask.height > 0 && (m.y < 0 || m.y > mMask.height)) return false;
    return (m.x>=x && m.x<= x+mStyle.width && m.y>=y && m.y<= y+mStyle.height);
}

void ofxXRSComponent::onMouseEnter(ofPoint m)
{
    mMouseOver = true;
}

void ofxXRSComponent::onMouseLeave(ofPoint m)
{
    mMouseOver = false;
}

void ofxXRSComponent::onMousePress(ofPoint m)
{
    mMouseDown = true;
}

void ofxXRSComponent::onMouseRelease(ofPoint m)
{
    mMouseDown = false;
}

void ofxXRSComponent::onFocus()
{
    mFocused = true;
    ofAddListener(ofEvents().keyPressed, this, &ofxXRSComponent::onKeyPressed);
}

void ofxXRSComponent::onFocusLost()
{
    mFocused = false;
    mMouseDown = false;
    ofRemoveListener(ofEvents().keyPressed, this, &ofxXRSComponent::onKeyPressed);
}

void ofxXRSComponent::onKeyPressed(int key) { }
void ofxXRSComponent::onMouseDrag(ofPoint m) { }

void ofxXRSComponent::onKeyPressed(ofKeyEventArgs &e)
{
    onKeyPressed(e.key);
    if ((e.key == OF_KEY_RETURN || e.key == OF_KEY_TAB)){
        onFocusLost();
        ofRemoveListener(ofEvents().keyPressed, this, &ofxXRSComponent::onKeyPressed);
    }
}

void ofxXRSComponent::onWindowResized()
{
    if (mAnchor == ofxXRSPanelAnchor::TOP_LEFT){
        setPosition(0, 0);
    }   else if (mAnchor == ofxXRSPanelAnchor::TOP_RIGHT){
        setPosition(ofGetWidth()-mStyle.width, 0);
    }
}

void ofxXRSComponent::onWindowResized(ofResizeEventArgs &e)
{
	onWindowResized();
}


