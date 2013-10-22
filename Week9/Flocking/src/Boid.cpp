//
//  Boid.cpp
//  Flocking
//
//  Created by Charlie Whitney on 10/20/13.
//
//

#include "Boid.h"

Boid::Boid() {
    damping = 0.98;
}

Boid::Boid( ofVec2f position, ofVec2f velocity) {
    pos = position;
    vel = velocity;
    damping = 0.98;
}

void Boid::addForce( ofVec2f force ){
    acc += force;
}

void Boid::pullToCenter( ofVec2f center ){
    
    // if the particles stray too far, we want to pull them back on screen
    
    ofVec2f dir = pos - center;
    float dist = dir.length();
    float maxDistance = 300.0;
    
    if( dir.length() > maxDistance ) {
        dir.normalize();
        vel -= dir * ( ( dist - maxDistance ) * 0.0001 );
    }
}

void Boid::update(){
    vel += acc;
    
    // don't move too fast
    vel.limit(5.0);
    
    pos += vel;
    vel *= damping;
    
    acc.set(0);
}

void Boid::draw(){
    ofCircle(pos, 4);
}