//
// Created by stef on 21.04.20.
//

#include <fstream>
#include "Scene.h"
#include "SkyBox.h"
#include "Camera.h"
#include "vendor/nlohmann/json.hpp"

void Scene::init(RenderEngine *re, Clock *clock) {
    mRenderEngine = re;
    mClock = clock;
}

Scene::~Scene() {
    delete mRenderEngine;
    delete mClock;
    delete mSkyBox;
}

void Scene::add(StepAheadAnimationChannel *saaChannel) {
    mSaaChannels.push_back(saaChannel);
//    renderEngine->add(saaChannel);
}

void Scene::add(CSkeleton *skeleton) {
    mSkeletons.push_back(skeleton);
}

void Scene::add(Plant *plant) {
    mPlants.push_back(plant);
}

void Scene::add(Camera *camera) {
    mCameras.push_back(camera);
}

void Scene::setSkyBox(std::vector<std::string> &faces) {
    if(mSkyBox != nullptr) {
        delete mSkyBox;
    }
    mSkyBox = new SkyBox(faces);
}

void Scene::update() {
    int currFrame = mClock->getFrameIndex();
    for(auto *camera : mCameras) {
        camera->update(currFrame);
    }
}

void Scene::save(std::string path) {
    nlohmann::json j;
    if(mSkyBox != nullptr)
        j["skybox"] = mSkyBox->to_json();
    std::fstream file(path, std::fstream::out);
    file << j;
    file.close();
}

void Scene::load(std::string path) {
    // reset all
    reset();
    // open file
    std::ifstream ifs(path);
    nlohmann::json j = nlohmann::json::parse(ifs);
    // load everything
    if(j.find("skybox") != j.end()) {
        mSkyBox = new SkyBox(j["skybox"].get<nlohmann::json>());
    }
    // close file
    ifs.close();
}

void Scene::reset() {
    // TODO: correctly free all memory
    delete mSkyBox;
//    for(auto *saaChannel : mSaaChannels)
//        delete saaChannel;
    mSaaChannels.clear();
    for(auto *skeleton : mSkeletons)
        delete skeleton;
    mSkeletons.clear();
    for(auto *plant : mPlants)
        delete plant;
    mPlants.clear();
//    for(auto *camera : mCameras)
//        delete camera;
    mCameras.clear();
}
