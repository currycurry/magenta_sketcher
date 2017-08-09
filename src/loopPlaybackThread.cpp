//
//  loopPlaybackThread.cpp
//  magenta_midi_gui
//
//  Created by Catherine McCurry on 8/8/17.
//
//

#include <stdio.h>
#include "loopPlaybackThread.h"

void LoopPlaybackThread::start( int h, int n, int i, int c, vector<vector<bool>> p, vector<int> no, ofxMidiOut mi ) {
    num_hits = h;
    num_notes = n;
    hit_interval = i;
    channel = c;
    pressed = p;
    notes = no;
    midiOut = mi;
    startThread();
}

void LoopPlaybackThread::stop() {
    stopThread();
}
void LoopPlaybackThread::update( int h, int n, int i, int c, vector<vector<bool>> p, vector<int> no, ofxMidiOut mi ) {
    num_hits = h;
    num_notes = n;
    hit_interval = i;
    channel = c;
    pressed = p;
    notes = no;
    midiOut = mi;

}

void LoopPlaybackThread::threadedFunction() {
    while ( isThreadRunning() ) {
        for ( int i = 0; i < num_hits; i ++ ) {
            for ( int j = 0; j < num_notes; j ++ ) {
                if ( pressed[ i ][ j ] ) {
                    midiOut.sendNoteOn( channel, notes[ j ], 100 );
                }
            }
            ofSleepMillis( hit_interval );
        }
    }
}
