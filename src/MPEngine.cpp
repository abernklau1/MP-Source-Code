#include "MPEngine.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

//*************************************************************************************
//
// Helper Functions

#ifndef M_PI
    #define M_PI 3.14159265f
#endif

/// \desc Simple helper function to return a random number between 0.0f and 1.0f.
GLfloat getRand( ) { return (GLfloat)rand( ) / (GLfloat)RAND_MAX; }

//*************************************************************************************
//
// Public Interface

MPEngine::MPEngine( )
    : CSCI441::OpenGLEngine( 4, 1, 640, 480, "MP: Tav" )
{
    _pFreeCam = new FreeCam( );

    for ( auto& _key : _keys )
        _key = GL_FALSE;

    _mousePosition        = glm::vec2( MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
}

MPEngine::~MPEngine( )
{
    delete _pArcballCam;
    delete _pTav;
    delete _pBeing;
    delete _pHorse;
    delete _pCar;
}

void MPEngine::handleKeyEvent( GLint key, GLint action )
{
    if ( key != GLFW_KEY_UNKNOWN )
        _keys[key] = ( ( action == GLFW_PRESS ) || ( action == GLFW_REPEAT ) );

    if ( action == GLFW_PRESS || action == GLFW_REPEAT )
    {
        glm::vec3 targetPosition;

        switch ( key )
        {
            case GLFW_KEY_1: _pActiveCamera = _pArcballCam; break;
            case GLFW_KEY_2:
                {
                    // Switch to FreeCam
                    if ( _currentCharacter == 0 )
                    {
                        targetPosition = _pTav->getPosition( );
                    }
                    else if ( _currentCharacter == 1 )
                    {
                        targetPosition = _pBeing->getPosition( );
                    }
                    else if ( _currentCharacter == 2 )
                    {
                        targetPosition = _pHorse->getHorsePos( );
                    }
                    else if ( _currentCharacter == 3 )
                    {
                        targetPosition = _pCar->getPosition( );
                    }

                    _pFreeCam->setPosition( _pArcballCam->getPosition( ) );

                    // Compute the direction vector for FreeCam
                    glm::vec3 freeCamDirection = glm::normalize( targetPosition - _pFreeCam->getPosition( ) );

                    // Compute Theta and Phi based on FreeCam's definitions
                    GLfloat phi   = acos( freeCamDirection.y );
                    GLfloat theta = atan2( freeCamDirection.z, freeCamDirection.x );

                    _pFreeCam->setTheta( theta );
                    _pFreeCam->setPhi( phi );
                    _pFreeCam->recomputeOrientation( );
                    _pActiveCamera = _pFreeCam;
                    break;
                }
            case GLFW_KEY_3: _toggleFirst = !_toggleFirst; break;
            case GLFW_KEY_Z: _currentCharacter = 0; break;
            case GLFW_KEY_X: _currentCharacter = 1; break;
            case GLFW_KEY_C: _currentCharacter = 2; break;
            case GLFW_KEY_V: _currentCharacter = 3; break;
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE: setWindowShouldClose( ); break;
            default: break;
        }

        // After updating _currentCharacter, update the camera's position and orientation
        if ( key == GLFW_KEY_Z || key == GLFW_KEY_X || key == GLFW_KEY_C || key == GLFW_KEY_V )
        {
            switch ( _currentCharacter )
            {
                case 0: targetPosition = _pTav->getPosition( ); break;
                case 1: targetPosition = _pBeing->getPosition( ); break;
                case 2: targetPosition = _pHorse->getHorsePos( ); break;
                case 3: targetPosition = _pCar->getPosition( ); break;
                default: targetPosition = glm::vec3( 0.0f ); break;
            }

            // Set ArcBall camera to new character
            _pArcballCam->setCameraLookAtPoint( targetPosition );
            _pArcballCam->setCameraPosition( targetPosition + _cameraOffset );

            glm::vec3 arcballDirection = glm::normalize( _cameraOffset );
            _pArcballCam->setTheta( atan2( arcballDirection.z, arcballDirection.x ) );
            _pArcballCam->setPhi( acos( arcballDirection.y ) );
            _pArcballCam->recomputeOrientation( );
        }
    }
}

void MPEngine::handleMouseButtonEvent( GLint button, GLint action )
{
    if ( button == GLFW_MOUSE_BUTTON_LEFT )
    {
        _leftMouseButtonState = action;
        if ( action == GLFW_PRESS )
        {
            // Initialize mouse position when the button is pressed
            double xpos, ypos;
            glfwGetCursorPos( mpWindow, &xpos, &ypos );
            _mousePosition.x = static_cast<float>( xpos );
            _mousePosition.y = static_cast<float>( ypos );
        }
    }
}

void MPEngine::handleCursorPositionEvent( glm::vec2 currMousePosition )
{
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if ( _mousePosition.x == MOUSE_UNINITIALIZED )
    {
        _mousePosition = currMousePosition;
    }

    // if the left mouse button is being held down while the mouse is moving
    if ( _leftMouseButtonState == GLFW_PRESS )
    {
        // Zoom based on vertical mouse movement

        // rotate the camera by the distance the mouse moved
        if ( _pActiveCamera == _pArcballCam )
        {
            _pArcballCam->rotate( ( currMousePosition.x - _mousePosition.x ) * 0.005f, ( _mousePosition.y - currMousePosition.y ) * 0.005f );
        }
        else
        {

            _pFreeCam->rotate( ( currMousePosition.x - _mousePosition.x ) * 0.005f, ( _mousePosition.y - currMousePosition.y ) * 0.005f );
        }
    }

    // update the mouse position
    _mousePosition = currMousePosition;
}

//*************************************************************************************
//
// Engine Setup

void MPEngine::mSetupGLFW( )
{
    CSCI441::OpenGLEngine::mSetupGLFW( );

    // set our callbacks
    glfwSetKeyCallback( mpWindow, a3_engine_keyboard_callback );
    glfwSetMouseButtonCallback( mpWindow, a3_engine_mouse_button_callback );
    glfwSetCursorPosCallback( mpWindow, a3_engine_cursor_callback );
}

void MPEngine::mSetupOpenGL( )
{
    glEnable( GL_DEPTH_TEST ); // enable depth testing
    glDepthFunc( GL_LESS );    // use less than depth test

    glEnable( GL_BLEND );                                // enable blending
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); // use one minus blending equation

    glClearColor( 0.6862f, 0.9358f, 0.94f, 1.0f ); // clear the frame buffer to black
}

