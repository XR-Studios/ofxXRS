/*
    Copyright (C) 2015 Stephen Braitsch [http://braitsch.io]

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once
#include "ofxXRSLabel.h"
#include "ofxXRSButton.h"
#include "ofxXRSSlider.h"
#include "ofxXRSTextInput.h"
#include "ofxXRSFRM.h"
#include "ofxXRS2dPad.h"
#include "ofxXRSColorPicker.h"
#include "ofxXRSMatrix.h"
#include "ofxXRSTimeGraph.h"
#include "ofxXRSScrollView.h"

class ofxXRSGroup : public ofxXRSButton {

    public:
    
        ofxXRSGroup(std::string label) : ofxXRSButton(label), mHeight(0)
        {
            mIsExpanded = false;
            layout();
        }
    
        ~ofxXRSGroup()
        {
        // color pickers are deleted automatically when the group is destroyed //
            for (auto i:children) if (i->getType() != ofxXRSType::COLOR_PICKER) delete i;
        }
    
        void setPosition(int x, int y)
        {
            ofxXRSComponent::setPosition(x, y);
            layout();
        }
    
        void expand()
        {
            mIsExpanded = true;
            layout();
            onGroupToggled();
        }
    
        void toggle()
        {
            mIsExpanded = !mIsExpanded;
            layout();
            onGroupToggled();
        }
    
        void collapse()
        {
            mIsExpanded = false;
            layout();
            onGroupToggled();
        }
    
        int getHeight()
        {
            return mHeight;
        }
    
        bool getIsExpanded()
        {
            return mIsExpanded;
        }
    
        void draw()
        {
            if (mVisible){
                ofPushStyle();
                ofxXRSButton::draw();
                if (mIsExpanded) {
                    int mHeight = mStyle.height;
                    ofSetColor(mStyle.guiBackground, mStyle.opacity);
                    ofDrawRectangle(x, y+mHeight, mStyle.width, mStyle.vMargin);
                    for(size_t i=0; i<children.size(); i++) {
                        mHeight += mStyle.vMargin;
                        children[i]->draw();
                        mHeight += children[i]->getHeight();
                        if (i == children.size()-1) break;
                        ofSetColor(mStyle.guiBackground, mStyle.opacity);
                        ofDrawRectangle(x, y+mHeight, mStyle.width, mStyle.vMargin);
                    }
                    ofSetColor(mIcon.color);
                    mIconOpen->draw(x+mIcon.x, y+mIcon.y, mIcon.size, mIcon.size);
                    for(size_t i=0; i<children.size(); i++) children[i]->drawColorPicker();
                }   else{
                    ofSetColor(mIcon.color);
                    mIconClosed->draw(x+mIcon.x, y+mIcon.y, mIcon.size, mIcon.size);
                }
                ofPopStyle();
            }
        }
    
    protected:
    
        void layout()
        {
            mHeight = mStyle.height + mStyle.vMargin;
            for (size_t i=0; i<children.size(); i++) {
                if (children[i]->getVisible() == false) continue;
                if (mIsExpanded == false){
                    children[i]->setPosition(x, y + mHeight);
                }   else{
                    children[i]->setPosition(x, y + mHeight);
                    mHeight += children[i]->getHeight() + mStyle.vMargin;
                }
                if (i == children.size()-1) mHeight -= mStyle.vMargin;
            }
        }
    
        void onMouseRelease(ofPoint m)
        {
            if (mFocused){
            // open & close the group when its header is clicked //
                ofxXRSComponent::onFocusLost();
                ofxXRSComponent::onMouseRelease(m);
                mIsExpanded ? collapse() : expand();
            }
        }
    
    	void onGroupToggled()
	   	{
        // dispatch an event out to the gui panel to adjust its children //
            if (internalEventCallback != nullptr){
                ofxXRSInternalEvent e(ofxXRSEventType::GROUP_TOGGLED, mIndex);
                internalEventCallback(e);
            }
    	}
    
        void dispatchInternalEvent(ofxXRSInternalEvent e)
        {
            if (e.type == ofxXRSEventType::VISIBILITY_CHANGED) layout();
            internalEventCallback(e);
        }
    
        int mHeight;
        std::shared_ptr<ofImage> mIconOpen;
        std::shared_ptr<ofImage> mIconClosed;
        bool mIsExpanded;
    
};

class ofxXRSFolder : public ofxXRSGroup {

    public:
    
        ofxXRSFolder(std::string label, ofColor color = ofColor::white) : ofxXRSGroup(label)
        {
        // all items within a folder share the same stripe color //
            mStyle.stripe.color = color;
            mType = ofxXRSType::FOLDER;
            setTheme(ofxXRSComponent::getTheme());
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mIconOpen = theme->icon.groupOpen;
            mIconClosed = theme->icon.groupClosed;
            setWidth(theme->layout.width, theme->layout.labelWidth);
        // reassign folder color to all components //
            for(auto i:children) i->setStripeColor(mStyle.stripe.color);
        }
    
        void setWidth(int width, float labelWidth = 1)
        {
            ofxXRSComponent::setWidth(width, labelWidth);
            mLabel.width = mStyle.width;
            mLabel.rightAlignedXpos = mIcon.x - mLabel.margin;
            ofxXRSComponent::positionLabel();
        }
    
        void drawColorPicker()
        {
            for(size_t i=0; i<pickers.size(); i++) pickers[i]->drawColorPicker();
        }
    
        void dispatchButtonEvent(ofxXRSButtonEvent e)
        {
            if (buttonEventCallback != nullptr) {
                buttonEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        void dispatchToggleEvent(ofxXRSToggleEvent e)
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
    
        void dispatchSliderEvent(ofxXRSSliderEvent e)
        {
            if (sliderEventCallback != nullptr) {
                sliderEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        void dispatchTextInputEvent(ofxXRSTextInputEvent e)
        {
            if (textInputEventCallback != nullptr) {
                textInputEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        void dispatchColorPickerEvent(ofxXRSColorPickerEvent e)
        {
            if (colorPickerEventCallback != nullptr) {
                colorPickerEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        void dispatch2dPadEvent(ofxXRS2dPadEvent e)
        {
            if (pad2dEventCallback != nullptr) {
                pad2dEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        void dispatchMatrixEvent(ofxXRSMatrixEvent e)
        {
            if (matrixEventCallback != nullptr) {
                matrixEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }

    /*
        component add methods
    */

        ofxXRSLabel* addLabel(std::string label)
        {
            ofxXRSLabel* lbl = new ofxXRSLabel(label);
            lbl->setStripeColor(mStyle.stripe.color);
            attachItem(lbl);
            return lbl;
        }

        ofxXRSButton* addButton(std::string label)
        {
            ofxXRSButton* button = new ofxXRSButton(label);
            button->setStripeColor(mStyle.stripe.color);
            button->onButtonEvent(this, &ofxXRSFolder::dispatchButtonEvent);
            attachItem(button);
            return button;
        }
    
        ofxXRSToggle* addToggle(std::string label, bool enabled = false)
        {
            ofxXRSToggle* toggle = new ofxXRSToggle(label, enabled);
            toggle->setStripeColor(mStyle.stripe.color);
            toggle->onToggleEvent(this, &ofxXRSFolder::dispatchToggleEvent);
            attachItem(toggle);
            return toggle;
        }
    
        ofxXRSSlider* addSlider(std::string label, float min, float max)
        {
            ofxXRSSlider* slider = addSlider(label, min, max, (max+min)/2);
            return slider;
        }

        ofxXRSSlider* addSlider(std::string label, float min, float max, double val)
        {
            ofxXRSSlider* slider = new ofxXRSSlider(label, min, max, val);
            slider->setStripeColor(mStyle.stripe.color);
            slider->onSliderEvent(this, &ofxXRSFolder::dispatchSliderEvent);
            attachItem(slider);
            return slider;
        }

        ofxXRSSlider* addSlider(ofParameter<int> & p){
            ofxXRSSlider* slider = new ofxXRSSlider(p);
            slider->setStripeColor(mStyle.stripe.color);
            slider->onSliderEvent(this, &ofxXRSFolder::dispatchSliderEvent);
            attachItem(slider);
            return slider;
        }

        ofxXRSSlider* addSlider(ofParameter<float> & p){
            ofxXRSSlider* slider = new ofxXRSSlider(p);
            slider->setStripeColor(mStyle.stripe.color);
            slider->onSliderEvent(this, &ofxXRSFolder::dispatchSliderEvent);
            attachItem(slider);
            return slider;
        }
    
        ofxXRSTextInput* addTextInput(std::string label, std::string value)
        {
            ofxXRSTextInput* input = new ofxXRSTextInput(label, value);
            input->setStripeColor(mStyle.stripe.color);
            input->onTextInputEvent(this, &ofxXRSFolder::dispatchTextInputEvent);
            attachItem(input);
            return input;
        }
    
        ofxXRSColorPicker* addColorPicker(std::string label, ofColor color = ofColor::black)
        {
	        std::shared_ptr<ofxXRSColorPicker> picker(new ofxXRSColorPicker(label, color));
            picker->setStripeColor(mStyle.stripe.color);
            picker->onColorPickerEvent(this, &ofxXRSFolder::dispatchColorPickerEvent);
            attachItem(picker.get());
            pickers.push_back(picker);
            return picker.get();
        }
    
        ofxXRSFRM* addFRM(float refresh = 1.0f)
        {
            ofxXRSFRM* monitor = new ofxXRSFRM(refresh);
            monitor->setStripeColor(mStyle.stripe.color);
            attachItem(monitor);
            return monitor;
        }

        ofxXRSBreak* addBreak()
        {
            ofxXRSBreak* brk = new ofxXRSBreak();
            attachItem(brk);
            return brk;
        }
    
        ofxXRS2dPad* add2dPad(std::string label)
        {
            ofxXRS2dPad* pad = new ofxXRS2dPad(label);
            pad->setStripeColor(mStyle.stripe.color);
            pad->on2dPadEvent(this, &ofxXRSFolder::dispatch2dPadEvent);
            attachItem(pad);
            return pad;
        }

        ofxXRSMatrix* addMatrix(std::string label, int numButtons, bool showLabels = false)
        {
            ofxXRSMatrix* matrix = new ofxXRSMatrix(label, numButtons, showLabels);
            matrix->setStripeColor(mStyle.stripe.color);
            matrix->onMatrixEvent(this, &ofxXRSFolder::dispatchMatrixEvent);
            attachItem(matrix);
            return matrix;
        }
    
        ofxXRSWaveMonitor* addWaveMonitor(std::string label, float frequency, float amplitude)
        {
            ofxXRSWaveMonitor* monitor = new ofxXRSWaveMonitor(label, frequency, amplitude);
            monitor->setStripeColor(mStyle.stripe.color);
            attachItem(monitor);
            return monitor;
        }
    
        ofxXRSValuePlotter* addValuePlotter(std::string label, float min, float max)
        {
            ofxXRSValuePlotter* plotter = new ofxXRSValuePlotter(label, min, max);
            plotter->setStripeColor(mStyle.stripe.color);
            attachItem(plotter);
            return plotter;
        }
    
        void attachItem(ofxXRSComponent* item)
        {
            item->setIndex(children.size());
            item->onInternalEvent(this, &ofxXRSFolder::dispatchInternalEvent);
            children.push_back(item);
        }
    
        ofxXRSComponent* getComponent(ofxXRSType type, std::string label)
        {
            for (size_t i=0; i<children.size(); i++) {
                if (children[i]->getType() == type){
                    if (children[i]->is(label)) return children[i];
                }
            }
            return NULL;
        }

        static ofxXRSFolder* getInstance() { return new ofxXRSFolder("X"); }

    protected:
        std::vector<std::shared_ptr<ofxXRSColorPicker>> pickers;
    
};

