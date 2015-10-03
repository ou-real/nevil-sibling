#include "nevil/sibling_arena.hpp"

nevil::sibling_arena::sibling_arena(int world_size_x, int world_size_y, bool sibling_neuron, double speed_A, double speed_B, double angle_A, double angle_B, const Enki::Color &arena_color)
 : nevil::arena(world_size_x, world_size_y, arena_color)
 , _sibling_neuron(sibling_neuron)
{
  // TODO: Setup your arena with objects
  const double OBJECT_SIZE_X = 6;
  const double OBJECT_SIZE_Y = 0.1;
  const double OBJECT_HEIGHT = 7;
  const double MASS = -1;

  //switch A
  _add_object(new nevil::switch_object(world_size_x / 4.0
    , world_size_y
    , OBJECT_SIZE_X
    , OBJECT_SIZE_Y
    , OBJECT_HEIGHT
    , MASS
    , 1));

    //light A
  _add_object(new nevil::light(world_size_x / 4.0
    , 0
    , OBJECT_SIZE_X
    , OBJECT_SIZE_Y
    , OBJECT_HEIGHT
    , MASS
    , 0.01));
  
  //switch B
  _add_object(new nevil::switch_object(world_size_x * (3 / 4.0)
    , world_size_y
    , OBJECT_SIZE_X
    , OBJECT_SIZE_Y
    , OBJECT_HEIGHT
    , MASS
    , 1));
  
  //light B
  _add_object(new nevil::light(world_size_x * (3 / 4.0)
    , 0
    , OBJECT_SIZE_X
    , OBJECT_SIZE_Y
    , OBJECT_HEIGHT
    , MASS
    , 0.01));
  
  //Wall in the middle of the field
  _add_object(new nevil::wall(world_size_x / 2.0
    , world_size_y / 2.0
    , 0.01
    , world_size_y
    , OBJECT_HEIGHT
    , MASS));

  std::cout << "speed_A " << speed_A << std::endl;
  std::cout << "speed_B " << speed_B << std::endl;

  //--robots--
  // Create a robot named A
  _add_robot(new nevil::sibling_robot(world_size_x / 4.0
    , world_size_y / 2.0
    , angle_A
    , _sibling_neuron
    , speed_A
    , "A"
    , Enki::Color(0.0, 0.0, 0.5)));

  // Create a robot named B
  _add_robot(new nevil::sibling_robot(world_size_x * (3 / 4.0)
    , world_size_y / 2.0
    , angle_B
    , _sibling_neuron
    , speed_B
    , "B"
    , Enki::Color(0.0, 0.5, 0.0)));
}

nevil::sibling_arena::~sibling_arena() {}

void nevil::sibling_arena::set_individuals(sibling_individual *sibling_a, sibling_individual *sibling_b)
{
  _robot_vector[0]->set_individual(sibling_a);
  _robot_vector[1]->set_individual(sibling_b);
}

bool nevil::sibling_arena::update()
{
  // Updating the environment
  for (auto r : _robot_vector)
  {
    // Turning the switches and lights on based on the position of the robots
    if(r->is_at_switch())
    {
      // Turn on everything
      for (auto o : _object_vector)
        o->turn_on();
      break; // Don't need to check the other robots
    }
  }

  // Updating the robots
  for (auto r : _robot_vector)
    r->update(_object_vector);
  return true;
}
