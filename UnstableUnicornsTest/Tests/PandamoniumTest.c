#include "DowngradeTests.h"

// tests for when unicorns enter the stable
int pandamonium_win_check(void) {
  int num_fails = 0;
  struct Unicorn panda_tmp = Base_DECK[107];
  struct Unicorn basic_tmp = Base_DECK[13];

  AddStable(0, panda_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);

  assert(player[0].stable.size == 8);
  assert(player[0].stable.numUnicorns == 7);
  assert(player[0].stable.numUnicorns >= WIN_CONDITION);
  assert(player[0].flags == PANDAMONIUM);

  // EndOfTurn returns 1 if the player won, or 0 if the game continues
  if (EndOfTurn(0)) {
    num_fails++;
    Red();
    fprintf(stderr, "    win condition test: EndOfTurn failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

int pandamonium_destroy_check(void) {
  int num_fails = 0;
  struct Unicorn panda_tmp = Base_DECK[107];
  struct Unicorn basic_tmp = Base_DECK[13];

  AddStable(0, panda_tmp);
  AddStable(0, basic_tmp);
  assert(player[0].stable.size == 2);
  assert(player[0].flags == PANDAMONIUM);

  // player[0][1] == basic_tmp
  if (CanBeDestroyed(0, 1, ANYUNICORN, FALSE)) {
    num_fails++;
    Red();
    fprintf(stderr, "    destroy test: CanBeDestroyed ANYUNICORN cType failed\n");
    ResetCol();
  }

  if (!CanBeDestroyed(0, 1, ANY, FALSE)) {
    num_fails++;
    Red();
    fprintf(stderr, "    destroy test: CanBeDestroyed ANY cType failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

int pandamonium_rainbow_mane_check(void) {
  int num_fails = 0;
  struct Unicorn panda_tmp = Base_DECK[107];
  struct Unicorn rainbow_tmp = Base_DECK[42];
  struct Unicorn basic_tmp = Base_DECK[13];

  AddStable(0, panda_tmp);
  AddStable(0, rainbow_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  assert(player[0].stable.size == 2);
  assert(player[0].hand.numCards == 1);
  assert(player[0].flags == PANDAMONIUM);
  Base_EnterStableEffects(0, rainbow_tmp.effect);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    rainbow mane test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 3 || player[0].stable.numUnicorns != 2 ||
      strcmp(player[0].stable.unicorns[2].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    rainbow mane test: stable verification failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// PANDAMONIUM
//
// All of your Unicorns are considered Pandas. Cards that affect
// Unicorn cards do not affect your Pandas.
int pandamonium_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting PANDAMONIUM tests...\n");

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

    // already tested it w/ barbed wire, rainbow aura, black knight unicorn, extremely destructive unicorn, sadistic ritual, tiny stable
    num_fails += pandamonium_win_check();
    num_fails += pandamonium_destroy_check();
    num_fails += pandamonium_rainbow_mane_check(); // just making sure that cards are only pandas when inside the stable, not in the hand

    fclose(fp);
  }
  return num_fails;
}
