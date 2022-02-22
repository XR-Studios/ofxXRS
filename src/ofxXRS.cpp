#include "ofxXRS.h"

ofxXRSPanel* ofxXRSPanel::mActiveGui;
std::vector<ofxXRSPanel*> ofxXRSPanel::mGuis;

ofxXRSPanel::ofxXRSPanel(int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
    mAnchor = ofxXRSPanelAnchor::NO_ANCHOR;
    init();
}

ofxXRSPanel::ofxXRSPanel(float x, float y) {
	mPosition.x = x;
	mPosition.y = y;
	mAnchor = ofxXRSPanelAnchor::NO_ANCHOR;
	init();
}

ofxXRSPanel::ofxXRSPanel(ofxXRSPanelAnchor anchor)
{
    init();
    mAnchor = anchor;
}

ofxXRSPanel::~ofxXRSPanel()
{
    for (auto i:items) delete i;
    mGuis.erase(std::remove(mGuis.begin(), mGuis.end(), this), mGuis.end());
    if (mActiveGui == this) mActiveGui = mGuis.size() > 0 ? mGuis[0] : nullptr;
    ofRemoveListener(ofEvents().draw, this, &ofxXRSPanel::onDraw, OF_EVENT_ORDER_AFTER_APP + mIndex);
    ofRemoveListener(ofEvents().update, this, &ofxXRSPanel::onUpdate, OF_EVENT_ORDER_BEFORE_APP - mIndex);
    ofRemoveListener(ofEvents().windowResized, this, &ofxXRSPanel::onWindowResized, OF_EVENT_ORDER_BEFORE_APP);
}

ofRectangle ofxXRSPanel::getRect() {
	return mGuiBounds;
}

void ofxXRSPanel::init()
{
    mMoving = false;
    mVisible = true;
    mEnabled = true;
    mExpanded = true;
    mGuiHeader = nullptr;
    mGuiFooter = nullptr;
    mAlphaChanged = false;
    mWidthChanged = false;
    mThemeChanged = false;
    mAlignmentChanged = false;
    mAlignment = ofxXRSAlignment::LEFT;
    mAlpha = 1.0f;
    mWidth = ofxXRSComponent::getTheme()->layout.width;
    mRowSpacing = ofxXRSComponent::getTheme()->layout.vMargin;
    mGuiBackground = ofxXRSComponent::getTheme()->color.guiBackground;
    
// enable autodraw by default //
    setAutoDraw(true, mGuis.size());
    
// assign focus to this newly created gui //
    mActiveGui = this;
    mGuis.push_back(this);
    ofAddListener(ofEvents().windowResized, this, &ofxXRSPanel::onWindowResized, OF_EVENT_ORDER_BEFORE_APP);
}

/* 
    public api
*/

void ofxXRSPanel::focus()
{
    if (mActiveGui!= this){
    // enable and make visible if hidden //
        mVisible = true;
        mEnabled = true;
        mActiveGui = this;
    // update the draw order //
        for (size_t i=0; i<mGuis.size(); i++) {
            if (mGuis[i] == mActiveGui) {
                std::swap(mGuis[i], mGuis[mGuis.size()-1]);
                break;
            }
        }
        for (size_t i=0; i<mGuis.size(); i++) {
            if (mGuis[i]->getAutoDraw()) mGuis[i]->setAutoDraw(true, i);
        }
    }
}

void ofxXRSPanel::expand()
{
    if (mGuiFooter != nullptr){
        mExpanded = true;
        mGuiFooter->setExpanded(mExpanded);
        mGuiFooter->setPosition(mPosition.x, mPosition.y + mHeight - mGuiFooter->getHeight() - mRowSpacing);
    }
}

void ofxXRSPanel::collapse()
{
    if (mGuiFooter != nullptr){
        mExpanded = false;
        mGuiFooter->setExpanded(mExpanded);
        mGuiFooter->setPosition(mPosition.x, mPosition.y);
    }
}

void ofxXRSPanel::toggle()
{
    mExpanded ? collapse() : expand();
}

bool ofxXRSPanel::getVisible()
{
    return mVisible;
}

