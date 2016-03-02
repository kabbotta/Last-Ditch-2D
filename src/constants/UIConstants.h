#ifndef UICONSTANTS_H
#define UICONSTANTS_H

#include <string>
#include <unordered_map>

#include "../components/ClipData.h"

namespace ld
{

constexpr unsigned MAIN_MENU_BUTTON_SIZE_X(120);
constexpr unsigned MAIN_MENU_BUTTON_SIZE_Y(20);

constexpr unsigned SUB_MENU_BASE_SIZE_X(600);
constexpr unsigned SUB_MENU_BASE_SIZE_Y(400);

static std::unordered_map<std::string, std::string>
UI_TYPE_TO_TEXTURE(
{
  {"backdrop1", "ui1"},
  {"list1", "ui1"},
});


static std::unordered_map<std::string, std::unordered_map<std::string, ClipData>>
UI_CLIP_DATA(
{
  {"backdrop1",
    {
      {"ct", {4, 4, 32, 16}},
      {"tl", {0, 0, 3, 3}},
      {"tt", {4, 0, 32, 3}},
      {"tr", {37, 0, 3, 3}},
      {"rr", {37, 4, 3, 16}},
      {"br", {37, 21, 3, 3}},
      {"bb", {4, 21, 3, 3}},
      {"bl", {0, 21, 3, 3}},
      {"ll", {0, 4, 3, 16}},
    }
  },
});

}

#endif /* UICONSTANTS_H */
