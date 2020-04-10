OBJS = source/erosion.cpp
IMGUI =  source/include/render/imgui/imgui.cpp source/include/render/imgui/imgui_demo.cpp source/include/render/imgui/imgui_draw.cpp source/include/render/imgui/imgui_widgets.cpp source/include/render/imgui/imgui_impl_opengl3.cpp source/include/render/imgui/imgui_impl_sdl.cpp

CC = g++ -std=c++17
COMPILER_FLAGS = -Wfatal-errors -O3
LINKER_FLAGS = -I/usr/local/include -L/usr/local/lib -lX11 -lpthread -lSDL2 -lnoise -lSDL2_image -lSDL2_ttf -lGL -lGLEW -lboost_system -lboost_filesystem
OBJ_NAME = erosion
all: $(OBJS)
			$(CC) $(OBJS) $(IMGUI) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
