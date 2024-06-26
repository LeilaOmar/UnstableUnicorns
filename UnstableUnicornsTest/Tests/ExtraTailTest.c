#include "UpgradeTests.h"

// sanity test
int extra_tail_basic_check(void) {
  int num_fails = 0;
  struct Unicorn tail_tmp = Base_DECK[95];
  struct Unicorn basic_tmp = Base_DECK[13];

  AddStable(0, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = tail_tmp;

  int ret;
  assert(player[0].stable.size == 1);
  assert(player[0].hand.numCards == 1);
  ret = Base_ConditionalEffects(0, tail_tmp, 0, 0);
  if (ret) {
    RearrangeHand(0, 0);
    AddStable(0, tail_tmp);
  }

  if (moveCount != 1 || ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: (pre-beginning turn) stable size failed\n");
    ResetCol();
  }

  Base_BeginningTurnEffects(0, tail_tmp);

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: (post-beginning turn) stable size failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: (post-beginning turn) draw/hand size failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// no basic unicorn in your stable
int extra_tail_empty_check(void) {
  int num_fails = 0;
  struct Unicorn tail_tmp = Base_DECK[95];

  player[0].hand.cards[player[0].hand.numCards++] = tail_tmp;

  int ret;
  assert(player[0].stable.size == 0);
  ret = Base_ConditionalEffects(0, tail_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty stable test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Extra Tail
//
// This card can only enter a Stable if there is a Basic Unicorn card in that Stable.
// If this card is in your Stable at the beginning of your turn, you may DRAW an extra card.
int extra_tail_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Extra Tail tests...\n");

    num_fails += extra_tail_basic_check();
    num_fails += extra_tail_empty_check();
  }
  return num_fails;
}
