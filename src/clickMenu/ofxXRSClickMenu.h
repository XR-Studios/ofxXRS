#pragma once
#include "ofMain.h"
#include "gl/ofGLUtils.h"
#include "xrsCDMEvent.h"
#include "ofxXRSClickFader.h"

#define PHASE_WAIT 0
#define PHASE_CLICK 1
#define PHASE_WAIT_SELECT 2
#define PHASE_BRANCH 3
#define PHASE_SELECT 4
#define PHASE_BACK 5

struct ofxXRSCDMButton{
	std::string message;
	bool isBranch;
	bool isFader;
	float* fader_Pointer;
	std::vector<std::string> branchmenu;
};

class ofxXRSClickMenu{
	friend class ofxXRSClickFader;

public:
	ofxXRSClickMenu();
	~ofxXRSClickMenu();

	void addButton(std::string name);
	void addSubMenu(std::string name, std::vector<std::string> *Menus);
	void deleteComponent(std::string name);
	void RegisterFader(std::string Menu, float *valueP);

	std::string getName();
	void setName(std::string name);

	bool getEnabled();
	void setEnabled(bool val);

	bool isOpen();

	void draw();

	//Mouse & Key Events
	void mousePressed(ofMouseEventArgs& mouse);
	void mouseDragged(ofMouseEventArgs& mouse);
	void mouseMoved(ofMouseEventArgs& mouse);
	void mouseScrolled(ofMouseEventArgs& mouse);
	void mouseEntered(ofMouseEventArgs& mouse);
	void mouseExited(ofMouseEventArgs& mouse);
	void mouseReleased(ofMouseEventArgs& mouse);
	void keyPressed(ofKeyEventArgs  &key);
	void keyReleased(ofKeyEventArgs &key);


protected:
	ofTrueTypeFont font;
	
	ofColor hilight;
	ofColor hl;

	void openMenu(int x,int y);
	void update();

	ofPoint menu_pos;
	std::deque<ofxXRSCDMButton> menus;

	ofPoint window_pos;
	ofPoint window_size;
	float window_y;
	float focus_y;
	int phase;
	float frame;
	int menu_focused;
	
	ofxXRSClickMenu *child;
	ofxXRSClickFader *fchild;
	bool haveChild;
	bool haveFChild;
	bool isChild;
	ofxXRSClickMenu *parent;
	
	bool Enable;
	bool useFont;
	bool OnlyRightClick;
	bool open;
	std::string menu_name;
};