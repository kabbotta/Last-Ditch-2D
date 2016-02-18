#include "EntitySystem.h"

#include <cassert>
#include <eigen3/Eigen/Geometry>
#include <iostream>
#include <random>

#include "../components/Door.h"
#include "../components/Item.h"
#include "../constants/ItemConstants.h"
#include "../constants/MapConstants.h"
#include "../constants/RenderConstants.h"

using namespace ld;
using namespace Eigen;
using namespace std;

EntitySystem::EntitySystem(
  mt19937& rng_, Input& input_, CameraSystem& camera_system_, MapSystem& map_system_
)
  : rng(rng_),
    input(input_),
    map_system(map_system_),
    camera_system(camera_system_),
    active_user(nullptr),
    users(NUM_FLOORS),
    frame_time(0.0),
    item_types()
{
  setup_users();
  setup_items();

  cout << "Entity system ready" << endl;
}


void EntitySystem::setup_users()
{
  User kadijah;
  kadijah.name = "Kadijah";
  kadijah.type = "kadijah";
  kadijah.texture_name = TYPE_TO_TEXTURE[kadijah.type];
  kadijah.animation = "kadijah-walk-side";
  kadijah.pos = {3, 9};
  kadijah.floor = 0;
  kadijah.radius = .48;
  kadijah.speed = 240;

  users[kadijah.floor].push_back(kadijah);
  active_user = &users[kadijah.floor].back();
}


std::string EntitySystem::get_random_type()
{
  uniform_int_distribution<> type_dist(0, ITEM_TYPES.size() - 1);

  return ITEM_TYPES[type_dist(rng)];
}


void EntitySystem::setup_items()
{
  for (auto floor(0); floor < NUM_FLOORS; ++floor)
  {
    for (auto i(0); i < NUM_ITEMS; ++i)
    {
      uniform_real_distribution<> x_dist(0, MAP_SIZE_X - 1);
      uniform_real_distribution<> y_dist(0, MAP_SIZE_Y - 1);

      Item item;
      item.type = get_random_type();
      item.texture_name = TYPE_TO_TEXTURE[item.type];
      item.floor = floor;

      while (1)
      {
	float x(x_dist(rng));
	float y(y_dist(rng));
	float size(2.f * item.radius);

	auto clear(
	  !map_system.get_main_tile(x,        y,        floor).solid &&
	  !map_system.get_main_tile(x + size, y,        floor).solid &&
	  !map_system.get_main_tile(x,        y + size, floor).solid &&
	  !map_system.get_main_tile(x + size, y + size, floor).solid);

	if (clear)
	{
	  item.pos = {x, y};
	  map_system.get_chunk(x, y, floor).items.push_back(item);

	  break;
	}
      }
    }
  }
}


void EntitySystem::update(double dt)
{
  apply_user_inputs();

  frame_time += dt;

  if (frame_time > .03)
  {
    frame_time = 0.0;

    for (auto& user : users[active_user->floor])
    {
      auto num_frames(ANIMATION_COORDS[user.animation][0]);

      if (user.frame >= num_frames - 1)
	user.frame = 0;
      else
	++user.frame;

      auto x(ANIMATION_COORDS[user.animation][1] + user.frame);
      auto y(ANIMATION_COORDS[user.animation][2]);

      user.clip_rect.x = PIXELS_PER_UNIT * x;
      user.clip_rect.y = PIXELS_PER_UNIT * y;
    }
  }

  if (input.activate) handle_activation();
}


void EntitySystem::apply_user_inputs()
{
  Vector2f direction(0, 0);

  if (input.left) direction.x() -= 1;
  if (input.right) direction.x() += 1;
  if (input.up) direction.y() -= 1;
  if (input.down) direction.y() += 1;

  if (direction.squaredNorm() == 0)
  {
    active_user->vel = Vector2f::Zero();

    auto animation(active_user->animation);
    string new_animation("");

    if (animation == "kadijah-walk-forward") new_animation = "kadijah-idle-forward";
    else if (animation == "kadijah-walk-back") new_animation = "kadijah-idle-back";
    else if (animation == "kadijah-walk-side") new_animation = "kadijah-idle-side";
    else new_animation = "kadijah-idle-forward";

    active_user->animation = new_animation;
  }
  else
  {
    direction.normalize();
    active_user->vel = active_user->speed * direction;

    if (direction.x() > 0)
      active_user->animation = "kadijah-walk-side";
    else if (direction.x() < 0)
      active_user->animation = "kadijah-walk-side";
    else if (direction.y() > 0)
      active_user->animation = "kadijah-walk-forward";
    else if (direction.y() < 0)
      active_user->animation = "kadijah-walk-back";
  }
}


void EntitySystem::handle_activation()
{
  input.activate = false;

  Vector2f selection_point(camera_system.to_world_coordinates(input.mouse_pos));

  auto out_of_bounds(
    selection_point.x() < 0 || selection_point.x() >= MAP_SIZE_X ||
    selection_point.y() < 0 || selection_point.y() >= MAP_SIZE_Y);

  if (out_of_bounds) return;

  auto& chunk(
    map_system.get_chunk(selection_point.x(), selection_point.y(), active_user->floor));

  if (find_door(selection_point, chunk)) return;
}


bool EntitySystem::find_door(Vector2f& selection_point, Chunk& chunk)
{
  for (auto& door : chunk.doors)
  {
    auto hit(
      selection_point.x() > door.pos.x() && selection_point.x() < door.pos.x() + 1 &&
      selection_point.y() > door.pos.y() && selection_point.y() < door.pos.y() + 1);

    if (!hit) continue;

    auto user_center(active_user->pos + Vector2f(.5, .5));
    auto in_range((user_center - selection_point).squaredNorm() < 2.8f);

    if (!in_range) continue;

    map_system.open_door(door);

    return true;
  }

  return false;
}
