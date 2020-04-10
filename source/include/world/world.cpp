 //Include Header File
//Dependencies
#include "../render/shader.h"
#include "../render/billboard.h"
#include "../render/model.h"
#include "../render/view.h"
#include "world.h"

/*
===================================================
          WORLD GENERATING FUNCTIONS
===================================================
*/

void World::generate(){
  std::cout<<"Generating New World"<<std::endl;
  SEED = time(NULL);
  std::cout<<"Seed: "<<SEED<<std::endl;
  //Seed the Random Generator
  srand(SEED);

  std::cout<<"... generating height ..."<<std::endl;

  //Initialize Heightmap
  noise::module::Perlin perlin;
  perlin.SetOctaveCount(15);
  perlin.SetFrequency(1.0);
  perlin.SetPersistence(0.6);

  float min, max = 0.0;
  for(int i = 0; i < dim.x; i++){
    for(int j = 0; j < dim.y; j++){
      heightmap[i][j] = perlin.GetValue(i*(1.0/dim.x), j*(1.0/dim.y), SEED);
      if(heightmap[i][j] > max) max = heightmap[i][j];
      if(heightmap[i][j] < min) min = heightmap[i][j];
    }
  }

  //Normalize
  for(int i = 0; i < dim.x; i++){
    for(int j = 0; j < dim.y; j++){
      heightmap[i][j] = (heightmap[i][j] - min)/(max - min);
    }
  }

  //Construct all Triangles...
  updated = true;
}

/*
===================================================
          HYDRAULIC EROSION FUNCTIONS
===================================================
*/

glm::vec3 World::surfaceNormal(int i, int j){
  /*
    Note: Surface normal is computed in this way, because the square-grid surface is meshed using triangles.
    To avoid spatial artifacts, you need to weight properly with all neighbors.
  */

  glm::vec3 n = glm::vec3(0.15) * glm::normalize(glm::vec3(scale*(heightmap[i][j]-heightmap[i+1][j]), 1.0, 0.0));  //Positive X
  n += glm::vec3(0.15) * glm::normalize(glm::vec3(scale*(heightmap[i-1][j]-heightmap[i][j]), 1.0, 0.0));  //Negative X
  n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale*(heightmap[i][j]-heightmap[i][j+1])));    //Positive Y
  n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale*(heightmap[i][j-1]-heightmap[i][j])));  //Negative Y

  //Diagonals! (This removes the last spatial artifacts)
  n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(heightmap[i][j]-heightmap[i+1][j+1])/sqrt(2), sqrt(2), scale*(heightmap[i][j]-heightmap[i+1][j+1])/sqrt(2)));    //Positive Y
  n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(heightmap[i][j]-heightmap[i+1][j-1])/sqrt(2), sqrt(2), scale*(heightmap[i][j]-heightmap[i+1][j-1])/sqrt(2)));    //Positive Y
  n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(heightmap[i][j]-heightmap[i-1][j+1])/sqrt(2), sqrt(2), scale*(heightmap[i][j]-heightmap[i-1][j+1])/sqrt(2)));    //Positive Y
  n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale*(heightmap[i][j]-heightmap[i-1][j-1])/sqrt(2), sqrt(2), scale*(heightmap[i][j]-heightmap[i-1][j-1])/sqrt(2)));    //Positive Y

  return n;
}

void World::erode(int cycles){

  /*
    Note: Everything is properly scaled by a time step-size "dt"
  */

  //Do a series of iterations! (5 Particles)
  for(int i = 0; i < cycles; i++){

    //Spawn New Particle
    glm::vec2 newpos = glm::vec2(rand()%(int)dim.x, rand()%(int)dim.y);
    Particle drop(newpos);

    //As long as the droplet exists...
    while(drop.volume > minVol){

      glm::ivec2 ipos = drop.pos;                   //Floored Droplet Initial Position
      glm::vec3 n = surfaceNormal(ipos.x, ipos.y);  //Surface Normal at Position

      //Accelerate particle using newtonian mechanics using the surface normal.
      drop.speed += dt*glm::vec2(n.x, n.z)/(drop.volume*density);//F = ma, so a = F/m
      drop.pos   += dt*drop.speed;
      drop.speed *= (1.0-dt*friction);       //Friction Factor

      /*
        Note: For multiplied factors (e.g. friction, evaporation)
        time-scaling is correctly implemented like above.
      */

      //Check if Particle is still in-bounds
      if(!glm::all(glm::greaterThanEqual(drop.pos, glm::vec2(0))) ||
         !glm::all(glm::lessThan(drop.pos, dim))) break;

      //Compute sediment capacity difference
      float maxsediment = drop.volume*glm::length(drop.speed)*(heightmap[ipos.x][ipos.y]-heightmap[(int)drop.pos.x][(int)drop.pos.y]);
      if(maxsediment < 0.0) maxsediment = 0.0;
      float sdiff = maxsediment - drop.sediment;

      //Act on the Heightmap and Droplet!
      drop.sediment += dt*depositionRate*sdiff;
      heightmap[ipos.x][ipos.y] -= dt*drop.volume*depositionRate*sdiff;

      //Evaporate the Droplet (Note: Proportional to Volume! Better: Use shape factor to make proportional to the area instead.)
      drop.volume *= (1.0-dt*evapRate);
    }
  }
}
