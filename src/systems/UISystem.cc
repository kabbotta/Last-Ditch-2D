#include "UISystem.h"

#include <iomanip>
#include <sstream>

#include "../constants/UIConstants.h"

using namespace ld;
using namespace Eigen;
using namespace std;

UISystem::UISystem(
  SDL_Interface& _sdl_interface,
  Input& _input,
  TimeSystem& _time_system,
  EntitySystem& _entity_system,
  InventorySystem& _inventory_system
)
  : sdl_interface(_sdl_interface),
    input(_input),
    time_system(_time_system),
    entity_system(_entity_system),
    inventory_system(_inventory_system),
    inventory_ui_system(_sdl_interface, _input, _entity_system, _inventory_system),
    production_ui_system(_sdl_interface, _input),
    management_ui_system(_sdl_interface, _input),
    status_ui_system(_sdl_interface, _input),
    base_active(true),
    main_active(false),
    active_user(_entity_system.get_active_user()),
    date_and_time()
{
  setup_base();
  setup_main();
}


void UISystem::update()
{
  if (input.menu) handle_menu_activation();

  if (base_active) update_base();
  if (main_active) update_main();

  if (inventory_ui_system.is_active()) inventory_ui_system.update();
  if (production_ui_system.is_active()) production_ui_system.update();
  if (management_ui_system.is_active()) management_ui_system.update();
  if (status_ui_system.is_active()) status_ui_system.update();
}


void UISystem::render()
{
  if (base_active) render_base();
  if (main_active) render_main();

  if (inventory_ui_system.is_active()) inventory_ui_system.render();
  if (production_ui_system.is_active()) production_ui_system.render();
  if (management_ui_system.is_active()) management_ui_system.render();
  if (status_ui_system.is_active()) status_ui_system.render();
}


void UISystem::render_base()
{
  sdl_interface.render_element(date_and_time);
}


void UISystem::render_main()
{
}


void UISystem::setup_base()
{
  date_and_time.pos = {2, 2};
  date_and_time.size = {130, 20};
  date_and_time.text = time_system.get_string();
  date_and_time.text_texture = "date_and_time_text";
}


void UISystem::setup_main()
{
  auto vert_offset(100);
  auto horz_offset(160);

  inventory_button.type = "backdrop1";
  inventory_button.pos =
    {SCREEN_SIZE_X / 2 - MENU_BUTTON_SIZE_X / 2,
     SCREEN_SIZE_Y / 2 - MENU_BUTTON_SIZE_Y / 2 - vert_offset};
  inventory_button.size = {MENU_BUTTON_SIZE_X, MENU_BUTTON_SIZE_Y};
  inventory_button.texture = "ui1";
  inventory_button.text = "Inventory";
  inventory_button.text_texture = "inventory_button_text";

  sdl_interface.create_texture_from_text(
    inventory_button.text, inventory_button.text_texture);

  production_button.type = "backdrop1";
  production_button.texture = "ui1";
  production_button.pos =
    {SCREEN_SIZE_X / 2 - MENU_BUTTON_SIZE_X / 2 + horz_offset,
     SCREEN_SIZE_Y / 2 - MENU_BUTTON_SIZE_Y / 2};
  production_button.size = {MENU_BUTTON_SIZE_X, MENU_BUTTON_SIZE_Y};
  production_button.text = "Production";
  production_button.text_texture = "production_button_text";

  sdl_interface.create_texture_from_text(
    production_button.text, production_button.text_texture);

  management_button.type = "backdrop1";
  management_button.texture = "ui1";
  management_button.pos =
    {SCREEN_SIZE_X / 2 - MENU_BUTTON_SIZE_X / 2,
     SCREEN_SIZE_Y / 2 - MENU_BUTTON_SIZE_Y / 2 + vert_offset};
  management_button.size = {MENU_BUTTON_SIZE_X, MENU_BUTTON_SIZE_Y};
  management_button.text = "Management";
  management_button.text_texture = "management_button_text";

  sdl_interface.create_texture_from_text(
    management_button.text, management_button.text_texture);

  status_button.type = "backdrop1";
  status_button.texture = "ui1";
  status_button.pos =
    {SCREEN_SIZE_X / 2 - MENU_BUTTON_SIZE_X / 2 - horz_offset,
     SCREEN_SIZE_Y / 2 - MENU_BUTTON_SIZE_Y / 2};
  status_button.size = {MENU_BUTTON_SIZE_X, MENU_BUTTON_SIZE_Y};
  status_button.text = "Status";
  status_button.text_texture = "status_button_text";

  sdl_interface.create_texture_from_text(
    status_button.text, status_button.text_texture);
}


void UISystem::update_base()
{
  date_and_time.text = time_system.get_string();

  sdl_interface.create_texture_from_text(date_and_time.text, date_and_time.texture);
}


void UISystem::update_main()
{
  if (input.activate)
  {
    auto element(find_scalable_element_at(input.left_mouse_released_pos));

    if (element)
    {
      input.activate = false;

      if (element->text == "Inventory")
      {
	main_active = false;
	inventory_ui_system.set_active(true);
      }
      else if (element->text == "Production")
      {
	main_active = false;
	production_ui_system.set_active(true);
      }
      else if (element->text == "Management")
      {
	main_active = false;
	management_ui_system.set_active(true);
      }
      else if (element->text == "Status")
      {
	main_active = false;
	status_ui_system.set_active(true);
      }
    }
  }
}


void UISystem::handle_menu_activation()
{
  input.menu = false;

  if (inventory_ui_system.is_active())
    inventory_ui_system.set_active(false);
  else if (production_ui_system.is_active())
    production_ui_system.set_active(false);
  else if (management_ui_system.is_active())
    management_ui_system.set_active(false);
  else if (status_ui_system.is_active())
    status_ui_system.set_active(false);
  else
    main_active = !main_active;
}


ScalableElement* UISystem::find_scalable_element_at(Vector2i& screen_pos)
{
  for (auto& element : main_scalable_elements)
  {
    auto hit(
      screen_pos.x() > element.pos.x() &&
      screen_pos.x() < element.pos.x() + element.size.x() &&
      screen_pos.y() > element.pos.y() &&
      screen_pos.y() < element.pos.y() + element.size.y());

    if (hit) return &element;
  }

  return nullptr;
}
