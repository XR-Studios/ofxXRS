#pragma once
#include "ofxXRSTheme.h"

/*
    Stock Themes
*/

class ofxXRSThemeSmoke : public ofxXRSTheme{

    public:
    
        ofxXRSThemeSmoke()
        {
            stripe.visible = false;
            color.label = hex(0xF8F3F0);
            color.guiBackground = hex(0x2C3137);
            color.background = hex(0x343B41);
            color.slider.fill = hex(0x60B9ED);
            color.slider.text = hex(0xFFFFFF);
            color.inputAreaBackground = hex(0x434A50);
            color.textInput.text = hex(0xFFFFFF);
            color.textInput.highlight = hex(0x434A50);
            color.textInput.backgroundOnActive = hex(0x2C3137);
            color.backgroundOnMouseOver = hex(0x434A50);
            color.backgroundOnMouseDown = hex(0x2C3137);
            color.matrix.hover.button = hex(0x60B9ED);
            color.matrix.selected.button = hex(0x2C3137);
            init();
        }
};

class ofxXRSThemeWireframe : public ofxXRSTheme{

    public:
    
        ofxXRSThemeWireframe()
        {
            stripe.visible = false;
            color.label = hex(0x6E6E6E);
            color.icons = hex(0x6E6E6E);
            color.background = hex(0xFCFAFD);
            color.guiBackground = hex(0xD8D8DB);
            color.inputAreaBackground = hex(0xE9E9E9);
            color.slider.fill = hex(0x6E6E6E);
            color.slider.text = hex(0x6E6E6E);
            color.textInput.text = hex(0x6E6E6E);
            color.textInput.highlight = hex(0xFCFAFD);
            color.colorPicker.border = hex(0xDFDDDF);
            color.textInput.backgroundOnActive = hex(0xD1D1D1);
            color.backgroundOnMouseOver = hex(0xECECEC);
            color.backgroundOnMouseDown = hex(0xDFDDDF);
            color.matrix.normal.button = hex(0xDFDDDF);
            color.matrix.hover.button = hex(0x9C9DA1);
            color.matrix.selected.button = hex(0x6E6E6E);
            color.pad2d.line = hex(0x6E6E6E);
            color.pad2d.ball = hex(0x6E6E6E);
            color.graph.fills = hex(0x6E6E6E);
            init();
        }
};

class ofxXRSThemeMidnight : public ofxXRSTheme{

    public:
    
        ofxXRSThemeMidnight()
        {
            stripe.visible = false;
            color.label = hex(0xffffff);
            color.background = hex(0x011726);
            color.guiBackground = hex(0x4C5B66);
            color.inputAreaBackground = hex(0x273946);
            color.slider.fill = hex(0x0A1E2E);
            color.slider.text = hex(0xffffff);
            color.textInput.text = hex(0xffffff);
            color.textInput.highlight = hex(0x596872);
            color.textInput.backgroundOnActive = hex(0x0A1E2E);
            color.backgroundOnMouseOver = hex(0x273946);
            color.backgroundOnMouseDown = hex(0x000000);
            color.matrix.hover.button = hex(0x596872);
            color.matrix.selected.button = hex(0x60B1CC);
            color.graph.fills = hex(0x596872);
            init();
        }
};

class ofxXRSThemeAqua : public ofxXRSTheme{

    public:
    
        ofxXRSThemeAqua()
        {
            stripe.visible = false;
            color.label = hex(0xF8F3F0);
            color.guiBackground = hex(0xF8F3F0);
            color.background = hex(0x445966);
            color.inputAreaBackground = hex(0x61717D);
            color.slider.fill = hex(0xF8F3F0);
            color.slider.text = hex(0xFFFFFF);
            color.textInput.text = hex(0xFFFFFF);
            color.textInput.highlight = hex(0x445966);
            color.textInput.backgroundOnActive = hex(0x334553);
            color.backgroundOnMouseOver = hex(0x61717D);
            color.backgroundOnMouseDown = hex(0x334553);
            color.matrix.hover.button = hex(0x55666F);
            color.matrix.selected.button = hex(0x334553);
            init();
        }
};

class ofxXRSThemeCharcoal : public ofxXRSTheme{

    public:
    
        ofxXRSThemeCharcoal()
        {
            stripe.visible = false;
            color.label = hex(0x9C9DA1);
            color.icons = hex(0x9C9DA1);
            color.background = hex(0x28292E);
            color.guiBackground = hex(0x1E1F24);
            color.inputAreaBackground = hex(0x42424A);
            color.slider.fill = hex(0xF4BF39);
            color.slider.text = hex(0x9C9DA1);
            color.textInput.text = hex(0x9C9DA1);
            color.textInput.highlight = hex(0x28292E);
            color.colorPicker.border = hex(0xEEEEEE);
            color.textInput.backgroundOnActive = hex(0x1D1E22);
            color.backgroundOnMouseOver = hex(0x42424A);
            color.backgroundOnMouseDown = hex(0x1D1E22);
            color.matrix.hover.button = hex(0x9C9DA1);
            color.graph.fills = hex(0x9C9DA1);
            init();
        }
};

class ofxXRSThemeAutumn : public ofxXRSTheme{

    public:
    
        ofxXRSThemeAutumn()
        {
            stripe.visible = false;
            color.label = hex(0xF8F3F0);
            color.guiBackground = hex(0x7d7066);
            color.background = hex(0x4C4743);
            color.inputAreaBackground = hex(0xB5BCB2);
            color.slider.fill = hex(0xFFB230);
            color.slider.text = hex(0xF8F3F0);
            color.textInput.text = hex(0xF8F3F0);
            color.textInput.highlight = hex(0x4C4743);
            color.textInput.backgroundOnActive = hex(0x7d7066);
            color.backgroundOnMouseOver = hex(0x7d7066);
            color.backgroundOnMouseDown = hex(0x333333);
            color.matrix.hover.button = hex(0xC3A279);
            color.matrix.selected.button = hex(0x7d7066);
            init();
        }
};

class ofxXRSThemeCandy : public ofxXRSTheme{

    public:
    
        ofxXRSThemeCandy()
        {
            stripe.visible = false;
            color.label = hex(0xFFFFFF);
            color.icons = hex(0xFFFFFF);
            color.background = hex(0xFF4081);
            color.guiBackground = hex(0xEEEEEE);
            color.inputAreaBackground = hex(0xFF80AB);
            color.slider.fill = hex(0xF50057);
            color.slider.text = hex(0xFFFFFF);
            color.textInput.text = hex(0xFFFFFF);
            color.textInput.highlight = hex(0xFF4081);
            color.colorPicker.border = hex(0xDFDDDF);
            color.textInput.backgroundOnActive = hex(0xF50057);
            color.backgroundOnMouseOver = hex(0xFF80AB);
            color.backgroundOnMouseDown = hex(0xF50057);
            color.matrix.normal.button = hex(0xFFFFFF);
            color.matrix.normal.label = hex(0xFF4081);
            color.matrix.hover.button = hex(0xFF4081);
            color.matrix.selected.button = hex(0xF50057);
            color.pad2d.line = hex(0xFFFFFF);
            color.pad2d.ball = hex(0xFFFFFF);
            color.graph.fills = hex(0xF50057);
            init();
        }
};



