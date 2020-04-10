//Load Dependencies
#include "../world/world.h"
#include "shader.h"
#include "model.h"
#include "billboard.h"
#include "interface.h"
//Load our Own Type!
#include "view.h"

/*
================================================================================
                              Setup / Cleanup
================================================================================
*/

bool View::Init(){
  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    return false;
  }
  //Initialize SDL_TTF
	TTF_Init();

  //Initialize the Window and Context
  gWindow = SDL_CreateWindow( "Hydraulic Erosion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  if( gWindow == NULL ){
    printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    return false;
  }
  SDL_SetWindowResizable(gWindow, SDL_TRUE);
  gContext = SDL_GL_CreateContext(	gWindow );

  //Initialize OPENGL Stuff
	SDL_GL_SetSwapInterval(vsync);
	glewExperimental = GL_TRUE;
	glewInit();

  //Setup the Guy
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = ImGui::GetIO(); (void)io;

  ImGui_ImplSDL2_InitForOpenGL(gWindow, gContext);
  ImGui_ImplOpenGL3_Init("#version 130");

  ImGui::StyleColorsCustom();

  //Configure Global OpenGL State
  glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
  glEnable(GL_BLEND) ;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
  glLineWidth(2.5f);

  setupShaders();

  image.setup(SCREEN_WIDTH, SCREEN_HEIGHT);
  shadow.setup2(SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}

void View::setupShaders(){
  //Setup Cube Shader
  defaultShader.setup("default.vs", "default.fs");
  defaultShader.addAttribute(0, "in_Position");
  defaultShader.addAttribute(1, "in_Normal");

  //Setup Depthshader
  depthShader.setup("depth.vs", "depth.fs");
  depthShader.addAttribute(0, "in_Position");

  //Setup Effect Shader
  effectShader.setup("effect.vs", "effect.fs");
  effectShader.addAttribute(0, "in_Quad");
  effectShader.addAttribute(1, "in_Tex");
}

void View::cleanup(){
  //Cleanup Models
  model.cleanup();

  //Cleanup Shaders
  defaultShader.cleanup();
  depthShader.cleanup();
  effectShader.cleanup();

  shadow.cleanup();
  image.cleanup();

  //Shutdown IMGUI
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  //Destroy Context and Window
	SDL_GL_DeleteContext( gContext );
	SDL_DestroyWindow( gWindow );

  //Quit SDL subsystems
  TTF_Quit();
  SDL_Quit();
}

/*
================================================================================
                              Model Generation
================================================================================
*/

void View::mesh(World &world){
  model.fromWorld(world);
  model.setup();
  model.reset();

  //Translate the thing inti proper space
  glm::vec3 axis = glm::vec3(-0.5*world.dim.x, -0.5*world.scale, -0.5*world.dim.y);
  model.translate(axis);
  world.updated = false;
}

void View::remesh(World &world){
  if(!world.updated) return;

  //Remesh the while thing!!
  model.fromWorld(world);
  model.update();
  world.updated = false;
}

/*
================================================================================
                                Rendering
================================================================================
*/

void View::render(World &world){

  if(autorotate){ //Auto Rotation
    glm::vec3 axis(0.0f, 1.0f, 0.0f);
    rotation += rate;
    camera = glm::rotate(camera, glm::radians(rate), axis);
  }

  /* SHADOW MAPPING */
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, shadow.fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  depthShader.useProgram();
  depthShader.setMat4("dmvp", depthProjection * depthCamera * model.model);
  model.render();             //Render Scene

  glBindVertexArray(0);

  //Regular Drawing

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, image.fbo);
  glClearColor(skyCol.x, skyCol.y, skyCol.z, 1.0f); //Blue
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //Use the Shader
  defaultShader.useProgram();

  //Texture Stuff
  glActiveTexture(GL_TEXTURE0+0);
  glBindTexture(GL_TEXTURE_2D, shadow.depthTexture);
  defaultShader.setInt("shadowMap", 0);
  defaultShader.setVec3("lightCol", lightCol);
  defaultShader.setVec3("lightPos", lightPos);
  defaultShader.setVec3("lookDir", lookPos-cameraPos);
  defaultShader.setFloat("lightStrength", lightStrength);
  defaultShader.setMat4("projectionCamera", projection * camera);
  defaultShader.setMat4("dbmvp", biasMatrix * depthProjection * depthCamera * glm::mat4(1.0f));
  defaultShader.setMat4("model", model.model);

  //Add uniforms for flat and steep color!
  defaultShader.setVec3("flatColor", flatColor);
  defaultShader.setVec3("steepColor", steepColor);
  defaultShader.setFloat("steepness", steepness);

  model.render();             //Render Scene

  //Render to screen using the effect shader!
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  effectShader.useProgram();
  /*
  blurShader.setFloat("mousex", focus.x);
  blurShader.setFloat("mousey", focus.y);
  blurShader.setFloat("width", SCREEN_WIDTH);
  blurShader.setFloat("height", SCREEN_HEIGHT);
  blurShader.setBool("vert", false);
  blurShader.setBool("_fog", fog);
  blurShader.setInt("_blur", blur);
  blurShader.setVec3("fogColor", fogColor);
  */
  glActiveTexture(GL_TEXTURE0+0);
  glBindTexture(GL_TEXTURE_2D, image.texture);
  effectShader.setInt("imageTexture", 0);
  glActiveTexture(GL_TEXTURE0+1);
  glBindTexture(GL_TEXTURE_2D, image.depthTexture);
  effectShader.setInt("depthTexture", 1);
  glBindVertexArray(image.vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  //Add the GUI
  renderGUI(world);

  //Swap the Window
  SDL_GL_SwapWindow(gWindow);
}

//User Interface
void View::renderGUI(World &world){
  //ImGUI Drawing Context
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(gWindow);
  ImGui::NewFrame();

  //Render this here interface...

  //Draw to ImGui
  if(showmenu){
    interface->render(*this, world);
    //ImGui::ShowDemoWindow();
  }

  //Render IMGUI
  ImGui::Render();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



/*
================================================================================
                                    Helpers
================================================================================
*/

template<typename D>
void View::calcFrameTime(){
  //Current Time!
  auto _new = std::chrono::high_resolution_clock::now();
  frameTime = std::chrono::duration_cast<D>(_new - _old).count();
  _old = std::chrono::high_resolution_clock::now();
}