void MPEngine::mSetupShaders( )
{
    _lightingShaderProgram                        = new CSCI441::ShaderProgram( "shaders/mp.v.glsl", "shaders/mp.f.glsl" );
    _lightingShaderUniformLocations.mvpMatrix     = _lightingShaderProgram->getUniformLocation( "mvpMatrix" );
    _lightingShaderUniformLocations.materialColor = _lightingShaderProgram->getUniformLocation( "materialColor" );
    // assign uniforms
    _lightingShaderUniformLocations.direction  = _lightingShaderProgram->getUniformLocation( "direction" );
    _lightingShaderUniformLocations.lightColor = _lightingShaderProgram->getUniformLocation( "lightColor" );
    _lightingShaderUniformLocations.nMatrix    = _lightingShaderProgram->getUniformLocation( "nMatrix" );

    _lightingShaderUniformLocations.modelMatrix    = _lightingShaderProgram->getUniformLocation( "modelMatrix" );
    _lightingShaderUniformLocations.cameraPosition = _lightingShaderProgram->getUniformLocation( "cameraPosition" );

    _lightingShaderUniformLocations.spotLightCutoff      = _lightingShaderProgram->getUniformLocation( "spotLightCutoff" );
    _lightingShaderUniformLocations.spotLightDirection   = _lightingShaderProgram->getUniformLocation( "spotLightDirection" );
    _lightingShaderUniformLocations.spotLightPosition    = _lightingShaderProgram->getUniformLocation( "spotLightPosition" );
    _lightingShaderUniformLocations.spotLightOuterCutoff = _lightingShaderProgram->getUniformLocation( "spotLightOuterCutoff" );

    _lightingShaderUniformLocations.pointLightPosition = _lightingShaderProgram->getUniformLocation( "pointLightPosition" );

    _lightingShaderAttributeLocations.vPos = _lightingShaderProgram->getAttributeLocation( "vPos" );
    // assign attributes
    _lightingShaderAttributeLocations.vNormal = _lightingShaderProgram->getAttributeLocation( "vNormal" );

    // Set up the skybox shader program
    _skyboxShaderProgram = new CSCI441::ShaderProgram( "shaders/skybox.v.glsl", "shaders/skybox.f.glsl" );

    // Get uniform locations
    _skyboxShaderUniformLocations.modelMatrix      = _skyboxShaderProgram->getUniformLocation( "model" );
    _skyboxShaderUniformLocations.viewMatrix       = _skyboxShaderProgram->getUniformLocation( "view" );
    _skyboxShaderUniformLocations.projectionMatrix = _skyboxShaderProgram->getUniformLocation( "projection" );
    _skyboxShaderUniformLocations.skyboxTexture    = _skyboxShaderProgram->getUniformLocation( "skyboxTexture" );
}

void MPEngine::mSetupTextures( ) { _skyTex = _loadAndRegisterSkyboxTexture( "textures/cubeMapFrozen.jpg" ); }

void MPEngine::mSetupBuffers( )
{
    // TODO #4: need to connect our 3D Object Library to our shader
    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos, _lightingShaderAttributeLocations.vNormal );

    // give the plane the normal matrix location
    _pTav =
        new Tav( _lightingShaderProgram->getShaderProgramHandle( ), _lightingShaderUniformLocations.mvpMatrix, _lightingShaderUniformLocations.nMatrix, _lightingShaderUniformLocations.materialColor );
    _pBeing = new Being( _lightingShaderProgram->getShaderProgramHandle( ),
                         _lightingShaderUniformLocations.mvpMatrix,
                         _lightingShaderUniformLocations.nMatrix,
                         _lightingShaderUniformLocations.materialColor );
    _pHorse = new horse( _lightingShaderProgram->getShaderProgramHandle( ),
                         _lightingShaderUniformLocations.mvpMatrix,
                         _lightingShaderUniformLocations.nMatrix,
                         _lightingShaderUniformLocations.materialColor,
                         GRID_WIDTH );
    _pCar   = new Car( _lightingShaderProgram->getShaderProgramHandle( ),
                     _lightingShaderUniformLocations.mvpMatrix,
                     _lightingShaderUniformLocations.nMatrix,
                     _lightingShaderUniformLocations.materialColor,
                     GRID_WIDTH / 2 + 5 );

    _pObjModel = new CSCI441::ModelLoader( );
    _pObjModel->enableAutoGenerateNormals( );
    if ( _pObjModel->loadModelFile( "models/plant.obj" ) )
    {
        _pObjModel->setAttributeLocations( _lightingShaderAttributeLocations.vPos, _lightingShaderAttributeLocations.vNormal );
    }
    else
    {
        fprintf( stderr, "[ERROR]: Could not open OBJ Model\n" );
        delete _pObjModel;
        _pObjModel = nullptr;
    }

    _pObjModelB = new CSCI441::ModelLoader( );
    _pObjModelB->enableAutoGenerateNormals( );
    if ( _pObjModelB->loadModelFile( "models/bunny.obj" ) )
    {
        _pObjModelB->setAttributeLocations( _lightingShaderAttributeLocations.vPos, _lightingShaderAttributeLocations.vNormal );
    }
    else
    {
        fprintf( stderr, "[ERROR]: Could not open OBJ Model\n" );
        delete _pObjModelB;
        _pObjModelB = nullptr;
    }
    _pObjModelC = new CSCI441::ModelLoader( );
    if ( _pObjModelC->loadModelFile( "models/cottage.obj" ) )
    {
        _pObjModelC->setAttributeLocations( _lightingShaderAttributeLocations.vPos, _lightingShaderAttributeLocations.vNormal );
    }
    else
    {
        fprintf( stderr, "[ERROR]: Could not open OBJ Model\n" );
        delete _pObjModelB;
        _pObjModelC = nullptr;
    }
    _createGroundBuffers( );
    _generateEnvironment( );

    // Create skybox buffers
    _createSkyboxBuffers( );
}

