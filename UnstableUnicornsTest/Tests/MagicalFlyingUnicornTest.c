#include "MagicalUnicornTests.h"

// sanity check
int magical_flying_basic_check(void) {
  int num_fails = 0;
  struct Unicorn poison_tmp = Base_DECK[67];
  struct Unicorn magical_tmp = Base_DECK[54];

  AddStable(0, magical_tmp);
  AddDiscard(poison_tmp);

  assert(discardpile.size == 1);
  assert(player[0].hand.numCards == 0);

  Base_EnterStableEffects(0, magical_tmp.effect);

  if (discardpile.size != 0 ||
      player[0].hand.numCards != 1 ||
      strcmp(player[0].hand.cards[0].name, poison_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: pulling magic card failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // check the sacrifice/destroy effects
  AddStable(0, magical_tmp);

  assert(player[0].hand.numCards == 0);
  Base_SacrificeDestroyEffects(0, 0, magical_tmp.effect);

  if (player[0].hand.numCards != 1 ||
    strcmp(player[0].hand.cards[0].name, magical_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand return failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// non-magic card in the discard pile
int magical_flying_empty_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn magical_tmp = Base_DECK[54];

  assert(player[0].hand.numCards == 0);

  AddStable(0, magical_tmp);
  AddDiscard(basic_tmp);

  Base_EnterStableEffects(0, magical_tmp.effect);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-magic test: hand size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Magical Flying Unicorn
//
// When this card enters your Stable, you may add a Magic card from the discard pile to your hand.
// If this card is sacrificed or destroyed, return it to your hand
int magical_flying_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Magical Flying Unicorn tests...\n");

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

    num_fails += magical_flying_basic_check();
    num_fails += magical_flying_empty_check();

    fclose(fp);
  }
  return num_fails;
}
