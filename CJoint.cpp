//
// Created by stef on 30.03.20.
//

#include "CJoint.h"
#include "CLink.h"
#include "StandardShader.h"
#include <glm/gtc/matrix_transform.hpp>

CJoint::CJoint(float linkOffset, float jointAngle, float minJointAngle, float maxJointAngle, int childrenCount)
  : mModel("base_models/green_cube.obj")
{
    mParentLink = nullptr;
    mLinkOffset = linkOffset;
    mJointAngles.push_back(jointAngle);
    mFrameIndices.push_back(0);
    mJointAngle_DEPRECATED = jointAngle;
    mMinJointAngle = minJointAngle;
    mMaxJointAngle = maxJointAngle;
    mMaxChildrenCount = childrenCount;
    mGlobalTransMat = glm::mat4(1.0f);
}

void CJoint::setGlobalTransMat(glm::mat4 transMat) {
    mGlobalTransMat = transMat;
    updateLocalTransMat();
    for(int i = 0; i < mMaxChildrenCount; ++i) {
        CJoint *child = mChildLinks[i]->child();
        if(child != nullptr) {
            child->setGlobalTransMat(mGlobalTransMat * mLocalTransMat);
        }
    }
}

/**
 * Update the local transformation matrix, based
 * on the DH-variables locally set.
 */
void CJoint::updateLocalTransMat() {
    // TODO: fix this function
    glm::mat4 rotZArray = {
            glm::cos(mJointAngle_DEPRECATED), glm::sin(mJointAngle_DEPRECATED), 0, 0,
            -glm::sin(mJointAngle_DEPRECATED), glm::cos(mJointAngle_DEPRECATED), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
    glm::mat4 transposeZArray = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, mLinkOffset, 1
    };
    mLocalTransMat = transposeZArray * rotZArray;
    if(mParentLink != nullptr) {
        glm::mat4 rotXArray = {
                1, 0, 0, 0,
                0, glm::cos(mParentLink->linkTwist()), glm::sin(mParentLink->linkTwist()), 0,
                0, -glm::sin(mParentLink->linkTwist()), glm::cos(mParentLink->linkTwist()), 0,
                0, 0, 0, 1
        };
        glm::mat4 translateXArray = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                mParentLink->linkLength(), 0, 0, 1
        };
        mLocalTransMat = translateXArray * rotXArray * mLocalTransMat;
    }
}

void CJoint::setParentLink(CLink *parentLink) {
    mParentLink = parentLink;
}

void CJoint::addChildLink(CLink *childLink) {
    mChildLinks.push_back(childLink);
}

bool CJoint::full() {
    return mChildLinks.size() == mMaxChildrenCount;
}

CLink *CJoint::parent() {
    return mParentLink;
}

void CJoint::renderAll_DEPRECTATED(StandardShader *standardShader) {
    glm::mat4 scaleDown = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));
    glm::mat4 transMat = mGlobalTransMat * mLocalTransMat * scaleDown;
    standardShader->setMatrix(TRANSFORMATION_MATRIX, transMat);
    mModel.Draw(0, standardShader->getUniLocTexture());
    for(int i = 0; i < mMaxChildrenCount; ++i) {
        CJoint *child = mChildLinks[i]->child();
        if(child != nullptr) {
            child->renderAll_DEPRECTATED(standardShader);
        }
    }
}

glm::vec3 CJoint::getGlobPos() {
    glm::vec4 globPos = mGlobalTransMat * mLocalTransMat * glm::vec4(glm::vec3(0.0f), 1.0f);
    float w = globPos.w;
    return glm::vec3(globPos.x / w, globPos.y / w, globPos.z / w);
}

glm::mat4 CJoint::transMat() {
    return mGlobalTransMat * mLocalTransMat;
}

glm::vec3 CJoint::getRotAxis() {
    return mGlobalTransMat * mLocalTransMat * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
}

void CJoint::setJointAngle(float jointAngle) {
    mJointAngle_DEPRECATED = jointAngle;
    if(mJointAngle_DEPRECATED < mMinJointAngle) {
        mJointAngle_DEPRECATED = mMinJointAngle;
    }
    else if(mJointAngle_DEPRECATED > mMaxJointAngle) {
        mJointAngle_DEPRECATED = mMaxJointAngle;
    }
}