void MPEngine::_createGroundBuffers( )
{
    // TODO #8: expand our struct
    struct Vertex
    {
            glm::vec3 position;
            glm::vec3 vNormal;
    };

    // Calculate the number of divisions based on grid size and spacing
    int numDivisionsX = static_cast<int>( GRID_WIDTH / GRID_SPACING_WIDTH );
    int numDivisionsZ = static_cast<int>( GRID_LENGTH / GRID_SPACING_LENGTH );

    // Total number of vertices
    int totalVertices = ( numDivisionsX + 1 ) * ( numDivisionsZ + 1 );
    std::vector<Vertex> groundVertices;
    groundVertices.reserve( totalVertices );

    // Generate vertices
    for ( int i = 0; i <= numDivisionsX; ++i )
    {
        for ( int j = 0; j <= numDivisionsZ; ++j )
        {
            GLfloat x = LEFT_END_POINT + i * GRID_SPACING_WIDTH;
            GLfloat z = BOTTOM_END_POINT + j * GRID_SPACING_LENGTH;
            GLfloat y = 0.0f; // Ground level

            groundVertices.push_back( Vertex{ glm::vec3( x, y, z ), glm::vec3( 0.0f, 1.0f, 0.0f ) } );
        }
    }

    // Generate indices for GL_TRIANGLES
    std::vector<GLuint> indices;
    for ( int i = 0; i < numDivisionsX; ++i )
    {
        for ( int j = 0; j < numDivisionsZ; ++j )
        {
            int topLeft     = i * ( numDivisionsZ + 1 ) + j;
            int bottomLeft  = ( i + 1 ) * ( numDivisionsZ + 1 ) + j;
            int topRight    = topLeft + 1;
            int bottomRight = bottomLeft + 1;

            // First triangle
            indices.push_back( topLeft );
            indices.push_back( bottomLeft );
            indices.push_back( topRight );

            // Second triangle
            indices.push_back( topRight );
            indices.push_back( bottomLeft );
            indices.push_back( bottomRight );
        }
    }

    _numGroundPoints = indices.size( );

    // Create VAO and VBOs
    glGenVertexArrays( 1, &_groundVAO );
    glBindVertexArray( _groundVAO );

    GLuint vbods[2]; // 0 - VBO, 1 - EBO
    glGenBuffers( 2, vbods );

    // Vertex Buffer
    glBindBuffer( GL_ARRAY_BUFFER, vbods[0] );
    glBufferData( GL_ARRAY_BUFFER, groundVertices.size( ) * sizeof( Vertex ), groundVertices.data( ), GL_STATIC_DRAW );

    // Element Buffer
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbods[1] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size( ) * sizeof( GLuint ), indices.data( ), GL_STATIC_DRAW );

    // Position Attribute
    glEnableVertexAttribArray( _lightingShaderAttributeLocations.vPos );
    glVertexAttribPointer( _lightingShaderAttributeLocations.vPos, // Attribute location
                           3,                                      // Number of components (x, y, z)
                           GL_FLOAT,                               // Data type
                           GL_FALSE,                               // Normalized?
                           sizeof( Vertex ),                       // Stride
                           (void*)offsetof( Vertex, position )     // Offset
    );

    // Normal Attribute
    glEnableVertexAttribArray( _lightingShaderAttributeLocations.vNormal );
    glVertexAttribPointer( _lightingShaderAttributeLocations.vNormal, // Attribute location
                           3,                                         // Number of components (nx, ny, nz)
                           GL_FLOAT,                                  // Data type
                           GL_FALSE,                                  // Normalized?
                           sizeof( Vertex ),                          // Stride
                           (void*)offsetof( Vertex, vNormal )         // Offset
    );

    glBindVertexArray( 0 ); // Unbind VAO
}

void MPEngine::_generateEnvironment( )
{
    //******************************************************************
    // cottage time

    //******************************************************************

    srand( time( 0 ) ); // seed our RNG

    // psych! everything's on a grid.
    for ( int i = LEFT_END_POINT; i < RIGHT_END_POINT; i += GRID_SPACING_WIDTH )
    {
        for ( int j = BOTTOM_END_POINT; j < TOP_END_POINT; j += GRID_SPACING_LENGTH )
        {
            if ( i % 2 && j % 2 && getRand( ) < 0.05f )
            {
                float r = getRand( );
                if ( r < 0.4f )
                {
                    glm::mat4 transToSpotMtx = glm::translate( glm::mat4( 1.0 ), glm::vec3( i, 0.0f, j ) );

                    // Calculate a uniform scale factor with reduced range
                    GLdouble scaleFactor = std::max( 1.0, pow( getRand( ), 2.0 ) * 5 ) * 1.2;

                    // Apply uniform scaling to all dimensions
                    glm::mat4 scaleToHeightMtx = glm::scale( glm::mat4( 1.0 ), glm::vec3( scaleFactor, scaleFactor, scaleFactor ) );

                    // Translate the tree to the ground level based on its height
                    glm::mat4 transToHeight = glm::translate( glm::mat4( 1.0 ), glm::vec3( 0, 0, 0 ) );

                    glm::mat4 modelMatrix = transToSpotMtx * transToHeight * scaleToHeightMtx;

                    TreeData currentTreeSet   = { modelMatrix };
                    currentTreeSet.isTwoTrees = getRand( ) < 0.5f;
                    _trees.emplace_back( currentTreeSet );
                }
                else if ( r < 0.8 )
                { // do it for plants
                    // translate to spot
                    glm::mat4 transToSpotMtxPlant = glm::translate( glm::mat4( 1.0 ), glm::vec3( i, 0.0f, j ) );

                    // translate up to grid
                    glm::mat4 transToHeightPlant = glm::translate( glm::mat4( 1.0 ), glm::vec3( 0, -0.5 - getRand( ), 0 ) );

                    // compute full model matrix
                    glm::mat4 modelMatrixPlant = transToHeightPlant * transToSpotMtxPlant;

                    // store building properties
                    PlantData currentPlant = { modelMatrixPlant };
                    _plants.emplace_back( currentPlant );
                }
                else
                {
                    // randomly rotate pur bunny
                    glm::mat4 rotateBunny = glm::rotate( glm::mat4( 1.0 ), getRand( ), CSCI441::Y_AXIS );
                    // translate to spot
                    glm::mat4 transToSpotMtxBunny = glm::translate( glm::mat4( 1.0 ), glm::vec3( i, 0.0f, j ) );

                    // translate up to grid
                    glm::mat4 transToHeightBunny = glm::translate( glm::mat4( 1.0 ), glm::vec3( 0, 1, 0 ) );
                    glm::mat4 scaleToHeightMtx   = glm::scale( glm::mat4( 1.0 ), glm::vec3( 0.75, 0.75, 0.75 ) );
                    // compute full model matrix
                    glm::mat4 modelMatrixBunny = rotateBunny * transToHeightBunny * transToSpotMtxBunny * scaleToHeightMtx;

                    // store building properties
                    BunnyData currentBunny = { modelMatrixBunny };
                    _bunnies.emplace_back( currentBunny );
                }
            }
        }
    }
}