bool ofxXRSPanel::getFocused()
{
    return mActiveGui == this;
}

void ofxXRSPanel::setHeight(int height, float labelHeight) {
	mHeight = height;
	mWidthChanged = true;
	if (mAnchor != ofxXRSPanelAnchor::NO_ANCHOR) positionGui();
}

void ofxXRSPanel::setWidth(int width, float labelWidth)
{
    mWidth = width;
    mLabelWidth = labelWidth;
    mWidthChanged = true;
    if (mAnchor != ofxXRSPanelAnchor::NO_ANCHOR) positionGui();
}

void ofxXRSPanel::setTheme(ofxXRSTheme* t, bool applyImmediately)
{
    if (applyImmediately){
        for(auto item:items) item->setTheme(t);
    }   else{
    // apply on next update call //
        mTheme = t;
        mThemeChanged = true;
    }
    mRowSpacing = t->layout.vMargin;
    mGuiBackground = t->color.guiBackground;
    setWidth(t->layout.width, t->layout.labelWidth);
}

void ofxXRSPanel::setOpacity(float opacity)
{
    mAlpha = opacity;
    mAlphaChanged = true;
}

void ofxXRSPanel::setPosition(int x, int y)
{
    moveGui(ofPoint(x, y));
}

void ofxXRSPanel::setPosition(ofxXRSPanelAnchor anchor)
{
    mAnchor = anchor;
    if (mAnchor != ofxXRSPanelAnchor::NO_ANCHOR) positionGui();
}

void ofxXRSPanel::setVisible(bool visible)
{
    mVisible = visible;
}

void ofxXRSPanel::setEnabled(bool enabled)
{
    mEnabled = enabled;
}

void ofxXRSPanel::setAutoDraw(bool autodraw, int priority)
{
    mAutoDraw = autodraw;
    ofRemoveListener(ofEvents().draw, this, &ofxXRSPanel::onDraw, OF_EVENT_ORDER_AFTER_APP + mIndex);
    ofRemoveListener(ofEvents().update, this, &ofxXRSPanel::onUpdate, OF_EVENT_ORDER_BEFORE_APP - mIndex);
    if (mAutoDraw){
        mIndex = priority;
        ofAddListener(ofEvents().draw, this, &ofxXRSPanel::onDraw, OF_EVENT_ORDER_AFTER_APP + mIndex);
        ofAddListener(ofEvents().update, this, &ofxXRSPanel::onUpdate, OF_EVENT_ORDER_BEFORE_APP - mIndex);
    }
}

bool ofxXRSPanel::getAutoDraw()
{
    return mAutoDraw;
}

bool ofxXRSPanel::getMouseDown()
{
    return mMouseDown;
}

void ofxXRSPanel::setLabelAlignment(ofxXRSAlignment align)
{
    mAlignment = align;
    mAlignmentChanged = true;
}

int ofxXRSPanel::getWidth()
{
    return mWidth;
}

int ofxXRSPanel::getHeight()
{
    return mHeight;
}

ofPoint ofxXRSPanel::getPosition()
{
    return ofPoint(mPosition.x, mPosition.y);
}

void ofxXRSPanel::setAssetPath(std::string path)
{
    ofxXRSTheme::AssetPath = path;
}

std::string ofxXRSPanel::getAssetPath()
{
    return ofxXRSTheme::AssetPath;
}

/* 
    add component methods
*/

ofxXRSHeader* ofxXRSPanel::addHeader(std::string label, bool draggable)
{
    if (mGuiHeader == nullptr){
        mGuiHeader = new ofxXRSHeader(label, draggable);
        if (items.size() == 0){
            items.push_back(mGuiHeader);
        }   else{
    // always ensure header is at the top of the panel //
            items.insert(items.begin(), mGuiHeader);
        }
        layoutGui();
	}
    return mGuiHeader;
}

ofxXRSFooter* ofxXRSPanel::addFooter()
{
    if (mGuiFooter == nullptr){
        mGuiFooter = new ofxXRSFooter();
        items.push_back(mGuiFooter);
        mGuiFooter->onInternalEvent(this, &ofxXRSPanel::onInternalEventCallback);
        layoutGui();
	}
    return mGuiFooter;
}

