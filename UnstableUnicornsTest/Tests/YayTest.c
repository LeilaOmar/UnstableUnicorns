#include "UpgradeTests.h"

// sanity test
int yay_basic_check(void) {
  int num_fails = 0;
  struct Unicorn yay_tmp = Base_DECK[100];

  AddStable(0, yay_tmp);

  assert(player[0].flags == YAY);

  if (CanBeNeighed(0)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: CanBeNeighed failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Yay
//
// Cards you play cannot be Neigh'd.
int yay_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Yay tests...\n");

    num_fails += yay_basic_check();
  }
  return num_fails;
}
