#pragma once
#include "ofxXRSComponent.h"

class ofxXRSMatrixButton : public ofxXRSInteractiveObject {

    public:
    
        ofxXRSMatrixButton(int size, int index, bool showLabels, std::string label = "")
        {
            mIndex = index;
            mSelected = false;
            mShowLabels = showLabels;
            mRect = ofRectangle(0, 0, size, size);
			mOriginalWidth = static_cast<float>(size);
            mHasLabel = false;
            mLabel = label;

            if(!label.empty()) {
                mHasLabel = true;
            }
        }
    
        void setPosition(float x, float y)
        {
            origin.x = x;
            origin.y = y;
        }

        std::string getLabel() {
            if(mHasLabel) {
                return mLabel;
            }

            return ofToString(mIndex+1);
        }
    
        void draw(int x, int y)	
        {
			mRect.x = x + origin.y;
            mRect.y = y + origin.y;
            ofPushStyle();
            ofFill();
            ofSetColor(mBkgdColor);
            ofDrawRectangle(mRect);
            if (mShowLabels) {
                ofSetColor(mLabelColor);
                if(!mHasLabel) {
                	mFont->draw(ofToString(mIndex+1), mRect.x + mRect.width/2 - mFontRect.width/2, mRect.y + mRect.height/2 + mFontRect.height/2);
                } else {
                	const float strWidth = mFont->width(mLabel);
                	mRect.setWidth(mOriginalWidth + strWidth);
                	mFont->draw(mLabel,
						static_cast<int>((mRect.x + mRect.width / 2.0f - mFontRect.width / 2.0f) - (strWidth / 2.0f)),
						static_cast<int>(mRect.y + mRect.height / 2.0f + mFontRect.height / 2.0f));
                   }
               }
            ofPopStyle();
        }
    
        void hitTest(ofPoint m, bool mouseDown)
        {
            if (mRect.inside(m) && !mSelected){
                if (mouseDown){
                    mBkgdColor = colors.selected.button;
                    mLabelColor = colors.selected.label;
                }   else{
                    mBkgdColor = colors.hover.button;
                    mLabelColor = colors.hover.label;
                }
            }   else{
                onMouseOut();
            }
        }

		bool isMouseOver() {
			bool result = mRect.inside(ofGetMouseX(), ofGetMouseY());

			/*
			std::cout << "MouseX: " << ofGetMouseX() << "\tMouseY: " << ofGetMouseY() << std::endl;
			std::cout << "mRect x: " << mRect.getX() << "\tmRect y: " << mRect.getY() << std::endl;
			std::cout << "mRect Width: " << mRect.getWidth() << std::endl;
			std::cout << "mRect Pos: " << mRect.position << std::endl;
			std::cout << "mRect Area: " << mRect.getArea() << std::endl;
			std::cout << "mRect Right: " << mRect.getRight() << std::endl;
			std::cout << "Result: " << result << std::endl;
			*/

			return result;
        }
    
        int getIndex()
        {
            return mIndex;
        }

		float getWidth() {
			return mRect.getWidth();
        }

		void setWidth(float width) {
			mRect.setWidth(width);
        }
    
        void setSelected(bool selected)
        {
            mSelected = selected;
        }
    
        bool getSelected()
        {
            return mSelected;
        }

		float getOriginalWidth() {
			return mOriginalWidth;
        }

		void setLabel(const std::string label) {
			mLabel = label;
			mHasLabel = true;
        }

		void setIndex(int index) {
			mIndex = index;
        }
    
        void onMouseOut()
        {
            if (mSelected){
                mBkgdColor = colors.selected.button;
                mLabelColor = colors.selected.label;
            }   else{
                mBkgdColor = colors.normal.button;
                mLabelColor = colors.normal.label;
            }
        }
    
        void onMouseRelease(ofPoint m)
        {
            if (mRect.inside(m)) {
                mSelected = !mSelected;
                ofxXRSInternalEvent e(ofxXRSEventType::MATRIX_BUTTON_TOGGLED, mIndex);
                internalEventCallback(e);
            }
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
			mFont = theme->font.ptr;
            mFontRect = mFont->rect(ofToString(mIndex+1));
            mBkgdColor = theme->color.matrix.normal.button;
            mLabelColor = theme->color.matrix.normal.label;
            colors.normal.label = theme->color.matrix.normal.label;
            colors.normal.button = theme->color.matrix.normal.button;
            colors.hover.label = theme->color.matrix.hover.label;
            colors.hover.button = theme->color.matrix.hover.button;
            colors.selected.label = theme->color.matrix.selected.label;
            colors.selected.button = theme->color.matrix.selected.button;
        }
    
