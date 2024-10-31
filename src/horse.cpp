#include "horse.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CSCI441/OpenGLUtils.hpp>
#include <CSCI441/objects.hpp>

horse::horse( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation, GLint bound )
{
  _legsAngle              = 0.0f;
  _legsAngleRotationSpeed = _PI / 16.0f;

  _shaderProgramHandle                         = shaderProgramHandle;
  _shaderProgramUniformLocations.mvpMtx        = mvpMtxUniformLocation;
  _shaderProgramUniformLocations.normalMtx     = normalMtxUniformLocation;
  _shaderProgramUniformLocations.materialColor = materialColorUniformLocation;

  _horsePosition = glm::vec3( 0.0f, 0.0f, 0.0f );

  _rotateHorseAngle = _PI / 2.0f;

  _colorBody = glm::vec3( 0.17, 0.1085, 0.0646 );
  _scaleBody = glm::vec3( 20, 20, 30 );

  _colorTail = glm::vec3( 0.0f, 0.0f, 0.0f );

  // set bound
  edge        = bound;
  _movespeed  = 0.1f;
  _horseAngle = 0.0f;
  _legRotate  = 0.0f;
  _forward    = true;
}
void horse::setPosition( glm::vec3 position ) { _horsePosition = position; }
glm::vec3 horse::getForwardDirection( ) { return _forwardDirection; }
void horse::setForwardDirection( ) { _forwardDirection = glm::vec3( cos( _horseAngle ), 0.0f, -sin( _horseAngle ) ); }
void horse::drawHorse( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx )
{
  //modelMtx = glm::translate( modelMtx, _horsePosition );
  modelMtx = glm::rotate( modelMtx, -_rotateHorseAngle, CSCI441::Y_AXIS );
  modelMtx = glm::rotate( modelMtx, _rotateHorseAngle, CSCI441::Z_AXIS );
  modelMtx = glm::rotate( modelMtx, _PI, CSCI441::X_AXIS );
  modelMtx = glm::rotate( modelMtx, _horseAngle, CSCI441::X_AXIS );
  modelMtx = glm::scale( modelMtx, glm::vec3( 0.5f, 0.5f, 0.5f ) );
  _drawHorseBody( modelMtx, viewMtx, projMtx );

  _drawHorseNeck( modelMtx, viewMtx, projMtx );
  _drawHorseHead( modelMtx, viewMtx, projMtx );
  _drawHorseTail( modelMtx, viewMtx, projMtx );
  modelMtx = glm::rotate( modelMtx, _legRotate, CSCI441::Y_AXIS );
  _drawHorseLegs( modelMtx, viewMtx, projMtx );
}

void horse::moveForward( )
{
  float nextx = _horsePosition.x - _forwardDirection.x*_movespeed * glm::cos( _horseAngle );
  float nextz = _horsePosition.z - _forwardDirection.y*_movespeed * glm::sin( _horseAngle );
  if ( nextx < edge / 2 && nextz < edge / 2 && nextx > -edge / 2 && nextz > -edge / 2 )
  {
    _horsePosition.x = nextx;
    _horsePosition.z = nextz;
  }
  if ( _legRotate > 0.2f )
  {
    _forward = false;
  }
  else if ( _legRotate < -0.2f )
    _forward = true;

  if ( _forward )
  {
    _legRotate += 0.01;
  }
  else
    _legRotate -= 0.01;
}

void horse::moveBackward( )
{
  float nextx = _horsePosition.x - _forwardDirection.x*_movespeed * glm::cos( _horseAngle );
  float nextz = _horsePosition.z - _forwardDirection.y*_movespeed * glm::sin( _horseAngle );
  if ( nextx < edge / 2 && nextz < edge / 2 && nextx > -edge / 2 && nextz > -edge / 2 )
  {
    _horsePosition.x = nextx;
    _horsePosition.z = nextz;
  }
  if ( _legRotate > 0.2f )
  {
    _forward = false;
  }
  else if ( _legRotate < -0.2f )
    _forward = true;

  if ( _forward )
  {
    _legRotate += 0.01;
  }
  else
    _legRotate -= 0.01;
}

