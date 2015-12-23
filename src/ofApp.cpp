#include "ofApp.h"
#include "ofxHttpUtils.h"

#define PEEPBOXSIDELENGTH 200

//--------------------------------------------------------------
void ofApp::setup(){


    ofAddListener(httpUtils.newResponseEvent,this,&ofApp::newResponse);
    httpUtils.start();

    ofSetFrameRate(5);

    screenHeightRetina = ofGetScreenHeight()*2;
    screenWidthRetina = ofGetScreenWidth()*2;
//    cout << screenHeightRetina << " " << screenWidthRetina << "\n";
    //Initialize the img variable to take full size screen shots
    img.allocate(screenWidthRetina, screenHeightRetina, OF_IMAGE_COLOR_ALPHA);
    

}

//--------------------------------------------------------------
void ofApp::update(){
    vector<ofxDisplay *> t = ofxDisplay::getActiveDisplays();
    grabImage(img, t[0]->displayId);
    

    //Set the cursor point to be the center of the peep show, handle borders elegantly
    int mousePosXcrop = ofClamp(mousePosX*2 - PEEPBOXSIDELENGTH/2, 0, screenWidthRetina);
    int mousePosYcrop = ofClamp(mousePosY*2 - PEEPBOXSIDELENGTH/2, 0, screenHeightRetina);
    
    //Copy subset of
    img2.cropFrom(img, mousePosXcrop, mousePosYcrop, PEEPBOXSIDELENGTH, PEEPBOXSIDELENGTH);

    img2.save("tempoutput.png");
    ofxHttpForm form;
    form.action = "http://peepshow.herokuapp.com/image";
    form.method = OFX_HTTP_POST;
    form.addFile("image","tempoutput.png");
    
    httpUtils.addForm(form);
    
}

//--------------------------------------------------------------
void ofApp::draw(){


    img2.draw(0,0,200,200);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    mousePosY = y + ofGetWindowPositionY();
    mousePosX = x + ofGetWindowPositionX();
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

void ofApp::newResponse(ofxHttpResponse & response){
    cout << (string)response.responseBody << "\n";
}

//---A copy
void ofApp::grabImage(ofImage& image, uint32 displayId) {
#ifdef TARGET_OSX
    
    CGRect bounds = CGDisplayBounds(displayId);
    ofRectangle rect = ofRectangle(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);

    CGRect r = {rect.x, rect.y, rect.width, rect.height};
    CGImageRef imgr = CGDisplayCreateImageForRect(displayId, r);
    int width = CGImageGetWidth(imgr);
    int height = CGImageGetHeight(imgr);
    int row = CGImageGetBytesPerRow(imgr);
    CGColorSpaceRef csr = CGImageGetColorSpace(imgr);
    
    CGDataProviderRef dpr = CGImageGetDataProvider(imgr);
    CFDataRef dr = CGDataProviderCopyData(dpr);
    const unsigned char* spixels = (const unsigned char*)CFDataGetBytePtr(dr);
    
    if (CGColorSpaceGetModel(csr) == kCGColorSpaceModelRGB) {
        CGBitmapInfo info = CGImageGetBitmapInfo(imgr);
        CGImageAlphaInfo alpha = CGImageGetAlphaInfo(imgr);
        
        if (!(info & kCGBitmapFloatComponents)) {
            
            if ((info & kCGBitmapByteOrderMask) == kCGBitmapByteOrder32Big && alpha & kCGImageAlphaLast) {
                image.setFromPixels(spixels, width, height, OF_IMAGE_COLOR_ALPHA);
            }
            if ((info & kCGBitmapByteOrderMask) == kCGBitmapByteOrder32Little && alpha & kCGImageAlphaLast) {
                unsigned char* dpixels = image.getPixels();
                int dwidth = image.getWidth() * 4;
                int dheight = image.getHeight();
                int srow = 0;
                int drow = 0;
                for (int y=0; y<dheight; y++) {
                    for (int x=0; x<dwidth; x+=4) {
                        dpixels[drow+x+0] = spixels[srow+x+2];
                        dpixels[drow+x+1] = spixels[srow+x+1];
                        dpixels[drow+x+2] = spixels[srow+x+0];
                        dpixels[drow+x+3] = spixels[srow+x+3];
                    }
                    srow += row;
                    drow += dwidth;
                }
                image.update();
            }
        }
        CFRelease(dr);
    }
    
    CGImageRelease(imgr);
#endif
}
