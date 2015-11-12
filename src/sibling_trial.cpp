#include "nevil/sibling_trial.hpp"

nevil::sibling_trial::sibling_trial() {}


nevil::sibling_trial::sibling_trial(nevil::args &cl_args)
{
  const int WORLD_SIZE_X = 40;
  const int WORLD_SIZE_Y = 50;

  _population_size = std::stoi(cl_args["ps"]);
  bool sibling_neuron = (cl_args["sn"] == "true");
  float bracket_ratio = std::stof(cl_args["br"]);
  float mutation_rate = std::stof(cl_args["mr"]);
  double speed_A = std::stod(cl_args["speedA"]);
  double speed_B = std::stod(cl_args["speedB"]);
  double agnle_B = std::stod(cl_args["angleA"]);
  double angle_B = std::stod(cl_args["angleB"]);

  _trial_arena = new nevil::sibling_arena(WORLD_SIZE_X, WORLD_SIZE_Y, sibling_neuron, speed_A, speed_B, agnle_B, angle_B);
  _population = nevil::sibling_population(_population_size, sibling_neuron, bracket_ratio, mutation_rate);
  _current_index = 0;
}

nevil::sibling_trial::~sibling_trial() 
{
  delete _trial_arena;
}

Enki::World *nevil::sibling_trial::get_trial_world()
{
  return _trial_arena->get_world();
}

bool nevil::sibling_trial::update()
{
  _trial_arena->tick();
  _trial_arena->update();
  return true;
}

bool nevil::sibling_trial::reset()
{
  _trial_arena->reset();
  _trial_arena->set_individuals(_population[_current_index], 
    _population[_current_index + _population_size]);
  ++_current_index;
  return true;
}

bool nevil::sibling_trial::epoch()
{
  _best_individual = _population.next_generation();
  _current_index = 0;

  return true;
}

nevil::sibling_individual nevil::sibling_trial::get_best_individual() const
{
  return _best_individual;
}

Json::Value nevil::sibling_trial::get_generation_data()
{
  Json::Value generation_info (Json::arrayValue);
  for (int i = 0; i < _population.size(); ++i)
    generation_info.append(_population[i]->json());

  return generation_info;
}
