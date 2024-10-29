#include "FreeCam.h"
#include<cmath>

void FreeCam::recomputeOrientation() {
    // TODO #2
    mCameraDirection.x = sin(mCameraPhi) * cos(mCameraTheta);
    mCameraDirection.y = cos(mCameraPhi);
    mCameraDirection.z = sin(mCameraPhi) * sin(mCameraTheta);

    mCameraDirection = glm::normalize(mCameraDirection);
        _updateFreeCameraViewMatrix();
}

void FreeCam::moveForward(GLfloat movementFactor) {
    mCameraPosition = mCameraPosition + mCameraDirection*movementFactor;
    recomputeOrientation();
}

void FreeCam::moveBackward(GLfloat movementFactor) {
    mCameraPosition = mCameraPosition - mCameraDirection*movementFactor;
    recomputeOrientation();
}

void FreeCam::_updateFreeCameraViewMatrix() {
    // TODO #3
    mCameraLookAtPoint = mCameraDirection+mCameraPosition;
    computeViewMatrix();
}