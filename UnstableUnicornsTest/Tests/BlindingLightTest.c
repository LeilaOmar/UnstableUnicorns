#include "DowngradeTests.h"

// checking for beginning of turn effects
int blinding_beginning_turn_check(void) {
  int num_fails = 0;
  struct Unicorn blinding_tmp = Base_DECK[112];
  struct Unicorn fertile_tmp = Base_DECK[38];
  struct Unicorn dutch_tmp = Base_DECK[104];

  // test w/ unicorn card
  Draw(0, 1);
  int tmp_size = nursery.size;
  AddStable(0, blinding_tmp);
  AddStable(0, fertile_tmp);

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].flags == BLINDING_LIGHT);

  // can't discard to bring baby unicorns into stable
  Base_BeginningTurnEffects(0, fertile_tmp);

  if (player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    beginning turn (unicorn) test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    beginning turn (unicorn) test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    beginning turn (unicorn) test: discard size failed\n");
    ResetCol();
  }

  if (nursery.size != tmp_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    beginning turn (unicorn) test: nursery size failed\n");
    ResetCol();
  }
  reset_players();
  reset_deck();
  reset_nursery();
  reset_discard();

  // test w/ non-unicorn card
  AddStable(0, dutch_tmp);
  AddStable(0, blinding_tmp);
  assert(player[0].flags == BLINDING_LIGHT);

  Base_BeginningTurnEffects(0, dutch_tmp);

  if (moveCount != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    beginning turn (non-unicorn) test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// checking for enter stable effects
int blinding_enter_stable_check(void) {
  int num_fails = 0;
  struct Unicorn blinding_tmp = Base_DECK[112];
  struct Unicorn classy_tmp = Base_DECK[65];
  struct Unicorn tail_tmp = Base_DECK[95];

  // test w/ unicorn card
  Draw(0, 1);
  AddStable(0, blinding_tmp);

  int tmp_size = deck.size;
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].flags == BLINDING_LIGHT);

  // can't pick an upgrade card from the deck
  Base_EnterStableEffects(0, classy_tmp.effect);

  if (player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    enter stable (unicorn) test: hand size failed\n");
    ResetCol();
  }

  if (deck.size != tmp_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    enter stable (unicorn) test: deck size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();

  // test w/ non-unicorn card; tail still can't enter
  AddStable(0, blinding_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = tail_tmp;

  int ret;
  assert(player[0].stable.size == 1);
  assert(player[0].flags == BLINDING_LIGHT);
  ret = Base_ConditionalEffects(0, tail_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    enter stable (non-unicorn) test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// checking for sacrifice & destroy effects
int blinding_sacrifice_destroy_check(void) {
  int num_fails = 0;
  struct Unicorn blinding_tmp = Base_DECK[112];
  struct Unicorn majestic_tmp = Base_DECK[56];

  AddStable(0, majestic_tmp);
  AddStable(0, blinding_tmp);

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 0);
  assert(player[0].flags == BLINDING_LIGHT);

  // card doesn't return to hand
  // player[0][0] == majestic_tmp
  Base_SacrificeDestroyEffects(0, 0, majestic_tmp.effect);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sacrifice test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
    strcmp(discardpile.cards[0].name, majestic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sacrifice test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// blinding light and PANDAMONIUM
//	- entering stable (no effect)
//	- beginning of next turn (effect should work)
//	- sacrificing/destroying (effect should work)
int blinding_pandamonium_check(void) {
  int num_fails = 0;
  struct Unicorn blinding_tmp = Base_DECK[112];
  struct Unicorn panda_tmp = Base_DECK[107];

  // entering the stable
  struct Unicorn llama_tmp = Base_DECK[46];

  AddStable(0, blinding_tmp);
  AddStable(0, panda_tmp);

  Draw(0, 1);
  int tmp_size_deck = deck.size;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert((player[0].flags & BLINDING_LIGHT) == BLINDING_LIGHT);
  assert((player[0].flags & PANDAMONIUM) == PANDAMONIUM);

  // can't discard cards
  Base_EnterStableEffects(0, llama_tmp.effect);

  if (player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM enter stable test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    pandamoniums enter stable test: discard size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();

  // beginning of turn
  struct Unicorn fertile_tmp = Base_DECK[38];

  Draw(0, 1);
  int tmp_size = nursery.size;
  AddStable(0, blinding_tmp);
  AddStable(0, fertile_tmp);
  AddStable(0, panda_tmp);

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].stable.size == 3);
  assert((player[0].flags & BLINDING_LIGHT) == BLINDING_LIGHT);
  assert((player[0].flags & PANDAMONIUM) == PANDAMONIUM);

  // should be able to discard to bring baby unicorns into stable
  Base_BeginningTurnEffects(0, fertile_tmp);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM beginning turn test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 4 || player[0].stable.numUnicorns != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM beginning turn test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM beginning turn test: discard size failed\n");
    ResetCol();
  }

  if (nursery.size != (tmp_size - 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM beginning turn test: nursery size failed\n");
    ResetCol();
  }
  reset_players();
  reset_deck();
  reset_nursery();
  reset_discard();

  // card does return to hand because they're actually pandas
  struct Unicorn majestic_tmp = Base_DECK[56];

  AddStable(0, majestic_tmp);
  AddStable(0, blinding_tmp);
  AddStable(0, panda_tmp);

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 0);
  assert((player[0].flags & BLINDING_LIGHT) == BLINDING_LIGHT);
  assert((player[0].flags & PANDAMONIUM) == PANDAMONIUM);

  // player[0][0] == majestic_tmp
  Base_SacrificeDestroyEffects(0, 0, majestic_tmp.effect);

  if (player[0].hand.numCards != 1 ||
      strcmp(player[0].hand.cards[0].name, majestic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM sacrifice test: hand verification failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM sacrifice test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM sacrifice test: discard pile size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// check for the win condition w/ ginormous unicorn
int blinding_ginormous_check(void) {
  int num_fails = 0;
  struct Unicorn blinding_tmp = Base_DECK[112];
  struct Unicorn ginormous_tmp = Base_DECK[48];
  struct Unicorn basic_tmp = Base_DECK[13];

  AddStable(0, blinding_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);

  assert((player[0].flags & BLINDING_LIGHT) == BLINDING_LIGHT);

  AddStable(0, ginormous_tmp);

  assert(discardpile.size == 0);
  assert(player[0].stable.numUnicorns == 6);
  assert((player[0].flags & GINORMOUS_UNICORN) == GINORMOUS_UNICORN);

  // player shouldn't win; i.e. EndOfTurn returns 0 instead of 1
  int ret = EndOfTurn(0);

  if (ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    ginormous unicorn test: (pre-flag) win check failed\n");
    ResetCol();
  }

  // taking away blinding light should allow the check to pass now

  // player[0][0] == blinding_tmp
  RearrangeStable(0, 0);
  ret = EndOfTurn(0);

  if (ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    ginormous unicorn test: (post-flag) win check failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Blinding Light
//
// All of your Unicorn cards are considered Basic Unicorns with no effects.
int blinding_light_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Blinding Light tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/blindinglight.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += blinding_beginning_turn_check();
    num_fails += blinding_enter_stable_check();
    num_fails += blinding_sacrifice_destroy_check();
    num_fails += blinding_pandamonium_check();
    num_fails += blinding_ginormous_check();

    fclose(fp);
  }
  return num_fails;
}
