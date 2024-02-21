#include <networkevents.h>
#include "MagicalUnicornTests.h"

// basic check
int llamacorn_basic_check(void) {
  int num_fails = 0;
  struct Unicorn llamacorn_tmp = Base_DECK[46];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];

  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = basic_tmp2;

  assert(player[0].hand.numCards == 1);
  assert(player[1].hand.numCards == 1);
  AddStable(0, llamacorn_tmp);

  if (player[0].hand.numCards != 0 ||
      player[1].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, basic_tmp2.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard pile size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// player[1] doesn't have any cards to discard, but player[0] does
int llamacorn_empty_check(void) {
  int num_fails = 0;
  struct Unicorn llamacorn_tmp = Base_DECK[46];
  struct Unicorn basic_tmp = Base_DECK[13];

  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  assert(player[0].hand.numCards == 1);
  assert(player[1].hand.numCards == 0);
  AddStable(0, llamacorn_tmp);

  if (player[0].hand.numCards != 0 ||
      player[1].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: hand size failed\n");
    ResetCol();
  }

  // only 1 card should have been discarded because player[1] did not have any
  // in their hand to discard, and EVERY player including player[0] must discard
  // a card if they have one
  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: discard pile size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Llamacorn
//
// When this card enters your Stable, each player (including you)
// must DISCARD a card.
int llamacorn_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Llamacorn tests...\n");
  FILE *fp;

  if (!isClient) {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += llamacorn_basic_check();
    num_fails += llamacorn_empty_check();
  }
  else {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/line_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // input = 1; card index is actually 0
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // empty, no input required
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }

  fclose(fp);
  return num_fails;
}