    private:
        int x;
        int y;
        int mIndex;
		float mOriginalWidth;
        std::string mLabel;
        ofPoint origin;
        ofRectangle mRect;
        ofColor mBkgdColor;
        ofColor mLabelColor;
        bool mSelected;
        bool mShowLabels;
        bool mHasLabel;
        ofRectangle mFontRect;
        std::shared_ptr<ofxSmartFont> mFont;
        struct {
            struct{
                ofColor label;
                ofColor button;
            } normal;
            struct{
                ofColor label;
                ofColor button;
            } hover;
            struct{
                ofColor label;
                ofColor button;
            } selected;
        } colors;
};

class ofxXRSMatrix : public ofxXRSComponent {

    public:
    
        ofxXRSMatrix(std::string label, int numButtons, bool showLabels = false) : ofxXRSComponent(label)
        {
            mRadioMode = false;
			mCustomized = false;
            mNumButtons = numButtons;
            mShowLabels = showLabels;
            mType = ofxXRSType::MATRIX;
            setTheme(ofxXRSComponent::getTheme());
        }
    
        void setTheme(const ofxXRSTheme* theme) override
        {
            setComponentStyle(theme);
            mFillColor = theme->color.inputAreaBackground;
            mButtonSize = theme->layout.matrix.buttonSize;
            mButtonPadding = theme->layout.matrix.buttonPadding;
            mStyle.stripe.color = theme->stripe.matrix;
        	attachButtons(theme);
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }

		void addButton(const ofxXRSTheme* theme, int size, bool showLabel, std::string label = "") {
			btns.emplace_back(*new ofxXRSMatrixButton(size, mNumButtons, !label.empty()));
			btns[mNumButtons].setTheme(theme);
			btns[mNumButtons].onInternalEvent(this, &ofxXRSMatrix::onButtonSelected);
			btns[mNumButtons].setPosition(mButtonPadding, mButtonPadding);

			if (!label.empty()) {
				btns[mNumButtons].setLabel(label);
				btns[mNumButtons].setTheme(theme);
			}

			mNumButtons++;
        }

		void setLabelAtIndex(int index, const std::string label) {
			ofxXRSMatrixButton* btn = getButtonAtIndex(index);
			btn->setLabel(label);

			const float strWidth = mFont->width(label);
			btn->setWidth(btn->getOriginalWidth() + strWidth);
			labels.at(index) = label;
		}
    
        void setWidth(int width, float labelWidth)
        {
            ofxXRSComponent::setWidth(width, labelWidth);
            mMatrixRect.x = x + mLabel.width;
            mMatrixRect.y = y + mStyle.padding;
            mMatrixRect.width = mStyle.width - mStyle.padding - mLabel.width;
            int nCols = floor(mMatrixRect.width / (mButtonSize + mButtonPadding));
            int nRows = ceil(btns.size() / float(nCols));
            float padding = (mMatrixRect.width - (mButtonSize * nCols)) / (nCols - 1);
            for(size_t i=0; i<btns.size(); i++){
                float bx = (mButtonSize + padding) * (i % nCols);
                float by = (mButtonSize + padding) * (floor(i/nCols));
                btns[i].setPosition(bx, by + mStyle.padding);
            }
            mStyle.height = (mStyle.padding*2) + ((mButtonSize + padding) * (nRows - 1)) + mButtonSize;
            mMatrixRect.height = mStyle.height - (mStyle.padding * 2);
        }
    
        void setPosition(int x, int y)
        {
            ofxXRSComponent::setPosition(x, y);
            mMatrixRect.x = x + mLabel.width;
            mMatrixRect.y = y + mStyle.padding;
        }
    
        void setRadioMode(bool enabled)
        {
            mRadioMode = enabled;
        }
    
        bool hitTest(ofPoint m)
        {
            if (mMatrixRect.inside(m)){
                for(size_t i=0; i<btns.size(); i++) btns[i].hitTest(m, mMouseDown);
                return true;
            }   else{
                for(size_t i=0; i<btns.size(); i++) btns[i].onMouseOut();
                return false;
            }
        }
    
