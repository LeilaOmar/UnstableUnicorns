#include "DowngradeTests.h"

// tests for when unicorns enter the stable
int barbed_enter_check(void) {
  int num_fails = 0;
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn ginormous_tmp = Base_DECK[48];

  AddStable(0, barbed_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].stable.size == 1);
  assert(player[0].flags == BARBED_WIRE);

  AddStable(0, ginormous_tmp);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    enter test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1 ||
      strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[1].name, ginormous_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    enter test: stable verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    enter test: discard pile verification failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // make sure it doesn't break if there are no cards to discard (a bit redundant given discard tests)
  AddStable(0, barbed_tmp);

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 0);
  assert(player[0].stable.size == 1);
  assert(player[0].flags == BARBED_WIRE);

  AddStable(0, ginormous_tmp);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    enter test (empty hand): hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1 ||
      strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[1].name, ginormous_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    enter test (empty hand): stable verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    enter test (empty hand): discard pile size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// tests for when unicorns leave the stable
int barbed_exit_check(void) {
  int num_fails = 0;
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn ginormous_tmp = Base_DECK[48];

  AddStable(0, barbed_tmp);
  AddStable(0, ginormous_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].stable.size == 2);
  assert((player[0].flags & BARBED_WIRE) == BARBED_WIRE);

  // player[0][1] == ginourmous_tmp
  RearrangeStable(0, 1);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    exit test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 0 ||
      strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    exit test: stable verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    exit test: discard pile verification failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // make sure it doesn't break if there are no cards to discard (a bit redundant given discard tests)
  AddStable(0, barbed_tmp);
  AddStable(0, ginormous_tmp);

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 0);
  assert(player[0].stable.size == 2);
  assert((player[0].flags & BARBED_WIRE) == BARBED_WIRE);

  // player[0][1] == ginourmous_tmp
  RearrangeStable(0, 1);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    exit test (empty hand): hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 0 ||
      strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    exit test (empty hand): stable verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    exit test (empty hand): discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// compatibility w/ PANDAMONIUM;
// enter - should discard because they are entering as unicorn CARDS; did not "cross the gate" yet so to speak
// exit - should not discard because they are pandas, not unicorns
int barbed_pandamonium_check(void) {
  int num_fails = 0;
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn ginormous_tmp = Base_DECK[48];
  struct Unicorn panda_tmp = Base_DECK[107];

  // entering the stable
  AddStable(0, barbed_tmp);
  AddStable(0, panda_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].stable.size == 2);
  assert((player[0].flags & BARBED_WIRE) == BARBED_WIRE);
  assert((player[0].flags & PANDAMONIUM) == PANDAMONIUM);

  AddStable(0, ginormous_tmp);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM enter test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 3 || player[0].stable.numUnicorns != 1 ||
      strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[1].name, panda_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[2].name, ginormous_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM enter test: stable verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM enter test: discard pile verification failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // leaving the stable
  AddStable(0, barbed_tmp);
  AddStable(0, panda_tmp);
  AddStable(0, ginormous_tmp);

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 0);
  assert(player[0].stable.size == 3);
  assert((player[0].flags & BARBED_WIRE) == BARBED_WIRE);
  assert((player[0].flags & PANDAMONIUM) == PANDAMONIUM);

  // player[0][2] == ginourmous_tmp
  RearrangeStable(0, 2);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM exit test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 0 ||
      strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[1].name, panda_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM exit test: stable verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM exit test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Barbed Wire
//
// Each time a Unicorn card enters or leaves your Stable, DISCARD a card.
int barbed_wire_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Barbed Wire tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/barbedwire.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // already tested it w/ unicorn shrinkray
    num_fails += barbed_enter_check();
    num_fails += barbed_exit_check();
    num_fails += barbed_pandamonium_check();

    fclose(fp);
  }
  return num_fails;
}
