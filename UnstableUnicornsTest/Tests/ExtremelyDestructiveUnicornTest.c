#include <networkevents.h>
#include "MagicalUnicornTests.h"

// sanity check
int destructive_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn destructive_tmp = Base_DECK[44];

  currentPlayers = 2;
  AddStable(1, basic_tmp);

  assert(player[0].stable.size == 0);
  assert(player[1].stable.size == 1);
  assert(player[0].stable.numUnicorns == 0);
  assert(player[1].stable.numUnicorns == 1);
  AddStable(0, destructive_tmp);

  if (player[0].stable.numUnicorns != 0 || player[0].stable.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[0] stable size failed\n");
    ResetCol();
  }

  if (player[1].stable.numUnicorns != 0 || player[1].stable.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[1] stable size failed\n");
    ResetCol();
  }


  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, destructive_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, basic_tmp.name) != 0) {
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

// there's no conditional to play this card, so technically you can play it
// even when other people don't have any unicorns in their stable
int destructive_non_unicorn_check(void) {
  int num_fails = 0;
  struct Unicorn barbed_wire_tmp = Base_DECK[106];
  struct Unicorn destructive_tmp = Base_DECK[44];

  currentPlayers = 2;
  AddStable(1, barbed_wire_tmp);

  assert(player[0].stable.size == 0);
  assert(player[1].stable.size == 1);
  assert(player[0].stable.numUnicorns == 0);
  assert(player[1].stable.numUnicorns == 0);
  AddStable(0, destructive_tmp);

  if (player[0].stable.numUnicorns != 0 || player[0].stable.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: player[0] stable size failed\n");
    ResetCol();
  }

  if (player[1].stable.numUnicorns != 0 || player[1].stable.size != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: player[1] stable size failed\n");
    ResetCol();
  }


  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, destructive_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// pandas are not unicorns
int destructive_pandamonium_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn destructive_tmp = Base_DECK[44];
  struct Unicorn panda_tmp = Base_DECK[107];

  currentPlayers = 2;
  AddStable(0, panda_tmp);
  AddStable(1, basic_tmp);

  assert(player[0].stable.size == 1);
  assert(player[1].stable.size == 1);
  assert(player[0].flags == PANDAMONIUM);
  AddStable(0, destructive_tmp);

  if (player[0].stable.numUnicorns != 1 || player[0].stable.size != 2 ||
      strcmp(player[0].stable.unicorns[0].name, panda_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[1].name, destructive_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: player[0] stable size failed\n");
    ResetCol();
  }

  if (player[1].stable.numUnicorns != 0 || player[1].stable.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: player[1] stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Extremely Destructive Unicorn
//
// When this card enters your Stable, each player (including you)
// must SACRIFICE a Unicorn card.
int extremely_destructive_unicorn_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Extremely Destructive Unicorn tests...\n");

  // file input stream setup
  FILE *fp;
  fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
  if (fp == NULL) {
    Magenta();
    fprintf(stderr, "    file input failed :(");
    ResetCol();
    return 1;
  }
  fpinput = fp;

  if (!isClient) {
    num_fails += destructive_basic_check();
    num_fails += destructive_non_unicorn_check();
    num_fails += destructive_pandamonium_check();
  }
  else {
    int events;

    // sanity check; input = 1 for the card at index 0
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // non-unicorn; auto sends -1
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // PANDAMONIUM; input = 1 for the card at index 0
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }

  fclose(fp);
  return num_fails;
}
