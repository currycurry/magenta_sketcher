#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackgroundGradient( ofColor::darkBlue, ofColor::white, OF_GRADIENT_LINEAR );
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetCircleResolution(120);
    bFullscreen = false;
    
    ///////////////Midi In
    // print input ports to console
    midiIn.listPorts(); // via instance
    //ofxMidiIn::listPorts(); // via static as well
    
    // open port by number (you may need to change this)
    //midiIn.openPort(4);
    midiIn.openPort("MPKmini2");	// by name
    //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
    
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    
    // add ofApp as a listener
    midiIn.addListener(this);
    
    // print received messages to the console
    midiIn.setVerbose(true);
    
    
    ///////////////Out
    // print the available output ports to the console
    midiOut.listPorts(); // via instance
    //ofxMidiOut::listPorts(); // via static too
    
    // connect
    //midiOut.openPort(0); // by number
    midiOut.openPort("IAC Driver IAC Bus 1"); // by name
    //midiOut.openVirtualPort("ofxMidiOut"); // open a virtual port
    
    channel = 1;
    currentPgm = 0;
    note = 0;
    velocity = 0;
    pan = 0;
    bend = 0;
    touch = 0;
    polytouch = 0;
    
    bShowMidi = false;
    

    ///////////////SKETCH
    num_notes = 10;
    max_num_notes = 40;
    
    bars_per_loop = 1;
    beats_per_bar = 4;
    hits_per_beat = 4;
    num_hits = bars_per_loop * beats_per_bar * hits_per_beat;
    max_num_hits = 256;
    
    bpm = 60;
    current_time = ofGetSystemTime();
    hit_interval = 60000 / ( bpm * hits_per_beat ); // ms/hit
    
    _num_notes = num_notes;
    _num_hits = num_hits;
    
    off_color = ofColor( 50 );
    on_color = ofColor( 255 );
    downbeat_off_color = ofColor( 75 );
    
    tonic = 48;
    /*C = 48;
     C# = 49;
     D = 50;
     D# = 51;
     E = 52;
     F = 53;
     F# = 54;
     G = 55;
     G# = 56;
     A = 57;
     A# = 58;
     B = 59;*/

    scale_notes.resize( max_num_notes );
    
    mode = 0;
    
    if ( mode == 0 ) {
        //pentatonic
        for ( int i = 0; i < max_num_notes; i ++ ) {
            if ( i % 5 == 0 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 5 );
            }
            if ( i % 5 == 1 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 5 ) + 2;
            }
            if ( i % 5 == 2 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 5 ) + 4;
            }
            if ( i % 5 == 3 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 5 ) + 7;
            }
            if ( i % 5 == 4 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 5 ) + 9;
            }
            cout << "scale_notes: " << scale_notes[ i ] << ", " << endl;
        }
    }
    
    if ( mode == 1 ) {
        //major
        for ( int i = 0; i < max_num_notes; i ++ ) {
            if ( i % 7 == 0 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 );
            }
            if ( i % 7 == 1 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 2;
            }
            if ( i % 7 == 2 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 4;
            }
            if ( i % 7 == 3 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 5;
            }
            if ( i % 7 == 4 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 7;
            }
            if ( i % 7 == 5 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 9;
            }
            if ( i % 7 == 6 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 11;
            }
            cout << "scale_notes: " << scale_notes[ i ] << ", " << endl;
        }
    }
    
    if ( mode == 3 ) {
        //minor
        for ( int i = 0; i < max_num_notes; i ++ ) {
            if ( i % 7 == 0 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 );
            }
            if ( i % 7 == 1 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 2;
            }
            if ( i % 7 == 2 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 3;
            }
            if ( i % 7 == 3 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 5;
            }
            if ( i % 7 == 4 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 7;
            }
            if ( i % 7 == 5 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 8;
            }
            if ( i % 7 == 6 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 10;
            }
            cout << "scale_notes: " << scale_notes[ i ] << ", " << endl;
        }
    }
    
    notes.resize( num_notes );
    for ( int i = 0; i < num_notes; i ++ ) {
        notes[ i ] = scale_notes[ num_notes - 1 - i ];
        cout << "notes: " << notes[ i ] << ", " << endl;

    }
    
    sketch_margin = 5;
    box_spacing = 2;
    offset = 214;
    
    sketch_container.setPosition( offset + sketch_margin, sketch_margin );
    sketch_container.setWidth( (ofGetWidth() - offset) / 2 - sketch_margin * 2 );
    sketch_container.setHeight( sketch_container.getWidth() * num_notes / num_hits );
    
    saved_sketch_container.resize( 8 );
    for ( int i = 0; i < saved_sketch_container.size(); i ++ ) {
        saved_sketch_container[ i ].setPosition( sketch_container.x + i % 4 * (( sketch_container.width - 3 * box_spacing ) / 4 + box_spacing), sketch_container.getHeight() + sketch_margin * 2 + floor( i / 4 ) * ( sketch_container.height / 4 + 2 ) );
        saved_sketch_container[ i ].setWidth(( sketch_container.width - 3 * box_spacing ) / 4);
        saved_sketch_container[ i ].setHeight( sketch_container.height / 4 );
    }
    
    fbo_sketch.allocate( sketch_container.width, sketch_container.height, GL_RGB );
    fbo_sketch.begin();
    ofClear( 255, 255, 255 );
    fbo_sketch.end();
    
    temp_pixels.allocate( fbo_sketch.getWidth(), fbo_sketch.getHeight(), GL_RGB );
    
    matrix_container.setPosition( sketch_container.getMaxX() + sketch_margin, sketch_margin );
    matrix_container.setSize( sketch_container.getWidth(), sketch_container.getHeight() );
    
    fbo_matrix.allocate( matrix_container.width, matrix_container.height, GL_RGB );
    fbo_sketch.begin();
    ofClear( 255, 255, 255 );
    fbo_sketch.end();
    
    box_size.x = ( matrix_container.getWidth() - box_spacing * ( num_hits - 1 ) ) / num_hits;
    box_size.y = ( matrix_container.getHeight() - box_spacing * ( num_notes - 1 ) ) / num_notes;
    
    matrix_width = box_size.x * num_hits + box_spacing * ( num_hits - 1 );
    matrix_height = box_size.y * num_notes + box_spacing * ( num_notes - 1 );
    matrix.resize( max_num_hits, vector<ofRectangle>( max_num_notes ) );
    matrix_pos.set(( matrix_container.getWidth() - matrix_width ) / 2, ( matrix_container.getHeight() - matrix_height ) / 2 );
    
    box_color.resize( max_num_hits, vector<ofColor>( max_num_notes, off_color) );
    
    for ( int i = 0; i < num_hits; i ++ ) {
        for ( int j = 0; j < num_notes; j ++ ) {
            matrix[ i ][ j ].setPosition( matrix_pos.x + i * ( box_size.x + box_spacing ), matrix_pos.y + j * ( box_size.y + box_spacing ));
            matrix[ i ][ j ].setSize( box_size.x, box_size.y );
            
            if ( i % hits_per_beat == 0 ) {
                box_color[ i ][ j ] = downbeat_off_color;
            }
            else {
                box_color[ i ][ j ] = off_color;
            }

            
        }
    }
    
    pressed.resize( max_num_hits, vector<bool>( max_num_notes, false ));
    _pressed.resize( max_num_hits, vector<bool>( max_num_notes, false ));
    
    temp_pixels.allocate( fbo_sketch.getWidth(), fbo_sketch.getHeight(), OF_PIXELS_RGB );
    temp_pixels.setColor( ofColor::black );
    
    temp_texture.allocate( fbo_sketch.getWidth(), fbo_sketch.getHeight(), GL_RGB );
    temp_texture.loadData( temp_pixels );
    
    latent_input_container.resize( 4 );
    latent_sketch.resize( 4 );
    for ( int i = 0; i < latent_input_container.size(); i ++ ) {
        latent_input_container[ i ].setWidth( saved_sketch_container[ 0 ].getWidth() );
        latent_input_container[ i ].setHeight( saved_sketch_container[ 0 ].getHeight() );
        latent_sketch[ i ].allocate( fbo_sketch.getWidth(), fbo_sketch.getHeight(), GL_RGB );
        latent_sketch[ i ].loadData( temp_pixels );

    }
    
    latent_container.setWidth( 400 );
    latent_container.setHeight( latent_container.getWidth() );
    latent_container.setPosition( ofGetWidth() - latent_input_container[ 0 ].getWidth() - sketch_margin - latent_container.getWidth(), ofGetHeight() - sketch_margin - latent_container.getHeight());
    
    latent_input_container[ 0 ].setPosition( latent_container.getX() - latent_input_container[ 0 ].getWidth(), latent_container.getY() );
    latent_input_container[ 1 ].setPosition( ofGetWidth() - latent_input_container[ 1 ].getWidth() - sketch_margin, latent_container.getY() );
    latent_input_container[ 2 ].setPosition( latent_input_container[ 1 ].getX(), latent_container.getMaxY() - latent_input_container[ 2 ].getHeight() );
    latent_input_container[ 3 ].setPosition( latent_input_container[ 0 ].getX(), latent_input_container[ 2 ].getY() );

    bClear = true;
    bSave = false;
    bSketch = true;
    bLoadSketch = false;
    bErase = false;
    _bErase = false;
    

    
    ofSetCircleResolution( 100 );
    ofSetLineWidth( 5 );
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    midi_codes.resize( max_num_hits, "( " );
    
    ///////////////GUI
    ddl = NULL;
    setGUI1();
    setGUI2();
    gui1->loadSettings("gui1Settings.xml");
    gui2->loadSettings("gui2Settings.xml");
    
    bPlayLoop = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetFullscreen( bFullscreen );
    
    if ( bPlayLoop ) {
        if ( !_bPlayLoop ) {
            loopPlayer.start( num_hits, num_notes, hit_interval, channel, pressed, notes, matrix, midiOut );
            cout << "startThread" << endl;

        }
        
    }
    else if ( !bPlayLoop && _bPlayLoop ) {
        loopPlayer.stop();
        cout << "stopThread" << endl;

    }


    // check when grid is resized
    if ( _num_notes != num_notes || _num_hits != num_hits ) {
        cout << "num_notes changed" << endl;
        midi_codes.clear();
        midi_codes.resize( num_hits, "( " );
        
        box_spacing = 2;
        box_size.x = ( matrix_container.getWidth() - box_spacing * ( num_hits - 1 ) ) / num_hits;
        box_size.y = ( matrix_container.getHeight() - box_spacing * ( num_notes - 1 ) ) / num_notes;
        
        matrix_width = box_size.x * num_hits + box_spacing * ( num_hits - 1 );
        matrix_height = box_size.y * num_notes + box_spacing * ( num_notes - 1 );
        
        matrix_pos.set(( matrix_container.getWidth() - matrix_width ) / 2, ( matrix_container.getHeight() - matrix_height ) / 2 );
        
        for ( int i = 0; i < num_hits; i ++ ) {
            for ( int j = 0; j < num_notes; j ++ ) {
                matrix[ i ][ j ].setPosition( matrix_pos.x + i * ( box_size.x + box_spacing ), matrix_pos.y + j * ( box_size.y + box_spacing ));
                matrix[ i ][ j ].setSize( box_size.x, box_size.y );
                
                if ( pressed[ i ][ j ] ) {
                    box_color[ i ][ j ] = on_color;
                }
                
                if ( !pressed[ i ][ j ] ) {
                    if ( i % hits_per_beat == 0 ) {
                        box_color[ i ][ j ] = downbeat_off_color;
                    }
                    else {
                        box_color[ i ][ j ] = off_color;
                    }
                }
                
            }
        }
        
        notes.clear();
        notes.resize( num_notes );
        for ( int i = 0; i < num_notes; i ++ ) {
            notes[ i ] = scale_notes[ num_notes - 1 - i ];
            //cout << "notes: " << notes[ i ] << ", " << endl;
            
        }
        
        loopPlayer.update( num_hits, num_notes, hit_interval, channel, pressed, notes, matrix, midiOut );

    }
    
    // look through for newly added notes
    for ( int i = 0; i < num_hits; i ++ ) {
        for ( int j = 0; j < num_notes; j ++ ) {
            if ( pressed[ i ][ j ] ) {
                if ( !_pressed[ i ][ j ] ) {
                    midiOut.sendNoteOn( channel, notes[ j ], 100 );
                }
                if ( j < num_notes - 1) {
                    midi_codes[ i ] += ofToString( notes[ j ] ) + ", ";
                }
                
                else {
                    midi_codes[ i ] += ofToString( notes[ j ] );
                }
            }
        }
        midi_codes[ i ] += " )";
        loopPlayer.update( num_hits, num_notes, hit_interval, channel, pressed, notes, matrix, midiOut );
    }
    
    //clear sketch and matrix
    if ( bClear == true ) {
        for ( int i = 0; i < num_hits; i ++ ) {
            for ( int j = 0; j < num_notes; j ++ ) {
                if ( i % hits_per_beat ==0 ) {
                    box_color[ i ][ j ] = downbeat_off_color;
                }
                else {
                    box_color[ i ][ j ] = off_color;
                }
                
                pressed[ i ][ j ] = false;
            }
        }
    }
    
    drawFbo();
    fbo_sketch.readToPixels( temp_pixels );
    
    if ( bSketch ){
        
        for ( int i = 0; i < num_hits; i ++ ) {
            for ( int j = 0; j < num_notes; j ++ ) {
                
                int r = 0;
                int g = 0;
                int b = 0;
                ofColor average_color = ofColor( 0, 0, 0 );
                for ( int x = 0; x < box_size.x; x ++ ) {
                    for ( int y = 0; y < box_size.y; y ++ ) {
                        ofColor pixelColor = temp_pixels.getColor( matrix[ i ][ j ].getMinX() + x, matrix[ i ][ j ].getMinY() + y );
                        r += pixelColor.r;
                    }
                }
                int samples = box_size.x * box_size.y; // The number of pixels we are averaging
                
                // Update the average color
                average_color.r = r / samples;
                
                if ( average_color.r > 20 ) {
                    pressed[ i ][ j ] = true;
                    box_color[ i ][ j ] = on_color;
                }
                else {
                    if ( i % hits_per_beat ==0 ) {
                        box_color[ i ][ j ] = downbeat_off_color;
                    }
                    else {
                        box_color[ i ][ j ] = off_color;
                    }
                    pressed[ i ][ j ] = false;
                }
                
                if ( pressed[ i ][ j ] && !_pressed[ i ][ j ] ) {
                    midiOut.sendNoteOn( channel, notes[ j ], 100 );
                }
            }
        }
    }
    

    
    draw_sketch = false;
    _pressed = pressed;
    _num_notes = num_notes;
    _num_hits = num_hits;
    _bPlayLoop = bPlayLoop;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient( ofColor::darkBlue, ofColor::white, OF_GRADIENT_LINEAR );

    if ( bShowMidi ){
        drawMidiDebug();
 
    }
    
    ofSetColor( 255 );
    fbo_matrix.draw( matrix_container );
    fbo_sketch.draw( sketch_container );
    
    
    ofSetColor( 255 );
    for ( int i = 0; i < saved_sketch.size(); i ++ ) {
        if ( i < saved_sketch_container.size() ) {
            saved_sketch[ i ].draw( saved_sketch_container[ i ] );
        }
    }
    
    if ( bSketchSelected ) {
        ofSetColor( 255, 0, 255, 75 );
        ofDrawRectangle( saved_sketch_container[ selected_sketch ]);
        ofSetColor( 255 );
    }
    
    for ( int i = 0; i < latent_input_container.size(); i ++ ) {
        ofSetColor( 0 );
        ofDrawRectangle( latent_input_container[ i ] );
        ofSetColor(255);
        latent_sketch[ i ].draw( latent_input_container[ i ] );
    }
    
    ofSetColor( 50 );
    ofDrawRectangle( latent_container );
    ofSetColor( 255 );
    
    bClear = false;
    bSave = false;
    
}
//--------------------------------------------------------------
void ofApp::drawFbo(){
    
    fbo_sketch.begin();
    ofEnableAlphaBlending();
    if( bClear ) {
        ofBackground( 0 );
        
    }
    
    if ( bLoadSketch ) {
        paths.clear();
        ofBackground( 0 );
        temp_texture.draw( 0, 0 );
    }
    
    if ( _bErase != bErase ) {
        temp_texture.draw( 0, 0 );
    }
    
    if ( !bErase ) {
        ofSetColor( 255 );
        ofSetLineWidth( 5 );
        if ( draw_sketch ) {
            current_path.addVertex( mouseX - offset, mouseY );
            current_path.draw();
        }
        
        if ( !draw_sketch && draw_sketch != _draw_sketch ){
            paths.push_back( current_path );
            num_strokes ++;
            current_path.clear();
        }
        
        for ( int i = 0; i < paths.size(); i ++ ) {
            paths[ i ].draw();
        }
        
    }
    
    else {
        
        ofSetColor( 0 );
        if ( draw_sketch ) {
            ofDrawCircle( mouseX - offset, mouseY, 20 );
        }
        
    }
    ofDisableAlphaBlending();
    fbo_sketch.end();
    
    fbo_matrix.begin();
    ofClear( 0 );
    ofBackground( 0 );
    for( int i = 0; i < num_hits; i ++ ) {
        for ( int j = 0; j < num_notes; j ++ ) {
            ofSetColor( box_color[ i ][ j ] );
            ofDrawRectangle( matrix[ i ][ j ] );
        }
    }
    if ( bPlayLoop ) {
       loopPlayer.draw();
    }
    
    fbo_matrix.end();
    
    bLoadSketch = false;
    _draw_sketch = draw_sketch;
    _bErase = bErase;
    
}

