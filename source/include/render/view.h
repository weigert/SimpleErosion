#pragma once
#include "../forward/view.fwd.h"
//Dependencies
#include "../forward/billboard.fwd.h"
#include "../forward/world.fwd.h"
#include "../forward/shader.fwd.h"
#include "../forward/model.fwd.h"
#include "../forward/interface.fwd.h"

class View{
  public:
    //Initialization
    bool Init();
    void cleanup();
    SDL_Window* gWindow;
    SDL_GLContext gContext;
    ImGuiIO io;
    unsigned int SCREEN_WIDTH = 1200, SCREEN_HEIGHT = 800;
    const unsigned int SHADOW_WIDTH = 1200, SHADOW_HEIGHT = 1200;
    bool fullscreen = false;
    bool vsync = false;

    //GUI Handler
    Interface* interface;
    bool showmenu = false;

    //Viewposition
    glm::vec3 viewPos = glm::vec3(50, 1, 50);
    bool autorotate = false;
    float rate = 0.1;
    float zoom = 0.2;
    float zoomInc = 0.001;

    //Loaded Chunk Models
    void mesh(World &world);  //When chunks are displayed
    void remesh(World &world);  //When chunks are updated
    Model model;

    //Rendering Targets
    Billboard shadow; //Shadowmap to render to
    Billboard image;  //Image for Effect Rendering

    //Shaders
    void setupShaders();
    Shader defaultShader;
    Shader depthShader;
    Shader effectShader;

    //Render the entire drawing pipeline
    void update();
    void render(World &world);
    void renderGUI(World &world);

    //Rotation and View
    float rotation = 0.0f;
    glm::vec3 cameraPos = glm::vec3(50, 50, 50);
    glm::vec3 lookPos = glm::vec3(0, 0, 0);
    glm::mat4 camera = glm::lookAt(cameraPos, lookPos, glm::vec3(0,1,0));
    glm::mat4 projection = glm::ortho(-(float)SCREEN_WIDTH*zoom, (float)SCREEN_WIDTH*zoom, -(float)SCREEN_HEIGHT*zoom, (float)SCREEN_HEIGHT*zoom, -800.0f, 500.0f);
    //glm::mat4 projection = glm::perspective(1.0f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.0f, 100.0f);

    //Shader Stuff
    float steepness = 0.8;
    glm::vec3 flatColor = glm::vec3(0.27, 0.64, 0.27);
    glm::vec3 steepColor = glm::vec3(0.7);

    //Lighting and Shading
    glm::vec3 skyCol = glm::vec3(0.3, 0.3f, 1.0f);
    glm::vec3 lightPos = glm::vec3(-100.0f, 100.0f, -150.0f);
    glm::vec3 lightCol = glm::vec3(1.0f, 1.0f, 0.9f);
    float lightStrength = 1.4;
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    glm::mat4 depthProjection = glm::ortho<float>(-300, 300, -300, 300, 0, 800);
    glm::mat4 depthCamera = glm::lookAt(lightPos, glm::vec3(0), glm::vec3(0,1,0));

    glm::mat4 biasMatrix = glm::mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    //Frametime Calculation
    template<typename D>
    void calcFrameTime();
    int frameTime = 0;
    std::chrono::high_resolution_clock::time_point _old;
};
