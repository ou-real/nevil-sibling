#include "nevil/parallel/parallel_simulator.hpp"

void nevil::parallel::simulator(nevil::args &cl_args)
{
  using namespace nevil;
  using namespace nevil::parallel;
  
  // Default values
  int num_threads = os::get_num_processors() - 1;
  int num_trials = 30;
  bool seeded_trial = false;

  args::const_iterator it;
  // If the number of threads is provided
  if ((it = cl_args.find("tn")) != cl_args.end())
  {
    num_threads = std::stoi(it->second);
    if (num_threads == 0)
      num_threads = os::get_num_processors();
    else if (num_threads < 0)
      num_threads = std::max(1, os::get_num_processors() + num_threads);
  }

  // If the number of trials is provided
  if ((it = cl_args.find("mt")) != cl_args.end())
    num_trials = std::stoi(it->second);

  // Don't spawn more threads than number of trials
  num_threads = std::min(num_threads, num_trials);
  
  if ((it = cl_args.find("rs")) != cl_args.end())
    seeded_trial = true;

  // If the seed is provided, only run one trial with that seed
  // Don't spawn threads if the number of threads is 1
  if (seeded_trial || num_threads == 1)
  {
    unsigned seed = rand();
    if (seeded_trial)
    {
      seed = std::stoll(cl_args["rs"]);
      // Run only one experiment if seed is provided
      num_trials = 1;
    }

    for (int i = 1; i <= num_trials; ++i)
    {
      trial_controller controller (i, seed, cl_args);
      while (controller.run());
    }
    return;
  }

  std::vector<std::thread *> threads(num_threads);
  const size_t load = num_trials / num_threads;
  int remeinder = num_trials % num_threads;
  size_t start_id = 1;
  size_t end_id = 1;

  // Launching threads
  for(int i = 0; i < num_threads; ++i)
  {
    end_id = start_id + load + (remeinder-- > 0);
    threads[i] = new std::thread (_run_trial, cl_args, start_id, end_id, rand());
    start_id = end_id;
  }

  for (auto i : threads)
  {
    i->join();
    delete i;
  }
}

void nevil::parallel::_run_trial(nevil::args cl_args, size_t start_id, size_t end_id, unsigned random_seed)
{
  srand(random_seed);
  for (int i = start_id; i < end_id; ++i)
  {
    nevil::trial_controller controller (i, rand(), cl_args);
    while (controller.run());
  }
}

