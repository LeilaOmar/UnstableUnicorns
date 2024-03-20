#include "MagicTests.h"

// sanity test
int blatant_basic_check(void) {
  int num_fails = 0;
  struct Unicorn blatant_tmp = Base_DECK[90];
  struct Unicorn basic_tmp = Base_DECK[13];

  currentPlayers = 3;
  Base_ToggleFlags(0, YAY_EFFECT);
  player[0].hand.cards[player[0].hand.numCards++] = blatant_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = basic_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[1].hand.numCards == 1);
  assert(player[2].hand.numCards == 0);
  assert(player[0].flags == YAY);
  PlayCard(0);

  if (moveCount != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 1 ||
      strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, blatant_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no cards to steal from hands
int blatant_empty_check(void) {
  int num_fails = 0;
  struct Unicorn blatant_tmp = Base_DECK[90];

  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = blatant_tmp;

  int ret;
  assert(player[0].hand.numCards == 1);
  assert(player[1].hand.numCards == 0);
  ret = Base_ConditionalEffects(0, blatant_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Blatant Thievery
//
// Look at another player's hand. Choose a card and add it to your hand.
int blatant_thievery_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Blatant Thievery tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/blatantthievery.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += blatant_basic_check();
    num_fails += blatant_empty_check();

    fclose(fp);
  }
  return num_fails;
}
