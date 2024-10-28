#ifndef _TAV_H
#define _TAV_H

#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <CSCI441/OpenGLUtils.hpp>
#include <CSCI441/objects.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

class Tav
{
  public:
    /*
     * \desc constructor for Tav
     * \param shaderProgramHandle handle to the shader program to use when drawing the Tav
     * \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
     * \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
     * \param materialColorUniformLocation uniform location for the material diffuse color
     */
    Tav( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation );

    /*
     * \desc draws the Tav for a given MVP matrix
     * \param modelMtx existing model matrix to apply to Tav
     * \param viewMtx camera view matrix to apply to Tav
     * \param projMtx camera projection matrix to apply to Tav
     */
    void drawTav( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void moveForward( GLfloat movementFactor );
    void moveBackward( GLfloat movementFactor );

    glm::vec3 getForwardDirection( );
    void setForwardDirection( );

    void rotate( GLfloat rotationFactor );

    glm::vec3 getPosition( );
    void setPosition( glm::vec3 position );

    GLfloat tavSpeed;
    GLfloat tavRotationSpeed;

    void update( );

  private:
    /// \desc handle of the shader program to use when drawing the plane
    GLuint _shaderProgramHandle;

    /// \desc stores the uniform locations needed for the plan information
    struct ShaderProgramUniformLocations
    {
        /// \desc location of the precomputed ModelViewProjection matrix
        GLint mvpMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normalMtx;
        /// \desc location of the material diffuse color
        GLint materialColor;
    } _shaderProgramUniformLocations;

    glm::vec3 _position;

    glm::vec3 _tavBodyColor;
    glm::vec3 _tavHeadColor;
    glm::vec3 _tavEyesColor;
    glm::vec3 _tavIrisColor;
    glm::vec3 _tavHatColor;
    glm::vec3 _tavArmsColor;
    glm::vec3 _tavStaffColor;
    glm::vec3 _tavStaffTipColor;

    GLfloat _rotationX;
    GLfloat _rotationY;

    glm::vec3 _forwardDirection;

    // Bobbing animation parameters
    float _bobbingAmplitude = 0.4f;
    float _bobbingFrequency = 2.0f;
    float _bobbingTime      = 0.0f;
    float _lastUpdateTime   = 0.0f;

    void _drawTavBody( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawTavHead( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawTavEyes( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawTavHat( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawTavArms( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawTavStaff( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    void _computeAndSendMatrixUniforms( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
};

#endif
