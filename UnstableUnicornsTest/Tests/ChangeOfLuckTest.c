#include <networkevents.h>
#include "MagicTests.h"

// sanity check
int luck_basic_check(void) {
  int num_fails = 0;
  struct Unicorn luck_tmp = Base_DECK[73];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn super_tmp = Base_DECK[128];
  struct Unicorn neigh_tmp = Base_DECK[127];

  Base_ToggleFlags(0, YAY_EFFECT);
  player[0].hand.cards[player[0].hand.numCards++] = luck_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 2);
  assert(player[0].flags == YAY);
  PlayCard(0);

  if (turnCount != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  // hand size is 1 because it restarted the beginning turn phase, so
  // the player drew again
  if (player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 4 ||
    strcmp(discardpile.cards[0].name, luck_tmp.name) != 0 ||
    strcmp(discardpile.cards[1].name, basic_tmp.name) != 0 ||
    strcmp(discardpile.cards[2].name, super_tmp.name) != 0 ||
    strcmp(discardpile.cards[3].name, neigh_tmp.name) != 0) {
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

// check to see if the beginning turn effect gets triggered
int luck_special_check(void) {
  int num_fails = 0;
  struct Unicorn luck_tmp = Base_DECK[73];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn fertile_tmp = Base_DECK[38];
  struct Unicorn baby_tmp = Base_DECK[0];

  AddStable(0, fertile_tmp);
  Base_ToggleFlags(0, YAY_EFFECT);
  player[0].hand.cards[player[0].hand.numCards++] = luck_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  int tmp_size = nursery.size;
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 3);
  assert(player[0].stable.size == 1);
  assert(player[0].flags == YAY);
  PlayCard(0);

  if (nursery.size != (tmp_size - 1) ||
      strcmp(player[0].stable.unicorns[1].name, baby_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    special test: extremely fertile unicorn effect failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    special test: stable size failed\n");
    ResetCol();
  }

  reset_players();
  reset_nursery();
  reset_deck();
  reset_discard();
  return num_fails;
}

// no cards in hand before the effect takes place
int luck_empty_check(void) {
  int num_fails = 0;
  struct Unicorn luck_tmp = Base_DECK[73];

  player[0].hand.cards[player[0].hand.numCards++] = luck_tmp;

  int ret;
  assert(player[0].hand.numCards == 1);
  ret = Base_ConditionalEffects(0, luck_tmp, 0, 0);

  if (ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty hand test: conditional effect return failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// compatibility w/ unicorn lasso & a magical unicorn that triggers upon entering the stable
int luck_unicorn_lasso_check(void) {
  int num_fails = 0;
  struct Unicorn luck_tmp = Base_DECK[73];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn lasso_tmp = Base_DECK[102];
  struct Unicorn rainbow_tmp = Base_DECK[42]; // brings basic unicorn from hand to stable

  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = luck_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp2;

  Draw(0, 1);
  AddStable(0, lasso_tmp);
  AddStable(1, rainbow_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  Base_ToggleFlags(0, YAY_EFFECT);

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 4);
  assert(player[0].stable.size == 1);
  assert(player[0].stable.numUnicorns == 0);
  assert(player[0].flags == YAY);

  Base_BeginningTurnEffects(0, lasso_tmp);
  assert(uniLassoIndex[0] != -1);
  PlayCard(0);

  // this is checking in the middle of the 2nd turn, so player[0] still
  // has player[1]'s card
  // 
  // beginning of turn #1:
  // stable[0] = unicorn lasso
  // unicorn lasso effect:
  // stable[0] = unicorn lasso
  // stable[1] = rainbow unicorn
  // enter stable effect: -- 4 cards in hand
  // stable[0] = unicorn lasso
  // stable[1] = rainbow unicorn
  // stable[2] = basic unicorn #1
  // change of luck: -- 3 cards in hand
  // --restart turn
  // end of turn #1: -- 1 card in hand
  // stable[0] = unicorn lasso
  // stable[1] = basic unicorn #1
  // beginning of turn #2, unicorn lasso effect + enter stable effect again: -- 0 cards in hand
  // stable[0] = unicorn lasso
  // stable[1] = basic unicorn #1
  // stable[2] = rainbow unicorn
  // stable[3] = basic unicorn #2
  // draw (after beginning turn effect) -- 1 card in hand
  // 
  if (uniLassoIndex[0] != 2 ||
      strcmp(player[0].stable.unicorns[uniLassoIndex[0]].name, rainbow_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: unicorn swap failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 4 || player[0].stable.numUnicorns != 3) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: stable size failed\n");
    ResetCol();
  }

  // this is just the last card that was drawn during the 2nd beginning turn phase
  if (player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: hand size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// Change of Luck
//
// DRAW 2 cards and DISCARD 3 cards, then take another turn.
int change_of_luck_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Change of Luck tests...\n");
  if (!isClient) {
    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/changeofluck.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += luck_basic_check();
    num_fails += luck_special_check();
    num_fails += luck_empty_check();
    num_fails += luck_unicorn_lasso_check();

    fclose(fp);
  }
  else {
    // should be no input required
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }
  return num_fails;
}
