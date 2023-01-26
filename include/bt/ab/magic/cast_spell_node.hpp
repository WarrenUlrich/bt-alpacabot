#pragma once

#include <bt/behavior_node.hpp>
#include <bt/ab/magic/spell.hpp>

namespace bt::ab::magic {
template <Magic::SPELL S, typename Context> class cast_spell_node {
public:
  static constexpr auto spell = spell_traits<spell<S>>{};

  status tick(Context &ctx) {
    std::cout << "Casting spell: " << spell.name << std::endl;

    for (const auto &component : spell.components) {
      std::cout << "Component required: " << component.name << " x "
                << component.count << std::endl;
    }
    
    return status::success;
  }
};
} // namespace bt::ab::magic