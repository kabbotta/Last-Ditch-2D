#include "ProductionUISystem.h"

#include "../../constants/UIConstants.h"

namespace ld
{

ProductionUISystem::ProductionUISystem(Input& _input, SDL_Interface& _sdl_interface)
  : input(_input),
    sdl_interface(_sdl_interface)
{
  setup();
}


void ProductionUISystem::update()
{


}


void ProductionUISystem::render()
{
  sdl_interface.render_element(base_window);
}


void ProductionUISystem::setup()
{
  base_window.type = "window1";
  base_window.source_texture = "ui1";
  base_window.batch_texture = "ui1-batch";

  base_window.dest_rect.x = (SCREEN_SIZE_X - BASE_WINDOW_SIZE_X) / 2;
  base_window.dest_rect.y = (SCREEN_SIZE_X - BASE_WINDOW_SIZE_X) / 2;
  base_window.dest_rect.w = BASE_WINDOW_SIZE_X;
  base_window.dest_rect.h = BASE_WINDOW_SIZE_Y;
}

}
