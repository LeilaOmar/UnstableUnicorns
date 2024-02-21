#include "MagicalUnicornTests.h"

// sanity check
int zombie_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn zombie_tmp = Base_DECK[43];

  AddStable(0, zombie_tmp);
  AddDiscard(basic_tmp);
  Draw(0, 1);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  int tmp_stable_size = player[0].stable.size;
  int tmp_discard_size = discardpile.size;

  Base_BeginningTurnEffects(0, player[0].stable.unicorns[0]);

  if (discardpile.size != tmp_discard_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard pile size failed\n");
    ResetCol();
  }

  if (player[0].stable.numUnicorns != (tmp_stable_size + 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  if (turnCount != -1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

int zombie_invalid_hand_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn zombie_tmp = Base_DECK[43];

  AddStable(0, zombie_tmp);
  AddDiscard(basic_tmp);
  Draw(0, 1); // this is a super neigh and not a unicorn card

  int tmp_stable_size = player[0].stable.size;
  int tmp_discard_size = discardpile.size;

  Base_BeginningTurnEffects(0, player[0].stable.unicorns[0]);

  if (discardpile.size != tmp_discard_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    invalid hand test: discard pile size failed\n");
    ResetCol();
  }

  if (player[0].stable.numUnicorns != tmp_stable_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    invalid hand test: unicorn size failed\n");
    ResetCol();
  }

  if (turnCount <= 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    invalid hand test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

int zombie_invalid_discard_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn zombie_tmp = Base_DECK[43];

  AddStable(0, zombie_tmp);
  AddDiscard(yay_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  int tmp_stable_size = player[0].stable.size;
  int tmp_discard_size = discardpile.size;

  Base_BeginningTurnEffects(0, player[0].stable.unicorns[0]);

  if (discardpile.size != tmp_discard_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    invalid discard pile test 1 failed\n");
    ResetCol();
  }

  if (player[0].stable.numUnicorns != tmp_stable_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    invalid discard pile test 2 failed\n");
    ResetCol();
  }

  if (turnCount <= 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    invalid discard pile test 3 failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// Zombie Unicorn
//
// If this card is in your Stable at the beginning of your turn, you may DISCARD a Unicorn card.
// If you do, choose a Unicorn card from the discard pile and bring it directly into your Stable,
// then immediately skip to your End of Turn Phase
int zombie_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Zombie Unicorn tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/zombieunicorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += zombie_basic_check();
    num_fails += zombie_invalid_hand_check();
    num_fails += zombie_invalid_discard_check();

    fclose(fp);
  }
  return num_fails;
}
