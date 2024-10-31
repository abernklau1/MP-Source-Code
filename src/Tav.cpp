#include "Tav.h"


Tav::Tav( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation )
{
  _shaderProgramHandle                         = shaderProgramHandle;
  _shaderProgramUniformLocations.mvpMtx        = mvpMtxUniformLocation;
  _shaderProgramUniformLocations.normalMtx     = normalMtxUniformLocation;
  _shaderProgramUniformLocations.materialColor = materialColorUniformLocation;

  tavSpeed         = 0.03f;
  tavRotationSpeed = 0.03f;

  _position = glm::vec3( 30.0f, 0.5f, 30.0f );

  _tavBodyColor     = glm::vec3( 0.0f, 0.4f, 0.8f );
  _tavHeadColor     = glm::vec3( 1.0f, 0.8f, 0.6f );
  _tavEyesColor     = glm::vec3( 1.0f, 1.0f, 1.0f );
  _tavIrisColor     = glm::vec3( 0.0f, 0.1f, 0.4f );
  _tavHatColor      = glm::vec3( 0.0f, 0.4f, 0.8f );
  _tavArmsColor     = glm::vec3( 1.0f, 0.8f, 0.6f );
  _tavStaffColor    = glm::vec3( 0.6f, 0.3f, 0.0f );
  _tavStaffTipColor = glm::vec3( 1.0f, 0.2f, 0.0f );
}

void Tav::drawTav( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx )
{
  modelMtx = glm::rotate( modelMtx, _rotationY, glm::vec3(0.0f, 1.0f, 0.0f) );
  _drawTavBody( modelMtx, viewMtx, projMtx );
  _drawTavHead( modelMtx, viewMtx, projMtx );
  _drawTavEyes( modelMtx, viewMtx, projMtx );
  _drawTavHat( modelMtx, viewMtx, projMtx );
  _drawTavArms( modelMtx, viewMtx, projMtx );
  _drawTavStaff( modelMtx, viewMtx, projMtx );
}

void Tav::moveForward( GLfloat movementFactor ) { _position += _forwardDirection * movementFactor; }

void Tav::moveBackward( GLfloat movementFactor ) { _position -= _forwardDirection * movementFactor; }

void Tav::rotate( GLfloat rotationFactor ) { _rotationY += rotationFactor;}

glm::vec3 Tav::getPosition( ) { return _position; }
void Tav::setPosition( glm::vec3 position ) { _position = position; }

glm::vec3 Tav::getForwardDirection( ) { return _forwardDirection; }
void Tav::setForwardDirection( ) { _forwardDirection = glm::vec3( cos( _rotationY ), 0.0f, -sin( _rotationY ) ); }

void Tav::_drawTavBody( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.0f, 0.0f, 0.0f ) );

  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavBodyColor ) );

  CSCI441::drawSolidCylinder( 0.08f, 0.09f, 0.4f, 20, 20 );

  modelMtx = glm::translate( modelMtx, glm::vec3( 0.0f, 0.001f, 0.0f ) );
  modelMtx = glm::rotate( modelMtx, -glm::half_pi<float>( ), CSCI441::X_AXIS );

  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  CSCI441::drawSolidHalfSphere( 0.08f, 20, 20 );
}

void Tav::_drawTavHead( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.0f, 0.53f, 0.0f ) );

  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavHeadColor ) );

  CSCI441::drawSolidSphere( 0.1f, 20, 20 );
}

void Tav::_drawTavEyes( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  // Position the first eye
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.08f, 0.55f, -0.03f ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );
  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavEyesColor ) );
  CSCI441::drawSolidSphere( 0.02f, 20, 20 );

  // Position the second eye
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.00f, 0.0f, 0.06f ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );
  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavEyesColor ) );
  CSCI441::drawSolidSphere( 0.02f, 20, 20 );

  // Draw the iris
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.0152f, 0.0f, 0.005f ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );
  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavIrisColor ) );
  CSCI441::drawSolidSphere( 0.01f, 20, 20 );

  // Draw the second iris
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.0f, 0.0f, -0.065f ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );
  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavIrisColor ) );
  CSCI441::drawSolidSphere( 0.01f, 20, 20 );
}

void Tav::_drawTavHat( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.0f, 0.58f, 0.0f ) );

  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavHatColor ) );

  CSCI441::drawSolidCone( 0.2f, 0.08f, 16, 16 );

  modelMtx = glm::translate( modelMtx, glm::vec3( 0.0f, 0.03f, 0.0f ) );

  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );
  CSCI441::drawSolidCone( 0.1f, 0.3f, 16, 16 );
}

void Tav::_drawTavArms( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  // Draw the first arm
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.0f, 0.15f, 0.15f ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );
  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavArmsColor ) );
  CSCI441::drawSolidSphere( 0.035f, 10, 10 );

  // Draw the second arm
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.0f, 0.0f, -0.30f ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );
  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavArmsColor ) );
  CSCI441::drawSolidSphere( 0.035f, 10, 10 );
}

void Tav::_drawTavStaff( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  // Draw the staff
  modelMtx = glm::translate( modelMtx, glm::vec3( -0.03f, -0.1f, 0.15f ) );
  modelMtx = glm::rotate( modelMtx, -0.1f, CSCI441::Z_AXIS );

  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavStaffColor ) );

  CSCI441::drawSolidCylinder( 0.02f, 0.02f, 0.6f, 20, 20 );

  // Draw the staff tip
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.0f, 0.65f, 0.0f ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );
  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _tavStaffTipColor ) );
  CSCI441::drawSolidSphere( 0.02f, 20, 20 );
}

void Tav::_computeAndSendMatrixUniforms( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.modelMtx, 1, GL_FALSE, glm::value_ptr( modelMtx ) );
  glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
  glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, glm::value_ptr( mvpMtx ) );
  glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx ) ) );
  glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, glm::value_ptr( normalMtx ) );
}

void Tav::update() {
    // Get the current time
    float currentTime = glfwGetTime();

    // Calculate the elapsed time since the last update
    float deltaTime = currentTime - _lastUpdateTime;

    // Update the time accumulator
    _bobbingTime += deltaTime;

    // Calculate the new vertical position using a sine wave
    float bobbingOffset = _bobbingAmplitude * sin(_bobbingFrequency * _bobbingTime);

    // Update Tav's position with the bobbing offset
    _position.y = 0.5f + bobbingOffset; // Assuming 0.5f is the base height

    // Update the last update time
    _lastUpdateTime = currentTime;
}