ofxXRSLabel* ofxXRSPanel::addLabel(std::string label)
{
    ofxXRSLabel* lbl = new ofxXRSLabel(label);
    attachItem(lbl);
    return lbl;
}

ofxXRSButton* ofxXRSPanel::addButton(std::string label)
{
    ofxXRSButton* button = new ofxXRSButton(label);
    button->onButtonEvent(this, &ofxXRSPanel::onButtonEventCallback);
    attachItem(button);
    return button;
}

ofxXRSToggle* ofxXRSPanel::addToggle(std::string label, bool enabled)
{
    ofxXRSToggle* button = new ofxXRSToggle(label, enabled);
    button->onToggleEvent(this, &ofxXRSPanel::onToggleEventCallback);
    attachItem(button);
    return button;
}

ofxXRSSlider* ofxXRSPanel::addSlider(ofParameter<int>& p)
{
    ofxXRSSlider* slider = new ofxXRSSlider(p);
    slider->onSliderEvent(this, &ofxXRSPanel::onSliderEventCallback);
    attachItem(slider);
    return slider;
}

ofxXRSSlider* ofxXRSPanel::addSlider(ofParameter<float>& p)
{
    ofxXRSSlider* slider = new ofxXRSSlider(p);
    slider->onSliderEvent(this, &ofxXRSPanel::onSliderEventCallback);
    attachItem(slider);
    return slider;
}

ofxXRSSlider* ofxXRSPanel::addSlider(std::string label, float min, float max)
{
// default to halfway between min & max values //
    ofxXRSSlider* slider = addSlider(label, min, max, (max+min)/2);
    return slider;
}

ofxXRSSlider* ofxXRSPanel::addSlider(std::string label, float min, float max, float val)
{
    ofxXRSSlider* slider = new ofxXRSSlider(label, min, max, val);
    slider->onSliderEvent(this, &ofxXRSPanel::onSliderEventCallback);
    attachItem(slider);
    return slider;
}

ofxXRSTextInput* ofxXRSPanel::addTextInput(std::string label, std::string value)
{
    ofxXRSTextInput* input = new ofxXRSTextInput(label, value);
    input->onTextInputEvent(this, &ofxXRSPanel::onTextInputEventCallback);
    attachItem(input);
    return input;
}

ofxXRSColorPicker* ofxXRSPanel::addColorPicker(std::string label, ofColor color)
{
    ofxXRSColorPicker* picker = new ofxXRSColorPicker(label, color);
    picker->onColorPickerEvent(this, &ofxXRSPanel::onColorPickerEventCallback);
    attachItem(picker);
    return picker;
}

ofxXRSWaveMonitor* ofxXRSPanel::addWaveMonitor(std::string label, float frequency, float amplitude)
{
    ofxXRSWaveMonitor* monitor = new ofxXRSWaveMonitor(label, frequency, amplitude);
    attachItem(monitor);
    return monitor;
}

ofxXRSValuePlotter* ofxXRSPanel::addValuePlotter(std::string label, float min, float max)
{
    ofxXRSValuePlotter* plotter = new ofxXRSValuePlotter(label, min, max);
    attachItem(plotter);
    return plotter;
}

ofxXRSDropdown* ofxXRSPanel::addDropdown(std::string label, std::vector<std::string> options)
{
    ofxXRSDropdown* dropdown = new ofxXRSDropdown(label, options);
    dropdown->onDropdownEvent(this, &ofxXRSPanel::onDropdownEventCallback);
    attachItem(dropdown);
    return dropdown;
}

ofxXRSFRM* ofxXRSPanel::addFRM(float refresh)
{
    ofxXRSFRM* monitor = new ofxXRSFRM(refresh);
    attachItem(monitor);
    return monitor;
}

ofxXRSBreak* ofxXRSPanel::addBreak()
{
    ofxXRSBreak* brk = new ofxXRSBreak();
    attachItem(brk);
    return brk;
}

