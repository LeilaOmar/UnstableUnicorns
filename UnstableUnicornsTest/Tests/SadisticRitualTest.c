#include "DowngradeTests.h"

// sanity check
int sadistic_basic_check(void) {
  int num_fails = 0;
  struct Unicorn sadistic_tmp = Base_DECK[108];
  struct Unicorn majestic_tmp = Base_DECK[56];

  AddStable(0, majestic_tmp);
  AddStable(0, sadistic_tmp);

  int tmp_size = deck.size;
  struct Unicorn corn = deck.cards[deck.size - 1];
  assert(discardpile.size == 0);
  assert(player[0].stable.size == 2);
  assert(player[0].hand.numCards == 0);
  Base_BeginningTurnEffects(0, sadistic_tmp);
  
  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 0 ||
      strcmp(player[0].stable.unicorns[0].name, sadistic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable verification failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 2 ||
      strcmp(player[0].hand.cards[0].name, majestic_tmp.name) != 0 ||
      strcmp(player[0].hand.cards[1].name, corn.name) != 0 ) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand verification failed\n");
    ResetCol();
  }

  // this should be 0 because majestic flying unicorn's special effect is to
  // return back to the hand when destroyed
  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard pile size failed\n");
    ResetCol();
  }

  if (deck.size != (tmp_size - 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: deck size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// no unicorn cards to sacrifice
int sadistic_empty_check(void) {
  int num_fails = 0;
  struct Unicorn sadistic_tmp = Base_DECK[108];
  struct Unicorn yay_tmp = Base_DECK[100];

  AddStable(0, yay_tmp);
  AddStable(0, sadistic_tmp);

  int tmp_size = deck.size;
  assert(discardpile.size == 0);
  assert(player[0].stable.size == 2);
  assert(player[0].hand.numCards == 0);
  Base_BeginningTurnEffects(0, sadistic_tmp);

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 0 ||
      strcmp(player[0].stable.unicorns[0].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: stable verification failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: discard pile size failed\n");
    ResetCol();
  }

  if (deck.size != tmp_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: deck size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// pandas aren't unicorn cards
int sadistic_pandamonium_check(void) {
  int num_fails = 0;
  struct Unicorn sadistic_tmp = Base_DECK[108];
  struct Unicorn majestic_tmp = Base_DECK[56];
  struct Unicorn panda_tmp = Base_DECK[107];

  AddStable(0, majestic_tmp);
  AddStable(0, sadistic_tmp);
  AddStable(0, panda_tmp);

  int tmp_size = deck.size;
  assert(discardpile.size == 0);
  assert(player[0].stable.size == 3);
  assert(player[0].hand.numCards == 0);
  assert((player[0].flags & PANDAMONIUM) == PANDAMONIUM);
  Base_BeginningTurnEffects(0, sadistic_tmp);

  if (player[0].stable.size != 3 || player[0].stable.numUnicorns != 1 ||
      strcmp(player[0].stable.unicorns[0].name, majestic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    pandmaonium test: stable verification failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: discard pile size failed\n");
    ResetCol();
  }

  if (deck.size != tmp_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: deck size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// puppicorn cannot be sacrificed
int sadistic_puppicorn_check(void) {
  int num_fails = 0;
  struct Unicorn sadistic_tmp = Base_DECK[108];
  struct Unicorn puppicorn_tmp = Base_DECK[41];

  AddStable(0, puppicorn_tmp);
  AddStable(0, sadistic_tmp);

  int tmp_size = deck.size;
  assert(discardpile.size == 0);
  assert(player[0].stable.size == 2);
  assert(player[0].hand.numCards == 0);
  Base_BeginningTurnEffects(0, sadistic_tmp);

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1 ||
    strcmp(player[0].stable.unicorns[0].name, puppicorn_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: stable verification failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: discard pile size failed\n");
    ResetCol();
  }

  if (deck.size != tmp_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: deck size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// just a malicious combo, should be no edge cases involved
int sadistic_barbed_wire_check(void) {
  int num_fails = 0;
  struct Unicorn sadistic_tmp = Base_DECK[108];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn corn = deck.cards[deck.size - 1];

  Draw(0, 5);
  AddStable(0, basic_tmp);
  AddStable(0, barbed_tmp);
  AddStable(0, sadistic_tmp);

  assert(discardpile.size == 0);
  assert(player[0].stable.size == 3);
  assert(player[0].hand.numCards == 5);
  assert((player[0].flags & BARBED_WIRE) == BARBED_WIRE);
  Base_BeginningTurnEffects(0, sadistic_tmp);

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 0 ||
      strcmp(player[0].stable.unicorns[0].name, barbed_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire test: stable verification failed\n");
    ResetCol();
  }

  // +1 from the draw, -1 from barbed wire
  if (player[0].hand.numCards != 5) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, corn.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// Sadistic Ritual
//
// If this card is in your Stable at the beginning of your turn,
// SACRIFICE a Unicorn card, then DRAW a card.
int sadistic_ritual_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Sadistic Ritual tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/sadisticritual.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += sadistic_basic_check();
    num_fails += sadistic_empty_check();
    num_fails += sadistic_pandamonium_check();
    num_fails += sadistic_puppicorn_check();
    num_fails += sadistic_barbed_wire_check();

    fclose(fp);
  }
  return num_fails;
}
