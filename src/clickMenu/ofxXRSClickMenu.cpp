#include "ofxXRSClickMenu.h"

ofxXRSClickMenu::ofxXRSClickMenu(){
	OnlyRightClick = true;
	useFont = true;
	
	font.load("ofxXRS_img/fonts/Aeonik.ttf", 9);
	phase = PHASE_WAIT;
	frame = 0;
	window_size = ofPoint(0,100);
	
	ofRegisterMouseEvents(this);
	ofRegisterKeyEvents(this);

	menu_focused = -1;
	haveChild = false;
	haveFChild = false;
	isChild = false;
	hilight.setHsb(235, 255, 255,180);
	focus_y = 1;
	Enable = true;
	open = false;
}

ofxXRSClickMenu::~ofxXRSClickMenu(){
	ofUnregisterMouseEvents(this);
	ofUnregisterKeyEvents(this);
}

std::string ofxXRSClickMenu::getName() {
	return menu_name;
}

void ofxXRSClickMenu::setName(std::string name) {
	menu_name = name;
}

void ofxXRSClickMenu::draw(){
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	if (phase == PHASE_CLICK){
		frame += 1.0f * (ofGetTargetFrameRate() * 3.33f) / ofGetFrameRate();
		window_y += (window_size.y - window_y) / 5.0f * 120.0f / ofGetFrameRate();
		window_y = MIN(window_size.y,window_y);
		ofSetColor(255, 255, 255,140);
		ofNoFill();
		ofDrawRectangle(window_pos,window_size.x,window_y);
		ofFill();
		ofSetColor(0, 0, 0,100);
		ofDrawRectangle(window_pos,window_size.x,window_y);
		
		for (size_t i = 0; i < menus.size(); i++){
			if (frame > i*3) {
				ofSetColor(255,255,255,100);
				if (i > 0) ofDrawLine(window_pos.x, window_pos.y + i*20, 
								  window_pos.x+window_size.x*(1-(powf((MIN(1.0,MAX(0.0,frame-i*3)/15.0))-1.0,4.0))),
								  window_pos.y+i*20);
				ofSetColor(255,255,255);
				std::string mes;

				//mes += menus[i].message;
				mes += menus[i].message.substr(0,MIN(menus[i].message.length(),(frame-i*3)/2));
				
				for (int j = 0;j < MAX(0,(float)menus[i].message.length()-MAX(0,(frame-i*3)/2));j++){
					mes += ofToString(static_cast<char>(ofRandom(33, 120)));
				}
				if (menus[i].isBranch) mes += " >";
				if (useFont){
					font.drawString(mes, window_pos.x+4,window_pos.y+i*20+15);					
				}else{
					ofDrawBitmapString(mes, window_pos.x+4,window_pos.y+i*20+13);					
				}
			}
			if (static_cast<size_t>(menu_focused) == i){
				ofEnableBlendMode(OF_BLENDMODE_ADD);
				ofSetColor(hilight);
//				ofRect(window_pos.x,window_pos.y+i*20, window_size.x, 20);
				ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			}
		}
		ofSetColor(hilight);
		focus_y += (menu_focused*20 - focus_y) / 2.0;
		focus_y = MAX(0,focus_y);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofDrawRectangle(window_pos.x, window_pos.y+focus_y, window_size.x, 20);
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	}
	if (phase == PHASE_SELECT){
		frame += 1.0f * (ofGetTargetFrameRate() * 3.33f) / ofGetFrameRate();
		ofSetHexColor(0xFFFFFF);
		if (frame > 7) ofSetColor(255*(ofGetFrameNum()%2));
		if (useFont){
			font.drawString(menus[menu_focused].message, window_pos.x+4,window_pos.y+menu_focused*20+15);				
		}else{
			ofDrawBitmapString(menus[menu_focused].message, window_pos.x+4,window_pos.y+menu_focused*20+13);	
		}
		
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		if (frame > 7) ofSetColor(hilight.r,hilight.g,hilight.b,255*ofGetFrameNum()%2);
		else		   ofSetColor(hilight);

		hl.a -= 20;
		ofSetColor(hl);
		ofNoFill();

		ofDrawRectangle(window_pos.x,window_pos.y+menu_focused*20+10, window_size.x, 5.0/(frame));
		ofDrawRectangle(window_pos.x, window_pos.y, window_size.x, window_size.y);
		ofFill();
		if (frame > 15){
			if (isChild){
				parent->haveChild = false;
				phase = PHASE_WAIT;
				open = false;
				delete this;
				return;
			}
			phase = PHASE_WAIT;
			open = false;
		}
	}				
	
	if (haveChild && child) child->draw();
	if (haveFChild && fchild)fchild->draw();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetRectMode(OF_RECTMODE_CORNER);
}

