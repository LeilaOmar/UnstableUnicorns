#include <networkevents.h>
#include "DowngradeTests.h"

// sanity test
int tiny_stable_basic_check(void) {
  int num_fails = 0;
  struct Unicorn tiny_tmp = Base_DECK[113];
  struct Unicorn fertile_tmp = Base_DECK[38];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn dutch_tmp = Base_DECK[104];

  // adding a unicorn card
  AddStable(0, fertile_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, tiny_tmp);

  assert(discardpile.size == 0);
  assert(player[0].stable.size == 6);
  assert(player[0].stable.numUnicorns == 5);
  assert(player[0].flags == TINY_STABLE);

  AddStable(0, basic_tmp);

  if (player[0].stable.size != 6 || player[0].stable.numUnicorns != 5) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (unicorn): hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, fertile_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (unicorn): discard size failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();
  
  // adding a non-unicorn card
  AddStable(0, fertile_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, tiny_tmp);

  assert(discardpile.size == 0);
  assert(player[0].stable.size == 6);
  assert(player[0].stable.numUnicorns == 5);
  assert(player[0].flags == TINY_STABLE);

  AddStable(0, dutch_tmp);

  if (player[0].stable.size != 7 || player[0].stable.numUnicorns != 5) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (non-unicorn): hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (non-unicorn): discard size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// tiny stable should trigger when puppicorn enters your stable, and
// once it leaves by the end of your turn, you should have 4 unicorns left
int tiny_stable_puppicorn_check(void) {
  int num_fails = 0;
  struct Unicorn tiny_tmp = Base_DECK[113];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn puppicorn_tmp = Base_DECK[41];

  currentPlayers = 2;
  AddStable(0, basic_tmp2);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, tiny_tmp);

  assert(discardpile.size == 0);
  assert(player[0].stable.size == 6);
  assert(player[0].stable.numUnicorns == 5);
  assert(player[0].flags == TINY_STABLE);

  AddStable(0, puppicorn_tmp);
  EndOfTurn(0);

  if (player[0].stable.size != 5 || player[0].stable.numUnicorns != 4) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp2.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: discard size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// do not have to sacrifice cards because they're pandas
int tiny_stable_pandamonium_check(void) {
  int num_fails = 0;
  struct Unicorn tiny_tmp = Base_DECK[113];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn panda_tmp = Base_DECK[107];

  // adding a unicorn card
  AddStable(0, tiny_tmp);
  AddStable(0, panda_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);

  assert(discardpile.size == 0);
  assert(player[0].stable.size == 7);
  assert(player[0].stable.numUnicorns == 5);
  assert((player[0].flags & TINY_STABLE) == TINY_STABLE);
  assert((player[0].flags & PANDAMONIUM) == PANDAMONIUM);

  AddStable(0, basic_tmp);

  if (player[0].stable.size != 8 || player[0].stable.numUnicorns != 6) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (unicorn): hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (unicorn): discard size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Tiny Stable
//
// If at any time you have more than 5 Unicorns in your Stable, SACRIFICE a Unicorn card.
int tiny_stable_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Tiny Stable tests...\n");
  if (!isClient) {
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

    num_fails += tiny_stable_basic_check();
    num_fails += tiny_stable_puppicorn_check();
    num_fails += tiny_stable_pandamonium_check();

    fclose(fp);
  }
  else {
    // puppicorn check, no input
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }
  return num_fails;
}
