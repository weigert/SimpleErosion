#include "erosion.h"

//Main Function
int main( int argc, char* args[] ) {

	//Load / Generate a World
	World world;
	world.generate();

	//Renderer Object
	View view;
	eventHandler events; //Event Handler

	//Initialize the View
	if(!view.Init()){
		std::cout<<"View could not be initialized."<<std::endl;
		return 0;
	}

	//Generate the Mesh from the World's Heightmap
	view.mesh(world);

	//Event, Input, Render and Audio Handling
	bool quit = false;
	bool paused = true;
	SDL_Event e;

	//Main Game Loop
	while(!quit){

		//Handle User Input
		events.input(&e, quit);
		events.update(world, view, paused);
		ImGui_ImplSDL2_ProcessEvent(&e);

		//Do Erosion Cycles!
    if(world.active && !paused){
    	world.erode(world.erosionstep);
			world.remaining -= world.erosionstep;
			world.updated = true;

			if(world.remaining <= 0){
				world.active = false;
				world.remaining = 0;
			}
		}

		//Render the View
		view.remesh( world );	//Only does something if world has update flag set to true
		view.render( world );	//Render the mesh to screen
		//view.calcFrameTime<std::chrono::microseconds>();	//Optional: Track the frame time
	}

	view.cleanup();

	return 0;
}