ofxXRS2dPad* ofxXRSPanel::add2dPad(std::string label)
{
    ofxXRS2dPad* pad = new ofxXRS2dPad(label);
    pad->on2dPadEvent(this, &ofxXRSPanel::on2dPadEventCallback);
    attachItem(pad);
    return pad;
}

ofxXRS2dPad* ofxXRSPanel::add2dPad(std::string label, ofRectangle bounds)
{
    ofxXRS2dPad* pad = new ofxXRS2dPad(label, bounds);
    pad->on2dPadEvent(this, &ofxXRSPanel::on2dPadEventCallback);
    attachItem(pad);
    return pad;
}

ofxXRSMatrix* ofxXRSPanel::addMatrix(std::string label, int numButtons, bool showLabels)
{
    ofxXRSMatrix* matrix = new ofxXRSMatrix(label, numButtons, showLabels);
    matrix->onMatrixEvent(this, &ofxXRSPanel::onMatrixEventCallback);
    attachItem(matrix);
    return matrix;
}

ofxXRSFolder* ofxXRSPanel::addFolder(std::string label, ofColor color)
{
    ofxXRSFolder* folder = new ofxXRSFolder(label, color);
    folder->onButtonEvent(this, &ofxXRSPanel::onButtonEventCallback);
    folder->onToggleEvent(this, &ofxXRSPanel::onToggleEventCallback);
    folder->onSliderEvent(this, &ofxXRSPanel::onSliderEventCallback);
    folder->on2dPadEvent(this, &ofxXRSPanel::on2dPadEventCallback);
    folder->onMatrixEvent(this, &ofxXRSPanel::onMatrixEventCallback);
    folder->onTextInputEvent(this, &ofxXRSPanel::onTextInputEventCallback);
    folder->onColorPickerEvent(this, &ofxXRSPanel::onColorPickerEventCallback);
    folder->onInternalEvent(this, &ofxXRSPanel::onInternalEventCallback);
    attachItem(folder);
    return folder;
}

ofxXRSFolder* ofxXRSPanel::addFolder(ofxXRSFolder* folder)
{
    attachItem(folder);
    return folder;
}

void ofxXRSPanel::attachItem(ofxXRSComponent* item)
{
    if (mGuiFooter != nullptr){
        items.insert(items.end()-1, item);
    }   else {
        items.push_back( item );
    }
    item->onInternalEvent(this, &ofxXRSPanel::onInternalEventCallback);
    layoutGui();
}

/*
    component retrieval methods
*/

ofxXRSLabel* ofxXRSPanel::getLabel(std::string bl, std::string fl){
    ofxXRSLabel* o = nullptr;
    if (fl != ""){
        ofxXRSFolder* f = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
        if (f) o = static_cast<ofxXRSLabel*>(f->getComponent(ofxXRSType::LABEL, bl));
    } else {
        o = static_cast<ofxXRSLabel*>(getComponent(ofxXRSType::LABEL, bl));
    }
    if (o==nullptr){
        o = ofxXRSLabel::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl!="" ? fl+"-"+bl : bl);
        trash.push_back(o);
    }
    return o;
}

ofxXRSButton* ofxXRSPanel::getButton(std::string bl, std::string fl)
{
    ofxXRSButton* o = nullptr;
    if (fl != ""){
        ofxXRSFolder* f = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
        if (f) o = static_cast<ofxXRSButton*>(f->getComponent(ofxXRSType::BUTTON, bl));
    }   else{
        o = static_cast<ofxXRSButton*>(getComponent(ofxXRSType::BUTTON, bl));
    }
    if (o==nullptr){
        o = ofxXRSButton::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl!="" ? fl+"-"+bl : bl);
        trash.push_back(o);
    }
    return o;
}

ofxXRSToggle* ofxXRSPanel::getToggle(std::string bl, std::string fl)
{
    ofxXRSToggle* o = nullptr;
    if (fl != ""){
        ofxXRSFolder* f = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
        if (f) o = static_cast<ofxXRSToggle*>(f->getComponent(ofxXRSType::TOGGLE, bl));
    }   else{
        o = static_cast<ofxXRSToggle*>(getComponent(ofxXRSType::TOGGLE, bl));
    }
    if (o==nullptr){
        o = ofxXRSToggle::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl!="" ? fl+"-"+bl : bl);
        trash.push_back(o);
    }
    return o;
}

