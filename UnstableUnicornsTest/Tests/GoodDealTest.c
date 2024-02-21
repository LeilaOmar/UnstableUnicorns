#include "MagicTests.h"

// sanity test
int good_deal_basic_check(void) {
  int num_fails = 0;
  struct Unicorn deal_tmp = Base_DECK[88];

  struct Unicorn corn = deck.cards[deck.size - 1];
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 0);
  Base_MagicEffects(0, deal_tmp.effect);

  if (player[0].hand.numCards != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }

  // corn should be "Super Neigh"
  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, corn.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// Good Deal
//
// DRAW 3 cards and DISCARD a card.
int good_deal_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Good Deal tests...\n");

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

    num_fails += good_deal_basic_check();

    fclose(fp);
  }
  return num_fails;
}