//--------------------------------------------------------------
void ofApp::drawMidiDebug() {
    ///////////////In
    // draw the last recieved message contents to the screen
    ofSetColor(255);
    inText << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofDrawBitmapString(inText.str(), 214, 20);
    inText.str(""); // clear
    
    inText << "channel: " << midiMessage.channel;
    ofDrawBitmapString(inText.str(), 214, 34);
    inText.str(""); // clear
    
    inText << "pitch: " << midiMessage.pitch;
    ofDrawBitmapString(inText.str(), 214, 48);
    inText.str(""); // clear
    ofDrawRectangle(214, 58, ofMap(midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);
    
    inText << "velocity: " << midiMessage.velocity;
    ofDrawBitmapString(inText.str(), 214, 96);
    inText.str(""); // clear
    ofDrawRectangle(214, 105, ofMap(midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);
    
    inText << "control: " << midiMessage.control;
    ofDrawBitmapString(inText.str(), 214, 144);
    inText.str(""); // clear
    ofDrawRectangle(214, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
    
    inText << "value: " << midiMessage.value;
    ofDrawBitmapString(inText.str(), 214, 192);
    inText.str(""); // clear
    if(midiMessage.status == MIDI_PITCH_BEND) {
        ofDrawRectangle(214, 202, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
    }
    else {
        ofDrawRectangle(214, 202, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
    }
    
    inText << "delta: " << midiMessage.deltatime;
    ofDrawBitmapString(inText.str(), 214, 240);
    inText.str(""); // clear
    
    
    ///////////////Out
    // let's see something
    ofSetColor(0);
    stringstream outText;
    outText << "connected to port " << midiOut.getPort()
    << " \"" << midiOut.getName() << "\"" << endl
    << "is virtual?: " << midiOut.isVirtual() << endl << endl
    << "sending to channel " << channel << endl << endl
    << "current program: " << currentPgm << endl << endl
    << "note: " << note << endl
    << "velocity: " << velocity << endl
    << "pan: " << pan << endl
    << "bend: " << bend << endl
    << "touch: " << touch << endl
    << "polytouch: " << polytouch;
    ofDrawBitmapString(outText.str(), 214, ofGetHeight()/2);
    
    
    for( int i = 0; i < num_hits; i ++ ) {
        ofSetColor( 0, 255, 255 );
        ofDrawBitmapString( midi_codes[ i ], matrix_container.getMinX(), matrix[ i ][ num_notes - 1 ].y + matrix[ i ][ num_notes - 1 ].height + ( i + 1) * 20);
    }
    
    for( int i = 0; i < num_hits; i ++ ) {
        midi_codes[ i ] = "( ";
    }

}
//--------------------------------------------------------------
void ofApp::exit() {
    
    // clean up
    midiIn.closePort();
    midiIn.removeListener(this);
    
    midiOut.closePort();
    
    gui1->saveSettings("gui1Settings.xml");
    loopPlayer.stop();

}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    // make a copy of the latest message
    midiMessage = msg;
    
    note = midiMessage.pitch;
    velocity = midiMessage.velocity;
    midiOut.sendNoteOn( channel, note, velocity );

}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
    int kind = e.getKind();
    cout << "got event from: " << name << endl;

    if(kind == OFX_UI_WIDGET_NUMBERDIALER)
    {
        ofxUINumberDialer *n = (ofxUINumberDialer *) e.widget;
        cout << n->getValue() << endl;
    }
    
    if ( name == "CLEAR LOOP" ){
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        bClear = button->getValue();
        if ( bClear ) {
            paths.clear();
            num_strokes = 0;
        }
    }
    else if ( name == "SAVE LOOP" ){
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        bSave = button->getValue();
        if ( bSave ) {
            fbo_sketch.readToPixels( temp_pixels );
            temp_texture.clear();
            
            temp_texture.loadData( temp_pixels );
            saved_sketch.push_back( temp_texture );
            paths.clear();

        }
    }
    else if(name == "SKETCH / EDIT MIDI")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        bSketch = toggle->getValue();
        
    }
    else if(name == "ERASE")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        bErase = toggle->getValue();
        
        //if ( bErase ) {
            fbo_sketch.readToPixels( temp_pixels );
            temp_texture.clear();
            temp_texture.loadData( temp_pixels );
            paths.clear();
        //}
    }
    
    else if(name == "PLAY LOOP")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        bPlayLoop = toggle->getValue();
               
    }
    
    else if(name == "pentatonic")
    {
        mode = 0;
        //pentatonic
        for ( int i = 0; i < max_num_notes; i ++ ) {
            if ( i % 5 == 0 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 5 );
            }
            if ( i % 5 == 1 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 5 ) + 2;
            }
            if ( i % 5 == 2 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 5 ) + 4;
            }
            if ( i % 5 == 3 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 5 ) + 7;
            }
            if ( i % 5 == 4 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 5 ) + 9;
            }
            cout << "scale_notes: " << scale_notes[ i ] << ", " << endl;
        }
        
        notes.resize( num_notes );
        for ( int i = 0; i < num_notes; i ++ ) {
            notes[ i ] = scale_notes[ num_notes - 1 - i ];
            cout << "notes: " << notes[ i ] << ", " << endl;
            
        }
        
    }
    else if(name == "major")
    {
        mode = 1;
        //major
        for ( int i = 0; i < max_num_notes; i ++ ) {
            if ( i % 7 == 0 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 );
            }
            if ( i % 7 == 1 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 2;
            }
            if ( i % 7 == 2 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 4;
            }
            if ( i % 7 == 3 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 5;
            }
            if ( i % 7 == 4 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 7;
            }
            if ( i % 7 == 5 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 9;
            }
            if ( i % 7 == 6 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 11;
            }
            cout << "scale_notes: " << scale_notes[ i ] << ", " << endl;
        }
        notes.resize( num_notes );
        for ( int i = 0; i < num_notes; i ++ ) {
            notes[ i ] = scale_notes[ num_notes - 1 - i ];
            cout << "notes: " << notes[ i ] << ", " << endl;
            
        }
        
    }
    else if(name == "minor")
    {
        mode = 2;
        //minor
        for ( int i = 0; i < max_num_notes; i ++ ) {
            if ( i % 7 == 0 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 );
            }
            if ( i % 7 == 1 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 2;
            }
            if ( i % 7 == 2 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 3;
            }
            if ( i % 7 == 3 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 5;
            }
            if ( i % 7 == 4 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 7;
            }
            if ( i % 7 == 5 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 8;
            }
            if ( i % 7 == 6 ) {
                scale_notes[ i ] = tonic + 12 * floor( i / 7 ) + 10;
            }
            cout << "scale_notes: " << scale_notes[ i ] << ", " << endl;
        }
        notes.resize( num_notes );
        for ( int i = 0; i < num_notes; i ++ ) {
            notes[ i ] = scale_notes[ num_notes - 1 - i ];
            cout << "notes: " << notes[ i ] << ", " << endl;
            
        }

        
    }
    
    num_hits = bars_per_loop * beats_per_bar * hits_per_beat;
    hit_interval = 60000 / ( bpm * hits_per_beat ); // ms/hit
    loopPlayer.update( num_hits, num_notes, hit_interval, channel, pressed, notes, matrix, midiOut );
    
    
}

