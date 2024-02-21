#include <networkevents.h>
#include "MagicalUnicornTests.h"

// sanity check
int shark_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn shark_tmp = Base_DECK[60];

  currentPlayers = 2;
  AddStable(0, shark_tmp);
  AddStable(1, basic_tmp);

  assert(discardpile.size == 0);
  assert(player[0].stable.numUnicorns == 1);
  assert(player[1].stable.numUnicorns == 1);
  Base_EnterStableEffects(0, shark_tmp.effect);

  if (player[0].stable.size != 0 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 0 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, shark_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: sacrifice failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no available unicorns to destroy
int shark_empty_check(void) {
  int num_fails = 0;
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn shark_tmp = Base_DECK[60];

  currentPlayers = 2;
  AddStable(0, shark_tmp);
  AddStable(1, yay_tmp);

  assert(discardpile.size == 0);
  assert(player[0].stable.numUnicorns == 1);
  assert(player[1].stable.size == 1);
  Base_EnterStableEffects(0, shark_tmp.effect);

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 1 ||
      player[1].stable.size != 1 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: discard size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Shark With a Horn
//
// When this card enters your Stable, you may SACRIFICE this card,
// then DESTROY a Unicorn card.
int shark_with_a_horn_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Shark With a Horn tests...\n");
  if (!isClient) {
    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/sharkwithahorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += shark_basic_check();
    num_fails += shark_empty_check();

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
