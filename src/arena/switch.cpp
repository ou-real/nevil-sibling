#include "nevil/arena/switch.hpp"

nevil::switch_object::switch_object() {}

nevil::switch_object::switch_object(int x, int y, double size_x, double size_y, double height, const Enki::Color &off_color, const Enki::Color &on_color)
{
  pos = Enki::Point(x, y);
  _off_color = off_color;
  _on_color = on_color;
  setRectangular(size_x, size_y, height, -1);
  turn_off();
}

nevil::switch_object::~switch_object() {}

void nevil::switch_object::turn_on()
{
  setColor(_on_color);
  _is_on = true;
}
void nevil::switch_object::turn_off()
{
  setColor(_off_color);
  _is_on = false;
}