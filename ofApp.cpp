#include "ofApp.h"
#include "printQ.h"
#include "listenerThread.h"
//--------------------------------------------------------------
void ofApp::setup(){
	MyPrinterManager.setup();
	//MyPrinterManager.listenerObject.GetPrinters()[0].maxJobs
}

//--------------------------------------------------------------
void ofApp::update(){
	MyPrinterManager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	MyPrinterManager.draw();
}

void ofApp::exit()
{
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
