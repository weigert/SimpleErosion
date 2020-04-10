#pragma once
#include "../forward/interface.fwd.h"
#include "../forward/world.fwd.h"
#include "../forward/view.fwd.h"

class Interface{
public:
  int curID;
  //Render the Interface Elements
  void render(View &view, World &world);
};
