#ifndef CLIPDATA_H
#define CLIPDATA_H

#include "../constants/RenderConstants.h"

namespace ld
{

struct ClipData
{
  ClipData()
    : x(0),
      y(0),
      w(PIXELS_PER_UNIT),
      h(PIXELS_PER_UNIT)
  {}

  ClipData(int _x, int _y, int _w, int _h)
    : x(_x),
      y(_y),
      w(_w),
      h(_h)
  {}

  int x, y;
  int w, h;
};

}

#endif /* CLIPDATA_H */