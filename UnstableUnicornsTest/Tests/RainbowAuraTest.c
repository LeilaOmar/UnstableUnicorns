#include <networkevents.h>
#include "UpgradeTests.h"

// sanity test
int rainbow_aura_basic_check(void) {
  int num_fails = 0;
  struct Unicorn rainaura_tmp = Base_DECK[103];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn poison_tmp = Base_DECK[67];

  currentPlayers = 2;
  player[1].hand.cards[player[1].hand.numCards++] = poison_tmp;
  AddStable(0, rainaura_tmp);
  AddStable(0, basic_tmp);

  // try destroying it with unicorn poison
  int ret;
  assert(player[0].stable.size == 2);
  assert(player[0].flags == RAINBOW_AURA);
  ret = Base_ConditionalEffects(1, poison_tmp, 0, 1);

  if (turnCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  // check CanBeDestroyed; card referenced is player[0][1]
  if (CanBeDestroyed(0, 1, ANY, FALSE)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: CanBeDestroyed failed\n");
    ResetCol();
  }


  reset_players();
  reset_discard();
  return num_fails;
}

int rainbow_aura_non_unicorn_check(void) {
  int num_fails = 0;
  struct Unicorn rainaura_tmp = Base_DECK[103];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn bomb_tmp = Base_DECK[98];

  currentPlayers = 2;
  AddStable(0, rainaura_tmp);
  AddStable(0, yay_tmp);
  AddStable(1, bomb_tmp);
  assert(player[0].stable.size == 2);
  assert((player[0].flags & RAINBOW_AURA) == RAINBOW_AURA);

  // first just check CanBeDestroyed; card referenced is player[0][1]
  if (!CanBeDestroyed(0, 1, ANY, FALSE)) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: CanBeDestroyed failed\n");
    ResetCol();
  }

  // try destroying it with glitter bomb
  assert(discardpile.size == 0);
  assert(player[1].stable.size == 1);
  Base_BeginningTurnEffects(1, bomb_tmp);

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 0 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, bomb_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

int rainbow_aura_pandamonium_check(void) {
  int num_fails = 0;
  struct Unicorn rainaura_tmp = Base_DECK[103];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn bomb_tmp = Base_DECK[98];
  struct Unicorn panda_tmp = Base_DECK[107];

  currentPlayers = 2;
  AddStable(1, rainaura_tmp);
  AddStable(1, panda_tmp);
  AddStable(1, basic_tmp);
  AddStable(0, bomb_tmp);
  assert(player[1].stable.size == 3);
  assert((player[1].flags & RAINBOW_AURA) == RAINBOW_AURA);
  assert((player[1].flags & PANDAMONIUM) == PANDAMONIUM);

  // first just check CanBeDestroyed; card referenced is player[0][1]
  if (!CanBeDestroyed(1, 2, ANY, FALSE)) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: CanBeDestroyed failed\n");
    ResetCol();
  }

  // try destroying it with glitter bomb
  assert(discardpile.size == 0);
  assert(player[0].stable.size == 1);
  Base_BeginningTurnEffects(0, bomb_tmp);


  if (player[0].stable.size != 0 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 2 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, bomb_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Rainbow Aura
//
// Your Unicorn cards cannot be destroyed.
int rainbow_aura_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Rainbow Aura tests...\n");
  if (!isClient) {
    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/rainbowaura.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += rainbow_aura_basic_check();
    num_fails += rainbow_aura_non_unicorn_check();
    num_fails += rainbow_aura_pandamonium_check();

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
