//
//  loopPlaybackThread.cpp
//  magenta_midi_gui
//
//  Created by Catherine McCurry on 8/8/17.
//
//

#include <stdio.h>
#include "loopPlaybackThread.h"

void LoopPlaybackThread::start( int h, int n, int i, int c, vector<vector<bool>> p, vector<int> no, vector<vector<ofRectangle>> mat, ofxMidiOut mi ) {
    num_hits = h;
    num_notes = n;
    hit_interval = i;
    channel = c;
    pressed = p;
    notes = no;
    matrix = mat;
    midiOut = mi;
    
    playX = matrix[ 0 ][ 0 ].getX();
    playY = matrix[ 0 ][ 0 ].getY();
    playW = matrix[ 0 ][ 0 ].getWidth();
    playH = ( matrix[ 0 ][ 0 ].getHeight() + 2 ) * num_notes;
    startThread();
}

void LoopPlaybackThread::stop() {
    for ( int i = 0; i < num_hits; i ++ ) {
        for ( int j = 0; j < num_notes; j ++ ) {
            if ( pressed[ i ][ j ] ) {
                midiOut.sendNoteOff( channel, notes[ j ], 100 );
            }
        }
    }
    stopThread();
}

void LoopPlaybackThread::update( int h, int n, int i, int c, vector<vector<bool>> p, vector<int> no, vector<vector<ofRectangle>> mat, ofxMidiOut mi ) {
    num_hits = h;
    num_notes = n;
    hit_interval = i;
    channel = c;
    pressed = p;
    notes = no;
    midiOut = mi;
    matrix = mat;
    playW = matrix[ 0 ][ 0 ].getWidth();
    
    /*ofSleepMillis( hit_interval );
    
    for ( int j = 0; j < num_notes; j ++ ) {
        midiOut.sendNoteOff( channel, notes[ j ], 100 );
    }*/
}

void LoopPlaybackThread::draw() {
    ofSetColor( 255, 0, 255, 100 );
    ofDrawRectangle( playX, playY, playW, playH );
}

void LoopPlaybackThread::threadedFunction() {
    while ( isThreadRunning() ) {
        for ( int i = 0; i < num_hits; i ++ ) {
            playX = matrix[ i ][ 0 ].getX();
            for ( int j = 0; j < num_notes; j ++ ) {
                if ( pressed[ i ][ j ] ) {
                    midiOut.sendNoteOn( channel, notes[ j ], 100 );
                }
                if ( i > 0 && pressed[ i - 1 ][ j ] ) {
                    midiOut.sendNoteOff( channel, notes[ j ], 100 );
                }
                else if ( i == 0 && pressed[ num_hits ][ j ] ) {
                    midiOut.sendNoteOff( channel, notes[ j ], 100 );
                }
                
            }
            ofSleepMillis( hit_interval );
        }
    }
}
