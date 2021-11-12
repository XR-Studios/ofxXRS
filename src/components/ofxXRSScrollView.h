#pragma once
#include "ofxXRSComponent.h"

class ofxXRSScrollViewItem : public ofxXRSButton {

    friend class ofxXRSScrollView;

    public:
   
        ofxXRSScrollViewItem(std::string label, int index) : ofxXRSButton(label)
        {
            mIndex = index;
        }
    
        int getIndex()
        {
            return mIndex;
        }
    
    private:
    
        int mIndex;

};

class ofxXRSScrollView : public ofxXRSComponent {

    public:
    
        ofxXRSScrollView(std::string name, int nVisible = 6) : ofxXRSComponent(name)
        {
            mAutoHeight = true;
            mNumVisible = nVisible;
            setTheme(ofxXRSComponent::getTheme());
            ofAddListener(ofEvents().mouseScrolled, this, &ofxXRSScrollView::onMouseScrolled, OF_EVENT_ORDER_BEFORE_APP);
        }
    
        ~ofxXRSScrollView()
        {
            mTheme = nullptr;
            ofRemoveListener(ofEvents().mouseScrolled, this, &ofxXRSScrollView::onMouseScrolled, OF_EVENT_ORDER_BEFORE_APP);
        }
    
    /*
        list manipulation
    */
    
        void add(std::string label)
        {
            int y = 0;
            if (mItems.size() > 0) y = mItems.back()->getY() + mItems.back()->getHeight() + mSpacing;
            mItems.push_back(new ofxXRSScrollViewItem( label, mItems.size() ));
            mItems.back()->setMask(mRect);
            mItems.back()->setTheme(mTheme);
            mItems.back()->setWidth(mRect.width, 0);
            mItems.back()->setPosition(0, y);
            mItems.back()->onButtonEvent(this, &ofxXRSScrollView::onButtonEvent);
        //  cout << "ofxXRSScrollView :: total items = " << mItems.size() << endl;
            if (mAutoHeight) autoSize();
        }
    
        ofxXRSScrollViewItem* getItemAtIndex(int index)
        {
            return mItems[index];
        }
    
        ofxXRSScrollViewItem* getItemByName(std::string name)
        {
            for(auto i:mItems) if (i->is(name)) return i;
            return nullptr;
        }
    
        void swap(int index1, int index2)
        {
            if (isValidIndex(index1) && isValidIndex(index2) && index1 != index2){
                std::swap(mItems[index1], mItems[index2]);
                positionItems();
            }
        }
    
        void move(int from, int to)
        {
            if (isValidIndex(from) && isValidIndex(to) && from != to){
                auto itr_from = mItems.begin() + from;
                auto itr_to = mItems.begin() + to;
                if (itr_from < itr_to ) {
                // move down //
                std::rotate(itr_from, itr_from+1, itr_to+1);
                } else if (itr_from > itr_to) {
                // move up //
                std::rotate(itr_to, itr_from, itr_from+1);
                }
                positionItems();
            }   else {
	            std::cout << "invalid move operation, check your indices" << std::endl;
            }
        }
    
        void move(ofxXRSComponent* item, int index)
        {
            for(size_t i=0; i<mItems.size(); i++){
                if (mItems[i] == item) {
                    move(i, index); return;
                }
            }
        }
    
        void clear()
        {
            for (auto& i:mItems) delete i;
            mItems.clear();
        }
    
        void remove(int index)
        {
            if (isValidIndex(index)) {
                delete mItems[index];
                mItems.erase(mItems.begin()+index);
            }
            positionItems();
        }
    
        void remove(ofxXRSComponent* item)
        {
            for(size_t i=0; i<mItems.size(); i++){
                if (mItems[i] == item) {
                    delete mItems[i];
                    mItems.erase(mItems.begin()+i);
                    positionItems(); return;
                }
            }
        }
   
    /*
        temporary getters until mRect is implemented in ofxXRSComponent
    */
    
        int getX()
        {
            return mRect.x;
        }
    
        int getY()
        {
            return mRect.y;
        }

        int getWidth()
        {
            return mRect.width;
        }
    
        int getHeight()
        {
            return mRect.height;
        }
    
        int getNumItems()
        {
            return mItems.size();
        }
    
    /*
        list presentation
    */

