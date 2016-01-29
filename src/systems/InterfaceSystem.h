#ifndef INTERFACESYSTEM_H
#define INTERFACESYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>

#include "../SDL_Interface.h"
#include "TimeSystem.h"
#include "EntitySystem.h"

namespace ld
{

class InterfaceSystem
{
  SDL_Interface& sdl_interface;

  TimeSystem& time_system;
  EntitySystem& entity_system;

  std::map<std::string, TTF_Font*> fonts;
  std::map<std::string, SDL_Texture*> textures;

public:
  InterfaceSystem(
    SDL_Interface& sdl_interface, TimeSystem& time_system, EntitySystem& entity_system);

  void update();
  void render();
};

}

#endif /* INTERFACESYSTEM_H */
