#pragma once
#include "ofxXRSComponent.h"

class ofxXRSTimeGraph : public ofxXRSComponent {

    public:
    
        void setDrawMode(ofxXRSGraph gMode)
        {
            switch (gMode) {
				case ofxXRSGraph::LINES : mDrawFunc = &ofxXRSTimeGraph::drawLines;
                break;
				case ofxXRSGraph::FILLED : mDrawFunc = &ofxXRSTimeGraph::drawFilled;
                break;
				case ofxXRSGraph::POINTS : mDrawFunc = &ofxXRSTimeGraph::drawPoints;
                break;
				case ofxXRSGraph::OUTLINE : mDrawFunc = &ofxXRSTimeGraph::drawOutline;
                break;
            }
        }

    protected:
    
        ofxXRSTimeGraph(string label) : ofxXRSComponent(label)
        {
            mDrawFunc = &ofxXRSTimeGraph::drawFilled;
            setTheme(ofxXRSComponent::getTheme());
        }
    
        void setTheme(const ofxXRSTheme* theme)
        {
            setComponentStyle(theme);
            mStyle.height = theme->layout.graph.height;
            mStyle.stripe.color = theme->stripe.graph;
            mColor.lines = theme->color.graph.lines;
            mColor.fills = theme->color.graph.fills;
            mPointSize = theme->layout.graph.pointSize;
            mLineWeight = theme->layout.graph.lineWeight;
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }
    
        void setWidth(int width, float labelWidth)
        {
            ofxXRSComponent::setWidth(width, labelWidth);
            mPlotterRect.x = mLabel.width;
            mPlotterRect.y = mStyle.padding;
            mPlotterRect.width = mStyle.width - mStyle.padding - mLabel.width;
            mPlotterRect.height = mStyle.height - (mStyle.padding*2);
        }
    
        void draw()
        {
            if (!mVisible) return;
            ofPushStyle();
                ofxXRSComponent::draw();
                ofSetColor(mStyle.color.inputArea);
                ofDrawRectangle(x + mPlotterRect.x, y + mPlotterRect.y, mPlotterRect.width, mPlotterRect.height);
                glColor3ub(mColor.fills.r, mColor.fills.g, mColor.fills.b);
                (*this.*mDrawFunc)();
            ofPopStyle();
        }
    
        void drawFilled()
        {
            float px = this->x + mPlotterRect.x;
            float py = this->y + mPlotterRect.y;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBegin(GL_TRIANGLE_STRIP);
            for (size_t i=0; i<pts.size(); i++) {
                glVertex2f(px+ pts[i].x, py + mPlotterRect.height);
                glVertex2f(px+ pts[i].x, py + pts[i].y);
            }
            glEnd();
        }

        void drawOutline()
        {
            float px = this->x + mPlotterRect.x;
            float py = this->y + mPlotterRect.y;
            glLineWidth(mLineWeight);
            glBegin(GL_LINE_LOOP);
            glVertex2f(px+mPlotterRect.width, py+mPlotterRect.height);
            for (size_t i=0; i<pts.size(); i++) glVertex2f(px+pts[i].x, py+pts[i].y);
            glVertex2f(px, py+mPlotterRect.height);
            glEnd();
        }
    
        void drawLines()
        {
            float px = this->x + mPlotterRect.x;
            float py = this->y + mPlotterRect.y;
            glLineWidth(mLineWeight);
            glBegin(GL_LINE_STRIP);
            for (size_t i=0; i<pts.size(); i++) glVertex2f(px+pts[i].x, py+pts[i].y);
            glEnd();
        }
    
        void drawPoints()
        {
            float px = this->x + mPlotterRect.x;
            float py = this->y + mPlotterRect.y;
            glPointSize(mLineWeight);
            glLineWidth(mLineWeight);
            glBegin(GL_POINTS);
            for (size_t i=0; i<pts.size(); i++) glVertex2f(px+pts[i].x, py+pts[i].y);
            glEnd();
        }
    
        void setPosition(int x, int y)
        {
            ofxXRSComponent::setPosition(x, y);
        }

        int mPointSize;
        int mLineWeight;
        struct{
            ofColor lines;
            ofColor fills;
        } mColor;
        vector<ofVec2f> pts;
        ofRectangle mPlotterRect;
        void (ofxXRSTimeGraph::*mDrawFunc)() = nullptr;
};

class ofxXRSWaveMonitor : public ofxXRSTimeGraph {

    public:

        ofxXRSWaveMonitor(string label, float frequency, float amplitude) : ofxXRSTimeGraph(label)
        {
            mFrequencyLimit = 100;
            setAmplitude(amplitude);
            setFrequency(frequency);
            mType = ofxXRSType::WAVE_MONITOR;
            setTheme(ofxXRSComponent::getTheme());
        }
    
