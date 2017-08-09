#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxUI.h"
#include "loopPlaybackThread.h"

class ofApp : public ofBaseApp, public ofxMidiListener {

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        bool bFullscreen;
    
        //////// Midi In
        void drawMidiDebug();
        void newMidiMessage(ofxMidiMessage& eventArgs);
    
        stringstream inText;
    
        ofxMidiIn midiIn;
        ofxMidiMessage midiMessage;
    
        //////// Midi Out
        ofxMidiOut midiOut;
        int channel;
        
        unsigned int currentPgm;
        int note, velocity;
        int pan, bend, touch, polytouch;
    
        bool bShowMidi;
    
        //////// GUI
        void setGUI1();
        void setGUI2();
    
        ofxUISuperCanvas *gui1;
        ofxUISuperCanvas *gui2;
    
    
        void guiEvent(ofxUIEventArgs &e);
    
        ofxUIDropDownList *ddl;
    
        ////////
        int num_notes, _num_notes, max_num_notes; //number of notes in the scale
    
        int num_hits, _num_hits, max_num_hits; //total hits per loop
        int hits_per_beat; //subdivision of beat, quarter, eighth, triplet etc
        int beats_per_bar; //meter, 2/4, 3/4, 4/4 etc
        int bars_per_loop;
    
    
        int tonic;
        vector<int> notes, scale_notes;
    
        vector<vector<ofRectangle>> matrix;
        vector<vector<bool>> pressed;
        vector<vector<bool>> _pressed;
    
        vector<string> midi_codes;
        int _mouseX, _mouseY;
        
        vector<vector<ofColor>> box_color;
        ofVec2f box_size;
        int box_spacing;
        ofVec2f matrix_pos;
        int matrix_width, matrix_height;
    
        void drawFbo();
        int offset;
    
        ofFbo fbo_matrix;
        ofRectangle matrix_container;
        int sketch_margin;
        ofColor off_color;
        ofColor on_color;
        ofColor downbeat_off_color;
        ofColor play_color;
    
        ofFbo fbo_sketch;
        ofRectangle sketch_container;
        vector<ofRectangle> saved_sketch_container;
        bool draw_sketch, _draw_sketch, bLoadSketch, bSketchSelected;
        int selected_sketch;
    
        vector<ofPolyline> paths;
        ofPolyline current_path;
        int num_strokes;
    
        ofTexture temp_texture;
        ofPixels temp_pixels;
        vector<ofTexture> saved_sketch;
    
        bool bClear, bSave, bSketch, bErase;
        bool _bErase;
    
        bool bPlayLoop, _bPlayLoop;
        int current_time, hit_interval;
        float bpm;
    
        LoopPlaybackThread loopPlayer;
    
        int line_position;
    
        
    





		
};
