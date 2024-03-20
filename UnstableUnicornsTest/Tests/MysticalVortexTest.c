#include <networkevents.h>
#include "MagicTests.h"

// sanity test
int mystical_vortex_basic_check(void) {
  int num_fails = 0;
  struct Unicorn vortex_tmp = Base_DECK[87];

  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = vortex_tmp;
  deck.size--; // to compensate for the vortex draw
  Draw(0, 2);
  Draw(1, 2);
  Base_ToggleFlags(0, YAY_EFFECT);
  int tmp_size = deck.size;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 3);
  assert(player[1].hand.numCards == 2);
  assert(player[0].flags == YAY);
  PlayCard(0);

  if (moveCount != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 1 ||
      player[1].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard size failed\n");
    ResetCol();
  }

  // deck.size = tmp_size + 1 (mystical vortex) + 1 (player[0]'s discarded card) + 1 (player[1]'s discarded card)
  if (deck.size != (tmp_size + 3)) {
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

// no cards to discard; the current player must have another card to discard
// in order to play this card
int mystical_vortex_empty_check(void) {
  int num_fails = 0;
  struct Unicorn vortex_tmp = Base_DECK[87];

  currentPlayers = 2;
  Draw(1, 1);
  int tmp_size = deck.size;
  player[0].hand.cards[player[0].hand.numCards++] = vortex_tmp;

  int ret;
  assert(player[0].hand.numCards == 1);
  ret = Base_ConditionalEffects(0, vortex_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// Mystical Vortex
//
// Each player (including you) must DISCARD a card. Shuffle the discard pile into the deck.
int mystical_vortex_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Mystical Vortex tests...\n");

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

  if (!isClient) {
    num_fails += mystical_vortex_basic_check();
    num_fails += mystical_vortex_empty_check();
  }
  else {
    // input = 1; card index is actually 0
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }

  fclose(fp);
  return num_fails;
}
