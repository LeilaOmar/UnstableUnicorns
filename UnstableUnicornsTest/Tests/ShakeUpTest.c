#include "MagicTests.h"

// sanity test
int shake_up_basic_check(void) {
  int num_fails = 0;
  struct Unicorn shake_tmp = Base_DECK[89];
  struct Unicorn basic_tmp = Base_DECK[13];

  player[0].hand.cards[player[0].hand.numCards++] = shake_tmp;
  Draw(0, 3);
  deck.size -= 2;
  AddDiscard(basic_tmp);

  int tmp_size = deck.size;
  assert(discardpile.size == 1);
  assert(player[0].hand.numCards == 4);
  Base_MagicEffects(0, shake_tmp.effect);

  if (player[0].hand.numCards != 5) {
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

  // deck.size = tmp_size + 1 (prev discardpile.size) + 1 (shake up) + 3 (rest of player[0]'s hand) - 5 (drawn cards)
  // deck.size = tmp_size + 5 - 5 = tmp_size
  if (deck.size != tmp_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: deck size failed\n");
    ResetCol();
  }
  reset_players();
  reset_deck();
  reset_discard();

  // with shake up as the only card in your hand
  player[0].hand.cards[player[0].hand.numCards++] = shake_tmp;
  deck.size -= 2;
  AddDiscard(basic_tmp);

  tmp_size = deck.size;
  assert(discardpile.size == 1);
  assert(player[0].hand.numCards == 1);
  Base_MagicEffects(0, shake_tmp.effect);

  if (player[0].hand.numCards != 5) {
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

  // deck.size = tmp_size + 1 (prev discardpile.size) + 1 (shake up) + 0 (rest of player[0]'s hand) - 5 (drawn cards)
  // deck.size = tmp_size + 2 - 5 = tmp_size - 3
  if (deck.size != (tmp_size - 3)) {
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

// Shake Up
//
// Shuffle this card, your hand, and the discard pile into the deck. DRAW 5 cards.
int shake_up_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Shake Up tests...\n");

    num_fails += shake_up_basic_check();
  }
  return num_fails;
}
