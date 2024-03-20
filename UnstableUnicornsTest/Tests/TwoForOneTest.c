#include <networkevents.h>
#include "MagicTests.h"

// sanity check
int twofer_basic_check(void) {
  int num_fails = 0;
  struct Unicorn twofer_tmp = Base_DECK[83];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn panda_tmp = Base_DECK[107];
  struct Unicorn basic_tmp3 = Base_DECK[20];
  struct Unicorn yay_tmp = Base_DECK[100];

  currentPlayers = 3;
  AddStable(0, basic_tmp);
  AddStable(0, yay_tmp);
  AddStable(1, panda_tmp);
  AddStable(1, basic_tmp3);
  player[0].hand.cards[player[0].hand.numCards++] = twofer_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].stable.size == 2);
  assert(player[1].stable.size == 2);
  assert((player[0].flags & YAY) == YAY);
  PlayCard(0);

  if (moveCount != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 0 || player[1].stable.numUnicorns != 0 ) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 4 ||
      strcmp(discardpile.cards[0].name, twofer_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, basic_tmp.name) != 0 ||
      strcmp(discardpile.cards[2].name, panda_tmp.name) != 0 ||
      strcmp(discardpile.cards[3].name, basic_tmp3.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// no cards to sacrifice or destroy
int twofer_empty_check(void) {
  int num_fails = 0;
  struct Unicorn twofer_tmp = Base_DECK[83];
  struct Unicorn puppicorn_tmp = Base_DECK[41]; // cannot be destroyed, periodt.
  struct Unicorn kitten_tmp = Base_DECK[39]; // cannot be destroyed by magic cards
  struct Unicorn basic_tmp = Base_DECK[13]; // [emojis]

  // no cards to sacrifice
  currentPlayers = 2;
  AddStable(0, puppicorn_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = twofer_tmp;

  int ret;
  assert(player[0].stable.size == 1);
  assert(player[1].stable.size == 2);
  ret = Base_ConditionalEffects(0, twofer_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty sacrifice test: turn count failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // no cards to destroy
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(1, puppicorn_tmp);
  AddStable(1, kitten_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = twofer_tmp;

  assert(player[0].stable.size == 1);
  assert(player[1].stable.size == 2);
  ret = Base_ConditionalEffects(0, twofer_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty destroy test: turn count failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // only one card available to destroy
  currentPlayers = 2;
  AddStable(0, kitten_tmp);
  AddStable(1, puppicorn_tmp);
  AddStable(1, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = twofer_tmp;

  assert(player[0].stable.size == 1);
  assert(player[1].stable.size == 2);
  ret = Base_ConditionalEffects(0, twofer_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    incomplete destroy test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Two-For-One
//
// SACRIFICE a card, then DESTROY 2 cards.
int two_for_one_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Two-For-One tests...\n");
  if (!isClient) {
    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/twoforone.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += twofer_basic_check();
    num_fails += twofer_empty_check();

    fclose(fp);
  }
  else {
    // basic check, no input
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }
  return num_fails;
}
