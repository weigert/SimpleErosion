#include "../world/world.h"
#include "../render/view.h"
#include "interface.h"


void Interface::render(View &view, World &world){

  //Position and Size
  ImGui::SetNextWindowSize(ImVec2(450, 280), ImGuiCond_Once);
  ImGui::SetNextWindowPos(ImVec2(50, 470), ImGuiCond_Once);

  ImGui::Begin("Erosion Controller");
  ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
  if(ImGui::BeginTabBar("MyTabBar", tab_bar_flags)){

    //Map Menu
    if(ImGui::BeginTabItem("Map")){
      //Reset the Heightmap!
      ImGui::Text("SEED: "); ImGui::SameLine();
      ImGui::Text("%i", world.SEED);
      if(ImGui::Button("Random Initialize")){
        world.generate();
      }

      //World-Map Scale
      static int scale = world.scale;
      ImGui::PushItemWidth(200);
      ImGui::DragInt("Height Scale", &scale, 1);
      if(ImGui::Button("Remesh")){
        world.scale = scale;
        world.updated = true;
      }

      ImGui::EndTabItem();
    }

    //Erosion Menu
    if(ImGui::BeginTabItem("Erosion")){


      //Erosion Step
      ImGui::PushItemWidth(200);
      ImGui::DragInt("Erosion Cycles", &world.remaining, 100);
      ImGui::DragInt("Stepsize", &world.erosionstep, 100);

      //Start / Stop Button
      if(ImGui::Button("Run Erosion")){
        world.active = true;
      } ImGui::SameLine();
      if(ImGui::Button("Stop Erosion")){
        world.active = false;
      }

      ImGui::Text("Parameters:");
      ImGui::DragFloat("Timestep", &world.dt, 0.01f, 0.01f, 5.0f);
      ImGui::DragFloat("Minimum Volume", &world.minVol, 0.0001f, 0.0001f, 0.1f);
      ImGui::DragFloat("Density", &world.density, 0.1f, 0.1f, 5.0f);
      ImGui::DragFloat("Deposition Rate", &world.depositionRate, 0.01f, 0.01f, 1.0f);
      ImGui::DragFloat("Evaporation Rate", &world.evapRate, 0.001f, 0.001f, 0.5f);
      ImGui::DragFloat("Friction", &world.friction, 0.001f, 0.001f, 0.5f);

      ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("Renderer")){
      ImGui::DragFloat("Steepness", &view.steepness, 0.05f, 0.0f, 1.0f);

      static float flat[3] = {view.flatColor.x, view.flatColor.y, view.flatColor.z};
      ImGui::ColorEdit3("Flat Color", flat);
      view.flatColor = glm::vec3(flat[0], flat[1], flat[2]);

      static float steep[3] = {view.steepColor.x, view.steepColor.y, view.steepColor.z};
      ImGui::ColorEdit3("Steep Color", steep);
      view.steepColor = glm::vec3(steep[0], steep[1], steep[2]);

      ImGui::DragFloat("Light Strength", &view.lightStrength, 0.01f, 0.0f, 2.0f);

      ImGui::Checkbox("Auto-Rotate", &view.autorotate);
      ImGui::DragFloat("Rate", &view.rate, 0.1f, 0.0f, 1.5f);

      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }




  ImGui::End();
}