void ofxXRSClickMenu::setEnabled(bool val) {
	Enable = val;
}

bool ofxXRSClickMenu::getEnabled() {
	return Enable;
}

bool ofxXRSClickMenu::isOpen() {
	return open;
}

void ofxXRSClickMenu::mousePressed(ofMouseEventArgs &mouse){
	hl = hilight;
	if (haveChild) return;
	if (haveFChild)return;
	if (phase == PHASE_SELECT) return;
	if ((phase == PHASE_WAIT)&&(Enable)){
		if (((mouse.button == 2)&&(OnlyRightClick))||(!OnlyRightClick)) {
			openMenu(mouse.x,mouse.y);
		}
	}else if (phase == PHASE_CLICK){
		if (menu_focused != -1){
			update();
		}else if (menus.size() > 0) {
			menu_focused = menus.size()-1;
			update();
		}
	}
}

void ofxXRSClickMenu::mouseDragged(ofMouseEventArgs &mouse){
	
}

void ofxXRSClickMenu::mouseMoved(ofMouseEventArgs &mouse){
	if (phase == PHASE_CLICK){
		if ((!haveChild)&&(!haveFChild)){
			if ((window_pos.x < mouse.x)&&(mouse.x < window_pos.x+window_size.x)&&
				(window_pos.y < mouse.y)&&(mouse.y < window_pos.y+window_size.y)){
				menu_focused = (mouse.y - window_pos.y)/20;
			}else{
				menu_focused = -1;
			}
		}
	}
}

void ofxXRSClickMenu::mouseReleased(ofMouseEventArgs &mouse){
	
}

void ofxXRSClickMenu::keyPressed(ofKeyEventArgs &key){
}

void ofxXRSClickMenu::keyReleased(ofKeyEventArgs &key){
	
}

void ofxXRSClickMenu::addButton(std::string name){
	if ((menus.size() > 0)&&(menus[menus.size()-1].message == " X Cancel")){
		menus.pop_back();
	}
	ofxXRSCDMButton bt;
	bt.message = name;
	bt.isBranch = false;
	bt.isFader  = false;
	
	menus.push_back(bt);
	
	if ((menus.size() > 0) && menus[menus.size()-1].message != " X Cancel") addButton(" X Cancel");

	if(menus.size() > 0) {
		window_size.y = menus.size() * 20;
	} else {
		window_size.y = 20;
	}
	
	window_size.x = MAX(bt.message.length()*9,window_size.x);
}

void ofxXRSClickMenu::addSubMenu(std::string name, std::vector<std::string> *Menus){
	if ((menus.size() > 0)&&(menus[menus.size()-1].message == " X Cancel")){
		menus.pop_back();
	}
	ofxXRSCDMButton bt;
	bt.message = name;
	bt.isBranch = true;
	bt.isFader  = false;

	for (size_t i = 0; i < Menus->size(); i++){
		bt.branchmenu.push_back((*Menus)[i]);
	}
	bt.branchmenu.emplace_back("<< Back");
	menus.emplace_back(bt);
	
	if (menus[menus.size()-1].message != " X Cancel") addButton(" X Cancel");
	
	window_size.y = menus.size()*20.0f;
	window_size.x = MAX(bt.message.length()*9.0f,window_size.x);
}

