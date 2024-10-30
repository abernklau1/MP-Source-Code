#ifndef LAB05_HORSE_H
#define LAB05_HORSE_H

#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class horse {
public:
    /// \desc creates a simple plane that gives the appearance of flight
    /// \param shaderProgramHandle shader program handle that the plane should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    horse(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation, int bound );

    /// \desc draws the model plane for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
    void drawHorse(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    /// \desc simulates the plane flying forward by rotating the propeller clockwise
    void moveForward();
    /// \desc simulates the plane flying backward by rotating the propeller counter-clockwise
    void moveBackward();

    void turnLeft();
    void turnRight();

    glm::vec3 getHorsePos();

private:
    int edge;
    float _movespeed;
    float _horseAngle;
    float _legRotate;
    bool _forward;
    glm::vec3 _horsePosition = {5.0f, 0.0f, 5.0f};
    /// \desc current angle of rotation for the propeller
    GLfloat _legsAngle;
    /// \desc one rotation step
    GLfloat _legsAngleRotationSpeed;

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

    /// \desc angle to rotate our plane at
    GLfloat _rotateHorseAngle;

    /// \desc color the plane's body
    glm::vec3 _colorBody;
    /// \desc amount to scale the plane's body by
    glm::vec3 _scaleBody;

    /// \desc color the plane's wing
    glm::vec3 _colorWing;
    /// \desc amount to scale the plane's wing by
    glm::vec3 _scaleWing;
    /// \desc amount to rotate the plane's wing by
    GLfloat _rotateWingAngle;

    /// \desc color the plane's nose
    glm::vec3 _colorNose;
    /// \desc amount to rotate the plane's nose by
    GLfloat _rotateNoseAngle;

    /// \desc color the plane's propeller
    glm::vec3 _colorProp;
    /// \desc amount to scale the plane's propeller by
    glm::vec3 _scaleProp;
    /// \desc amount to translate the plane's propeller by
    glm::vec3 _transProp;

    /// \desc color the plane's tail
    glm::vec3 _colorTail;

    const GLfloat _PI = glm::pi<float>();
    const GLfloat _2PI = glm::two_pi<float>();
    const GLfloat _PI_OVER_2 = glm::half_pi<float>();

    /// \desc draws just the horse's body
    /// \param modelMtx existing model matrix to apply
    /// \param viewMtx camera view matrix to apply
    /// \param projMtx camera projection matrix to apply
    void _drawHorseBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws just the horse's tail
    /// \param modelMtx existing model matrix to apply
    /// \param viewMtx camera view matrix to apply
    /// \param projMtx camera projection matrix to apply
    void _drawHorseTail(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws just the horse's front legs
    /// \param modelMtx existing model matrix to apply
    /// \param viewMtx camera view matrix to apply
    /// \param projMtx camera projection matrix to apply
    void _drawHorseFLegR(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws just the horse's back legs
    /// \param modelMtx existing model matrix to apply
    /// \param viewMtx camera view matrix to apply
    /// \param projMtx camera projection matrix to apply
    void _drawHorseBLegR(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws just the horse's front legs
    /// \param modelMtx existing model matrix to apply
    /// \param viewMtx camera view matrix to apply
    /// \param projMtx camera projection matrix to apply
    void _drawHorseFLegL(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws just the horse's back legs
    /// \param modelMtx existing model matrix to apply
    /// \param viewMtx camera view matrix to apply
    /// \param projMtx camera projection matrix to apply
    void _drawHorseBLegL(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws just the horse's legs
    /// \param modelMtx existing model matrix to apply
    /// \param viewMtx camera view matrix to apply
    /// \param projMtx camera projection matrix to apply
    void _drawHorseLegs(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc draws just the horse's neck
    /// \param modelMtx existing model matrix to apply
    /// \param viewMtx camera view matrix to apply
    /// \param projMtx camera projection matrix to apply
    void _drawHorseNeck(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws just the horse's head
    /// \param modelMtx existing model matrix to apply
    /// \param viewMtx camera view matrix to apply
    /// \param projMtx camera projection matrix to apply
    void _drawHorseHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
};


#endif //LAB05_HORSE_H
