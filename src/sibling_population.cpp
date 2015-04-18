#include "nevil/sibling_population.hpp"

int nevil::sibling_population::_individual_counter = 0;

nevil::sibling_population::sibling_population() {}

nevil::sibling_population::sibling_population(size_t pop_size, bool sibling_neurons, float bracket_ratio, float mutation_rate)
  : _population_size(pop_size)
  , _bracket_size(int(bracket_ratio * pop_size))
  , _mutation_rate(mutation_rate)
{
  _individual_list = std::vector<nevil::sibling_individual *>(2 * _population_size);
  for (int i = 0; i < _population_size; ++i)
  {
    _individual_list[i] = new nevil::sibling_individual(sibling_neurons ? 42 : 38, true);
    _individual_list[i]->set_id(_individual_counter);
    _individual_list[i + _population_size] = new nevil::sibling_individual(_individual_list[i]->get_chromosome(), false);
    _individual_list[i + _population_size]->set_id(_individual_counter);
    ++_individual_counter;
  }
}

nevil::sibling_population::~sibling_population()
{
  for (auto i : _individual_list)
    delete i;
}

size_t nevil::sibling_population::size() const
{
  return _individual_list.size();
}

nevil::sibling_individual nevil::sibling_population::next_generation()
{
  //sort part
  std::sort(_individual_list.begin(), _individual_list.end(), [](nevil::sibling_individual *a, nevil::sibling_individual *b) {
    return (*b) < (*a);
  });

  nevil::sibling_individual best_individual(*_individual_list[0]);

  // tournament part
  auto selected_individuals = nevil::evolution::tournament_selection(_individual_list, _population_size, _bracket_size);

  for (int i = 0; i < _population_size; ++i)
  {
    delete _individual_list[i];
    _individual_list[i] = selected_individuals[i]->clone(true);
    _individual_list[i]->set_id(_individual_counter);
    _individual_list[i]->mutate(_mutation_rate);

    delete _individual_list[i + _population_size];
    _individual_list[i + _population_size] = selected_individuals[i]->clone(false);
    _individual_list[i + _population_size]->set_id(_individual_counter);
    _individual_list[i + _population_size]->mutate(_mutation_rate);

    delete selected_individuals[i];
    ++_individual_counter;
  }

  return best_individual;
}


nevil::sibling_population &nevil::sibling_population::operator=(const nevil::sibling_population &rhs)
{
  for (auto i : _individual_list)
    delete i;
  _population_size = rhs._population_size;
  _mutation_rate = rhs._mutation_rate;
  _bracket_size = rhs._bracket_size;
  _individual_list = std::vector<nevil::sibling_individual *> (rhs.size());
  for (int i = 0; i < _individual_list.size(); ++i)
    _individual_list[i] = new nevil::sibling_individual(*rhs._individual_list[i]);

  return (*this);
}


nevil::sibling_individual* nevil::sibling_population::operator[](int i)
{
  return _individual_list[i];
}