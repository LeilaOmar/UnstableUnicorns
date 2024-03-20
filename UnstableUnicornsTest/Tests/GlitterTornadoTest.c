#include "MagicTests.h"

// sanity check
int glitter_tornado_basic_check(void) {
  int num_fails = 0;
  struct Unicorn tornado_tmp = Base_DECK[75];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn baby_tmp = Base_DECK[12];

  nursery.size = 0;
  int tmp_size = nursery.size;
  currentPlayers = 3;
  AddStable(0, basic_tmp);
  AddStable(1, basic_tmp2);
  AddStable(2, baby_tmp);
  Base_ToggleFlags(0, YAY_EFFECT);
  player[0].hand.cards[player[0].hand.numCards++] = tornado_tmp;

  assert(player[0].hand.numCards == 1);
  assert(player[1].hand.numCards == 0);
  assert(player[2].hand.numCards == 0);

  assert(player[0].stable.size == 1);
  assert(player[1].stable.size == 1);
  assert(player[2].stable.size == 1);
  assert(player[0].flags == YAY);
  PlayCard(0);

  if (moveCount != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 0 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 0 ||	player[1].stable.numUnicorns != 0 ||
      player[2].stable.size != 0 ||	player[2].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 1 ||
      player[1].hand.numCards != 1 || 
      player[2].hand.numCards != 0 ||
      strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0 ||
      strcmp(player[1].hand.cards[0].name, basic_tmp2.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand verification failed\n");
    ResetCol();
  }

  // the baby narwhal goes back to index 0 because nursery.size was set to 0 earlier
  if (nursery.size != (tmp_size + 1) ||
      strcmp(nursery.cards[0].name, baby_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: nursery verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_nursery();
  reset_discard();
  return num_fails;
}

// check to see if flags get toggled when cards return to hand
int glitter_tornado_special_check(void) {
  int num_fails = 0;
  struct Unicorn tornado_tmp = Base_DECK[75];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn ginormous_tmp = Base_DECK[48];

  currentPlayers = 2;
  AddStable(0, yay_tmp);
  AddStable(1, ginormous_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = tornado_tmp;

  assert((player[0].flags & YAY) == YAY);
  assert((player[1].flags & GINORMOUS_UNICORN) == GINORMOUS_UNICORN);
  assert(player[0].hand.numCards == 1);
  Base_MagicEffects(0, tornado_tmp.effect);

  if ((player[0].flags & YAY) != 0 ||
      (player[1].flags & GINORMOUS_UNICORN) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    special test: toggle flags failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no cards in every player's stable, so glitter tornado cannot be played
int glitter_tornado_empty_check(void) {
  int num_fails = 0;
  struct Unicorn tornado_tmp = Base_DECK[75];
  struct Unicorn basic_tmp = Base_DECK[13];

  currentPlayers = 2;
  AddStable(1, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = tornado_tmp;

  int ret;
  assert(player[0].stable.size == 0);
  ret = Base_ConditionalEffects(0, tornado_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty stable test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// compatibility w/ unicorn lasso & returning a stolen card to their own hand
int glitter_tornado_unicorn_lasso_check(void) {
  int num_fails = 0;
  struct Unicorn tornado_tmp = Base_DECK[75];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn lasso_tmp = Base_DECK[102];

  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = tornado_tmp;

  AddStable(0, lasso_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp2);
  Base_ToggleFlags(0, YAY_EFFECT);

  assert(player[0].hand.numCards == 1);
  assert(player[0].stable.size == 1);
  assert(player[0].stable.numUnicorns == 0);

  assert(player[1].hand.numCards == 0);
  assert(player[1].stable.size == 2);
  assert(player[1].stable.numUnicorns == 2);

  Base_BeginningTurnEffects(0, lasso_tmp);
  assert(uniLassoIndex[0] != -1);
  assert(player[0].flags == YAY);
  PlayCard(0);

  // check if the lasso flag has been reset before the turn is over
  if (uniLassoIndex[0] != -1) {
    num_fails++;
    Red();
    fprintf(stderr, "\n    unicorn lasso test: flag reset failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 1 || player[1].hand.numCards != 1 ||
      strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0 ||
      strcmp(player[1].hand.cards[0].name, basic_tmp2.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: hand verification pre-end of turn failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 0 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: stable size pre-end of turn failed\n");
    ResetCol();
  }

  EndOfTurn(0);

  // check if the stolen card is still in their hand and the stable sizes are the same
  if (player[0].hand.numCards != 1 || player[1].hand.numCards != 1 ||
    strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0 ||
    strcmp(player[1].hand.cards[0].name, basic_tmp2.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: hand verification post-end of turn failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 0 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: stable size post-end of turn failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Glitter Tornado
//
// Return a card in each player's Stable (including yours) to their hand.
int glitter_tornado_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Glitter Tornado tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/glittertornado.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += glitter_tornado_basic_check();
    num_fails += glitter_tornado_special_check();
    num_fails += glitter_tornado_empty_check();
    num_fails += glitter_tornado_unicorn_lasso_check();

    fclose(fp);
  }
  return num_fails;
}
