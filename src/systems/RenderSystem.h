#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <eigen3/Eigen/Geometry>
#include <map>
#include <SDL2/SDL.h>

#include "../DebugDraw.h"
#include "../SDL_Interface.h"
#include "MapSystem.h"
#include "EntitySystem.h"
#include "CameraSystem.h"
#include "InterfaceSystem.h"
#include "PhysicsSystem.h"

namespace ld
{

class RenderSystem
{
  void setup_textures();
  SDL_Texture* load_texture(std::string name);

  void render_chunks();
  void render_tiles(Chunk& chunk);
  void render_items();
  void render_entities();

  SDL_Interface& sdl_interface;

  DebugDraw debug_draw;

  MapSystem& map_system;
  EntitySystem& entity_system;
  CameraSystem& camera_system;
  InterfaceSystem& interface_system;
  PhysicsSystem& physics_system;

  std::map<std::string, SDL_Texture*> textures;
  std::map<std::string, Eigen::Vector2i> tileset1_coords;
  std::map<std::string, SDL_Rect> items1_coords;

  int current_floor;

public:
  RenderSystem(
    SDL_Interface& sdl_interface,
    MapSystem& map_system,
    EntitySystem& entity_system,
    CameraSystem& camera_system,
    InterfaceSystem& interface_system,
    PhysicsSystem& physics_system);

  void update();
  void shutdown();
};

}

#endif /* RENDERSYSTEM_H */