ofxXRSSlider* ofxXRSPanel::getSlider(std::string sl, std::string fl)
{
    ofxXRSSlider* o = nullptr;
    if (fl != ""){
        ofxXRSFolder* f = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
        if (f) o = static_cast<ofxXRSSlider*>(f->getComponent(ofxXRSType::SLIDER, sl));
    }   else{
        o = static_cast<ofxXRSSlider*>(getComponent(ofxXRSType::SLIDER, sl));
    }
    if (o==nullptr){
        o = ofxXRSSlider::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl!="" ? fl+"-"+sl : sl);
        trash.push_back(o);
    }
    return o;
}

ofxXRSTextInput* ofxXRSPanel::getTextInput(std::string tl, std::string fl)
{
    ofxXRSTextInput* o = nullptr;
    if (fl != ""){
        ofxXRSFolder* f = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
        if (f) o = static_cast<ofxXRSTextInput*>(f->getComponent(ofxXRSType::TEXT_INPUT, tl));
    }   else{
        o = static_cast<ofxXRSTextInput*>(getComponent(ofxXRSType::TEXT_INPUT, tl));
    }
    if (o==nullptr){
        o = ofxXRSTextInput::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl!="" ? fl+"-"+tl : tl);
        trash.push_back(o);
    }
    return o;
}

ofxXRS2dPad* ofxXRSPanel::get2dPad(std::string pl, std::string fl)
{
    ofxXRS2dPad* o = nullptr;
    if (fl != ""){
        ofxXRSFolder* f = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
        if (f) o = static_cast<ofxXRS2dPad*>(f->getComponent(ofxXRSType::PAD2D, pl));
    }   else{
        o = static_cast<ofxXRS2dPad*>(getComponent(ofxXRSType::PAD2D, pl));
    }
    if (o==nullptr){
        o = ofxXRS2dPad::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl!="" ? fl+"-"+pl : pl);
        trash.push_back(o);
    }
    return o;
}

ofxXRSColorPicker* ofxXRSPanel::getColorPicker(std::string cl, std::string fl)
{
    ofxXRSColorPicker* o = nullptr;
    if (fl != ""){
        ofxXRSFolder* f = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
        if (f) o = static_cast<ofxXRSColorPicker*>(f->getComponent(ofxXRSType::COLOR_PICKER, cl));
    }   else{
        o = static_cast<ofxXRSColorPicker*>(getComponent(ofxXRSType::COLOR_PICKER, cl));
    }
    if (o==nullptr){
        o = ofxXRSColorPicker::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl!="" ? fl+"-"+cl : cl);
        trash.push_back(o);
    }
    return o;
}

ofxXRSWaveMonitor* ofxXRSPanel::getWaveMonitor(std::string cl, std::string fl)
{
    ofxXRSWaveMonitor* o = nullptr;
    if (fl != ""){
        ofxXRSFolder* f = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
        if (f) o = static_cast<ofxXRSWaveMonitor*>(f->getComponent(ofxXRSType::WAVE_MONITOR, cl));
    }   else{
        o = static_cast<ofxXRSWaveMonitor*>(getComponent(ofxXRSType::WAVE_MONITOR, cl));
    }
    if (o==nullptr){
        o = ofxXRSWaveMonitor::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl!="" ? fl+"-"+cl : cl);
        trash.push_back(o);
    }
    return o;
}

ofxXRSValuePlotter* ofxXRSPanel::getValuePlotter(std::string cl, std::string fl)
{
    ofxXRSValuePlotter* o = nullptr;
    if (fl != ""){
        ofxXRSFolder* f = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
        if (f) o = static_cast<ofxXRSValuePlotter*>(f->getComponent(ofxXRSType::VALUE_PLOTTER, cl));
    }   else{
        o = static_cast<ofxXRSValuePlotter*>(getComponent(ofxXRSType::VALUE_PLOTTER, cl));
    }
    if (o==nullptr){
        o = ofxXRSValuePlotter::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl!="" ? fl+"-"+cl : cl);
        trash.push_back(o);
    }
    return o;
}

