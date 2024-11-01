#ifndef MP_ENGINE_H
#define MP_ENGINE_H

#include <CSCI441/OpenGLEngine.hpp>
#include <CSCI441/ShaderProgram.hpp>
#include <CSCI441/objects.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ArcBall.hpp"
#include "Tav.h"
#include "Being.h"
#include "FreeCam.h"
#include "horse.h"
#include "Car.h"
#include <vector>
#include <CSCI441/ModelLoader.hpp>
#include <CSCI441/TextureUtils.hpp>

class MPEngine final : public CSCI441::OpenGLEngine
{
  public:
    MPEngine( );
    ~MPEngine( ) final;

    void run( ) final;

    /// \desc handle any key events inside the engine
    /// \param key key as represented by GLFW_KEY_ macros
    /// \param action key event action as represented by GLFW_ macros
    void handleKeyEvent( GLint key, GLint action );

    /// \desc handle any mouse button events inside the engine
    /// \param button mouse button as represented by GLFW_MOUSE_BUTTON_ macros
    /// \param action mouse event as represented by GLFW_ macros
    void handleMouseButtonEvent( GLint button, GLint action );

    /// \desc handle any cursor movement events inside the engine
    /// \param currMousePosition the current cursor position
    void handleCursorPositionEvent( glm::vec2 currMousePosition );

    /// \desc value off-screen to represent mouse has not begun interacting with window yet
    static constexpr GLfloat MOUSE_UNINITIALIZED = -9999.0f;

  private:
    FreeCam* _pFreeCam;
    // parameters to make up our grid size and spacing, feel free to
    // play around with this
    const GLfloat GRID_WIDTH          = WORLD_SIZE * 1.8f;
    const GLfloat GRID_LENGTH         = WORLD_SIZE * 1.8f;
    const GLfloat GRID_SPACING_WIDTH  = 1.0f;
    const GLfloat GRID_SPACING_LENGTH = 1.0f;
    // precomputed parameters based on above
    const GLfloat LEFT_END_POINT   = -GRID_WIDTH / 2.0f - 5.0f;
    const GLfloat RIGHT_END_POINT  = GRID_WIDTH / 2.0f + 5.0f;
    const GLfloat BOTTOM_END_POINT = -GRID_LENGTH / 2.0f - 5.0f;
    const GLfloat TOP_END_POINT    = GRID_LENGTH / 2.0f + 5.0f;

    void mSetupGLFW( ) final;
    void mSetupOpenGL( ) final;
    void mSetupShaders( ) final;
    void mSetupBuffers( ) final;
    void mSetupScene( ) final;

    void mCleanupBuffers( ) final;
    void mCleanupShaders( ) final;

    /// \desc draws everything to the scene from a particular point of view
    /// \param viewMtx the current view matrix for our camera
    /// \param projMtx the current projection matrix for our camera
    void _renderScene( glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc handles moving our FreeCam as determined by keyboard input
    void _updateScene( );

    /// \desc tracks the number of different keys that can be present as determined by GLFW
    static constexpr GLuint NUM_KEYS = GLFW_KEY_LAST;
    /// \desc boolean array tracking each key state.  if true, then the key is in a pressed or held
    /// down state.  if false, then the key is in a released state and not being interacted with
    GLboolean _keys[NUM_KEYS];

    /// \desc last location of the mouse in window coordinates
    glm::vec2 _mousePosition;
    /// \desc current state of the left mouse button
    GLint _leftMouseButtonState;

    /// \desc the static fixed camera in our world
    ArcBall* _pArcballCam;
    CSCI441::PerspectiveCamera* _pActiveCamera;

    /// \desc pair of values to store the speed the camera can move/rotate.
    /// \brief x = forward/backward delta, y = rotational delta
    glm::vec2 _cameraSpeed;

    Tav* _pTav;
    Being* _pBeing;
    horse* _pHorse;
    Car* _pCar;
    int _currentCharacter = 0;

    GLint _skyTex;
    /// \desc the size of the world (controls the ground size and locations of buildings)
    static constexpr GLfloat WORLD_SIZE = 55.0f;
    /// \desc VAO for our ground
    GLuint _groundVAO;
    /// \desc the number of points that make up our ground object
    GLsizei _numGroundPoints;

    glm::vec3 _cameraOffset;

    /// \desc creates the ground VAO
    void _createGroundBuffers( );

    /// \desc smart container to store information specific to each building we wish to draw
    struct BuildingData
    {
        /// \desc transformations to position and size the building
        glm::mat4 modelMatrix;
        /// \desc color to draw the building
        glm::vec3 color;
    };

    struct TreeData
    {
        glm::mat4 modelMatrix;
        bool isTwoTrees;
    };
    struct PlantData {
        /// \desc transformations to position and size the building
        glm::mat4 modelMatrix;
        /// \desc color to draw the building
        glm::vec3 color = glm::vec3(0.1063, 0.58, 0);
    };
    struct BunnyData {
        /// \desc transformations to position and size the building
        glm::mat4 modelMatrix;
        /// \desc color to draw the building
        glm::vec3 color = glm::vec3(1, 1, 1);
    };
    std::vector<TreeData> _trees;
    std::vector<PlantData> _plants;
    std::vector<BunnyData> _bunnies;
    void _drawSingleTree( const TreeData& treeData, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawTwoTrees( const TreeData& treeData, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc information list of all the buildings to draw
    std::vector<BuildingData> _buildings;

    CSCI441::ModelLoader* _pObjModel;
    CSCI441::ModelLoader* _pObjModelB;
    CSCI441::ModelLoader* _pObjModelC;

    /// \desc generates building information to make up our scene
    void _generateEnvironment( );

    /// \desc shader program that performs lighting
    CSCI441::ShaderProgram* _lightingShaderProgram = nullptr; // the wrapper for our shader program

    /// \desc stores the locations of all of our shader uniforms
    struct LightingShaderUniformLocations
    {
        /// \desc precomputed MVP matrix location
        GLint mvpMatrix;
        /// \desc material diffuse color location
        GLint materialColor;
        // TODO #1: add new uniforms
        GLint direction;
        GLint nMatrix;
        GLuint lightColor;

        GLint cameraDirection;

    } _lightingShaderUniformLocations;

    /// \desc stores the locations of all of our shader attributes
    struct LightingShaderAttributeLocations
    {
        /// \desc vertex position location
        GLint vPos;
        // TODO #2: add new attributes
        GLint vNormal;

    } _lightingShaderAttributeLocations;

    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    GLuint _loadAndRegisterTexture(const char *FILENAME);

    void mSetupTextures();

    void mCleanupTextures();
};

void a3_engine_keyboard_callback( GLFWwindow* window, int key, int scancode, int action, int mods );
void a3_engine_cursor_callback( GLFWwindow* window, double x, double y );
void a3_engine_mouse_button_callback( GLFWwindow* window, int button, int action, int mods );

#endif // MP_ENGINE_H
