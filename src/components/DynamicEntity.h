#ifndef DYNAMICENTITY_H
#define DYNAMICENTITY_H

#include <Box2D/Box2D.h>
#include <eigen3/Eigen/Geometry>
#include <SDL2/SDL.h>
#include <string>

namespace ld
{

struct DynamicEntity
{
  DynamicEntity()
    : name(std::string()),
      type(std::string()),
      texture_name(std::string()),
      pos(0, 0),
      vel(0, 0),
      speed(100),
      floor(0),
      radius(.5),
      clip_rect(),
      body(nullptr)
  {}

  std::string name, type;
  std::string texture_name;
  Eigen::Vector2f pos, vel;
  double speed;
  int floor;
  double radius;
  SDL_Rect clip_rect;
  b2Body* body;
};

}

#endif /* DYNAMICENTITY_H */