void ofxXRSClickMenu::RegisterFader(std::string Menu, float *valueP){
	if ((menus.size() > 0)&&(menus[menus.size()-1].message == " X Cancel")){
		menus.pop_back();
	}
	ofxXRSCDMButton bt;
	bt.message = Menu;
	bt.isBranch = false;
	bt.isFader  = true;
	bt.fader_Pointer = valueP;
	
	menus.push_back(bt);
	
	if (menus[menus.size()-1].message != " X Cancel") addButton(" X Cancel");
	
	window_size.y = menus.size()*20;
	window_size.x = MAX(bt.message.length()*9,window_size.x);

}


void ofxXRSClickMenu::deleteComponent(std::string name){
	size_t cnt = 0;
	while (cnt < menus.size()) {
		if (name == menus[cnt].message){
			menus.erase(menus.begin()+cnt);
		}else{
			cnt++;
		}
	}
	window_size.y = menus.size()*20;
	for (size_t i = 0;i < menus.size();i++){
		window_size.x = MAX(menus[i].message.length()*9.0f,window_size.x);
	}
}

void ofxXRSClickMenu::update() {
	if ( menus.at(menu_focused).isBranch) {
		// Gen Branch menu
		if (!haveChild){
			child = new ofxXRSClickMenu();
			child->menu_name = menu_name + "::" + menus[menu_focused].message;
			for (size_t i = 0;i < menus[menu_focused].branchmenu.size();i++){
				child->addButton(menus[menu_focused].branchmenu[i]);
			}
			child->openMenu(window_pos.x+window_size.x, window_pos.y);
			child->isChild = true;
			child->parent = this;
			haveChild = true;		
		}else{
			frame = 0;
			phase = PHASE_SELECT;
		}
	}else if( menus.at(menu_focused).isFader){
		//Gen Fader
		if (!haveFChild){
			fchild = new ofxXRSClickFader(menus[menu_focused].fader_Pointer, window_pos+ofPoint(window_size.x,0));
			fchild->parent = this;
			haveFChild = true;
		}else{
			xrsCDMEvent ev;
			ev.message = menu_name + "::" + menus[menu_focused].message;
			ofNotifyEvent(xrsCDMEvent::MenuPressed,ev);

			frame = 0;
			phase = PHASE_SELECT;
		}
	}else {
		//Gen normal event handler
		frame = 0;
		phase = PHASE_SELECT;
		if (haveChild){
			
		}else{
			if (isChild){
				if (menus.at(menu_focused).message == "<< Back"){
					
				}else {
					xrsCDMEvent ev;
					ev.message = menu_name + "::" + menus[menu_focused].message;
					ofNotifyEvent(xrsCDMEvent::MenuPressed,ev);
					parent->update();
				}
			}else {
				xrsCDMEvent ev;
				ev.message = menu_name + "::" + menus[menu_focused].message;
				ofNotifyEvent(xrsCDMEvent::MenuPressed,ev);
			}
		}

	}
}

void ofxXRSClickMenu::openMenu(int x,int y){
	open = true;
	window_pos = ofPoint(x,y);
	if ((ofGetHeight() - y) < window_size.y) window_pos.y = ofGetHeight() - window_size.y;
	window_y = 0;
	frame = 0;
	menu_focused = -1;
	phase = PHASE_CLICK;		
	xrsCDMEvent ev;
	ev.message = menu_name + "::" + "mouseFix";
	ofNotifyEvent(xrsCDMEvent::MenuPressed,ev);
}

void ofxXRSClickMenu::mouseScrolled(ofMouseEventArgs& mouse) {
	
}

void ofxXRSClickMenu::mouseEntered(ofMouseEventArgs& mouse) {

}

void ofxXRSClickMenu::mouseExited(ofMouseEventArgs& mouse) {

}