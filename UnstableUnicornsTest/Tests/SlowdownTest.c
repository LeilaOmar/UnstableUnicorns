#include "DowngradeTests.h"

// sanity check
int slowdown_basic_check(void) {
  int num_fails = 0;
  struct Unicorn slowdown_tmp = Base_DECK[109];

  AddStable(0, slowdown_tmp);
  assert(player[0].flags == SLOWDOWN);

  if (CanNeighOthers(0)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: CanNeighOthers failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// SLOWDOWN
//
// You cannot play Instant cards.
int slowdown_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Slowdown tests...\n");

    num_fails += slowdown_basic_check();
  }
  return num_fails;
}
