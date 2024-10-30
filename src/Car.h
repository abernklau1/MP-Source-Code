//
// Created by Gabrielle Christensen on 10/20/24.
//

#ifndef LAB05_CAR_H
#define LAB05_CAR_H
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Car {
public:
    Car( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation );
    void drawCar( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );
    void moveForward();
    void moveBackward();
    glm::vec3 getPosition() const;
    void turnLeft();
    void turnRight();

private:
    void _drawCarBody( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawCarWheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;

    float _carMoveAngle = 0.0f;
    glm::vec3 _scaleBody = glm::vec3( 1.0f, 0.5f, 0.5f ); // Rectangular prism
    glm::vec3 _scaleWheel = glm::vec3( 0.2f, 0.2f, 0.2f ); // Wheel size

    float _rotateWheelAngle = 0.0f;
    float _moveSpeed = 0.0f;
    float _wheelRotationSpeed = 0.1f;

    glm::vec3 _colorBody;
    glm::vec3 _colorWheel;
    glm::vec3 _carPosition;

    glm::mat4 frontLeft = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, -0.25f, -0.25f));
    glm::mat4 frontRight = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, -0.25f, 0.25f));
    glm::mat4 backLeft = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, -0.25f, -0.25f));
    glm::mat4 backRight = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, -0.25f, 0.25f));

    const GLfloat _PI = glm::pi<float>();

    /// \desc handle of the shader program to use when drawing the plane
    GLuint _shaderProgramHandle;
    /// \desc stores the uniform locations needed for the plan information
    struct ShaderProgramUniformLocations {
        /// \desc location of the precomputed ModelViewProjection matrix
        GLint mvpMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normalMtx;
        /// \desc location of the material diffuse color
        GLint materialColor;
    } _shaderProgramUniformLocations;

    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
};

#endif //LAB05_CAR_H
