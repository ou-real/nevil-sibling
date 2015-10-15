#ifndef _NEVIL_ARENA_SWITCH_HPP_
#define _NEVIL_ARENA_SWITCH_HPP_

#include "nevil/arena/object.hpp"

namespace nevil
{
  class switch_object : public object
  {
  public:
    switch_object();
    switch_object(int x, int y, double size_x, double size_y, double height,
      double mass, double dry_friction_coefficient,
      const Enki::Color &off_color=Enki::Color(0.4, 1.0, 1.0),
      const Enki::Color &on_color=Enki::Color(0.4, 0.0, 1.0));
    virtual ~switch_object();
    void turn_on();
    void turn_off();
  };
}

#endif // _NEVIL_ARENA_SWITCH_HPP_