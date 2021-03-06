#ifndef LASTDITCH_H
#define LASTDITCH_H

#include <cassert>
#include <random>
#include <SDL2/SDL.h>

#include "src/components/Input.h"
#include "src/components/User.h"
#include "src/SDL_Interface.h"
#include "src/Utils.h"
#include "src/systems/CameraSystem.h"
#include "src/systems/ConfigurationSystem.h"
#include "src/systems/EntitySystem.h"
#include "src/systems/InputSystem.h"
#include "src/systems/InventorySystem.h"
#include "src/systems/MapSystem.h"
#include "src/systems/PhysicsSystem.h"
#include "src/systems/RenderSystem.h"
#include "src/systems/TimeSystem.h"
#include "src/systems/ui/UISystem.h"

namespace ld
{

constexpr long SEED(1);

class LastDitch
{
  std::mt19937 rng;

  Input input;
  SDL_Interface sdl_interface;

  std::vector<User> users;

  ConfigurationSystem configuration_system;
  TimeSystem time_system;
  InputSystem input_system;
  MapSystem map_system;
  CameraSystem camera_system;
  EntitySystem entity_system;
  InventorySystem inventory_system;
  PhysicsSystem physics_system;
  UISystem ui_system;
  RenderSystem render_system;

public:
  LastDitch();
};

}

#endif /* LASTDITCH_H */
