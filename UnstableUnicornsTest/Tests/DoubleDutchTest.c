#include "UpgradeTests.h"

// sanity test
int dutch_basic_check(void) {
  int num_fails = 0;
  struct Unicorn dutch_tmp = Base_DECK[104];
  struct Unicorn basic_tmp = Base_DECK[13];

  AddStable(0, dutch_tmp);
  player[0].flags = YAY;
  assert(player[0].stable.size == 1);
  Base_BeginningTurnEffects(0, dutch_tmp);

  // first, check the turn count
  if (moveCount != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: pre-phase turn count failed\n");
    ResetCol();
  }

  // now check if it actually plays 2 cards during the action phase
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  assert(player[0].hand.numCards == 2);

  ActionPhase(0);

  if (moveCount != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: post-phase turn count failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 3 || player[0].stable.numUnicorns != 2 ||
      strcmp(player[0].stable.unicorns[0].name, dutch_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[1].name, basic_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[2].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable verification failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Double Dutch
//
// If this card is in your Stable at the beginning of your turn,
// you may play 2 cards during your Action phase.
int double_dutch_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Double Dutch tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/doubledutch.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += dutch_basic_check();

    fclose(fp);
  }
  return num_fails;
}
