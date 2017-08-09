//
//  loopPlaybackThread.h
//  magenta_midi_gui
//
//  Created by Catherine McCurry on 8/7/17.
//
//

#ifndef loopPlaybackThread_h
#define loopPlaybackThread_h

#pragma once

#include "ofMain.h"
#include "ofThread.h"
#include "ofxMidi.h"


class LoopPlaybackThread: public ofThread {
public:
    void start( int, int, int, int, vector<vector<bool>>, vector<int>, vector<vector<ofRectangle>>, ofxMidiOut );
    void stop();
    void update( int, int, int, int, vector<vector<bool>>, vector<int>, vector<vector<ofRectangle>>, ofxMidiOut );
    void draw();
    
private:
    void threadedFunction();
    int num_hits, num_notes, hit_interval, channel;
    vector<vector<bool>> pressed;
    vector<int> notes;
    vector<vector<ofRectangle>> matrix;

    ofxMidiOut midiOut;
    float playX, playY, playW, playH;
    ofColor playColor;
};

#endif

