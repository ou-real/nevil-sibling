#ifndef _NEVIL_ARENA_LIGHT_HPP_
#define _NEVIL_ARENA_LIGHT_HPP_

#include "nevil/arena/object.hpp"

namespace nevil
{
  class light : public object
  {
  public:
    light();
    light(int x, int y, double size_x, double size_y, double height,
     double mass, double dry_friction_coefficient,
     const Enki::Color &off_color=Enki::Color(1.0, 1.0, 0.0),
     const Enki::Color &on_color=Enki::Color(0.5, 0.5, 0.0));
    virtual ~light();
    void turn_on();
    void turn_off();
  };
}

#endif // _NEVIL_ARENA_LIGHT_HPP_