        void setTheme(const ofxXRSTheme* theme)
        {
            mTheme = theme;
            mSpacing = theme->layout.vMargin;
            mBackground = theme->color.guiBackground;
            for (auto i:mItems) i->setTheme(theme);
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }
    
        void setWidth(int width, float labelWidth = 1)
        {
            mRect.width = width;
            for (auto i:mItems) i->setWidth(mRect.width, labelWidth);
            if (mAutoHeight) autoSize();
        }
    
        void setHeight(int height)
        {
            mAutoHeight = false;
            mRect.height = height;
            if (mRect.width > 0 && mRect.height > 0) mView.allocate( mRect.width, mRect.height );
        }

        void setPosition(int x, int y)
        {
            mRect.x = x;
            mRect.y = y;
        // update each component's mask so mouse events track correctly //
            for(size_t i=0; i<mItems.size(); i++) mItems[i]->setMask(mRect);
        }
    
        void setItemSpacing(int spacing)
        {
            mSpacing = spacing;
        }
    
        void setBackgroundColor(ofColor color)
        {
            mBackground = color;
        }
    
    /*
        update & draw
    */

        void update()
        {
            for(auto i:mItems) i->update();
        }
    
        void draw()
        {
            ofPushStyle();
                ofFill();
            // draw a background behind the fbo //
                ofSetColor(ofColor::black);
                ofDrawRectangle(mRect);
            // draw into the fbo //
                mView.begin();
                ofClear(255,255,255,0);
                ofSetColor(mBackground);
                ofDrawRectangle(0, 0, mRect.width, mRect.height);
                for(auto i:mItems) i->draw();
                mView.end();
            // draw the fbo of list content //
                ofSetColor(ofColor::white);
                mView.draw(mRect.x, mRect.y);
            ofPopStyle();
        }
    
        void dispatchEvent()
        {
            if (scrollViewEventCallback != nullptr) {
                ofxXRSScrollViewEvent e(this, mLastItemSelected);
                scrollViewEventCallback(e);
            }   else{
                ofxXRSLog::write(ofxXRSMsg::EVENT_HANDLER_NULL);
            }
        }

    private:
    
        ofFbo mView;
        ofRectangle mRect;
        ofColor mBackground;
        const ofxXRSTheme* mTheme;
    
        int mY = 0;
        int mSpacing;
        int mNumVisible;
        bool mAutoHeight;
        std::vector<ofxXRSScrollViewItem*> mItems;
        ofxXRSScrollViewItem* mLastItemSelected;
    
        void autoSize()
        {
            mRect.height = ((mTheme->layout.height + mSpacing) * mNumVisible) - mSpacing;
            if (mRect.width > 0 && mRect.height > 0) mView.allocate( mRect.width, mRect.height );
        }
    
        void onMouseScrolled(ofMouseEventArgs &e)
        {
            if (mItems.size() > 0 && mRect.inside(e.x, e.y) == true){
                float sy = e.scrollY * 2;
                int btnH = mItems.front()->getHeight() + mSpacing;
                int minY = mRect.height + mSpacing  - (mItems.size() * btnH);
                bool allowScroll = false;
                mY = mItems.front()->getY();
                if (sy < 0){
                    if (mY > minY){
                        mY += sy;
                        if (mY < minY) mY = minY;
                        allowScroll = true;
                    }
                }   else if (sy > 0){
                    if (mY < 0){
                        mY += sy;
                        if (mY > 0) mY = 0;
                        allowScroll = true;
                    }
                }
                if (allowScroll){
                    mItems.front()->setPosition(0, mY);
                    for(size_t i=0; i<mItems.size(); i++) mItems[i]->setPosition(0, mY + (btnH * i));
                }
            }
        }
    
        void onButtonEvent(ofxXRSButtonEvent e)
        {
            for(size_t i=0; i<mItems.size(); i++) {
                if (mItems[i] == e.target) {
                    mLastItemSelected = mItems[i];
                    dispatchEvent(); break;
                }
            }
        }
    
        void positionItems()
        {
            int y = mY;
            for(size_t i=0; i<mItems.size(); i++){
                mItems[i]->mIndex = i;
                mItems[i]->setPosition(0, y);
                y = mItems[i]->getY() + mItems[i]->getHeight() + mSpacing;
            }
        }
    
        bool isValidIndex(int index)
        {
			size_t i = static_cast<size_t>(index);
            return index >= 0 && i < mItems.size();
        }

};