ofxXRSMatrix* ofxXRSPanel::getMatrix(std::string ml, std::string fl)
{
    ofxXRSMatrix* o = nullptr;
    if (fl != ""){
        ofxXRSFolder* f = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
        if (f) o = static_cast<ofxXRSMatrix*>(f->getComponent(ofxXRSType::MATRIX, ml));
    }   else{
        o = static_cast<ofxXRSMatrix*>(getComponent(ofxXRSType::MATRIX, ml));
    }
    if (o==nullptr){
        o = ofxXRSMatrix::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl!="" ? fl+"-"+ml : ml);
        trash.push_back(o);
    }
    return o;
}

ofxXRSDropdown* ofxXRSPanel::getDropdown(std::string dl)
{
    ofxXRSDropdown* o = static_cast<ofxXRSDropdown*>(getComponent(ofxXRSType::DROPDOWN, dl));
    if (o==NULL){
        o = ofxXRSDropdown::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, dl);
        trash.push_back(o);
    }
    return o;
}

ofxXRSFolder* ofxXRSPanel::getFolder(std::string fl)
{
    ofxXRSFolder* o = static_cast<ofxXRSFolder*>(getComponent(ofxXRSType::FOLDER, fl));
    if (o==NULL){
        o = ofxXRSFolder::getInstance();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, fl);
        trash.push_back(o);
    }
    return o;
}

ofxXRSHeader* ofxXRSPanel::getHeader()
{
    ofxXRSHeader* o;
    if (mGuiHeader != nullptr){
        o = mGuiHeader;
    }   else{
        o = new ofxXRSHeader("X");
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, "HEADER");
        trash.push_back(o);
    }
    return o;
}

ofxXRSFooter* ofxXRSPanel::getFooter()
{
    ofxXRSFooter* o;
    if (mGuiFooter != nullptr){
        o = mGuiFooter;
    }   else{
        o = new ofxXRSFooter();
        ofxXRSLog::write(ofxXRSMsg::COMPONENT_NOT_FOUND, "FOOTER");
        trash.push_back(o);
    }
    return o;
}

ofxXRSComponent* ofxXRSPanel::getComponent(ofxXRSType type, std::string label)
{
    for (size_t i=0; i<items.size(); i++) {
        if (items[i]->getType() == type){
            if (items[i]->is(label)) return items[i];
        }
    // iterate over component's children and return the first match we find //
        for (size_t j=0; j<items[i]->children.size(); j++) {
            if (items[i]->children[j]->is(label)) return items[i]->children[j];
        }
    }
    return NULL;
}

ofxXRSComponent* ofxXRSPanel::getComponent(std::string label) {
    for (size_t i = 0; i < items.size(); i++) {
        ofxXRSComponent* item = items[i];
        if(item->is(label)) return item;

        // iterate over item's children and return the first match we find //
        for (size_t j = 0; j < item->children.size(); j++) {
            if(item->children[j]->is(label)) return item->children[j];
        }
    }

    return NULL;
}

/*
    event callbacks
*/

void ofxXRSPanel::onButtonEventCallback(ofxXRSButtonEvent e)
{
    if (buttonEventCallback != nullptr) {
        buttonEventCallback(e);
    }   else{
        ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
    }
}

void ofxXRSPanel::onToggleEventCallback(ofxXRSToggleEvent e)
{
    if (toggleEventCallback != nullptr) {
        toggleEventCallback(e);
// allow toggle events to decay into button events //
    }   else if (buttonEventCallback != nullptr) {
        buttonEventCallback(ofxXRSButtonEvent(e.target));
    }   else{
        ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
    }
}

void ofxXRSPanel::onSliderEventCallback(ofxXRSSliderEvent e)
{
    if (sliderEventCallback != nullptr) {
        sliderEventCallback(e);
    }   else{
        ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
    }
}

void ofxXRSPanel::onTextInputEventCallback(ofxXRSTextInputEvent e)
{
    if (textInputEventCallback != nullptr) {
        textInputEventCallback(e);
    }   else{
        ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
    }
}

