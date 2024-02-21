#include "MagicTests.h"

// trading hands where player[0].numCards > player[1].numCards
int bargain_greater_check(void) {
  int num_fails = 0;
  struct Unicorn bargain_tmp = Base_DECK[81];

  currentPlayers = 3;
  player[0].hand.cards[player[0].hand.numCards++] = bargain_tmp;

  struct Unicorn corn = deck.cards[deck.size - 1];
  Draw(0, 4);
  Draw(1, 2);
  Base_ToggleFlags(0, YAY_EFFECT);

  assert(player[0].hand.numCards == 5);
  assert(player[1].hand.numCards == 2);
  assert(player[2].hand.numCards == 0);
  assert(player[0].flags == YAY);
  PlayCard(0);

  if (turnCount != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    greater test: turn count failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 2 ||
      player[1].hand.numCards != 4 ||
      strcmp(player[1].hand.cards[0].name, corn.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    greater test: hand verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, bargain_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    greater test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// trading hands where player[0].numCards < player[1].numCards
int bargain_lesser_check(void) {
  int num_fails = 0;
  struct Unicorn bargain_tmp = Base_DECK[81];

  currentPlayers = 3;
  player[0].hand.cards[player[0].hand.numCards++] = bargain_tmp;

  struct Unicorn corn = deck.cards[deck.size - 1];
  Draw(1, 5);
  Draw(0, 1);
  Base_ToggleFlags(0, YAY_EFFECT);

  assert(player[0].hand.numCards == 2);
  assert(player[1].hand.numCards == 5);
  assert(player[2].hand.numCards == 0);
  assert(player[0].flags == YAY);
  PlayCard(0);

  if (turnCount != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    lesser test: turn count failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 5 ||
      player[1].hand.numCards != 1 ||
      strcmp(player[0].hand.cards[0].name, corn.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    lesser test: hand verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
    strcmp(discardpile.cards[0].name, bargain_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    lesser test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// no cards in player[0]'s or other player's hands
int bargain_empty_check(void) {
  int num_fails = 0;
  struct Unicorn bargain_tmp = Base_DECK[81];
  struct Unicorn basic_tmp = Base_DECK[13];

  // no cards in other player's hands
  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = bargain_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  int ret;
  assert(player[0].hand.numCards == 2);
  assert(player[1].hand.numCards == 0);
  ret = Base_ConditionalEffects(0, bargain_tmp, 0, 0);

  if (turnCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test (player[1]): turn count failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // no cards in player[0]'s hands; should still work, actually :p
  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = bargain_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = basic_tmp;

  Base_ToggleFlags(0, YAY_EFFECT);
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[1].hand.numCards == 1);
  assert(player[0].flags == YAY);
  PlayCard(0);

  if (turnCount != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test (player[0]): turn count failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 1 ||
      player[1].hand.numCards != 0 ||
      strcmp(player[0].hand.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test (player[0]): hand verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, bargain_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test (player[0]): discard size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}


// Unfair Bargain
//
// Trade hands with any other player.
int unfair_bargain_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Unfair Bargain tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/unfairbargain.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += bargain_greater_check();
    num_fails += bargain_lesser_check();
    num_fails += bargain_empty_check();

    fclose(fp);
  }
  return num_fails;
}
