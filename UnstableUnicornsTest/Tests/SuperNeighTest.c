#include <networkevents.h>
#include "InstantTests.h"

// odd number of neighs, the card goes to the discard pile
int super_neigh_odd_check(void) {
  int num_fails = 0;
  struct Unicorn neigh_tmp = Base_DECK[115];
  struct Unicorn super_neigh_tmp = Base_DECK[128];
  struct Unicorn basic_tmp = Base_DECK[13];

  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = neigh_tmp;

  player[1].hand.cards[player[1].hand.numCards++] = super_neigh_tmp;
  currentPlayers = 2;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 2);
  assert(player[1].hand.numCards == 1);
  assert(player[0].stable.size == 0);
  assert(player[1].stable.size == 0);

  PlayCard(0);

  // player[0] lost the card they originally tried to play
  // player[1] lost 1 neigh
  if (player[0].hand.numCards != 1 || player[1].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    odd test: hand size failed\n");
    ResetCol();
  }

  if (strcmp(player[0].hand.cards[0].name, neigh_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    odd test: player[1] hand verification failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 0 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 0 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    odd test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, super_neigh_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    odd test: discard verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// even number of neighs, the card is in play and goes to the stable
int super_neigh_even_check(void) {
  int num_fails = 0;
  struct Unicorn neigh_tmp = Base_DECK[115];
  struct Unicorn super_neigh_tmp = Base_DECK[128];

  struct Unicorn basic_tmp = Base_DECK[13];

  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = super_neigh_tmp;

  player[1].hand.cards[player[1].hand.numCards++] = neigh_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = neigh_tmp;
  currentPlayers = 2;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 2);
  assert(player[1].hand.numCards == 2);
  assert(player[0].stable.size == 0);
  assert(player[1].stable.size == 0);

  PlayCard(0);

  // player[0] lost 1 neigh and the card they originally tried to play
  // player[1] lost 1 neigh
  if (player[0].hand.numCards != 0 || player[1].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    even test: hand size failed\n");
    ResetCol();
  }

  if (strcmp(player[1].hand.cards[0].name, neigh_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    even test: player[1] hand verification failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 1 ||
      strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    even test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, neigh_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, super_neigh_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    even test: discard verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Super Neigh
//
// Play this card when another player tries to play a card.
// Stop their card from being played and send it to the discard pile.
// This card cannot be Neigh'd.
int super_neigh_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Super Neigh tests...\n");
  FILE *fp;

  if (!isClient) {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/superneigh.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // already tested with YAY, SLOWDOWN, and ginormous unicorn
    num_fails += super_neigh_odd_check();
    num_fails += super_neigh_even_check();
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

    // super neigh odd test; input is 1
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // super neigh even test; input is 1
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }

  fclose(fp);
  return num_fails;
}
