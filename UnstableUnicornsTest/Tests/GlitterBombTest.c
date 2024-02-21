#include <networkevents.h>
#include "UpgradeTests.h"

// sanity test
int glitter_bomb_basic_check(void) {
  int num_fails = 0;
  struct Unicorn bomb_tmp = Base_DECK[98];
  struct Unicorn basic_tmp = Base_DECK[13];

  currentPlayers = 3;
  AddStable(0, bomb_tmp);
  AddStable(0, basic_tmp);
  AddStable(1, basic_tmp);

  assert(player[0].stable.size == 2);
  assert(player[1].stable.size == 1);
  Base_BeginningTurnEffects(0, bomb_tmp);

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 0 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no cards to destroy; you can always sacrifice glitter bomb itself!
int glitter_bomb_empty_check(void) {
  int num_fails = 0;
  struct Unicorn bomb_tmp = Base_DECK[98];
  struct Unicorn puppicorn_tmp = Base_DECK[41];
  struct Unicorn basic_tmp = Base_DECK[13];

  // no cards to destroy
  currentPlayers = 2;
  AddStable(0, bomb_tmp);
  AddStable(0, basic_tmp);
  AddStable(1, puppicorn_tmp);

  assert(player[0].stable.size == 2);
  assert(player[1].stable.size == 1);
  Base_BeginningTurnEffects(0, bomb_tmp);

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1 ||
      player[1].stable.size != 1 || player[1].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: discard size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Glitter Bomb
//
// If this card is in your Stable at the beginning of your turn,
// you may SACRIFICE a card, then DESTROY a card.
int glitter_bomb_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Glitter Bomb tests...\n");
  if (!isClient) {
    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/glitterbomb.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += glitter_bomb_basic_check();
    num_fails += glitter_bomb_empty_check();

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
