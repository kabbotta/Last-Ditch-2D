#ifndef ENTITY_H
#define ENTITY_H

#include <Box2D/Box2D.h>
#include <eigen3/Eigen/Geometry>
#include <string>

#include "../components/Properties.h"

namespace ld
{

struct Entity
{
  Entity()
    : type(std::string()),
      texture_name(std::string()),
      properties(),
      pos(0.f, 0.f),
      rotation(0.f),
      solid(false),
      body(nullptr)
  {}

  std::string type;
  std::string texture_name;
  Properties properties;
  Eigen::Vector2f pos;
  float rotation;
  bool solid;
  bool dirty;

  b2Body* body;
};

}

#endif /* ENTITY_H */