        void draw()
        {
            if (!mVisible) return;
            ofPushStyle();
                ofxXRSComponent::draw();
                ofSetColor(mFillColor);
				ofDrawRectangle(mMatrixRect);

				float prevOffset = 0.0f;
				for (size_t i = 0; i < btns.size(); i++) {
					btns[i].draw(x + mLabel.width + prevOffset, y);
					prevOffset += btns[i].getWidth() + mButtonPadding;
				}

            ofPopStyle();
        }
    
        void clear()
        {
            for (size_t i=0; i<btns.size(); i++) btns[i].setSelected(false);
        }
    
        void setSelected(std::vector<int> v)
        {
            clear();
            for (size_t i=0; i<v.size(); i++) btns[v[i]].setSelected(true);
            mLastItemSelected = &btns[v.back()];
        }

        std::vector<int> getSelected()
        {
	        std::vector<int> selected;
            for(size_t i=0; i<btns.size(); i++) if (btns[i].getSelected()) selected.push_back(i);
            return selected;
        }
    
        ofxXRSMatrixButton* getButtonAtIndex(int index)
        {
            return &btns[index];
        }

		void removeButtonAtIndex(int index) {
			mNumButtons--;
			for(int i = index; i < mNumButtons - 1; i++) {
				btns.at(i).setLabel(btns.at(i + 1).getLabel());
			}
			btns.erase(btns.begin() + index);
			labels.erase(labels.begin() + index);
			draw();

			std::cout << "LABELS: " << std::endl;
			for(auto& label : labels) {
				std::cout << label << std::endl;
			}
			std::cout << "========" << std::endl;
		}

		int getNumButtons() {
			return mNumButtons;
        }
    
        void dispatchEvent()
        {
            if (matrixEventCallback != nullptr) {
                if (btns.size() != 0){
                    if (mLastItemSelected == nullptr){
                        mLastItemSelected = &btns.back();
                    }
                    ofxXRSMatrixEvent e(this, mLastItemSelected->getIndex(), mLastItemSelected->getSelected());
                    matrixEventCallback(e);
                }   else{
                    ofxXRSLog::write(ofxXRSMsg::MATRIX_EMPTY);
                }
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }
    
        static ofxXRSMatrix* getInstance() { return new ofxXRSMatrix("X", 0); }
    
    protected:
    
        void onMouseRelease(ofPoint m)
        {
            ofxXRSComponent::onFocusLost();
            ofxXRSComponent::onMouseRelease(m);
            for(size_t i=0; i<btns.size(); i++) btns[i].onMouseRelease(m);
        }
    
        void onButtonSelected(ofxXRSInternalEvent e)
        {
            if (mRadioMode) {
        // deselect all buttons save the one that was selected //
                for(size_t i=0; i<btns.size(); i++) btns[i].setSelected(e.index == i);
            }
			if (btns.size() > e.index) {
				mLastItemSelected = &btns.at(e.index);
			}
            dispatchEvent();
        }
    
        void attachButtons(const ofxXRSTheme* theme)
        {
			std::vector<std::string> oldLabels = labels;
        	btns.clear();
			labels.clear();

        	for (size_t i = 0; i < mNumButtons; i++) {
				if (i < oldLabels.size()) {
					ofxXRSMatrixButton btn(mButtonSize, i, mShowLabels, oldLabels.at(i));
					btn.setTheme(theme);
					btn.onInternalEvent(this, &ofxXRSMatrix::onButtonSelected);
					btns.push_back(btn);
					labels.push_back(btn.getLabel());
				} else {
					ofxXRSMatrixButton btn(mButtonSize, i, mShowLabels);
					btn.setTheme(theme);
					btn.onInternalEvent(this, &ofxXRSMatrix::onButtonSelected);
					btns.push_back(btn);
					labels.push_back(btn.getLabel());
				}
        	}

			std::cout << "============" << std::endl;
			std::cout << "OLDLABELS: " << std::endl;
			for (auto& label : oldLabels) {
				std::cout << label << std::endl;
			}
			std::cout << "NEW LABELS: " << std::endl;
			for (auto& label : labels) {
				std::cout << label << std::endl;
			}
			std::cout << "==========" << std::endl;

			
        }
    
    private:
    
        int mButtonSize;
        int mNumButtons;
        int mButtonPadding;
        bool mRadioMode;
        bool mShowLabels;
		bool mCustomized;
        ofColor mFillColor;
        ofRectangle mMatrixRect;
        std::vector<ofxXRSMatrixButton> btns;
        std::vector<std::string> labels;
        ofxXRSMatrixButton* mLastItemSelected;
};



