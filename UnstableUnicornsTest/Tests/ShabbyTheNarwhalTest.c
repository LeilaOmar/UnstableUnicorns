#include "MagicalUnicornTests.h"

// sanity check
int shabby_basic_check(void) {
  int num_fails = 0;
  struct Unicorn shabby_tmp = Base_DECK[61];
  int tmp_size = deck.size;

  Base_EnterStableEffects(0, shabby_tmp.effect);

  if (player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }

  if (player[0].hand.cards[0].cType != DOWNGRADE) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: downgrade check failed\n");
    ResetCol();
  }

  if (deck.size != (tmp_size - 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: deck shuffle size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  return num_fails;
}

// Shabby the Narwhal
//
// When this card enters your Stable, you may search the deck for a
// Downgrade card and add it to your hand, then shuffle the deck.
int shabby_the_narwhal_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Shabby the Narwhal tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/shabbythenarwhal.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += shabby_basic_check();

    fclose(fp);
  }
  return num_fails;
}