void horse::turnLeft( ) { _horseAngle += 0.01; }

void horse::turnRight( ) { _horseAngle -= 0.01; }

glm::vec3 horse::getHorsePos( ) { return _horsePosition; }

void horse::_drawHorseBody( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::scale( modelMtx, _scaleBody );
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.15, 0, 0 ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _colorBody ) );

  CSCI441::drawSolidCube( 0.1f );
}

void horse::_drawHorseBLegL( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::scale( modelMtx, glm::vec3( 20, 8, 8 ) );
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.05, .1, .12 ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( glm::vec3( 0.53, 0.2703, 0.0848 ) ) );

  CSCI441::drawSolidCube( 0.1f );
}

void horse::_drawHorseBLegR( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::scale( modelMtx, glm::vec3( 20, 8, 8 ) );
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.05, -.1, .12 ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( glm::vec3( 0.53, 0.2703, 0.0848 ) ) );

  CSCI441::drawSolidCube( 0.1f );
}

void horse::_drawHorseFLegL( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::scale( modelMtx, glm::vec3( 20, 8, 8 ) );
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.05, .1, -.12 ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( glm::vec3( 0.53, 0.2703, 0.0848 ) ) );

  CSCI441::drawSolidCube( 0.1f );
}

void horse::_drawHorseFLegR( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::scale( modelMtx, glm::vec3( 20, 8, 8 ) );
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.05, -.1, -.12 ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( glm::vec3( 0.53, 0.2703, 0.0848 ) ) );

  CSCI441::drawSolidCube( 0.1f );
}

void horse::_drawHorseLegs( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  _drawHorseBLegL( modelMtx, viewMtx, projMtx );
  _drawHorseBLegR( modelMtx, viewMtx, projMtx );
  _drawHorseFLegL( modelMtx, viewMtx, projMtx );
  _drawHorseFLegR( modelMtx, viewMtx, projMtx );
}

void horse::_drawHorseHead( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::rotate( modelMtx, -0.1f, CSCI441::Y_AXIS );
  modelMtx = glm::scale( modelMtx, glm::vec3( 12, 13, 16 ) );
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.5, 0, .13 ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _colorBody ) );

  CSCI441::drawSolidCube( 0.1f );
}

void horse::_drawHorseNeck( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::rotate( modelMtx, -0.4f, CSCI441::Y_AXIS );
  modelMtx = glm::scale( modelMtx, glm::vec3( 22, 12, 10 ) );
  modelMtx = glm::translate( modelMtx, glm::vec3( 0.23, 0, -0.03 ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( glm::vec3( 0.53, 0.2703, 0.0848 ) ) );

  CSCI441::drawSolidCube( 0.1f );
}

void horse::_drawHorseTail( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  modelMtx = glm::rotate( modelMtx, -0.4f, CSCI441::Y_AXIS );
  modelMtx = glm::scale( modelMtx, glm::vec3( 30, 4, 4 ) );
  modelMtx = glm::translate( modelMtx, glm::vec3( .05, 0, -0.7 ) );
  _computeAndSendMatrixUniforms( modelMtx, viewMtx, projMtx );

  glProgramUniform3fv( _shaderProgramHandle, _shaderProgramUniformLocations.materialColor, 1, glm::value_ptr( _colorTail ) );

  CSCI441::drawSolidCube( 0.1f );
}

void horse::_computeAndSendMatrixUniforms( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
  // precompute the Model-View-Projection matrix on the CPU
  glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
  // then send it to the shader on the GPU to apply to every vertex

  glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.modelMtx, 1, GL_FALSE, glm::value_ptr(modelMtx) );

  glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, glm::value_ptr( mvpMtx ) );

  glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx ) ) );
  glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, glm::value_ptr( normalMtx ) );
}
