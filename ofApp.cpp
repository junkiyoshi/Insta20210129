#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");
	
	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->number_of_targets = 30;
	for (int i = 0; i < this->number_of_targets; i++) {

		this->target_list.push_back(glm::vec2());
		this->color_list.push_back(glm::vec3());
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}
//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	for (int i = 0; i < this->number_of_targets; i++) {

		auto location_noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
		this->target_list[i] = glm::vec2(
			ofMap(ofNoise(location_noise_seed.x, ofGetFrameNum() * 0.0005), 0, 1, 30, ofGetWidth() - 30),
			ofMap(ofNoise(location_noise_seed.y, ofGetFrameNum() * 0.0005), 0, 1, 30, ofGetHeight() - 30));
		
		auto color_noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		this->color_list[i] = glm::vec3(
			ofMap(ofNoise(color_noise_seed.x, ofGetFrameNum() * 0.005), 0, 1, 0.25, 1),
			ofMap(ofNoise(color_noise_seed.y, ofGetFrameNum() * 0.005), 0, 1, 0.25, 1),
			ofMap(ofNoise(color_noise_seed.z, ofGetFrameNum() * 0.005), 0, 1, 0.25, 1));
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofFill();
	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform2fv("targets", &this->target_list[0].x, this->number_of_targets);
	this->shader.setUniform3fv("colors", &this->color_list[0].x, this->number_of_targets);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();

	for (int i = 0; i < this->number_of_targets; i++) {

		ofSetColor(this->color_list[i].x * 255, this->color_list[i].y * 255, this->color_list[i].z * 255);

		ofBeginShape();
		for (int deg = 0; deg <= 360; deg += 1) {

			ofVertex(this->target_list[i] + glm::vec2(30 * cos(deg * DEG_TO_RAD), 30 * sin(deg * DEG_TO_RAD)));
		}
		for (int deg = 360; deg >= 0; deg -= 1) {

			ofVertex(this->target_list[i] + glm::vec2(33 * cos(deg * DEG_TO_RAD), 33 * sin(deg * DEG_TO_RAD)));
		}
		ofEndShape(true);
	}
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}