#include "MagicalUnicornTests.h"

// sanity check
int majestic_flying_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn majestic_tmp = Base_DECK[56];

  AddStable(0, majestic_tmp);
  AddDiscard(basic_tmp);

  assert(discardpile.size == 1);
  assert(player[0].hand.numCards == 0);

  Base_EnterStableEffects(0, majestic_tmp.effect);

  if (discardpile.size != 0 ||
    player[0].hand.numCards != 1 ||
    strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: pulling unicorn card failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // check the sacrifice/destroy effects
  AddStable(0, majestic_tmp);

  assert(player[0].hand.numCards == 0);
  Base_SacrificeDestroyEffects(0, 0, majestic_tmp.effect);

  if (player[0].hand.numCards != 1 ||
    strcmp(player[0].hand.cards[0].name, majestic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand return failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// non-unicorn card in the discard pile
int majestic_flying_empty_check(void) {
  int num_fails = 0;
  struct Unicorn neigh_tmp = Base_DECK[128];
  struct Unicorn majestic_tmp = Base_DECK[56];

  assert(player[0].hand.numCards == 0);

  AddStable(0, majestic_tmp);
  AddDiscard(neigh_tmp);

  Base_EnterStableEffects(0, majestic_tmp.effect);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: hand size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Majestic Flying Unicorn
//
// When this card enters your Stable, you may add a Unicorn card from the discard pile to your hand.
// If this card is sacrificed or destroyed, return it to your hand.
int majestic_flying_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Majestic Flying Unicorn tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/line_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += majestic_flying_basic_check();
    num_fails += majestic_flying_empty_check();

    fclose(fp);
  }
  return num_fails;
}
