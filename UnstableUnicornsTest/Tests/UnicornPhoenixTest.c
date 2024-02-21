#include "MagicalUnicornTests.h"

// sanity check
int phoenix_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn phoenix_tmp = Base_DECK[57];

  // this would only get triggered if i disabled the hand size check in
  // CanBeDestroyed
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 0);
  assert(player[0].stable.size == 0);

  AddStable(0, phoenix_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  Base_SacrificeDestroyEffects(0, 0, phoenix_tmp.effect);

  if (player[0].stable.numUnicorns != 1 || player[0].stable.size != 1 ||
      strcmp(player[0].stable.unicorns[0].name, phoenix_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      player[0].hand.numCards != 0 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand discard failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();
  return num_fails;
}

// no cards in hand to discard
int phoenix_empty_check(void) {
  int num_fails = 0;
  struct Unicorn phoenix_tmp = Base_DECK[57];

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 0);
  assert(player[0].stable.size == 0);

  AddStable(0, phoenix_tmp);

  Base_SacrificeDestroyEffects(0, 0, phoenix_tmp.effect);

  if (player[0].stable.size != 0 || player[0].stable.numUnicorns != 0 ||
      strcmp(discardpile.cards[0].name, phoenix_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty hand test: sacrifice/destroy failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Unicorn Phoenix
//
// If this card would be sacrificed or destroyed, you may DISCARD a card instead.
int unicorn_phoenix_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Unicorn Phoenix tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/line_y_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += phoenix_basic_check();
    num_fails += phoenix_empty_check();

    fclose(fp);
  }
  return num_fails;
}
