#include "nevil/arena/wall.hpp"

nevil::wall::wall()
{
}

nevil::wall::wall(int x, int y, double size_x, double size_y, double height , double mass, const Enki::Color &color)
{
  pos = Enki::Point(x, y);
  _off_color = color;
  _on_color = color;
  setRectangular(size_x, size_y, height, mass);
  setColor(color);
}

nevil::wall::~wall() {}

void nevil::wall::turn_on() {}
void nevil::wall::turn_off() {}