void ofxXRSPanel::onDropdownEventCallback(ofxXRSDropdownEvent e)
{
    if (dropdownEventCallback != nullptr) {
        dropdownEventCallback(e);
    }   else{
        ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
    }
// adjust the gui after a dropdown is closed //
    layoutGui();
}

void ofxXRSPanel::on2dPadEventCallback(ofxXRS2dPadEvent e)
{
    if (pad2dEventCallback != nullptr) {
        pad2dEventCallback(e);
    }   else{
        ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
    }
}

void ofxXRSPanel::onColorPickerEventCallback(ofxXRSColorPickerEvent e)
{
    if (colorPickerEventCallback != nullptr) {
        colorPickerEventCallback(e);
    }   else{
        ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
    }
}

void ofxXRSPanel::onMatrixEventCallback(ofxXRSMatrixEvent e)
{
    if (matrixEventCallback != nullptr) {
        matrixEventCallback(e);
    }   else{
        ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
    }
}

void ofxXRSPanel::onInternalEventCallback(ofxXRSInternalEvent e)
{
// these events are not dispatched out to the main application //
    if (e.type == ofxXRSEventType::GROUP_TOGGLED){
        layoutGui();
    }   else if (e.type == ofxXRSEventType::GUI_TOGGLED){
        mExpanded ? collapse() : expand();
    }   else if (e.type == ofxXRSEventType::VISIBILITY_CHANGED){
        layoutGui();
    }
}

/*
    layout, position, anchor and check for focus
*/

bool ofxXRSPanel::hitTest(ofPoint pt)
{
    if (mMoving){
        return true;
    }   else{
        return mGuiBounds.inside(pt);
    }
}

void ofxXRSPanel::moveGui(ofPoint pt)
{
    mPosition.x = pt.x;
    mPosition.y = pt.y;
    mAnchor = ofxXRSPanelAnchor::NO_ANCHOR;
    positionGui();
}

void ofxXRSPanel::layoutGui()
{
    mHeight = 0;
    for (size_t i=0; i<items.size(); i++) {
        items[i]->setIndex(i);
    // skip over any components that are currently invisible //
        if (items[i]->getVisible() == false) continue;
        mHeight += items[i]->getHeight() + mRowSpacing;
    }
    positionGui();
}

void ofxXRSPanel::positionGui()
{
/*
    ofGetWidth/ofGetHeight returns incorrect values after retina windows are resized in version 0.9.1 & 0.9.2
    https://github.com/openframeworks/openFrameworks/pull/4858
*/
    int multiplier = 1;
    if (ofxXRSIsHighResolution() && ofGetVersionMajor() == 0 && ofGetVersionMinor() == 9 && (ofGetVersionPatch() == 1 || ofGetVersionPatch() == 2)){
        multiplier = 2;
    }
    if (mAnchor == ofxXRSPanelAnchor::TOP_LEFT){
        mPosition.y = 0;
        mPosition.x = 0;
    }   else if (mAnchor == ofxXRSPanelAnchor::TOP_RIGHT){
        mPosition.y = 0;
        mPosition.x = (ofGetWidth() / multiplier) - mWidth;
    }   else if (mAnchor == ofxXRSPanelAnchor::BOTTOM_LEFT){
        mPosition.x = 0;
        mPosition.y = (ofGetHeight() / multiplier) - mHeight;
    }   else if (mAnchor == ofxXRSPanelAnchor::BOTTOM_RIGHT){
        mPosition.x = (ofGetWidth() / multiplier) - mWidth;
        mPosition.y = (ofGetHeight() / multiplier) - mHeight;
    }
    int h = 0;
    for (size_t i=0; i<items.size(); i++) {
    // skip over any components that are currently invisible //
        if (items[i]->getVisible() == false) continue;
        items[i]->setPosition(mPosition.x, mPosition.y + h);
        h += items[i]->getHeight() + mRowSpacing;
    }
    // move the footer back to the top of the gui //
    if (!mExpanded) mGuiFooter->setPosition(mPosition.x, mPosition.y);
    mGuiBounds = ofRectangle(mPosition.x, mPosition.y, mWidth, mHeight);
}

