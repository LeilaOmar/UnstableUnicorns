#include <networkevents.h>
#include "InstantTests.h"

// odd number of neighs, the card goes to the discard pile
int neigh_odd_check(void) {
  int num_fails = 0;
  struct Unicorn neigh_tmp = Base_DECK[115];
  struct Unicorn rainbow_tmp = Base_DECK[42];
  struct Unicorn fertile_tmp = Base_DECK[38];
  struct Unicorn ginormous_tmp = Base_DECK[48];
  struct Unicorn puppicorn_tmp = Base_DECK[41];

  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn basic_tmp3 = Base_DECK[20];

  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = neigh_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp2;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp3;

  player[1].hand.cards[player[1].hand.numCards++] = fertile_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = rainbow_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = neigh_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = neigh_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = ginormous_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = neigh_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = puppicorn_tmp;
  currentPlayers = 2;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 4);
  assert(player[1].hand.numCards == 7);
  assert(player[0].stable.size == 0);
  assert(player[1].stable.size == 0);

  PlayCard(0);

  // player[0] lost 1 neigh and the card they originally tried to play
  // player[1] lost 2 neighs
  if (player[0].hand.numCards != 2 || player[1].hand.numCards != 5) {
    num_fails++;
    Red();
    fprintf(stderr, "    odd test: hand size failed\n");
    ResetCol();
  }

  if (strcmp(player[0].hand.cards[0].name, basic_tmp2.name) != 0 ||
      strcmp(player[0].hand.cards[1].name, basic_tmp3.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    odd test: player[0] hand verification failed\n");
    ResetCol();
  }

  // this technically doesn't fully check for dupes because the neigh's
  // could have swapped, but it is good enough for now
  if (strcmp(player[1].hand.cards[0].name, fertile_tmp.name) != 0 ||
      strcmp(player[1].hand.cards[1].name, rainbow_tmp.name) != 0 ||
      strcmp(player[1].hand.cards[2].name, neigh_tmp.name) != 0 ||
      strcmp(player[1].hand.cards[3].name, ginormous_tmp.name) != 0 ||
      strcmp(player[1].hand.cards[4].name, puppicorn_tmp.name) != 0) {
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

  if (discardpile.size != 4 ||
      strcmp(discardpile.cards[0].name, neigh_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, neigh_tmp.name) != 0 ||
      strcmp(discardpile.cards[2].name, neigh_tmp.name) != 0 ||
      strcmp(discardpile.cards[3].name, basic_tmp.name) != 0) {
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
int neigh_even_check(void) {
  int num_fails = 0;
  struct Unicorn neigh_tmp = Base_DECK[115];
  struct Unicorn rainbow_tmp = Base_DECK[42];
  struct Unicorn fertile_tmp = Base_DECK[38];
  struct Unicorn ginormous_tmp = Base_DECK[48];
  struct Unicorn puppicorn_tmp = Base_DECK[41];

  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn basic_tmp3 = Base_DECK[20];

  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = neigh_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp2;
  player[0].hand.cards[player[0].hand.numCards++] = neigh_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp3;

  player[1].hand.cards[player[1].hand.numCards++] = fertile_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = rainbow_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = neigh_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = neigh_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = ginormous_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = neigh_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = puppicorn_tmp;
  currentPlayers = 2;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 5);
  assert(player[1].hand.numCards == 7);
  assert(player[0].stable.size == 0);
  assert(player[1].stable.size == 0);

  PlayCard(0);

  // player[0] lost 2 neighs and brought the original card to their stable
  // player[1] lost 2 neighs
  if (player[0].hand.numCards != 2 || player[1].hand.numCards != 5) {
    num_fails++;
    Red();
    fprintf(stderr, "    even test: hand size failed\n");
    ResetCol();
  }

  if (strcmp(player[0].hand.cards[0].name, basic_tmp2.name) != 0 ||
      strcmp(player[0].hand.cards[1].name, basic_tmp3.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    even test: player[0] hand verification failed\n");
    ResetCol();
  }

  // this technically doesn't fully check for dupes because the neigh's
  // could have swapped, but it is good enough for now
  if (strcmp(player[1].hand.cards[0].name, fertile_tmp.name) != 0 ||
      strcmp(player[1].hand.cards[1].name, rainbow_tmp.name) != 0 ||
      strcmp(player[1].hand.cards[2].name, ginormous_tmp.name) != 0 ||
      strcmp(player[1].hand.cards[3].name, neigh_tmp.name) != 0 ||
      strcmp(player[1].hand.cards[4].name, puppicorn_tmp.name) != 0) {
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

  if (discardpile.size != 4 ||
      strcmp(discardpile.cards[0].name, neigh_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, neigh_tmp.name) != 0 ||
      strcmp(discardpile.cards[2].name, neigh_tmp.name) != 0 ||
      strcmp(discardpile.cards[3].name, neigh_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    even test: discard verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Neigh
//
// Play this card when another player tries to play a card.
// Stop their card from being played and send it to the discard pile.
int neigh_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Neigh tests...\n");
  FILE *fp;
  if (!isClient) {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/neigh.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // already tested with YAY, SLOWDOWN, and ginormous unicorn
    num_fails += neigh_odd_check();
    num_fails += neigh_even_check();
  }
  else {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/neighclient.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // neigh odd test; inputs are 3 and 5
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // neigh even test; inputs are 3, 3, and 0
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }

  fclose(fp);
  return num_fails;
}