class ofxXRSDropdownOption : public ofxXRSButton {

    public:
    
        ofxXRSDropdownOption(std::string label) : ofxXRSButton(label)
        {
            mType = ofxXRSType::DROPDOWN_OPTION;
            setTheme(ofxXRSComponent::getTheme());
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            ofxXRSButton::setTheme(theme);
            mStyle.stripe.color = theme->stripe.dropdown;
        }
    
        void setWidth(int width, float labelWidth = 1)
        {
            ofxXRSComponent::setWidth(width, labelWidth);
            mLabel.width = mStyle.width;
            mLabel.rightAlignedXpos = mIcon.x - mLabel.margin;
            ofxXRSComponent::positionLabel();
        }

};

class ofxXRSDropdown : public ofxXRSGroup {

    public:

        ofxXRSDropdown(std::string label, const std::vector<std::string>& options = std::vector<std::string>()) : ofxXRSGroup(label)
        {
            mOption = 0;
            mType = ofxXRSType::DROPDOWN;
            for(size_t i=0; i<options.size(); i++){
                ofxXRSDropdownOption* opt = new ofxXRSDropdownOption(options[i]);
                opt->setIndex(children.size());
                opt->onButtonEvent(this, &ofxXRSDropdown::onOptionSelected);
                children.push_back(opt);
            }
            setTheme(ofxXRSComponent::getTheme());
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mIconOpen = theme->icon.groupOpen;
            mIconClosed = theme->icon.groupClosed;
            mStyle.stripe.color = theme->stripe.dropdown;
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }
    