        static ofxXRSWaveMonitor* getInstance()
        {
            return new ofxXRSWaveMonitor("X", 0, 0);
        }
    
    // amplitude is a multiplier that affect the vertical height of the wave and should be a value between 0 & 1 //
        void setAmplitude(float amp)
        {
            mAmplitude = amp;
            if (mAmplitude < 0) {
                mAmplitude = 0;
            }   else if (mAmplitude > MAX_AMPLITUDE){
                mAmplitude = MAX_AMPLITUDE;
            }
            graph();
        }
    
   // frequency is a percentage of the limit to ensure the value is always in range //
        void setFrequency(float freq)
        {
            mFrequency = (freq/mFrequencyLimit)*100.0f;
            graph();
        }
    
        void setFrequencyLimit(float limit)
        {
            mFrequencyLimit = limit;
            setFrequency(mFrequency);
        }
    
        void setTheme(const ofxXRSTheme* tmplt)
        {
            ofxXRSTimeGraph::setTheme(tmplt);
            graph();
        }
    
        void setWidth(int width, float labelWidth)
        {
            ofxXRSTimeGraph::setWidth(width, labelWidth);
            graph();
        }
    
        void graph()
        {
            pts.clear();
            float step = 1.0/mPlotterRect.width;
            float yAmp = (mPlotterRect.height/2) * (mAmplitude/float(MAX_AMPLITUDE));
            for (int i=mPlotterRect.width; i>0; i--) {
                float yp = mPlotterRect.height/2 + (sin((i*step)*(2*mFrequency)*PI) * yAmp);
                pts.push_back(ofVec2f(i, yp));
            }
        }
    
        void update(bool ignoreMouseEvents)
        {
            pts[0].y = pts[pts.size()-1].y;
            for (int i=mPlotterRect.width-1; i>0; i--) pts[i].y = pts[i-1].y;
        }
    
    private:
        float mAmplitude;
        float mFrequency;
        float mFrequencyLimit;
        static const int MAX_AMPLITUDE = 1;

};

class ofxXRSValuePlotter : public ofxXRSTimeGraph {

    public:

        ofxXRSValuePlotter(string label, float min, float max) : ofxXRSTimeGraph(label)
        {
            mSpeed = 5.0f;
            setRange(min, max);
            mType = ofxXRSType::VALUE_PLOTTER;
        }

        ofxXRSValuePlotter(ofParameter<float>& f) : ofxXRSValuePlotter(f.getName(), f.getMin(), f.getMax()) {
            mParamF = &f;
            mParamF->addListener(this, &ofxXRSValuePlotter::onParam);
        }
    
        static ofxXRSValuePlotter* getInstance()
        {
            return new ofxXRSValuePlotter("X", 0, 0);
        }

        void setMin(float min) {
            mMin = min;
            setValue((mMax+min)/2);
        }

        void setMax(float max) {
            mMax = max;
            setValue((max+mMin)/2);
        }
    
        void setRange(float min, float max)
        {
            mMin = min;
            mMax = max;
            setValue((max+min)/2);
        }
    
        void setSpeed(float speed)
        {
            if (speed != mSpeed){
                pts.clear();
                mSpeed = speed;
            }
        }

        void setValue(float value)
        {
            mVal = value;
            if (mVal > mMax){
                mVal = mMax;
            }   else if (mVal < mMin){
                mVal = mMin;
            }

            if(mParamF != nullptr) {
                mParamF->set(mVal);
            }
        }
    
        float getMin()
        {
            return mMin;
        }
    
        float getMax()
        {
            return mMax;
        }
    
        float getRange()
        {
            return mMax-mMin;
        }
    
        void update(bool ignoreMouseEvents)
        {
        // shift all points over before adding new value //
            for (size_t i=0; i<pts.size(); i++) pts[i].x -= mSpeed;
            size_t i = 0;
            while(i < pts.size())
            {
                if (pts.at(i).x <= 0) {
                    pts.erase(pts.end() - 1);
                } else if (pts.at(i).x <= mSpeed) {
                    pts.at(i).x = mLineWeight / 2;
                }
                i++;
            }
            float height = mPlotterRect.height - (mPlotterRect.height * ofxXRSScale(mVal, mMin, mMax));
            pts.insert(pts.begin(), ofVec2f(mPlotterRect.width, height));
        }
    
    private:
        float mVal;
        float mMin;
        float mMax;
        float mSpeed;
        ofParameter<float>* mParamF = nullptr;

        void onParam(float& value) {
            mVal = value;
            if (mVal > mMax){
                mVal = mMax;
            }   else if (mVal < mMin){
                mVal = mMin;
            }
        }
};