#include "PhysicsSystem.h"

#include <eigen3/Eigen/Geometry>
#include <iostream>

#include "../constants/MapConstants.h"

using namespace Eigen;
using namespace std;

namespace ld
{

PhysicsSystem::PhysicsSystem(
  vector<User>& _users, MapSystem& _map_system, EntitySystem& _entity_system
)
  : users(_users),
    map_system(_map_system),
    entity_system(_entity_system),
    world(new b2World({0, 0}))
{
  setup_user_bodies();
  setup_tile_bodies();
  setup_door_bodies();

  printf("PhysicsSystem ready\n");
}


PhysicsSystem::~PhysicsSystem()
{
  delete world;
}


void PhysicsSystem::update(const double& dt)
{
  for (auto& user : users)
  {
    auto velocity(user.base_speed * user.direction);

    b2Vec2 impulse(dt * velocity.x(), dt * velocity.y());
    user.body->ApplyLinearImpulse(impulse, user.body->GetWorldCenter(), true);
  }

  world->Step(B2D_TIMESTEP, B2D_VELOCITY_ITERATIONS, B2D_POSITION_ITERATIONS);
  world->ClearForces();

  for (auto& user : users)
    user.pos = {user.body->GetPosition().x, user.body->GetPosition().y};
}


void PhysicsSystem::set_debug_draw(DebugDraw& debug_draw)
{
  world->SetDebugDraw(&debug_draw);
}


void PhysicsSystem::render_debug()
{
  world->DrawDebugData();
}


void PhysicsSystem::setup_user_bodies()
{
  for (auto& user : users)
  {
    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.position.Set(user.pos.x(), user.pos.y());
    body_def.linearDamping = 8;
    body_def.allowSleep = true;
    body_def.fixedRotation = true;
    body_def.active = true;

    auto body(world->CreateBody(&body_def));

    b2CircleShape circle_shape;
    circle_shape.m_radius = user.size.x();

    b2FixtureDef fixture_def;
    fixture_def.shape = &circle_shape;
    fixture_def.friction = 0;

    body->CreateFixture(&fixture_def);

    user.body = body;
  }
}


void PhysicsSystem::setup_tile_bodies()
{
  for (auto floor(0); floor < NUM_FLOORS; ++floor)
  {
    for (auto x(0); x < MAP_SIZE_X; ++x)
    {
      for (auto y(0); y < MAP_SIZE_Y; ++y)
      {
	auto& tile(map_system.get_main_tile(x, y, floor));

	if (tile.solid)
	{
	  b2BodyDef body_def;
	  body_def.type = b2_staticBody;
	  body_def.position.Set(x, y);
	  body_def.allowSleep = true;
	  body_def.fixedRotation = true;
	  body_def.active = true;

	  auto body(world->CreateBody(&body_def));

	  b2PolygonShape polygon_shape;
	  polygon_shape.SetAsBox(.5, .5);

	  b2FixtureDef fixture_def;
	  fixture_def.shape = &polygon_shape;
	  fixture_def.friction = 0;

	  body->CreateFixture(&fixture_def);

	  tile.body = body;
	}
      }
    }
  }
}


void PhysicsSystem::setup_door_bodies()
{
  for (auto floor(0); floor < NUM_FLOORS; ++floor)
  {
    for (auto cx(0); cx < NUM_CHUNKS_X; ++cx)
    {
      for (auto cy(0); cy < NUM_CHUNKS_Y; ++cy)
      {
	for (auto& door : map_system.get_chunk(cx, cy, floor).doors)
	{
	  if (door.solid)
	  {
	    b2BodyDef body_def;
	    body_def.type = b2_staticBody;
	    body_def.position.Set(door.pos.x(), door.pos.y());
	    body_def.allowSleep = true;
	    body_def.fixedRotation = true;
	    body_def.active = true;

	    auto body(world->CreateBody(&body_def));

	    b2PolygonShape polygon_shape;
	    polygon_shape.SetAsBox(.5, .5);

	    b2FixtureDef fixture_def;
	    fixture_def.shape = &polygon_shape;
	    fixture_def.friction = 0;

	    body->CreateFixture(&fixture_def);

	    door.body = body;
	  }
	}
      }
    }
  }
}


b2Body* PhysicsSystem::create_body(float x, float y, float hw, float hh)
{
  b2BodyDef body_def;
  body_def.type = b2_staticBody;
  body_def.position.Set(x, y);
  body_def.allowSleep = true;
  body_def.fixedRotation = true;
  body_def.active = true;

  auto body(world->CreateBody(&body_def));

  b2PolygonShape polygon_shape;
  polygon_shape.SetAsBox(hw, hh);

  b2FixtureDef fixture_def;
  fixture_def.shape = &polygon_shape;
  fixture_def.friction = 0;

  body->CreateFixture(&fixture_def);

  return body;
}


void PhysicsSystem::destroy_body(b2Body* body)
{
  world->DestroyBody(body);
}

}
