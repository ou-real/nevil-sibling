#include "nevil/arena/robot.hpp"

nevil::robot::robot()
  : Enki::EPuck(EPuck::CAPABILITY_CAMERA) {}

nevil::robot::robot(double x, double y, double angle ,const std::string &robot_name, const Enki::Color &color, double max_speed)
  : Enki::EPuck(EPuck::CAPABILITY_CAMERA)
  , _initial_angle(angle * M_PI)
  , _initial_position(Enki::Point(x, y))
  , _max_speed(max_speed)
  , _robot_name(robot_name)
{
  if (max_speed > 12.8)
    std::cout << "Warning the max speed of Epuck is 12.8." << std::endl;

  setColor(color);
  reset_position();
}

nevil::robot::~robot() {}

void nevil::robot::reset_position()
{
  leftSpeed = 0;
  rightSpeed = 0;
  pos = _initial_position;
  angle = _initial_angle;
}

/*
Color sets for switch and light
0.5 | 0.5 | 0.0 light turned off
1.0 | 1.0 | 0.0 light turned On

0.4 | 0.0 | 1.0 switch turned off
0.4 | 0.5 | 1.0 switch turned On  
*/
const std::vector<double> nevil::robot::_get_sensor_inputs()
{
  //reset the counters
  std::vector<double> sensor_counter(SENSOR_NUM, 0);

  // Each color has 6 groups of pixels
  for (size_t i = 0; i < 6; ++i)
  {
    // One group is 10 pixels
    for (size_t j = 0; j < 10; ++j)
    {
      double red_value = camera.image[i * 10 + j].r();
      // switch
      if (red_value == 0.4)
        ++sensor_counter[i];
      // light off
      if (red_value == 0.5)
        ++sensor_counter[i + 6];
      // light on
      if (red_value == 1.0)
        ++sensor_counter[i + 12];
    }
  }

  // adjust the sensor information
  for (size_t i = 0; i < SENSOR_NUM; ++i) 
  {
    if (sensor_counter[i] > 7)
      sensor_counter[i] = 1;
    else
      sensor_counter[i] = 0;
  }

  return sensor_counter;
}

std::string nevil::robot::get_name() const
{
  return _robot_name;
}

double nevil::robot::_clamp(double val, double min, double max)
{
  if (val < min)
    return min;
  else if (val > max)
    return max;
  else
    return val;
}

void nevil::robot::_set_wheels_speed(double left, double right)
{
  leftSpeed = _clamp(left, -1 * _max_speed, _max_speed);
  rightSpeed = _clamp(right, -1 * _max_speed, _max_speed);
}

bool nevil::robot::is_at_switch() const
{
  for (int i = 0; i < 60; ++i)
    if (camera.image[i].r() != 0.4)
      return false;

  return true;
}

bool nevil::robot::is_at_light() const
{
  for (int i = 0; i < 60; ++i)
    if (camera.image[i].r() != 1.0)
      return false;

  return true;
}
