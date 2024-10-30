#include "Car.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

Car::Car( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation ) {
    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor    = materialColorUniformLocation;

    _carPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    _colorBody = glm::vec3( 1.0f, 0.73f, 0.89f );
    _colorWheel = glm::vec3( 0.11f, 0.11f, 0.11f );

    _carMoveAngle = 0.0f;
    _moveSpeed = 0.01;
}

void Car::drawCar( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) {
    modelMtx = glm::translate(modelMtx, _carPosition);
    modelMtx = glm::translate(modelMtx, glm::vec3(0, 0.5, 0));
    modelMtx = glm::rotate(modelMtx, -_carMoveAngle, CSCI441::Y_AXIS );
    _drawCarBody(modelMtx, viewMtx, projMtx);

    _drawCarWheel(glm::rotate(modelMtx * frontLeft, -_PI/2, glm::vec3(0, 0, 1)), viewMtx, projMtx);  //front-left
    _drawCarWheel(glm::rotate(modelMtx * frontRight, -_PI/2, glm::vec3(0, 0, 1)), viewMtx, projMtx); //front-right
    _drawCarWheel(glm::rotate(modelMtx * backLeft, -_PI/2, glm::vec3(0, 0, 1)), viewMtx, projMtx); //back-left
    _drawCarWheel(glm::rotate(modelMtx * backRight, -_PI/2, glm::vec3(0, 0, 1)), viewMtx, projMtx); //back-right
}

void Car::moveForward() {
    _carPosition.x += _moveSpeed * glm::cos(_carMoveAngle);
    _carPosition.z += _moveSpeed * glm::sin(_carMoveAngle);

    _rotateWheelAngle += 0.05;
}

void Car::moveBackward() {
    _carPosition.x -= _moveSpeed * glm::cos(_carMoveAngle);
    _carPosition.z -= _moveSpeed * glm::sin(_carMoveAngle);

    _rotateWheelAngle -= 0.05;
}

void Car::turnLeft() {
    _carMoveAngle += 0.05;
}

void Car::turnRight() {
    _carMoveAngle -= 0.05;
}

glm::vec3 Car::getPosition() const {
    return _carPosition;
}

void Car::_drawCarBody( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr(_colorBody));
    CSCI441::drawSolidCube(0.5f);
}

void Car::_drawCarWheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::rotate(modelMtx, -_rotateWheelAngle, CSCI441::Z_AXIS);
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glProgramUniform3fv(_shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f))); // Red
    CSCI441::drawSolidCube(0.1);
}

void Car::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, glm::value_ptr(mvpMtx) );

    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, glm::value_ptr(normalMtx) );
}