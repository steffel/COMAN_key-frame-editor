//
// Created by stef on 18.02.20.
//

#include <iostream>
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


const float MAX_VERTICAL_CAM_ROT = 90;
const float MAX_HORIZONTAL_CAM_ROT = 180;


/**
 * Constructor
 */
Camera::Camera(int windowWidth, int windowHeight) {
    mPos = glm::vec3(1.2f, 1.2f, 1.2f);
    setRotXSafe(-45.0f);
    setRotYSafe(45.0f);
    // set projection matrix
    mProjectionMatrix = genProjectionMatrix(45.0f, windowWidth, windowHeight, 0.01f, 40.0f);
}


glm::mat4 Camera::getViewMatrix() {
    glm::mat4 viewMatrix = glm::lookAt(
            mPos,
            mPos + calcViewDir(),
            glm::vec3(0.0f, 1.0f, 0.0f)
            );
    return viewMatrix;
}


void Camera::move(glm::vec3 movement) {
    glm::vec3 horizontalViewDir = calcViewDir();
    horizontalViewDir.y = 0;
    horizontalViewDir = glm::normalize(horizontalViewDir);
    // forward or backwards movement
    mPos += horizontalViewDir * movement.z;
    // up or down movement
    mPos += glm::vec3(0.0f, 1.0f, 0.0f) * movement.y;
    // left or right movement
    glm::vec4 sideWaysViewDirHomo = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(horizontalViewDir, 1.0f);
    glm::vec3 sideWaysViewDir = glm::vec3(sideWaysViewDirHomo.x/sideWaysViewDirHomo.w, sideWaysViewDirHomo.y/sideWaysViewDirHomo.w, sideWaysViewDirHomo.z/sideWaysViewDirHomo.w);
    mPos += sideWaysViewDir * movement.x;
}


void Camera::relativeMove(glm::vec3 movement) {
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(mRotY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotMat = glm::rotate(rotMat, glm::radians(mRotX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotMat = glm::rotate(rotMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 absMoveVec = rotMat * glm::vec4(movement, 1.0f);
    mPos += absMoveVec;
}


glm::vec3 Camera::calcViewDir() {
    glm::mat4 rotViewMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mRotY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotViewMatrix = glm::rotate(rotViewMatrix, glm::radians(mRotX), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec4 homogeneViewDir = rotViewMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
    return glm::vec3(homogeneViewDir.x/homogeneViewDir.w, homogeneViewDir.y/homogeneViewDir.w, homogeneViewDir.z/homogeneViewDir.w);
}


void Camera::rotHor(float angle) {
    setRotYSafe(getRotY() - angle);
}


void Camera::rotVert(float angle) {
    setRotXSafe(getRotX() - angle);
}


void Camera::rot(float horAngle, float verAngle) {
    rotHor(horAngle);
    rotVert(verAngle);
}


void Camera::setRotXSafe(float rotX) {
    if(rotX > MAX_VERTICAL_CAM_ROT ) {
        mRotX = MAX_VERTICAL_CAM_ROT;
    }
    else if(rotX < -MAX_VERTICAL_CAM_ROT) {
        mRotX = -MAX_VERTICAL_CAM_ROT;
    }
    else {
        mRotX = rotX;
    }
}

void Camera::setRotYSafe(float rotY) {
    if(rotY > MAX_HORIZONTAL_CAM_ROT) {
        mRotY = -2 * MAX_HORIZONTAL_CAM_ROT + rotY;
    }
    else if(rotY < -MAX_HORIZONTAL_CAM_ROT) {
        mRotY = 2 * MAX_HORIZONTAL_CAM_ROT + rotY;
    }
    else {
        mRotY = rotY;
    }
}

glm::mat4 Camera::genProjectionMatrix(float fov, int windowWidth, int windowHeight, float nearClipping, float farClipping) {
    return glm::perspective(
            glm::radians(fov),
            ((float)windowWidth) / ((float)windowHeight),
            nearClipping,
            farClipping
    );
}