void MPEngine::mSetupScene( )
{
    _pArcballCam     = new ArcBall( );
    _pFreeCam        = new FreeCam( );
    _pFirstPersonCam = new FirstPerson( );
    glm::vec3 targetPosition;
    if ( _currentCharacter == 0 )
    {
        targetPosition = _pTav->getPosition( );
    }
    else if ( _currentCharacter == 1 )
    {
        targetPosition = _pBeing->getPosition( );
    }
    else if ( _currentCharacter == 2 )
    {
        targetPosition = _pHorse->getHorsePos( );
    }
    else if ( _currentCharacter == 3 )
    {
        targetPosition = _pCar->getPosition( );
    }

    // Define an offset vector for the camera
    _cameraOffset = glm::vec3( 5.0f, 3.0f, 5.0f );

    // Set the camera's look-at point to be the player's position
    _pArcballCam->setCameraLookAtPoint( targetPosition );

    // Set the camera's position to be offset from the player's position
    _pArcballCam->setCameraPosition( targetPosition + _cameraOffset );

    glm::vec3 arcballDirection = glm::normalize( ( targetPosition + _cameraOffset ) - targetPosition );

    _pArcballCam->setTheta( atan2( arcballDirection.z, arcballDirection.x ) );
    _pArcballCam->setPhi( acos( arcballDirection.y ) );
    _pArcballCam->recomputeOrientation( );

    _pFreeCam->setPosition( _pArcballCam->getPosition( ) );

    _pFreeCam->setPosition( _pArcballCam->getPosition( ) );

    // Compute the direction vector for FreeCam
    glm::vec3 freeCamDirection = glm::normalize( targetPosition - _pFreeCam->getPosition( ) );

    // Compute Theta and Phi based on FreeCam's definitions
    GLfloat phi   = acos( freeCamDirection.y );
    GLfloat theta = atan2( freeCamDirection.z, freeCamDirection.x );

    _pFreeCam->setTheta( theta );
    _pFreeCam->setPhi( phi );
    _pFreeCam->recomputeOrientation( );

    _pActiveCamera = _pArcballCam;
    _cameraSpeed   = glm::vec2( 0.25f, 0.02f );

    // TODO #6: set lighting uniforms
    glm::vec3 direction = glm::vec3( -1, -1, -1 );
    glm::vec3 color     = glm::vec3( 1, 1, 1 );

    glm::vec3 spotLightPosition  = glm::vec3( 20.0f, 25.0f, 0.0f );
    glm::vec3 spotLightDirection = glm::vec3( 0.0f, -1.0f, 0.0f );
    GLfloat spotLightCutoff      = glm::cos( glm::radians( 20.5f ) );
    GLfloat spotLightOuterCutoff = glm::cos( glm::radians( 23.5f ) );

    glm::vec3 pointLightPosition = glm::vec3( 0.0f, 0.0f, 0.0f );

    glProgramUniform3fv( _lightingShaderProgram->getShaderProgramHandle( ), _lightingShaderUniformLocations.pointLightPosition, 1, glm::value_ptr( pointLightPosition ) );

    glProgramUniform3fv( _lightingShaderProgram->getShaderProgramHandle( ), _lightingShaderUniformLocations.spotLightPosition, 1, glm::value_ptr( spotLightPosition ) );

    glProgramUniform3fv( _lightingShaderProgram->getShaderProgramHandle( ), _lightingShaderUniformLocations.spotLightDirection, 1, glm::value_ptr( spotLightDirection ) );

    glProgramUniform1f( _lightingShaderProgram->getShaderProgramHandle( ), _lightingShaderUniformLocations.spotLightOuterCutoff, spotLightOuterCutoff );

    glProgramUniform1f( _lightingShaderProgram->getShaderProgramHandle( ), _lightingShaderUniformLocations.spotLightCutoff, spotLightCutoff );

    glProgramUniform3fv( _lightingShaderProgram->getShaderProgramHandle( ), _lightingShaderUniformLocations.direction, 1, glm::value_ptr( direction ) );

    glProgramUniform3fv( _lightingShaderProgram->getShaderProgramHandle( ), _lightingShaderUniformLocations.lightColor, 1, glm::value_ptr( color ) );
}

//*************************************************************************************
//
// Engine Cleanup

void MPEngine::mCleanupShaders( )
{
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    delete _lightingShaderProgram;
    delete _skyboxShaderProgram;
}

void MPEngine::mCleanupBuffers( )
{
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs( );
    glDeleteVertexArrays( 1, &_groundVAO );

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs( );

    fprintf( stdout, "[INFO]: ...deleting models..\n" );

    glDeleteVertexArrays( 1, &_skyboxVAO );
    glDeleteBuffers( 1, &_skyboxVBO );
}

