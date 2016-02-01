#ifndef DYNAMICENTITY_H
#define DYNAMICENTITY_H

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
      pos(0, 0),
      vel(0, 0),
      speed(80.0),
      floor(0),
      size(.48),
      clip_rect()
  {}

  std::string name, type;
  Eigen::Vector2f pos, vel;
  double speed;
  int floor;
  double size;
  SDL_Rect clip_rect;
};

}

#endif /* DYNAMICENTITY_H */