//--------------------------------------------------------------
void ofApp::setGUI1()
{
    gui1 = new ofxUISuperCanvas("INPUT CONTROLS");
    
    gui1->addSpacer();
    gui1->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    gui1->addToggle("PLAY LOOP", false, 44, 44);
    gui1->addButton("SAVE LOOP", false, 44, 44);
    gui1->addButton("CLEAR LOOP", false, 44, 44);
    gui1->addToggle("SKETCH / EDIT MIDI", true, 44, 44);
    gui1->addToggle("ERASE", false, 44, 44 );
    
    gui1->addSpacer();
    gui1->addLabel("EDIT LOOP MATRIX");
    gui1->addIntSlider( "NOTES IN SCALE", 5, max_num_notes, &num_notes );
    gui1->addIntSlider( "BARS PER LOOP", 1, 4, &bars_per_loop );
    gui1->addIntSlider( "BEATS PER BAR", 1, 8, &beats_per_bar );
    gui1->addIntSlider( "HITS PER BEAT", 1, 8, &hits_per_beat );

    gui1->addSpacer();
    gui1->addLabel("EDIT SCALE", OFX_UI_FONT_MEDIUM);
    vector<string> items;
    items.push_back( "C" );
    items.push_back( "C#" );
    items.push_back( "D" );
    items.push_back( "D#" );
    items.push_back( "E" );
    items.push_back( "F" );
    items.push_back( "F" );
    items.push_back( "F#" );
    items.push_back( "G" );
    items.push_back( "G#" );
    items.push_back( "A" );
    items.push_back( "A#" );
    items.push_back( "B" );
    ddl = gui1->addDropDownList("CHOOSE TONIC", items);
    ddl->setAllowMultiple(false);
    
    vector<string> scale;
    scale.push_back( "pentatonic" );
    scale.push_back( "major" );
    scale.push_back( "minor" );
    /*scale.push_back( "diatonic" );
    scale.push_back( "melodic" );
    scale.push_back( "harmonic" );
    scale.push_back( "phrygian" );
    scale.push_back( "lydian" );
    scale.push_back( "mixolydian" );
    scale.push_back( "dorian" );
    scale.push_back( "ionian" );
    scale.push_back( "locrian" );*/
    ddl = gui1->addDropDownList("CHOOSE MODE", scale);
    ddl->setAllowMultiple(false);

    gui1->setPosition(0,0);
    gui1->autoSizeToFitWidgets();
    
    ofAddListener(gui1->newGUIEvent,this,&ofApp::guiEvent);
}
//--------------------------------------------------------------
void ofApp::setGUI2()
{
    gui2 = new ofxUISuperCanvas("CONTROLS");
    gui2->addSpacer();
    gui2->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    
    gui2->addLabel("BPM", OFX_UI_FONT_MEDIUM);
    gui2->addRotarySlider("BPMSLIDER", 40, 200, &bpm);
    
    gui2->addSpacer();
    gui2->addLabel("TEMPERATURE", OFX_UI_FONT_MEDIUM);
    gui2->addRotarySlider("TEMPSLIDER", 0, 100, 50 );

    gui2->setPosition( 0, 515 );
    gui2->autoSizeToFitWidgets();
    
    ofAddListener(gui2->newGUIEvent,this,&ofApp::guiEvent);

}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch( key ) {
        case 'm':
            bShowMidi = !bShowMidi;
            break;
            
        case 'f':
            bFullscreen = !bFullscreen;
            break;
            
        case OF_KEY_BACKSPACE:
            if ( bSketchSelected ) {
                saved_sketch.erase( saved_sketch.begin() + selected_sketch );
                bSketchSelected = false;
            }

    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if ( bSketch ) {
        for ( int i = 0; i < num_hits; i ++ ) {
            for( int j = 0; j < num_notes; j ++ ) {
                
                if( matrix[ i ][ j ].inside( x - matrix_container.getMinX(), y - matrix_container.getMinY() ) && !matrix[ i ][ j ].inside( _mouseX - matrix_container.getMinX(), _mouseY - matrix_container.getMinY() )) {
                    
                    pressed[ i ][ j ] = !pressed[ i ][ j ];
                    
                    if ( pressed[ i ][ j ] ) {
                        box_color[ i ][ j ] = on_color;
                    }
                    
                    if ( !pressed[ i ][ j ] ) {
                        if ( i % hits_per_beat == 0 ) {
                            box_color[ i ][ j ] = downbeat_off_color;
                        }
                        else {
                            box_color[ i ][ j ] = off_color;
                        }
                        //pressed[ i ][ j ] = false;

                        //box_color[ i ][ j ] = off_color;
                    }
                }
            }
        }
    }
    
    else {
        for ( int i = 0; i < num_hits; i ++ ) {
            for( int j = 0; j < num_notes; j ++ ) {
                if( matrix[ i ][ j ].inside( x - matrix_container.getMinX(), y - matrix_container.getMinY() ) && !matrix[ i ][ j ].inside( _mouseX - matrix_container.getMinX(), _mouseY - matrix_container.getMinY() )) {
                    pressed[ i ][ j ] = !pressed[ i ][ j ];
                    
                    if ( pressed[ i ][ j ] ) {
                        box_color[ i ][ j ] = on_color;
                    }
                    
                    if ( !pressed[ i ][ j ] ) {
                        if ( i % hits_per_beat ==0 ) {
                            box_color[ i ][ j ] = downbeat_off_color;
                        }
                        else {
                            box_color[ i ][ j ] = off_color;
                        }
                    }
                }
            }
        }
        
    }
    
    if ( sketch_container.inside( x, y ) ) {
        draw_sketch = true;
    }
    else {
        draw_sketch = false;
    }
    
    
    _mouseX = x;
    _mouseY = y;


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    for ( int i = 0; i < num_hits; i ++ ) {
        for( int j = 0; j < num_notes; j ++ ) {
            
            if( matrix[ i ][ j ].inside( x - matrix_container.getMinX(), y - matrix_container.getMinY() ) ) {
                
                pressed[ i ][ j ] = !pressed[ i ][ j ];
                
                if ( pressed[ i ][ j ] ) {
                    box_color[ i ][ j ] = on_color;
                }
                
                if ( !pressed[ i ][ j ] ) {
                    if ( i % hits_per_beat ==0 ) {
                        box_color[ i ][ j ] = downbeat_off_color;
                    }
                    else {
                        box_color[ i ][ j ] = off_color;
                    }
                }
                
            }
        }
    }
    
    
    if ( sketch_container.inside( x, y  ) ) {
        draw_sketch = true;
        if ( bSketchSelected ) {
            bLoadSketch = true;
            bSketchSelected = false;
        }
        
    }
    else {
        draw_sketch = false;
    }

    for ( int i = 0; i < latent_sketch.size(); i ++ ) {
        if ( latent_input_container[ i ].inside( x, y ) ) {
            if ( bSketchSelected ) {
                latent_sketch[ i ] = temp_texture;
                bSketchSelected = false;
            }
        }
    }
    
    for ( int i = 0; i < saved_sketch.size(); i ++ ) {
        if ( saved_sketch_container[ i ].inside( x, y ) ) {
            selected_sketch = i;
            temp_texture = saved_sketch[ i ];
            bSketchSelected = true;
        }
    }
    

    

    
    _mouseX = x;
    _mouseY = y;
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    if ( sketch_container.inside( x, y  ) ) {
        if ( bSketchSelected ) {
            bLoadSketch = true;
            bSketchSelected = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
    /*if ( bFullscreen ){
        gui2->setPosition( 0, 725 );
        gui2->autoSizeToFitWidgets();
    }
    else {
        gui2->setPosition( 214,0 );
        gui2->autoSizeToFitWidgets();
    }*/
    
    /*latent_container.setWidth( 400 );
    latent_container.setHeight( latent_container.getWidth() );
    latent_container.setPosition( ofGetWidth() - latent_input_container[ 0 ].getWidth() - sketch_margin - latent_container.getWidth(), ofGetHeight() - sketch_margin - latent_container.getHeight());
    
    latent_input_container[ 0 ].setPosition( latent_container.getX() - latent_input_container[ 0 ].getWidth(), latent_container.getY() );
    latent_input_container[ 1 ].setPosition( ofGetWidth() - latent_input_container[ 1 ].getWidth() - sketch_margin, latent_container.getY() );
    latent_input_container[ 2 ].setPosition( latent_input_container[ 1 ].getX(), latent_container.getMaxY() - latent_input_container[ 2 ].getHeight() );
    latent_input_container[ 3 ].setPosition( latent_input_container[ 0 ].getX(), latent_input_container[ 2 ].getY() );*/
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
