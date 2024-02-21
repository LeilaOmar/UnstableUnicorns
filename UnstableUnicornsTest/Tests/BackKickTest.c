#include <networkevents.h>
#include "MagicTests.h"

// sanity check
int back_kick_basic_check(void) {
  int num_fails = 0;
  struct Unicorn back_kick_tmp = Base_DECK[70];
  struct Unicorn rainbow_tmp = Base_DECK[42];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn basic_tmp3 = Base_DECK[20];

  currentPlayers = 2;
  AddStable(1, rainbow_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, yay_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = back_kick_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp3;
  player[1].hand.cards[player[1].hand.numCards++] = basic_tmp2;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 2);
  assert(player[1].hand.numCards == 1);
  assert(player[1].stable.size == 3);
  assert(player[1].stable.numUnicorns == 2);
  Base_MagicEffects(0, back_kick_tmp.effect);

  // this includes back kick
  if (player[0].hand.numCards != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[0] hand size failed\n");
    ResetCol();
  }

  if (player[1].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[1] hand size failed\n");
    ResetCol();
  }

  if (player[1].stable.size != 2 || player[1].stable.numUnicorns != 1 ||
      strcmp(player[1].stable.unicorns[1].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable verification failed\n");
    ResetCol();
  }

  // this does not include back kick, although normally it would
  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp2.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// check to make sure toggles are applied;
// this also checks if it works when a player's hand was empty
int back_kick_special_check(void) {
  int num_fails = 0;
  struct Unicorn back_kick_tmp = Base_DECK[70];
  struct Unicorn yay_tmp = Base_DECK[100];

  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = back_kick_tmp;
  AddStable(1, yay_tmp);

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[1].hand.numCards == 0);
  assert(player[1].stable.size == 1);
  assert((player[1].flags & YAY) == YAY);
  Base_MagicEffects(0, back_kick_tmp.effect);


  if ((player[1].flags & YAY) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    toggle test: YAY flag toggle failed\n");
    ResetCol();
  }

  if (player[1].stable.size != 0 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    toggle test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    toggle test: discard verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no cards in stable before the effect takes place
int back_kick_empty_stable_check(void) {
  int num_fails = 0;
  struct Unicorn back_kick_tmp = Base_DECK[70];

  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = back_kick_tmp;

  int ret;
  assert(player[1].stable.size == 0);
  assert(player[1].stable.numUnicorns == 0);
  ret = Base_ConditionalEffects(0, back_kick_tmp, 0, 0);

  if (turnCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty stable test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// check against a player w/ only 1 baby unicorn in their stable and 0 cards in their hand
int back_kick_baby_check(void) {
  int num_fails = 0;
  struct Unicorn back_kick_tmp = Base_DECK[70];
  struct Unicorn baby_tmp = Base_DECK[12];

  currentPlayers = 2;
  AddStable(1, baby_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = back_kick_tmp;

  nursery.size = 5;
  int tmp_size = nursery.size;
  assert(discardpile.size == 0);
  assert(player[1].hand.numCards == 0);
  assert(player[1].stable.size == 1);
  Base_MagicEffects(0, back_kick_tmp.effect);

  if (player[1].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    baby unicorn test: player[1] hand size failed\n");
    ResetCol();
  }

  if (player[1].stable.size != 0 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    baby unicorn test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard size failed\n");
    ResetCol();
  }

  if (nursery.size != (tmp_size + 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    baby unicorn test: nursery check failed\n");
    ResetCol();
  }

  reset_nursery();
  reset_players();
  reset_discard();
  return num_fails;
}

// Back Kick
//
// Return a card in another player's Stable to their hand.
// That player must DISCARD a card.
int back_kick_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Back Kick tests...\n");
  FILE *fp;

  if (!isClient) {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/backkick.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += back_kick_basic_check();
    num_fails += back_kick_special_check();
    num_fails += back_kick_empty_stable_check();
    num_fails += back_kick_baby_check();
  }
  else {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // basic check
    // input = 1; card index is actually 0
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // special check
    // input = 1; card index is actually 0
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // empty stable check
    // should be nothing

    // baby unicorn check
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }

  fclose(fp);
  return num_fails;
}
