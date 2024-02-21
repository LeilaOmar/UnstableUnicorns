#include "MagicalUnicornTests.h"

// sanity check
int ginormous_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn ginormous_tmp = Base_DECK[48];

  currentPlayers = 2;
  AddStable(0, ginormous_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);

  int ret = EndOfTurn(0);

  assert(WIN_CONDITION == 7);
  assert(player[0].stable.numUnicorns == 6);
  assert((player[0].flags & GINORMOUS_UNICORN) == GINORMOUS_UNICORN);

  if (ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: win condition failed\n");
    ResetCol();
  }

  if (CanNeighOthers(0)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: CanNeighOthers failed\n");
    ResetCol();
  }

  Base_ToggleFlags(0, ginormous_tmp.effect);
  ret = EndOfTurn(0);

  if (ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: toggle flag failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  return num_fails;
}

// Ginormous Unicorn
//
// This card counts for 2 Unicorns. You cannot play any Neigh cards.
int ginormous_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Ginormous Unicorn tests...\n");

    num_fails += ginormous_basic_check();
  }
  return num_fails;
}
