#pragma once

#include <Game/Interfaces/GameTabs/Magic.hpp>

#include <array>

namespace bt::ab::magic {
template <Magic::SPELL S> class spell;

class spell_component {
public:
  std::string_view name;
  int count;
  bool is_equipment = false;

  constexpr spell_component(std::string_view name, int count, bool is_rune)
      : name(name), count(count), is_rune(is_rune) {}
  
  bool has_required() const {
    if (is_equipment) {
      return Equipment::Contains
    }
  }
};

template <> class spell<Magic::NORMAL_HIGH_LEVEL_ALCHEMY> {
public:
  static constexpr std::string_view name = "High level alchemy";

  static constexpr std::array<spell_component, 2> components = {
      spell_component{.name = "Nature rune", .count = 1},
      spell_component{.name = "Fire rune", .count = 5}};
};

template <typename Spell> class spell_traits {
public:
  static constexpr std::string_view name = Spell::name;

  static constexpr std::array<spell_component, Spell::components.size()>
      components = Spell::components;
};

} // namespace bt::ab::magic