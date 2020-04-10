#pragma once
#include "../forward/world.fwd.h"
//Dependency Forward Declaration
#include "../forward/view.fwd.h"
#include "../forward/shader.fwd.h"
#include "../forward/billboard.fwd.h"

class World{
public:
  //Constructor
  void generate();                        //Initialize Heightmap
  void erode(int cycles);                 //Perform n erosion cycles
  glm::vec3 surfaceNormal(int i, int j);

  int SEED = 10;
  std::chrono::milliseconds tickLength = std::chrono::milliseconds(1000);
  glm::vec2 dim = glm::vec2(256, 256);  //Size of the heightmap array
  bool updated = false;                 //Flag for remeshing

  double scale = 60.0;                  //"Physical" Height scaling of the map
  double heightmap[256][256] = {0.0};

  //Erosion Steps
  bool active = false;
  int remaining = 200000;
  int erosionstep = 1000;

  //Particle Properties
  float dt = 1.2;
  float density = 1.0;  //This gives varying amounts of inertia and stuff...
  float evapRate = 0.001;
  float depositionRate = 0.1;
  float minVol = 0.01;
  float friction = 0.05;
};

struct Particle{
  //Construct Particle at Position
  Particle(glm::vec2 _pos){ pos = _pos; }

  glm::vec2 pos;
  glm::vec2 speed = glm::vec2(0.0);

  float volume = 1.0;   //This will vary in time
  float sediment = 0.0; //Fraction of Volume that is Sediment!
};
