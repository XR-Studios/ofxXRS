#pragma once
#include "ofxXRSThemes.h"
#include "ofxXRSEvents.h"
#include "ofxXRSConstants.h"

namespace ofxXRSMsg
{
    const string EVENT_HANDLER_NULL = "[WARNING] :: Event Handler Not Set";
    const string COMPONENT_NOT_FOUND = "[ERROR] :: Component Not Found";
    const string MATRIX_EMPTY = "[WARNING] :: Matrix is Empty";
}

class ofxXRSLog {

    public:
        static void write(string m1, string m2="")
        {
            if (!mQuiet) {
                cout << m1;
                if (m2!="") cout << " : " << m2;
                cout << endl;
            }
        }
        static void quiet()
        {
            mQuiet = true;
        }
        static bool mQuiet;
};

inline static float ofxXRSScale(float val, float min, float max)
{
    if (min<0){
        float n = abs(min);
        float a = min+n;
        float b = max+n;
        float c = val+n;
        return (c-a)/(b-a);
    }   else{
        return (val-min)/(max-min);
    }
}

class ofxXRSInteractiveObject{

    public:

    // button events //
        typedef std::function<void(ofxXRSButtonEvent)> onButtonEventCallback;
        onButtonEventCallback buttonEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onButtonEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            buttonEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
        }
    
        void onButtonEvent(onButtonEventCallback callback) {
            buttonEventCallback = callback;
        }
    
    // toggle events //
        typedef std::function<void(ofxXRSToggleEvent)> onToggleEventCallback;
        onToggleEventCallback toggleEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onToggleEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            toggleEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
        }

        void onToggleEvent(onToggleEventCallback callback) {
            toggleEventCallback = callback;
        }
    
    // slider events //
        typedef std::function<void(ofxXRSSliderEvent)> onSliderEventCallback;
        onSliderEventCallback sliderEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onSliderEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            sliderEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
        }
    
        void onSliderEvent(onSliderEventCallback callback) {
            sliderEventCallback = callback;
        }

    // text input events //
        typedef std::function<void(ofxXRSTextInputEvent)> onTextInputEventCallback;
        onTextInputEventCallback textInputEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onTextInputEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            textInputEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
        }
    
        void onTextInputEvent(onTextInputEventCallback callback) {
            textInputEventCallback = callback;
        }

    // color picker events //
        typedef std::function<void(ofxXRSColorPickerEvent)> onColorPickerEventCallback;
        onColorPickerEventCallback colorPickerEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onColorPickerEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            colorPickerEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
        }
    
        void onColorPickerEvent(onColorPickerEventCallback callback) {
            colorPickerEventCallback = callback;
        }
    
    // dropdown events //
        typedef std::function<void(ofxXRSDropdownEvent)> onDropdownEventCallback;
        onDropdownEventCallback dropdownEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onDropdownEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            dropdownEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
        }
    
        void onDropdownEvent(onDropdownEventCallback callback) {
            dropdownEventCallback = callback;
        }

    // 2d pad events //
        typedef std::function<void(ofxXRS2dPadEvent)> on2dPadEventCallback;
        on2dPadEventCallback pad2dEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void on2dPadEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            pad2dEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
        }
    
        void on2dPadEvent(on2dPadEventCallback callback) {
            pad2dEventCallback = callback;
        }

    // matrix events //
        typedef std::function<void(ofxXRSMatrixEvent)> onMatrixEventCallback;
        onMatrixEventCallback matrixEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onMatrixEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            matrixEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
        }
    
        void onMatrixEvent(onMatrixEventCallback callback) {
            matrixEventCallback = callback;
        }

    // scrollview events //
        typedef std::function<void(ofxXRSScrollViewEvent)> onScrollViewEventCallback;
        onScrollViewEventCallback scrollViewEventCallback;
    
        template<typename T, typename args, class ListenerClass>
        void onScrollViewEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            scrollViewEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
        }

        void onScrollViewEvent(onScrollViewEventCallback callback) {
            scrollViewEventCallback = callback;
        }

    // internal events //
        typedef std::function<void(ofxXRSInternalEvent)> onInternalEventCallback;
        onInternalEventCallback internalEventCallback;
        
        template<typename T, typename args, class ListenerClass>
        void onInternalEvent(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
            internalEventCallback = std::bind(listenerMethod, owner, std::placeholders::_1);
        }

        void onInternalEvent(onInternalEventCallback callback) {
            internalEventCallback = callback;
        }
};

