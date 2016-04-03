#include "Utils.h"

#include <algorithm>
#include <eigen3/Eigen/Geometry>

#include "components/ui/Element.h"

using namespace Eigen;
using namespace ld;

float Utils::clamp(float value, float lower_bound, float upper_bound)
{
  return std::max(lower_bound, std::min(value, upper_bound));
}


bool Utils::ends_with(const std::string& string, const std::string& suffix)
{
  if (string.length() >= suffix.length())
  {
    auto found(string.compare(string.length() - suffix.length(), suffix.length(), suffix));

    if (found) return true;
  }

  return false;
}


bool Utils::point_intersects_element(Vector2i point, Element& element)
{
  auto hit(
    point.x() >= element.dest_rect.x &&
    point.x() < element.dest_rect.x + element.dest_rect.w &&
    point.y() >= element.dest_rect.y &&
    point.y() < element.dest_rect.y + element.dest_rect.h);

  return hit;
}
