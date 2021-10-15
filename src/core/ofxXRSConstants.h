#pragma once

enum class ofxXRSPanelAnchor
{
    NO_ANCHOR = 0,
    TOP_LEFT = 1,
    TOP_RIGHT = 2,
    BOTTOM_LEFT = 3,
    BOTTOM_RIGHT = 4
};

enum class ofxXRSGraph
{
    LINES = 0,
    FILLED,
    POINTS,
    OUTLINE,
};

enum class ofxXRSAlignment
{
    LEFT = 1,
    CENTER = 2,
    RIGHT = 3
};

enum class ofxXRSInputType
{
    NUMERIC = 1,
    ALPHA_NUMERIC = 2,
    COLORPICKER = 3
};

enum class ofxXRSType
{
    LABEL = 0,
    BREAK,
    BUTTON,
    TOGGLE,
    PAD2D,
    HEADER,
    FOOTER,
    MATRIX,
    SLIDER,
    FOLDER,
    DROPDOWN,
    DROPDOWN_OPTION,
    TEXT_INPUT,
    FRAME_RATE,
    COLOR_PICKER,
    WAVE_MONITOR,
    VALUE_PLOTTER
};