void MPEngine::mCleanupTextures( )
{
    fprintf( stdout, "[INFO]: ...deleting textures\n" );
    // TODO #23 - delete textures
    glDeleteTextures( 1, reinterpret_cast<const GLuint*>( &_skyTex ) );
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void MPEngine::_renderScene( glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
    // Save current depth function and depth mask state
    GLint prevDepthFunc;
    glGetIntegerv( GL_DEPTH_FUNC, &prevDepthFunc );
    GLboolean prevDepthMask;
    glGetBooleanv( GL_DEPTH_WRITEMASK, &prevDepthMask );

    // First, render the skybox
    glDepthFunc( GL_LEQUAL ); // Change depth function for skybox
    glDepthMask( GL_FALSE );  // Disable depth writing

    _skyboxShaderProgram->useProgram( );

    // Remove translation from the view matrix
    glm::mat4 view = glm::mat4( glm::mat3( viewMtx ) );

    // Set shader uniforms
    _skyboxShaderProgram->setProgramUniform( _skyboxShaderUniformLocations.modelMatrix, glm::mat4( 1.0f ) );
    _skyboxShaderProgram->setProgramUniform( _skyboxShaderUniformLocations.viewMatrix, view );
    _skyboxShaderProgram->setProgramUniform( _skyboxShaderUniformLocations.projectionMatrix, projMtx );

    // Bind the skybox texture
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, _skyTex );
    _skyboxShaderProgram->setProgramUniform( _skyboxShaderUniformLocations.skyboxTexture, 0 );

    // Render the skybox cube
    glBindVertexArray( _skyboxVAO );
    glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );
    glBindVertexArray( 0 );

    // Restore depth function and depth mask
    glDepthFunc( prevDepthFunc );
    glDepthMask( prevDepthMask );

    // Then, render the rest of the scene using your lighting shader
    _lightingShaderProgram->useProgram( );

    //// BEGIN DRAWING THE GROUND PLANE ////
    _lightingShaderProgram->useProgram( );

    // Set the ground's model matrix (scale to actual ground size)
    glm::mat4 groundModelMtx = glm::scale( glm::mat4( 1.0f ), glm::vec3( GRID_WIDTH, 1.0f, GRID_LENGTH ) );
    _computeAndSendMatrixUniforms( groundModelMtx, viewMtx, projMtx );

    // Set the ground color
    glm::vec3 groundColor( 0.15f, 0.35f, 0.15f ); // Slightly lighter green color
    _lightingShaderProgram->setProgramUniform( _lightingShaderUniformLocations.materialColor, groundColor );

    glBindVertexArray( _groundVAO );
    glDrawElements( GL_TRIANGLES, _numGroundPoints, GL_UNSIGNED_INT, 0 );
    glBindVertexArray( 0 );
    //// END DRAWING THE GROUND PLANE ////

    //// END SKYBOX /////
    //// BEGIN DRAWING THE BUILDINGS ////
    for ( const TreeData& currentTree : _trees )
    {
        _computeAndSendMatrixUniforms( currentTree.modelMatrix, viewMtx, projMtx );

        if ( !currentTree.isTwoTrees )
        {
            _drawSingleTree( currentTree, viewMtx, projMtx );
        }
        else
        {
            _drawTwoTrees( currentTree, viewMtx, projMtx );
        }
    }
    for ( const PlantData& currentPlant : _plants )
    {
        _computeAndSendMatrixUniforms( currentPlant.modelMatrix, viewMtx, projMtx );

        _lightingShaderProgram->setProgramUniform( _lightingShaderUniformLocations.materialColor, currentPlant.color );

        if ( _pObjModel != nullptr )
        {
            if ( !_pObjModel->draw( _lightingShaderProgram->getShaderProgramHandle( ) ) )
            {
                fprintf( stderr, "[ERROR]: Could not draw OBJ Model\n" );
                glfwSetWindowShouldClose( mpWindow, GLFW_TRUE );
            }
        }
    }
    for ( const BunnyData& currentBunny : _bunnies )
    {
        _computeAndSendMatrixUniforms( currentBunny.modelMatrix, viewMtx, projMtx );

        _lightingShaderProgram->setProgramUniform( _lightingShaderUniformLocations.materialColor, currentBunny.color );

        if ( _pObjModelB != nullptr )
        {
            if ( !_pObjModelB->draw( _lightingShaderProgram->getShaderProgramHandle( ) ) )
            {
                fprintf( stderr, "[ERROR]: Could not draw OBJ Model\n" );
                glfwSetWindowShouldClose( mpWindow, GLFW_TRUE );
            }
        }
    }
    // draw the cottage for extra spice
    //  translate to spot
    glm::mat4 transToSpotMtx = glm::translate( glm::mat4( 1.0 ), glm::vec3( -25, 0.0f, -25 ) );

    // compute full model matrix
    glm::mat4 modelMatrix = transToSpotMtx;

    _computeAndSendMatrixUniforms( modelMatrix, viewMtx, projMtx );

    _lightingShaderProgram->setProgramUniform( _lightingShaderUniformLocations.materialColor, glm::vec3( 0.45, 0.3065, 0.0585 ) );

    if ( _pObjModelC != nullptr )
    {
        if ( !_pObjModelC->draw( _lightingShaderProgram->getShaderProgramHandle( ) ) )
        {
            fprintf( stderr, "[ERROR]: Could not draw OBJ Model\n" );
            glfwSetWindowShouldClose( mpWindow, GLFW_TRUE );
        }
    }

    //// END DRAWING THE BUILDINGS ////

    //// BEGIN DRAWING TAV ////
    glm::mat4 modelMtx1( 1.0f );

    modelMtx1 = glm::translate( modelMtx1, _pTav->getPosition( ) );

    // TODO: rotate tav without arcball

    // draw our Tav now
    _computeAndSendMatrixUniforms( modelMtx1, viewMtx, projMtx );
    _pTav->drawTav( modelMtx1, viewMtx, projMtx );
    glm::mat4 modelMtx2( 1.0f );
    modelMtx2 = glm::translate( modelMtx2, _pBeing->getPosition( ) );
    _computeAndSendMatrixUniforms( modelMtx2, viewMtx, projMtx );
    _pBeing->drawPerson( modelMtx2, viewMtx, projMtx );
    glm::mat4 modelMtx3( 1.0f );
    glm::mat4 lightingMat = glm::translate( modelMtx3, _pHorse->getHorsePos( ) );
    _computeAndSendMatrixUniforms( lightingMat, viewMtx, projMtx );
    _pHorse->drawHorse( modelMtx3, viewMtx, projMtx );
    glm::mat4 modelMtx4( 1.0f );
    glm::mat4 lightingMat2 = glm::translate( modelMtx4, _pCar->getPosition( ) );
    _pCar->drawCar( modelMtx4, viewMtx, projMtx );
    _computeAndSendMatrixUniforms( lightingMat2, viewMtx, projMtx );

    //// END DRAWING TAV ////
}

