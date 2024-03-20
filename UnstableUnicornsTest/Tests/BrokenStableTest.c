#include "DowngradeTests.h"

// sanity test
int broken_basic_check(void) {
  int num_fails = 0;
  struct Unicorn broken_tmp = Base_DECK[111];
  struct Unicorn yay_tmp = Base_DECK[100];

  AddStable(0, broken_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = yay_tmp;
  assert(player[0].flags == BROKEN_STABLE);

  player[0].flags |= YAY;
  PlayCard(0);

  if (moveCount != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 1 ||
      strcmp(player[0].hand.cards[0].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Broken Stable
//
// You cannot play Upgrade cards.
int broken_stable_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Broken Stable tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += broken_basic_check();

    fclose(fp);
  }
  return num_fails;
}
