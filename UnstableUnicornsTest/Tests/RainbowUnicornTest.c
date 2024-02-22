#include "MagicalUnicornTests.h"

// sanity check
int rainbow_unicorn_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn barbed_wire_tmp = Base_DECK[106];
  struct Unicorn rainbow_tmp = Base_DECK[42];

  // stable thing went through
  AddStable(0, rainbow_tmp);
  player[0].hand.cards[0] = basic_tmp;
  player[0].hand.numCards++;
  Base_EnterStableEffects(0, rainbow_tmp.effect);

  if (player[0].stable.numUnicorns != 2 || player[0].stable.size != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }
  reset_players();

  // no basic unicorns
  AddStable(0, rainbow_tmp);
  player[0].hand.cards[0] = barbed_wire_tmp;
  player[0].hand.numCards++;
  Base_EnterStableEffects(0, rainbow_tmp.effect);

  if (player[0].stable.numUnicorns != 1 || player[0].stable.size != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-basic unicorn test: stable size failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-basic unicorn test: hand size failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Rainbow Unicorn
//
// When this card enters your Stable, you may bring a Basic Unicorn card 
// from your hand into your Stable
int rainbow_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Rainbow Unicorn tests...\n");

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

    num_fails += rainbow_unicorn_basic_check();

    fclose(fp);
  }
  return num_fails;
}
