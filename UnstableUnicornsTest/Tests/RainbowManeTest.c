#include "UpgradeTests.h"

// sanity test
int rainbow_mane_basic_check(void) {
  int num_fails = 0;
  struct Unicorn rainmane_tmp = Base_DECK[92];
  struct Unicorn basic_tmp = Base_DECK[13];

  AddStable(0, basic_tmp);
  Base_ToggleFlags(0, YAY_EFFECT);
  player[0].hand.cards[player[0].hand.numCards++] = rainmane_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  assert(player[0].stable.size == 1);
  assert(player[0].hand.numCards == 2);
  assert(player[0].flags == YAY);
  PlayCard(0);

  if (turnCount != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: (pre-beginning turn) stable size failed\n");
    ResetCol();
  }

  Base_BeginningTurnEffects(0, rainmane_tmp);

  if (player[0].stable.size != 3 || player[0].stable.numUnicorns != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: (post-beginning turn) stable size failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: (post-beginning turn) hand size failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// no basic unicorn in your stable
int rainbow_mane_empty_check(void) {
  int num_fails = 0;
  struct Unicorn rainmane_tmp = Base_DECK[92];

  player[0].hand.cards[player[0].hand.numCards++] = rainmane_tmp;

  int ret;
  assert(player[0].stable.size == 0);
  ret = Base_ConditionalEffects(0, rainmane_tmp, 0, 0);

  if (turnCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty stable test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Rainbow Mane
//
// This card can only enter a Stable if there is a Basic Unicorn card in that Stable.
// If this card is in your Stable at the beginning of your turn, you may bring a
// Basic Unicorn card from your hand directly into your Stable.
int rainbow_mane_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Rainbow Mane tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/line_1_1_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += rainbow_mane_basic_check();
    num_fails += rainbow_mane_empty_check();

    fclose(fp);
  }
  return num_fails;
}