void MPEngine::_updateScene( )
{
    if ( _keys[GLFW_KEY_SPACE] )
    {

        // go backward if shift held down
        if ( _keys[GLFW_KEY_LEFT_SHIFT] || _keys[GLFW_KEY_RIGHT_SHIFT] )
        {
            if ( _pActiveCamera == _pArcballCam )
            {
                _pArcballCam->zoom( 0.2 );
            }
            else
            {
                _pFreeCam->moveBackward( 0.2 );
            }
        }
        // go forward
        else
        {
            if ( _pActiveCamera == _pArcballCam )
            {
                _pArcballCam->zoom( -0.2 );
            }
            else
            {
                _pFreeCam->moveForward( 0.2 );
            }
        }
    }
    // Define the boundaries of the grid
    const GLfloat minX = LEFT_END_POINT;
    const GLfloat maxX = RIGHT_END_POINT;
    const GLfloat minZ = BOTTOM_END_POINT;
    const GLfloat maxZ = TOP_END_POINT;

    _pTav->update( );
    _pBeing->moveNose( );

    // Get the current position of the character
    glm::vec3 currentPosition;
    if ( _currentCharacter == 0 )
    {
        currentPosition = _pTav->getPosition( );
    }
    if ( _currentCharacter == 1 )
    {
        currentPosition = _pBeing->getPosition( );
    }
    if ( _currentCharacter == 2 )
    {
        currentPosition = _pHorse->getHorsePos( );
    }
    if ( _currentCharacter == 3 )
    {
        currentPosition = _pCar->getPosition( );
    }

    // Calculate the new position based on input
    glm::vec3 newPosition = currentPosition;
    float edge            = GRID_WIDTH / 2.0f + 5.0f;
    if ( _keys[GLFW_KEY_W] || _keys[GLFW_KEY_UP] )
    {
        if ( _currentCharacter == 0 )
        {
            float nextx = newPosition.x + _pTav->getForwardDirection( ).x * _pTav->tavSpeed;
            float nextz = newPosition.z + _pTav->getForwardDirection( ).z * _pTav->tavSpeed;

            if ( nextx < edge && nextz < edge && nextx > -edge && nextz > -edge )
            {
                newPosition += _pTav->getForwardDirection( ) * _pTav->tavSpeed;
            }
        }
        if ( _currentCharacter == 1 )
        {
            float nextx = newPosition.x + _pBeing->getForwardDirection( ).x * _pTav->tavSpeed;
            float nextz = newPosition.z + _pBeing->getForwardDirection( ).z * _pTav->tavSpeed;

            if ( nextx < edge && nextz < edge && nextx > -edge && nextz > -edge )
            {
                newPosition += _pBeing->getForwardDirection( ) * _pTav->tavSpeed;
            }
        }
        if ( _currentCharacter == 2 )
        {
            _pHorse->moveForward( );
            newPosition = _pHorse->getHorsePos( );
        }
        if ( _currentCharacter == 3 )
        {
            _pCar->moveForward( );
            newPosition = _pCar->getPosition( );
        }
    }
    if ( _keys[GLFW_KEY_S] || _keys[GLFW_KEY_DOWN] )
    {
        if ( _currentCharacter == 0 )
        {
            float nextx = newPosition.x - _pTav->getForwardDirection( ).x * _pTav->tavSpeed;
            float nextz = newPosition.z - _pTav->getForwardDirection( ).z * _pTav->tavSpeed;
            if ( nextx < edge && nextz < edge && nextx > -edge && nextz > -edge )
            {
                newPosition -= _pTav->getForwardDirection( ) * _pTav->tavSpeed;
            }
        }
        if ( _currentCharacter == 1 )
        {
            float nextx = newPosition.x - _pBeing->getForwardDirection( ).x * _pTav->tavSpeed;
            float nextz = newPosition.z - _pBeing->getForwardDirection( ).z * _pTav->tavSpeed;
            if ( nextx < edge && nextz < edge && nextx > -edge && nextz > -edge )
            {
                newPosition -= _pBeing->getForwardDirection( ) * _pTav->tavSpeed;
            }
        }
        if ( _currentCharacter == 2 )
        {
            _pHorse->moveBackward( );
            newPosition = _pHorse->getHorsePos( );
        }
        if ( _currentCharacter == 3 )
        {
            _pCar->moveBackward( );
            newPosition = _pCar->getPosition( );
        }
    }
    if ( _keys[GLFW_KEY_D] || _keys[GLFW_KEY_RIGHT] )
    {
        if ( _currentCharacter == 0 )
        {
            _pTav->rotate( -_pTav->tavRotationSpeed );
        }
        if ( _currentCharacter == 1 )
        {
            _pBeing->rotateSelf( -_pTav->tavRotationSpeed );
        }
        else if ( _currentCharacter == 2 )
        {
            _pHorse->turnLeft( );
        }
        else if ( _currentCharacter == 3 )
        {
            _pCar->turnLeft( );
        }
        _angle -= _pTav->tavRotationSpeed;
    }
    if ( _keys[GLFW_KEY_A] || _keys[GLFW_KEY_LEFT] )
    {
        if ( _currentCharacter == 0 )
        {
            _pTav->rotate( _pTav->tavRotationSpeed );
        }
        else if ( _currentCharacter == 1 )
        {
            _pBeing->rotateSelf( _pTav->tavRotationSpeed );
        }
        else if ( _currentCharacter == 2 )
        {
            _pHorse->turnRight( );
        }
        else if ( _currentCharacter == 3 )
        {
            _pCar->turnRight( );
        }
        _angle += _pTav->tavRotationSpeed;
    }

    switch ( _currentCharacter )
    {
        case 0:
            _pTav->setForwardDirection( );
            _pTav->setPosition( newPosition );
            break;
        case 1:
            _pBeing->setForwardDirection( );
            _pBeing->setPosition( newPosition );
            break;
        case 2: _pHorse->setForwardDirection( ); break;
        case 3: _pCar->setForwardDirection( ); break;
        default: break;
    }

    // Update the camera's position and look-at point based on the current character
    glm::vec3 updatedPosition;
    switch ( _currentCharacter )
    {
        case 0: updatedPosition = _pTav->getPosition( ); break;
        case 1: updatedPosition = _pBeing->getPosition( ); break;
        case 2: updatedPosition = _pHorse->getHorsePos( ); break;
        case 3: updatedPosition = _pCar->getPosition( ); break;
        default: updatedPosition = glm::vec3( 0.0f ); break;
    }

    // Set camera position directly based on the updated character position and offset
    _pArcballCam->setCameraPosition( updatedPosition + _cameraOffset );
    _pArcballCam->setCameraLookAtPoint( updatedPosition );
    _pArcballCam->recomputeOrientation( );

    // Update the first-person camera if active
    if ( _toggleFirst )
    {
        if ( _currentCharacter == 2 )
        {
            _pFirstPersonCam->setTheta( _pHorse->_horseAngle );
        }
        else
        {
            _pFirstPersonCam->setTheta( -_angle );
        }
        _pFirstPersonCam->setPhi( 0.0f );
        _pFirstPersonCam->setCameraDirection( glm::normalize( updatedPosition - _pArcballCam->getPosition( ) ) );
        if ( _currentCharacter == 2 )
        {
            _pFirstPersonCam->setCameraPosition( newPosition + glm::vec3( 0.1f, 4.0f, 0.0f ) );
        }
        else
        {
            _pFirstPersonCam->setCameraPosition( newPosition + glm::vec3( 0.0f, 1.0f, 0.0f ) );
        }
        _pFirstPersonCam->recomputeOrientation( );
    }

    // Update shader with camera position
    glProgramUniform3fv( _lightingShaderProgram->getShaderProgramHandle( ), _lightingShaderUniformLocations.cameraPosition, 1, glm::value_ptr( _pActiveCamera->getPosition( ) ) );
}

