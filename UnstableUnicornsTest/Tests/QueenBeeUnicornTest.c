#include "MagicalUnicornTests.h"

// sanity test
int queen_bee_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn queen_tmp = Base_DECK[51];

  currentPlayers = 3;
  AddStable(1, queen_tmp);

  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[2].hand.cards[player[2].hand.numCards++] = basic_tmp;

  int nsize0 = player[0].stable.size;
  int ncorn0 = player[0].stable.numUnicorns;
  int nsize2 = player[2].stable.size;
  int ncorn2 = player[2].stable.numUnicorns;

  Base_EnterStableEffects(1, player[1].stable.unicorns[0].effect);

  assert(turnCount == 1);
  player[0].flags |= YAY;
  player[2].flags |= YAY;

  PlayCard(0);
  PlayCard(2);

  // it should go up twice because both player[0] and player[2] were unable to play basic unicorns
  if (turnCount != 3) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].stable.numUnicorns != ncorn0 || player[0].stable.size != nsize0 ||
      player[2].stable.numUnicorns != ncorn2 || player[2].stable.size != nsize2) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size test failed\n");
    ResetCol();
  }

  if ((player[0].flags & QUEEN_BEE_UNICORN) != QUEEN_BEE_UNICORN ||
      (player[2].flags & QUEEN_BEE_UNICORN) != QUEEN_BEE_UNICORN) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: flag check test failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// Queen Bee Unicorn
//
// Basic Unicorn cards cannot enter any player's Stable except yours.
int queen_bee_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Queen Bee Unicorn tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/queenbeeunicorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += queen_bee_basic_check();

    fclose(fp);
  }
  return num_fails;
}
