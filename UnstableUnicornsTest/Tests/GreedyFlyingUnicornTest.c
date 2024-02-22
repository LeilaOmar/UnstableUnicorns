#include "MagicalUnicornTests.h"

// sanity check
int greedy_basic_check(void) {
  int num_fails = 0;
  struct Unicorn greedy_tmp = Base_DECK[52];

  assert(player[0].hand.numCards == 0);

  AddStable(0, greedy_tmp);

  if (player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: draw failed\n");
    ResetCol();
  }
  reset_players();

  // check the sacrifice/destroy effects
  player[0].stable.unicorns[0] = greedy_tmp;

  assert(player[0].hand.numCards == 0);
  Base_SacrificeDestroyEffects(0, 0, greedy_tmp.effect);

  if (player[0].hand.numCards != 1 ||
      strcmp(player[0].hand.cards[0].name, greedy_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand return failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Greedy Flying Unicorn
//
// When this card enters your Stable, DRAW a card.
// If this card is sacrificed or destroyed, return it to your hand.
int greedy_flying_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Greedy Flying Unicorn tests...\n");

    num_fails += greedy_basic_check();
  }
  return num_fails;
}
