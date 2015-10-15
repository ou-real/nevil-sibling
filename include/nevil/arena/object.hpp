#ifndef _NEVIL_ARENA_OBJECT_HPP_
#define _NEVIL_ARENA_OBJECT_HPP_

#include <enki/PhysicalEngine.h>

namespace nevil
{
  class object : public Enki::PhysicalObject
  {
  public:
    virtual void turn_on() = 0;
    virtual void turn_off() = 0;
    bool is_on() const { return _is_on; };

    protected:
      Enki::Color _on_color;
      Enki::Color _off_color;
      bool _is_on;
  };
}

#endif // _NEVIL_ARENA_OBJECT_HPP_