/* 
    update & draw loop
*/

void ofxXRSPanel::update()
{
    if (!mVisible) return;

    // check if we need to update components //
    for (size_t i=0; i<items.size(); i++) {
        if (mAlphaChanged) items[i]->setOpacity(mAlpha);
        if (mThemeChanged) items[i]->setTheme(mTheme);
        if (mWidthChanged) items[i]->setWidth(mWidth, mLabelWidth);
        if (mAlignmentChanged) items[i]->setLabelAlignment(mAlignment);
    }
    
    if (mThemeChanged || mWidthChanged) layoutGui();

    mTheme = nullptr;
    mAlphaChanged = false;
    mWidthChanged = false;
    mThemeChanged = false;
    mAlignmentChanged = false;
    
    // check for gui focus change //
    if (ofGetMousePressed() && mActiveGui->mMoving == false){
        ofPoint mouse = ofPoint(ofGetMouseX(), ofGetMouseY());
        for (int i=mGuis.size()-1; i>-1; i--){
        // ignore guis that are invisible //
            if (mGuis[i]->getVisible() && mGuis[i]->hitTest(mouse)){
                if (mGuis[i] != mActiveGui) mGuis[i]->focus();
                break;
            }
        }
    }

    if (!getFocused() || !mEnabled){
    // update children but ignore mouse & keyboard events //
        for (size_t i=0; i<items.size(); i++) items[i]->update(false);
    }   else {
        mMoving = false;
        mMouseDown = false;
    // this gui has focus so let's see if any of its components were interacted with //
        if (mExpanded == false){
            mGuiFooter->update();
            mMouseDown = mGuiFooter->getMouseDown();
        }   else{
            bool hitComponent = false;
            for (size_t i=0; i<items.size(); i++) {
                if (hitComponent == false){
                    items[i]->update(true);
                    if (items[i]->getFocused()) {
                        hitComponent = true;
                        mMouseDown = items[i]->getMouseDown();
                        if (mGuiHeader != nullptr && mGuiHeader->getDraggable() && mGuiHeader->getFocused()){
                    // track that we're moving to force preserve focus //
                            mMoving = true;
                            ofPoint mouse = ofPoint(ofGetMouseX(), ofGetMouseY());
                            moveGui(mouse - mGuiHeader->getDragOffset());
                        }
                    }   else if (items[i]->getIsExpanded()){
                    // check if one of its children has focus //
                        for (size_t j=0; j<items[i]->children.size(); j++) {
                            if (items[i]->children[j]->getFocused()){
                                hitComponent = true;
                                mMouseDown = items[i]->children[j]->getMouseDown();
                                break;
                            }
                        }
                    }
                }   else{
            // update component but ignore mouse & keyboard events //
                    items[i]->update(false);
                    if (items[i]->getFocused()) items[i]->setFocused(false);
                }
            }
        }
    }
// empty the trash //
    for (size_t i=0; i<trash.size(); i++) delete trash[i];
    trash.clear();
}

void ofxXRSPanel::draw()
{
    if (mVisible == false) return;
    ofPushStyle();
        ofFill();
        ofSetColor(mGuiBackground, mAlpha * 255);
        if (mExpanded == false){
            ofDrawRectangle(mPosition.x, mPosition.y, mWidth, mGuiFooter->getHeight());
            mGuiFooter->draw();
        }   else{
            ofDrawRectangle(mPosition.x, mPosition.y, mWidth, mHeight - mRowSpacing);
            for (size_t i=0; i<items.size(); i++) items[i]->draw();
        // color pickers overlap other components when expanded so they must be drawn last //
            for (size_t i=0; i<items.size(); i++) items[i]->drawColorPicker();
        }
    ofPopStyle();
}

void ofxXRSPanel::onDraw(ofEventArgs &e)
{
    draw();
}

void ofxXRSPanel::onUpdate(ofEventArgs &e)
{
    update();
}

void ofxXRSPanel::onWindowResized(ofResizeEventArgs &e)
{
    if (mAnchor != ofxXRSPanelAnchor::NO_ANCHOR) positionGui();
}


