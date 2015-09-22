#include "nevil/trial_controller.hpp"

nevil::trial_controller::trial_controller() {}

nevil::trial_controller::trial_controller(int id, unsigned seed, nevil::args &cl_args)
  : _trial_id(id)
{
  srand(seed);

  //Default Values
  _population_size = 80;
  _max_generation_num = 200;
  _max_step_num = 1000;
  std::string trial_name = "SiblingTrial";
  bool sibling_neuron = false;
  float mutation_rate = 0.25;
  float bracket_ratio = 0.1;

  // Reading the command line arguments
  nevil::args::const_iterator it;

  if ((it = cl_args.find("xn")) != cl_args.end())
    trial_name = it->second;

  if ((it = cl_args.find("mg")) != cl_args.end())
    _max_generation_num = stoi(it->second);

  if ((it = cl_args.find("ms")) != cl_args.end())
    _max_step_num = stoi(it->second);

  if ((it = cl_args.find("sn")) != cl_args.end())
    sibling_neuron = (it->second == "true");
  else
    cl_args["sn"] = (sibling_neuron ? "true":"false");

  if ((it = cl_args.find("ps")) != cl_args.end())
    _population_size = stoi(it->second);
  else
    cl_args["ps"] = std::to_string(_population_size);

  if ((it = cl_args.find("br")) != cl_args.end())
  {
    bracket_ratio = stof(it->second);
    if (bracket_ratio < 0 || bracket_ratio > 1)
    {
      printf("Bracket ratio must be between 0 and 1. '%.2f' was given.\nTerminating ...", bracket_ratio);
      exit(-1); 
    }
  }
  else
    cl_args["br"] = std::to_string(bracket_ratio);

  if ((it = cl_args.find("mr")) != cl_args.end())
  {
    mutation_rate = stof(it->second);
    if (mutation_rate < 0 || mutation_rate > 1)
    {
      printf("Mutation rate must be between 0 and 1. '%.2f' was given.\nTerminating ...", mutation_rate);
      exit(-1); 
    }
  }
  else
    cl_args["mr"] = std::to_string(mutation_rate);

  // Creating a log file
  std::string file_name = "Trial_" + std::to_string(_trial_id) + ".txt";
  _trial_logger.start_new_file(cl_args["xp_path"], file_name);

  // Output arguments to file
  _trial_logger << "==Controller Config==" << std::endl;
  _trial_logger << "-Random seed: " << seed << std::endl;
  _trial_logger << "-Number of generations: " << _max_generation_num << std::endl;
  _trial_logger << "-Number of timesteps: " << _max_step_num << std::endl;
  _trial_logger << "==Trial config==" << std::endl;
  _trial_logger << "-Name: " << trial_name << std::endl;
  _trial_logger << "-Population size: " << _population_size << std::endl;
  _trial_logger << "-Sibling Neuron: " <<  (sibling_neuron ? "True":"False") << std::endl;
  _trial_logger << "-Bracket Ratio: " << bracket_ratio << " (" << (_population_size * bracket_ratio) << ")" << std::endl;
  _trial_logger << "-Mutation Rate: " << mutation_rate << std::endl;
  _trial_logger << "==Starting Trial==" << std::endl;

  // Instantiating a controller
  // If you have more than one controller you can use the controller name to instantiate the right one
  _trial = new nevil::sibling_trial(cl_args);

  _current_generation = 0;
  _current_individual = 0;
  _current_step = 0;
  printf("-Trial %d: starting\n", _trial_id);
  printf("-Trial %d: running generation %d\n", _trial_id, _current_generation);
}

nevil::trial_controller::~trial_controller() 
{
  delete _trial;
}

Enki::World *nevil::trial_controller::get_trial_world()
{
  return _trial->get_trial_world();
}

bool nevil::trial_controller::run()
{
  if (_current_generation < _max_generation_num)
  {
    if (_current_individual < _population_size)
    {
      if (_current_step == 0 && _current_individual != _population_size)
        _trial->reset();

      if (_current_step < _max_step_num)
      {
        _trial->update();
        ++_current_step;
      }
      else
      {
        ++_current_individual;
        _current_step = 0;
      }
    }
    else
    {
      _evaluate();
      ++_current_generation;
      _current_individual = 0;
      _current_step = 0;
      printf("-Trial %d: running generation %d\n", _trial_id, _current_generation);
    }
    return true;
  }

  _end();
  return false;
}


void nevil::trial_controller::_evaluate()
{
  std::string generation_info = _trial->get_generation_data();
  _trial->epoch();

  _trial_logger << _current_generation << "\t" << _trial->get_best_individual().str() << std::endl;
  _trial_logger << generation_info << std::endl;
}

void nevil::trial_controller::_end()
{
  printf("-Trial %d: finished\n", _trial_id);
  _trial_logger << "==Trial Ended==" << std::endl;
  nevil::sibling_individual best_individual = _trial->get_best_individual();
  _trial_logger << _current_generation << "\t" << best_individual.str() << ":" << best_individual.get_chromosome() << std::endl;
  _trial_logger.close_file();
}