        void setWidth(int width, float labelWidth = 1)
        {
            ofxXRSComponent::setWidth(width, labelWidth);
            mLabel.width = mStyle.width;
            mLabel.rightAlignedXpos = mIcon.x - mLabel.margin;
            ofxXRSComponent::positionLabel();
        }
    
        void select(int cIndex)
        {
        // ensure value is in range //
			size_t i = static_cast<size_t>(cIndex);
            if (cIndex < 0 || i >= children.size()){
                ofLogError() << "ofxXRSDropdown->select("<<cIndex<<") is out of range";
            }   else{
                setLabel(children[cIndex]->getLabel());
            }
        }

        int size()
        {
            return children.size();
        }
    
        ofxXRSDropdownOption* getChildAt(int index)
        {
            return static_cast<ofxXRSDropdownOption*>(children[index]);
        }
    
        ofxXRSDropdownOption* getSelectedObj()
        {
            return static_cast<ofxXRSDropdownOption*>(children[mOption]);
        }

        std::string getSelected() {
			return getSelectedObj()->getLabel();
        }
    
        void dispatchEvent()
        {
            if (dropdownEventCallback != nullptr) {
                ofxXRSDropdownEvent e(this, mIndex, mOption);
                dropdownEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        static ofxXRSDropdown* getInstance() { return new ofxXRSDropdown("X"); }
    
    private:
    
        void onOptionSelected(ofxXRSButtonEvent e)
        {
            for(size_t i=0; i<children.size(); i++) if (e.target == children[i]) mOption = i;
            setLabel(children[mOption]->getLabel());
           	collapse();
            dispatchEvent();
        }
    
        int mOption;
    
};


