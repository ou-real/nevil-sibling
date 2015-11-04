#ifndef _NEVIL_SIBLING_POPULATION_HPP_
#define _NEVIL_SIBLING_POPULATION_HPP_

#include <vector>
#include <algorithm>

#include "nevil/sibling_individual.hpp"
#include "nevil/util/evolution.hpp"

namespace nevil
{
  class sibling_population
  {
  public:
    sibling_population();
    sibling_population(size_t pop_size, bool sibling_neurons, float bracket_ratio ,float mutation_rate);
    virtual ~sibling_population();

    size_t size() const;
    nevil::sibling_individual next_generation();

    sibling_population &operator=(const sibling_population &rhs);
    sibling_individual *operator[](int i);

  protected:
    // Number of individuals created so far
    int _individual_counter;
    size_t _bracket_size;
    float _mutation_rate;
    size_t _population_size;
    std::vector <nevil::sibling_individual *> _individual_list;

  };
}

#endif // _NEVIL_SIBLING_POPULATION_HPP_