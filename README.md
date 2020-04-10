# SimpleErosion
Simple C++ implementatoin of particle-based hydraulic erosion on a square grid

![Banner Image](/screenshots/banner.png)

[Link to Blog!](https://weigert.vsos.ethz.ch)

## Compiling
Use the makefile to compile the binary:

    make all
    
### Dependencies
- gcc
- SDL2 (core, image, ttf)
- OpenGL3
- GLEW
- Boost (System, Filesystem)
- libnoise
- ImGUI (included as binaries in the project, no install required)

## Usage
Launch the simulation using the binary:

    ./erosion
    
### Controls
Mouse and Keyboard Inputs:
    
    - Toggle Control Panel: ESC
    - Toggle Pause: ESC **NOTE: PAUSED BY DEFAULT**
    - Change Camera Vertical Angle: Up / Down
    - Change Camera Zoom and Rotation: Scroll
    - Move Camera Vertically: Space / C
 
### Panel
There are three tabs: Map, Erosion and Renderer
 
    Map:
      - Reinitialize the Heightmap using a random seed (i.e. perlin noise)
      - Change the map-scaling and remesh! This affects the PHYSICAL height of the map and therefore erosion characteristics!
      
    Erosion (Note: Default settings produced the graphics in the blog post and in this repo):
      - Erosion Cycles: Number of cycles remaining to do
      - Stepsize: Number of cycles to do in a batch before updating the image
      - Run / Stop Erosion: Run or pause the cycles! (Note: After clicking run, unpause simulation)
      - Parameters: Erosion physical parameters.
        - Timestep: Integration timestep. Higher = More erosion, but the relative parameter scales stay the same
        - Minimum Volume: Volume below which a droplet is killed
        - Density: Density of droplet, to give their inertia
        - Deposition Rate: Rate of approach to equilibrium sediment capacity
        - Evaporation Rate: Volume loss factor every timestep
        - Friction: Speed loss factor every timestep
        
    Renderer:
      - Steepness: Angle at which you transition from flat to steep color
      - Flat / Steep Color: Shader colors
      - Light Strength: ...
      - Auto-Rotate / Rate: If you want the map to turn (made this for cinematics)
      
## Screenshots
 ![Erosion Process](/screenshots/erode_reduced.gif)
 Example erosion process.

 ![Results Gif](/screenshots/results.gif)
 Sample of 10 results, only the results shown here.
 
 ![Before / After Comparison Gif](/screenshots/before_after.gif)
 A before after comparison of 10 OTHER samples.
 
## Source
If you are interested in the erosion simulation, read the `world.h` and `world.cpp` files. This is where all the erosion code takes place. To see how this integrates into the actual project, read `erosion.cpp`. Everything else is just to make the results renderable in OpenGL3.
 
The renderer / engine is taken from another project of mine, but I stripped it down to the bare necessities for this project. If you are interested in rendering your own heightmap using OpenGL3, you should study the `view.h` and `view.cpp` files. These are the engine master files.

Otherwise some of the details are explained on my blog!

## License
Mozilla Public License.
