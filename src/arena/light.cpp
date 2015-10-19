#include "nevil/arena/light.hpp"

nevil::light::light() {}

nevil::light::light(int x, int y, double size_x, double size_y, double height, const Enki::Color &off_color, const Enki::Color &on_color)
{
  pos = Enki::Point(x, y);
  _off_color = off_color;
  _on_color = on_color;
  setRectangular(size_x, size_y, height, -1);
  turn_off();
}

nevil::light::~light() {}

void nevil::light::turn_on()
{
  setColor(_on_color);
  _is_on = true;
}

void nevil::light::turn_off()
{
  setColor(_off_color);
  _is_on = false;
}