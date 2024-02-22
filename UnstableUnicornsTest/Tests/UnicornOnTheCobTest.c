#include "MagicalUnicornTests.h"

// sanity check
int cob_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn cob_tmp = Base_DECK[58];

  int tmp_size = deck.size;

  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);

  AddStable(0, cob_tmp);

  if (player[0].hand.numCards != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: draw failed\n");
    ResetCol();
  }

  if (deck.size != (tmp_size - 2) ||
      discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand discard failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  reset_deck();
  return num_fails;
}

// Unicorn on the Cob
//
// When this card enters your Stable, DRAW 2 cards and DISCARD a card.
int unicorn_on_the_cob_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Unicorn on the Cob tests...\n");

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

    num_fails += cob_basic_check();

    fclose(fp);
  }
  return num_fails;
}
