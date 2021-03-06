#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
	ofBackground(255, 255, 255);
    
    // initialize all of our variables
	bIsRecording = false;
	playbackStartTime = 0;
	startTime = 0;
    
    // load the sound in our data folder
    loopingSound.loadSound("drawbar_c4_a.aif");
    loopingSound.setVolume(0);
    loopingSound.setLoop(true);
    loopingSound.play();
}


//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
	// -------------------------- draw the line
	ofSetColor(0,0,0);
	ofNoFill();
	ofBeginShape();
	for (int i = 0; i < pointList.size(); i++){
		ofVertex(pointList[i].x, pointList[i].y);
	}

    /* // alternate way using iterators!
     for( vector<TimePoint>::iterator it=pointList.begin(); it!=pointList.end(); it++ ){
        ofVertex( it->x, it->y );
     }
     //*/
    
	ofEndShape();
	
	// -------------------------- draw the point at the current time
	ofPoint pos = getPositionForTime(ofGetElapsedTimef() - playbackStartTime);
	ofFill();
	ofSetColor(255,0,0);
    
    // we are getting the length of the vector here, which is essentially a measure of it's size
    float vecLength = getVelocityForTime(ofGetElapsedTimef() - playbackStartTime).length();
    
    // lets go nuts and set a bunch of sound attribtues
    float soundVolume = ofMap(vecLength, 0,200, 0,1);
    loopingSound.setVolume(soundVolume);
    
    float pitch = ofMap(pos.y, 0, (float)ofGetHeight(), 2.5, 0.1);
    loopingSound.setSpeed(pitch);
    
    float pan = ofMap(pos.x, 0, (float)ofGetWidth(), -1, 1);
    loopingSound.setPan(pan);
    
    // the radius is a little bit when we test, so we'll divide it so it's not too outrageous
	ofCircle(pos.x, pos.y, vecLength/5);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
    // startTime is reset every time we press the mouse, so we're recornding the time since we've pressed the mouse
    
	TimePoint temp;
	temp.x = x;
	temp.y = y;
	temp.t = ofGetElapsedTimef() - startTime;
	pointList.push_back(temp);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    // reset all of our points
	
	bIsRecording = true;
	pointList.clear();
	startTime = ofGetElapsedTimef();
	TimePoint temp;
	temp.x = x;
	temp.y = y;
	temp.t = 0;
	pointList.push_back(temp);
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
	bIsRecording = false;
	playbackStartTime = ofGetElapsedTimef();
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
ofPoint testApp::getPositionForTime(float time){
	
	// are we recording or do we have less than 2 points? If so return a blank placeholder point.
	if (bIsRecording == true || pointList.size() < 2){
		return ofPoint(0,0,0);
	}
	
	// Now we figure out where we are time-wise in the drawing
	
    // The variable "time" we are passing in is relative to how long the program has been running, not relative to
    // our timeframe.  We essentially need a modulus operator (%).  Instead we'll mimic is.
    
    // This function acts like a big modulus. The last element in our list (pointList.size()-1) is how long the whole drawing took.
    // We'll keep subtracting it from time until it's smaller than "time".  Now we can work with it on the right time scale.  This also
    // allows it to loop!
	while (time > pointList[pointList.size()-1].t){
		time -= pointList[pointList.size()-1].t;
	}
	
	
    // Create our point we'll return
	ofPoint pos;
	
	for (int i = 0; i < pointList.size()-1; i++){
        
        // find out which two points we're between
		if (time >= pointList[i].t && time < pointList[i+1].t){
			
			// Since we want a smooth playback, we'll interpolate between these two points
			float part = time - pointList[i].t;
			float whole = pointList[i+1].t - pointList[i].t;
			float pct = part / whole;
			
			// Figure out where we are between a and b
            // We've done this before with our linear interpolation where we take a percentage of both, where both
            // percentages add up to 100.  We do this by taking "pct" from one of them and "1-pct" from the other.
			pos.x = (1-pct) * pointList[i].x + (pct) * pointList[i+1].x;
			pos.y = (1-pct) * pointList[i].y + (pct) * pointList[i+1].y;
		}
	}
	
    // return our point which represents a position interpolated between two other points
	return pos;
}

ofVec2f testApp::getVelocityForTime(float time){
    // To get the velocity we'll look at the position of two points and get their difference.
    // We want this relative to time, so we'll choose an arbitrary amount of time to look back into.  For this, I chose 0.09
    
    // We use MAX so we're never asking for a point less than 0 (because our first point exists at 0, there is nothing before it!)
    ofPoint prevPt = getPositionForTime( MAX(time - 0.09f, 0));		// Where we were 0.09 seconds ago. 
	ofPoint currPt = getPositionForTime(time);                      // Where we are now!
    
    ofPoint diff;
	diff.x = currPt.x - prevPt.x;
	diff.y = currPt.y - prevPt.y;
	
	return diff;
}


