// Thanks to @num3ric for sharing this:
// http://discourse.libcinder.org/t/360-vr-video-player-for-ios-in-cinder/294/6

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(false);
	ofDisableArbTex();

	// We need to pass the method we want ofxOpenVR to call when rending the scene
	openVR.setup(std::bind(&ofApp::render, this, std::placeholders::_1));



	image.load("DSCN0143.JPG");
	sphere.set(sphere_rad, 10);
	sphere.setPosition(glm::vec3(.0f, .0f, .0f));

	//Shader setup
	//shader_.load("sphericalProjection");

	// Vertex shader source
	string vertex = "#version 150\n";
	vertex += STRINGIFY(
	uniform mat4 modelViewProjectionMatrix;
	uniform mat4 normalMatrix;

	in vec4 position;
	in vec4 normal;

	out vec4 modelNormal;

	void main() {
		modelNormal = normal;
		gl_Position = modelViewProjectionMatrix * position;

	}
	);

	// Fragment shader source
	string fragment = "#version 150\n";
	fragment += STRINGIFY(	
	uniform sampler2D tex0;
	in vec4	modelNormal;
	out vec4 fragColor;

	const float ONE_OVER_PI = 1.0 / 3.14159265;

	void main() {
		vec3 normal = normalize(modelNormal.xyz);
		// spherical projection based on the surface normal
		vec2 coord = vec2(0.5 + 0.5 * atan(normal.x, -normal.z) * ONE_OVER_PI, acos(normal.y) * ONE_OVER_PI);
		fragColor = texture(tex0, coord);
	}
	);

	// Shader
	shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment);
	shader.bindDefaults();
	shader.linkProgram();

	inited_ = true;
}

//--------------------------------------------------------------
void ofApp::exit() {
	openVR.exit();
}

//--------------------------------------------------------------
void ofApp::update(){
	openVR.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	openVR.render();
	openVR.renderDistortion();

	openVR.drawDebugInfo(10.0f, 500.0f);

	// Help
	if (bShowHelp) {
		_strHelp.str("");
		_strHelp.clear();
		_strHelp << "HELP (press h to toggle): " << endl;
		_strHelp << "Drag and drop a 360 spherical (equirectangular) image to load it in the player. " << endl;
		_strHelp << "Toggle OpenVR mirror window (press: m)." << endl;
		ofDrawBitmapStringHighlight(_strHelp.str(), ofPoint(10.0f, 20.0f), ofColor(ofColor::black, 100.0f));
	}
}

//--------------------------------------------------------------
void  ofApp::render(vr::Hmd_Eye nEye) {

	ofPushView();
	ofSetMatrixMode(OF_MATRIX_PROJECTION);
	ofLoadMatrix(openVR.getCurrentProjectionMatrix(nEye));
	ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	ofMatrix4x4 currentViewMatrixInvertY = openVR.getCurrentViewMatrix(nEye);
	currentViewMatrixInvertY.scale(1.0f, -1.0f, 1.0f);
	ofLoadMatrix(currentViewMatrixInvertY);

	ofSetColor(ofColor::white);

	shader.begin();
	shader.setUniformTexture("tex0", image, 1);
	sphere.draw();
	shader.end();

	ofPopView();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
		case 'h':
			bShowHelp = !bShowHelp;
			break;

		case 'm':
			openVR.toggleMirrorWindow();
			break;

		default:
			break;
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
void ofApp::dragEvent(ofDragInfo dragInfo) {
	//TODO: Why do we need to parse the path to replace the \ by / in order to work?
	std::string path = dragInfo.files[0];
	std::replace(path.begin(), path.end(), '\\', '/');

	image.load(path);
	image.update();
}
