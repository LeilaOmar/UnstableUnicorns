#include <networkevents.h>
#include "MagicTests.h"

// sanity check
int poison_basic_check(void) {
  int num_fails = 0;
  struct Unicorn poison_tmp = Base_DECK[67];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn basic_tmp = Base_DECK[13];

  currentPlayers = 2;
  AddStable(1, yay_tmp);
  AddStable(1, basic_tmp);
  Base_ToggleFlags(0, YAY_EFFECT);
  player[0].hand.cards[player[0].hand.numCards++] = poison_tmp;

  assert(turnCount == 1);
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].flags == YAY);
  PlayCard(0);

  if (turnCount != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }

  if (player[1].stable.size != 1 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, poison_tmp.name) != 0 ||
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

// destroying a card w/ a special effect upon getting destroyed
int poison_special_check(void) {
  int num_fails = 0;
  struct Unicorn poison_tmp = Base_DECK[67];
  struct Unicorn stabby_tmp = Base_DECK[40];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn ginormous_tmp = Base_DECK[48];

  // stabby the unicorn test
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(1, stabby_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = poison_tmp;

  assert(discardpile.size == 0);
  Base_MagicEffects(0, poison_tmp.effect);

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, stabby_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    special test: stabby unicorn effect failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // ginormous unicorn test (toggle flag)
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(1, ginormous_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = poison_tmp;

  assert((player[1].flags & GINORMOUS_UNICORN) == GINORMOUS_UNICORN);
  Base_MagicEffects(0, poison_tmp.effect);

  if ((player[1].flags & GINORMOUS_UNICORN) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    special test: toggle ginormous unicorn effect failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no unicorn cards to destroy
int poison_empty_check(void) {
  int num_fails = 0;
  struct Unicorn poison_tmp = Base_DECK[67];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn barbed_tmp = Base_DECK[106];

  currentPlayers = 2;
  AddStable(1, yay_tmp);
  AddStable(1, barbed_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = poison_tmp;

  int ret;
  assert(turnCount == 1);
  assert(player[1].stable.numUnicorns == 0);
  ret = Base_ConditionalEffects(0, poison_tmp, 0, 0);

  if (turnCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Unicorn Poison
//
// DESTROY a Unicorn card.
int unicorn_poison_tests(void) {
  int num_fails = 0;

  FILE *fp;
  rainbow_error("\nStarting Unicorn Poison tests...\n");

  if (!isClient) {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/unicornpoison.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += poison_basic_check();
    num_fails += poison_special_check();
    num_fails += poison_empty_check();
  }
  else {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/unicornpoisonclient.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // basic check, no input
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // stabby the unicorn; input = y 1 1
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // ginormous unicorn, no input
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }

  fclose(fp);
  return num_fails;
}
