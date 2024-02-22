#include "MagicalUnicornTests.h"

// sanity check; this doubles as a check for toggled flags
int great_narwhal_basic_check(void) {
  int num_fails = 0;
  struct Unicorn great_tmp = Base_DECK[66];
  struct Unicorn classy_tmp = Base_DECK[65];
  struct Unicorn corn = deck.cards[42];
  // corn is magical flying unicorn w/ the base deck; the index is notably
  // before any upgrade cards, and the shuffling assumes the same seed each time

  int tmp_size = deck.size;
  assert(player[0].hand.numCards == 0);

  AddStable(0, great_tmp);

  if (deck.size != (tmp_size - 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "\n    sanity test: deck size failed\n");
    ResetCol();
  }

  if (strcmp(corn.name, deck.cards[42].name) == 0) {
    num_fails++;
    Red();
    fprintf(stderr, "\n    sanity test: deck shuffle failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 1 ||
      strcmp(player[0].hand.cards[0].name, classy_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand return failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  return num_fails;
}

// The Great Narwhal
//
// When this card enters your Stable, you may search the deck for a card with
// "Narwhal" in its name and add it to your hand, then shuffle the deck.
int the_great_narwhal_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting The Great Narwhal tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/thegreatnarwhal.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += great_narwhal_basic_check();

    fclose(fp);
  }
  return num_fails;
}