void MPEngine::run( )
{
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while ( !glfwWindowShouldClose( mpWindow ) )
    {                                                         // check if the window was instructed to be closed
        glDrawBuffer( GL_BACK );                              // work with our back frame buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the current color contents and depth buffer in the window

        // Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
        // when using a Retina display the actual window can be larger than the requested window.  Therefore,
        // query what the actual size of the window we are rendering to is.
        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( mpWindow, &framebufferWidth, &framebufferHeight );

        // update the viewport - tell OpenGL we want to render to the whole window
        glViewport( 0, 0, framebufferWidth, framebufferHeight );

        // draw everything to the window
        _renderScene( _pActiveCamera->getViewMatrix( ), _pActiveCamera->getProjectionMatrix( ) );

        if ( _toggleFirst )
        {
            glViewport( 0, 0, 200, 200 );
            glClear( GL_DEPTH_BUFFER_BIT );
            _renderScene( _pFirstPersonCam->getViewMatrix( ), _pFirstPersonCam->getProjectionMatrix( ) );
        }

        _updateScene( );

        glfwSwapBuffers( mpWindow ); // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents( );           // check for any events and signal to redraw screen
    }
}

// for our beautiful textures
GLuint MPEngine::_loadAndRegisterTexture( const char* FILENAME )
{
    // our handle to the GPU
    GLuint textureHandle = 0;

    // enable setting to prevent image from being upside down
    stbi_set_flip_vertically_on_load( true );

    // will hold image parameters after load
    GLint imageWidth, imageHeight, imageChannels;
    // load image from file
    GLubyte* data = stbi_load( FILENAME, &imageWidth, &imageHeight, &imageChannels, 0 );

    // if data was read from file
    if ( data )
    {
        const GLint STORAGE_TYPE = ( imageChannels == 4 ? GL_RGBA : GL_RGB );

        // TODO #01 - generate a texture handle
        glGenTextures( 1, &textureHandle );
        // TODO #02 - bind it to be active
        glBindTexture( GL_TEXTURE_2D, textureHandle );
        // set texture parameters
        // TODO #03 - mag filter
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        // TODO #04 - min filter
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        // TODO #05 - wrap s
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        // TODO #06 - wrap t
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        // TODO #07 - transfer image data to the GPU
        glTexImage2D( GL_TEXTURE_2D, 0, STORAGE_TYPE, imageWidth, imageHeight, 0, STORAGE_TYPE, GL_UNSIGNED_BYTE, data );

        fprintf( stdout, "[INFO]: %s texture map read in with handle %d\n", FILENAME, textureHandle );

        // release image memory from CPU - it now lives on the GPU
        stbi_image_free( data );
    }
    else
    {
        // load failed
        fprintf( stderr, "[ERROR]: Could not load texture map \"%s\"\n", FILENAME );
    }

    // return generated texture handle
    return textureHandle;
}

GLuint MPEngine::_loadAndRegisterSkyboxTexture( const char* FILENAME )
{
    // Our handle to the GPU texture
    GLuint textureHandle = 0;

    // Disable vertical flipping to load the image as-is
    stbi_set_flip_vertically_on_load( true );

    // Will hold image parameters after load
    GLint imageWidth, imageHeight, imageChannels;

    // Load image from file
    GLubyte* data = stbi_load( FILENAME, &imageWidth, &imageHeight, &imageChannels, 0 );

    // Check if data was read from file
    if ( data )
    {
        // Determine the storage format
        GLint STORAGE_TYPE = ( imageChannels == 4 ? GL_RGBA : GL_RGB );

        // Generate a texture handle
        glGenTextures( 1, &textureHandle );
        // Bind it to be active
        glBindTexture( GL_TEXTURE_2D, textureHandle );

        // Set texture parameters
        // Set texture filtering
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        // Set texture wrapping to clamp to edge
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); // Wrap horizontally
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); // Wrap vertically

        // Transfer image data to the GPU
        glTexImage2D( GL_TEXTURE_2D, 0, STORAGE_TYPE, imageWidth, imageHeight, 0, STORAGE_TYPE, GL_UNSIGNED_BYTE, data );

        // Print out image info for debugging
        fprintf( stdout, "[INFO]: Skybox texture loaded: %s (Width: %d, Height: %d, Channels: %d)\n", FILENAME, imageWidth, imageHeight, imageChannels );

        // Release image memory from CPU - it now lives on the GPU
        stbi_image_free( data );
    }
    else
    {
        // Load failed
        fprintf( stderr, "[ERROR]: Could not load skybox texture \"%s\"\n", FILENAME );
    }

    // Return generated texture handle
    return textureHandle;
}

//*************************************************************************************
//
// Private Helper Functions

void MPEngine::_computeAndSendMatrixUniforms( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    _lightingShaderProgram->setProgramUniform( _lightingShaderUniformLocations.mvpMatrix, mvpMtx );

    _lightingShaderProgram->setProgramUniform( _lightingShaderUniformLocations.modelMatrix, modelMtx );

    // compute and send the normal matrix
    glm::mat3 nMatrix = glm::mat3( glm::transpose( glm::inverse( modelMtx ) ) );

    _lightingShaderProgram->setProgramUniform( _lightingShaderUniformLocations.nMatrix, nMatrix );

    glm::vec3 cameraPosition = _pArcballCam->getPosition( );
    _lightingShaderProgram->setProgramUniform( _lightingShaderUniformLocations.cameraPosition, cameraPosition );
}

void MPEngine::_drawSingleTree( const TreeData& treeData, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
    // Draw the trunk of the tree
    glm::mat4 trunkModelMtx = glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.1f, 0.5f, 0.1f ) );
    trunkModelMtx           = treeData.modelMatrix * trunkModelMtx;
    _computeAndSendMatrixUniforms( trunkModelMtx, viewMtx, projMtx );
    _lightingShaderProgram->setProgramUniform( _lightingShaderUniformLocations.materialColor, glm::vec3( 0.55f, 0.27f, 0.07f ) );
    CSCI441::drawSolidCylinder( 0.5f, 0.5f, 1.1f, 16, 16 );

    // Draw the foliage of the tree
    glm::mat4 foliageModelMtx = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.5f, 0.0f ) );
    foliageModelMtx           = glm::scale( foliageModelMtx, glm::vec3( 0.5f, 0.7f, 0.5f ) );
    foliageModelMtx           = treeData.modelMatrix * foliageModelMtx;
    _computeAndSendMatrixUniforms( foliageModelMtx, viewMtx, projMtx );
    _lightingShaderProgram->setProgramUniform( _lightingShaderUniformLocations.materialColor, glm::vec3( 0.0f, 0.5f, 0.0f ) );
    CSCI441::drawSolidCone( 0.8f, 1.8f, 16, 16 );
}

