#ifndef _ARCBALL_H_
#define _ARCBALL_H_

#include <CSCI441/PerspectiveCamera.hpp>
#include <glm/glm.hpp>
//#include <glm/gtx/string_cast.hpp>
#include <iostream>

class ArcBall final : public CSCI441::PerspectiveCamera
{
  public:
    explicit ArcBall( GLfloat aspectRatio = 1.0f, GLfloat fovy = 45.0f, GLfloat nearClipPlane = 0.001f, GLfloat farClipPlane = 1000.0f, glm::vec3 targetPosition = glm::vec3( 0.0f, 0.0f, 0.0f ) );

    void recomputeOrientation( ) final;

    void zoom( GLfloat zoomFactor );

    void rotate( GLfloat xAngle, GLfloat yAngle );

    void moveForward( GLfloat movementFactor );
    void moveBackward( GLfloat movementFactor );

    void setCameraLookAtPoint( glm::vec3 targetPosition );

    void setCameraPosition( glm::vec3 targetPosition );

    glm::vec3 getCameraDirection( ) const;

  private:
};

inline ArcBall::ArcBall( GLfloat aspectRatio, GLfloat fovy, GLfloat nearClipPlane, GLfloat farClipPlane, glm::vec3 targetPosition )
    : CSCI441::PerspectiveCamera( aspectRatio, fovy, nearClipPlane, farClipPlane )
{
}

inline void ArcBall::recomputeOrientation( )
{
  mCameraDirection = glm::normalize( mCameraLookAtPoint - mCameraPosition );

  mCameraUpVector = glm::vec3( 0.0f, 1.0f, 0.0f );

  computeViewMatrix( );
}

inline void ArcBall::zoom( GLfloat zoomFactor )
{
  glm::vec3 newCameraPosition = mCameraPosition + mCameraDirection * zoomFactor;
  GLfloat minDistance         = 0.01f;
  GLfloat maxDistance         = 1.0f;

  // Calculate and print the distance from the new position to the look-at point
  GLfloat distanceToLookAt = glm::length( newCameraPosition - mCameraLookAtPoint );

  // Allow movement in both directions
  if ( distanceToLookAt >= minDistance || zoomFactor < 0 )
  {
    mCameraPosition = newCameraPosition;
    recomputeOrientation( );
  }
}

inline void ArcBall::rotate( GLfloat xAngle, GLfloat yAngle )
{
  mCameraTheta += xAngle;
  mCameraPhi += yAngle;

  // Clamp phi to avoid flipping the camera
  if ( mCameraPhi < 0.1f )
    mCameraPhi = 0.1f;
  if ( mCameraPhi > glm::pi<GLfloat>( ) - 0.1f )
    mCameraPhi = glm::pi<GLfloat>( ) - 0.1f;

  // Convert spherical coordinates (theta, phi) to Cartesian coordinates
  GLfloat radius    = glm::length( mCameraPosition - mCameraLookAtPoint );
  mCameraPosition.x = mCameraLookAtPoint.x + radius * sin( mCameraPhi ) * cos( mCameraTheta );
  mCameraPosition.y = mCameraLookAtPoint.y + radius * cos( mCameraPhi );
  mCameraPosition.z = mCameraLookAtPoint.z + radius * sin( mCameraPhi ) * sin( mCameraTheta );

  recomputeOrientation( );
}

inline void ArcBall::moveForward( GLfloat movementFactor ) { }

inline void ArcBall::moveBackward( GLfloat movementFactor ) { }

inline void ArcBall::setCameraLookAtPoint( glm::vec3 targetPosition )
{
  mCameraLookAtPoint = targetPosition;
  recomputeOrientation( );
}

inline void ArcBall::setCameraPosition( glm::vec3 targetPosition )
{
  mCameraPosition = targetPosition;
  recomputeOrientation( );
}

inline glm::vec3 ArcBall::getCameraDirection( ) const { return mCameraDirection; }

#endif // _ARCBALL_H_
