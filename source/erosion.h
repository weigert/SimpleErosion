//Dear IMgui Dependencies
#include "include/render/imgui/imgui.h"
#include "include/render/imgui/imgui_impl_sdl.h"
#include "include/render/imgui/imgui_impl_opengl3.h"

//Drawing Dependencies
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

//Libnoise
#include <noise/noise.h>

//Containers, Math, Other
#include <stdlib.h>
#include <string>
#include <vector>
#include <chrono>

//File / General IO
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

//World Classes
#include "include/world/world.cpp"

//View Classes (i.e the Engine)
#include "include/render/interface.cpp"
#include "include/render/billboard.cpp"
#include "include/render/view.cpp"
#include "include/render/model.cpp"
#include "include/render/shader.cpp"
#include "include/render/event.cpp"