void MPEngine::_drawTwoTrees( const TreeData& treeData, glm::mat4 viewMtx, glm::mat4 projMtx ) const
{
    // Draw the first tree
    _drawSingleTree( treeData, viewMtx, projMtx );

    // Draw the second tree
    glm::mat4 secondTreeModelMtx = glm::translate( treeData.modelMatrix, glm::vec3( 3.0f, 0.0f, 0.0f ) );
    secondTreeModelMtx           = glm::scale( secondTreeModelMtx, glm::vec3( 1.3f, 1.3f, 1.3f ) );
    TreeData secondTreeData      = { secondTreeModelMtx };
    _drawSingleTree( secondTreeData, viewMtx, projMtx );
}

void MPEngine::_createSkyboxBuffers( )
{
    struct SkyboxVertex
    {
            glm::vec3 position;
            glm::vec2 texCoords;
    };

    const float texWidth  = 1.0f / 4.0f;
    const float texHeight = 1.0f / 3.0f;

    std::vector<SkyboxVertex> vertices = {
        // Right face (+X)
        {  glm::vec3( 1, -1, -1 ), glm::vec2( 2 * texWidth,     texHeight ) }, // Bottom-left
        {  glm::vec3( 1, -1,  1 ), glm::vec2( 3 * texWidth,     texHeight ) }, // Bottom-right
        {  glm::vec3( 1,  1,  1 ), glm::vec2( 3 * texWidth, 2 * texHeight ) }, // Top-right
        {  glm::vec3( 1,  1, -1 ), glm::vec2( 2 * texWidth, 2 * texHeight ) }, // Top-left

        // Left face (-X)
        { glm::vec3( -1, -1,  1 ), glm::vec2( 0 * texWidth,     texHeight ) }, // Bottom-left
        { glm::vec3( -1, -1, -1 ), glm::vec2( 1 * texWidth,     texHeight ) }, // Bottom-right
        { glm::vec3( -1,  1, -1 ), glm::vec2( 1 * texWidth, 2 * texHeight ) }, // Top-right
        { glm::vec3( -1,  1,  1 ), glm::vec2( 0 * texWidth, 2 * texHeight ) }, // Top-left

        // Top face (+Y)
        { glm::vec3( -1,  1, -1 ), glm::vec2( 1 * texWidth, 2 * texHeight ) }, // Bottom-left
        {  glm::vec3( 1,  1, -1 ), glm::vec2( 2 * texWidth, 2 * texHeight ) }, // Bottom-right
        {  glm::vec3( 1,  1,  1 ), glm::vec2( 2 * texWidth, 3 * texHeight ) }, // Top-right
        { glm::vec3( -1,  1,  1 ), glm::vec2( 1 * texWidth, 3 * texHeight ) }, // Top-left

        // Bottom face (-Y)
        { glm::vec3( -1, -1,  1 ), glm::vec2( 1 * texWidth, 0 * texHeight ) }, // Bottom-left
        {  glm::vec3( 1, -1,  1 ), glm::vec2( 2 * texWidth, 0 * texHeight ) }, // Bottom-right
        {  glm::vec3( 1, -1, -1 ), glm::vec2( 2 * texWidth, 1 * texHeight ) }, // Top-right
        { glm::vec3( -1, -1, -1 ), glm::vec2( 1 * texWidth, 1 * texHeight ) }, // Top-left

        // Front face (+Z)
        { glm::vec3( -1, -1, -1 ), glm::vec2( 1 * texWidth,     texHeight ) }, // Bottom-left
        {  glm::vec3( 1, -1, -1 ), glm::vec2( 2 * texWidth,     texHeight ) }, // Bottom-right
        {  glm::vec3( 1,  1, -1 ), glm::vec2( 2 * texWidth, 2 * texHeight ) }, // Top-right
        { glm::vec3( -1,  1, -1 ), glm::vec2( 1 * texWidth, 2 * texHeight ) }, // Top-left

        // Back face (-Z)
        {  glm::vec3( 1, -1,  1 ), glm::vec2( 3 * texWidth,     texHeight ) }, // Bottom-left
        { glm::vec3( -1, -1,  1 ), glm::vec2( 4 * texWidth,     texHeight ) }, // Bottom-right
        { glm::vec3( -1,  1,  1 ), glm::vec2( 4 * texWidth, 2 * texHeight ) }, // Top-right
        {  glm::vec3( 1,  1,  1 ), glm::vec2( 3 * texWidth, 2 * texHeight ) }, // Top-left
    };

    // Indices for drawing the cube with GL_TRIANGLES
    std::vector<GLuint> indices = {
        // Right face
        0,
        1,
        2,
        2,
        3,
        0,

        // Left face
        4,
        5,
        6,
        6,
        7,
        4,

        // Top face
        8,
        9,
        10,
        10,
        11,
        8,

        // Bottom face
        12,
        13,
        14,
        14,
        15,
        12,

        // Front face
        16,
        17,
        18,
        18,
        19,
        16,

        // Back face
        20,
        21,
        22,
        22,
        23,
        20,
    };

    // Create VAO and VBO
    glGenVertexArrays( 1, &_skyboxVAO );
    glGenBuffers( 1, &_skyboxVBO );
    GLuint skyboxEBO;
    glGenBuffers( 1, &skyboxEBO );

    glBindVertexArray( _skyboxVAO );

    glBindBuffer( GL_ARRAY_BUFFER, _skyboxVBO );
    glBufferData( GL_ARRAY_BUFFER, vertices.size( ) * sizeof( SkyboxVertex ), &vertices[0], GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, skyboxEBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size( ) * sizeof( GLuint ), &indices[0], GL_STATIC_DRAW );

    // Position attribute
    glEnableVertexAttribArray( 0 ); // Corresponds to location 0 in shader
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( SkyboxVertex ), (void*)0 );

    // Texture coordinate attribute
    glEnableVertexAttribArray( 1 ); // Corresponds to location 1 in shader
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( SkyboxVertex ), (void*)offsetof( SkyboxVertex, texCoords ) );

    glBindVertexArray( 0 );
}

//*************************************************************************************
//
// Callbacks

void a3_engine_keyboard_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    auto engine = (MPEngine*)glfwGetWindowUserPointer( window );

    // pass the key and action through to the engine
    engine->handleKeyEvent( key, action );
}

void a3_engine_cursor_callback( GLFWwindow* window, double x, double y )
{
    auto engine = (MPEngine*)glfwGetWindowUserPointer( window );

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent( glm::vec2( x, y ) );
}

void a3_engine_mouse_button_callback( GLFWwindow* window, int button, int action, int mods )
{
    auto engine = (MPEngine*)glfwGetWindowUserPointer( window );

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent( button, action );
}