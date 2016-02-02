#include "PhysicsSystem.h"

#include <eigen3/Eigen/Geometry>
#include <iostream>

#include "../constants/MapConstants.h"

using namespace ld;
using namespace Eigen;
using namespace std;

PhysicsSystem::PhysicsSystem(
  SDL_Renderer* renderer, MapSystem& map_system_, EntitySystem& entity_system_
)
  : map_system(map_system_),
    entity_system(entity_system_),
    world(new b2World({0, 0})),
    dynamic_bodies(),
    tile_bodies()
{
  setup_entity_bodies();
  setup_tile_bodies();

  cout << "Physics system ready" << endl;
}


PhysicsSystem::~PhysicsSystem()
{
  delete world;
}


void PhysicsSystem::update(double dt)
{
  auto& dynamic_entities = entity_system.get_dynamic_entities();

  for (auto& entity : dynamic_entities)
  {
    b2Vec2 impulse(dt * entity.vel.x(), dt * entity.vel.y());
    entity.body->ApplyLinearImpulse(impulse, entity.body->GetWorldCenter(), true);
  }

  world->Step(B2D_TIMESTEP, B2D_VELOCITY_ITERATIONS, B2D_POSITION_ITERATIONS);
  world->ClearForces();

  for (auto& entity : dynamic_entities)
    entity.pos = {entity.body->GetPosition().x, entity.body->GetPosition().y};
}


void PhysicsSystem::set_debug_draw(DebugDraw& debug_draw)
{
  world->SetDebugDraw(&debug_draw);
}


void PhysicsSystem::render_debug()
{
  world->DrawDebugData();
}


void PhysicsSystem::setup_entity_bodies()
{
  for (auto& entity : entity_system.get_dynamic_entities())
  {
    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.position.Set(entity.pos.x(), entity.pos.y());
    body_def.linearDamping = 8;
    body_def.allowSleep = true;
    body_def.fixedRotation = true;
    body_def.active = true;

    auto body = world->CreateBody(&body_def);

    b2CircleShape circle_shape;
    circle_shape.m_radius = entity.size;

    b2FixtureDef fixture_def;
    fixture_def.shape = &circle_shape;
    fixture_def.friction = 0;

    body->CreateFixture(&fixture_def);

    entity.body = body;
    dynamic_bodies.push_back(body);
  }
}


void PhysicsSystem::setup_tile_bodies()
{
  for (auto x = 0; x < MAP_SIZE_X; ++x)
  {
    for (auto y = 0; y < MAP_SIZE_Y; ++y)
    {
      if (map_system.get_tile(x, y, 0).type != "")
      {
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position.Set(x, y);
	body_def.allowSleep = true;
	body_def.fixedRotation = true;
	body_def.active = true;

	auto body = world->CreateBody(&body_def);

	b2PolygonShape polygon_shape;
	polygon_shape.SetAsBox(.5, .5);

	b2FixtureDef fixture_def;
	fixture_def.shape = &polygon_shape;
	fixture_def.friction = 0;

	body->CreateFixture(&fixture_def);

	tile_bodies.push_back(body);
      }
    }
  }
}
