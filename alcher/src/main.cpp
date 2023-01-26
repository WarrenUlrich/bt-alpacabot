#include <Core/Script.hpp>
#include <Game/Core.hpp>

#include <bt/action_node.hpp>
#include <bt/selector_node.hpp>
#include <bt/sequence_node.hpp>
#include <bt/conditional_node.hpp>
#include <bt/ab/antiban/idle_node.hpp>

void Setup() {
  ScriptInfo Info;
  Info.Name = "Alcher";
  Info.Description = "Test script for bt-alpacabot";
  Info.Version = "1.00";
  Info.Category = "Magic";
  Info.Author = "Warren";
  Info.UID = "UID";
  Info.ForumPage = "forum.alpacabot.org";
  SetScriptInfo(Info);
}

class alcher_context {
public:
  bool done = false;
  std::string item_name;
};

auto behavior_tree = bt::make_sequence_node<alcher_context>( // root
    bt::make_conditional_node<alcher_context>([](auto &ctx) {
      if (!Inventory::Contains(ctx.item_name)) {
        std::cout << "No more items to alch" << std::endl;
        ctx.done = true;
        return false;
      }

      if (!Equipment::Contains("Staff of fire")) {
        std::cout << "No staff of fire" << std::endl;
        ctx.done = true;
        return false;
      }

      if (Inventory::Count("Nature rune") < 1) {
        std::cout << "Not enough nature runes" << std::endl;
        ctx.done = true;
        return false;
      }

      return true;
    }),
    bt::ab::antiban::idle_node<alcher_context>(
        std::chrono::seconds(10),
        std::chrono::seconds(5),
        std::chrono::seconds(5),
        std::chrono::seconds(2)),
    bt::make_selector_node<alcher_context>(
        bt::make_sequence_node<alcher_context>( // Select spell sequence
            bt::make_conditional_node<alcher_context>([](auto &ctx) {
              return !Magic::IsSpellSelected(Magic::NORMAL_HIGH_LEVEL_ALCHEMY);
            }),
            bt::make_action_node<alcher_context>([](auto &ctx) {
              std::cout << "Selecting spell" << std::endl;
              if (!Magic::SelectSpell(Magic::NORMAL_HIGH_LEVEL_ALCHEMY))
                return bt::status::failure;

              if (!WaitFunc(1000, 50, []() {
                    return Magic::IsSpellSelected(
                        Magic::NORMAL_HIGH_LEVEL_ALCHEMY);
                  }))
                return bt::status::failure;

              return bt::status::success;
            })),
        bt::make_sequence_node<alcher_context>( // Cast spell sequence
            bt::make_conditional_node<alcher_context>([](auto &ctx) {
              return Magic::IsSpellSelected(Magic::NORMAL_HIGH_LEVEL_ALCHEMY);
            }),
            bt::make_action_node<alcher_context>([](auto &ctx) {
              const auto item = Inventory::GetItem(ctx.item_name);
              if (!item)
                return bt::status::failure;

              std::cout << "Casting spell" << std::endl;
              if (!item.Interact("Cast"))
                return bt::status::failure;

              if (!WaitFunc(1000, 50, Mainscreen::IsAnimating))
                return bt::status::failure;

              Wait(UniformRandom(1000, 1500));
              return bt::status::success;
            }))));

auto context = alcher_context{.item_name = "Iron arrow"};

bool OnStart() { return true; }

bool Loop() {
  behavior_tree.tick(context);
  return !context.done;
}

bool OnBreak() { return true; }

void OnEnd() {}
