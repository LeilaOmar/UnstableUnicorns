#include "MagicTests.h"

// sanity test
int reset_button_basic_check(void) {
  int num_fails = 0;
  struct Unicorn reset_tmp = Base_DECK[91];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn rainaura_tmp = Base_DECK[103];
  struct Unicorn dutch_tmp = Base_DECK[104];
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn panda_tmp = Base_DECK[107];
  struct Unicorn basic_tmp = Base_DECK[13];

  currentPlayers = 3;
  AddStable(0, yay_tmp);
  AddStable(0, barbed_tmp);

  AddStable(1, basic_tmp);

  AddStable(2, rainaura_tmp);
  AddStable(2, dutch_tmp);
  AddStable(2, panda_tmp);
  AddStable(2, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = reset_tmp;

  deck.size -= 7; // to accomodate for the added stable cards and reset button
  int tmp_size = deck.size;

  int ret;
  assert(discardpile.size == 0);
  assert(player[0].stable.size == 2);
  assert(player[1].stable.size == 1);
  assert(player[2].stable.size == 4);
  assert(player[0].hand.numCards == 1);

  assert((player[0].flags & YAY) == YAY);
  assert((player[0].flags & BARBED_WIRE) == BARBED_WIRE);
  assert((player[2].flags & RAINBOW_AURA) == RAINBOW_AURA);
  assert((player[2].flags & PANDAMONIUM) == PANDAMONIUM);
  ret = Base_ConditionalEffects(0, reset_tmp, 0, 0);
  if (ret) Base_MagicEffects(0, reset_tmp.effect);

  if (moveCount != 1 || ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 0 ||
      player[1].stable.size != 1 || player[1].stable.numUnicorns != 1 ||
      player[2].stable.size != 1 || player[2].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  // discard pile gets shuffled into the deck
  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard size failed\n");
    ResetCol();
  }

  // 1 reset button + 2 downgrade cards + 3 upgrade cards = 6 (in-game)
  // this actually isn't including reset button though, so it should check for 5
  if (deck.size != (tmp_size + 5)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: deck size failed\n");
    ResetCol();
  }

  // flags should be gone
  if (player[0].flags != 0 ||
      player[2].flags != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: toggle flags failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// no downgrade cards in player[0]'s stable
int reset_button_empty_upgrade_check(void) {
  int num_fails = 0;
  struct Unicorn reset_tmp = Base_DECK[91];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn basic_tmp = Base_DECK[13];

  // no upgrade
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(0, barbed_tmp);
  AddStable(1, yay_tmp);
  AddStable(1, barbed_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = reset_tmp;

  deck.size -= 5; // to accomodate for the added stable cards and reset button
  int tmp_size = deck.size;

  int ret;
  assert(player[0].stable.size == 2);
  assert(player[1].stable.size == 2);
  ret = Base_ConditionalEffects(0, reset_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty upgrade test (player[0]): turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// no downgrade cards in player[0]'s stable
int reset_button_empty_downgrade_check(void) {
  int num_fails = 0;
  struct Unicorn reset_tmp = Base_DECK[91];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn basic_tmp = Base_DECK[13];

  // no upgrade
  currentPlayers = 3;
  AddStable(0, basic_tmp);
  AddStable(0, yay_tmp);
  AddStable(1, yay_tmp);
  AddStable(1, barbed_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = reset_tmp;

  int ret;
  assert(player[0].stable.size == 2);
  assert(player[0].stable.numUnicorns == 1);
  assert(player[1].stable.size == 2);
  assert(player[1].stable.numUnicorns == 0);
  ret = Base_ConditionalEffects(0, reset_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty downgrade test (player[0]): turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// no upgrade or downgrade cards in other players' stables
int reset_button_empty_other_check(void) {
  int num_fails = 0;
  struct Unicorn reset_tmp = Base_DECK[91];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn basic_tmp = Base_DECK[13];

  // no upgrade
  currentPlayers = 2;
  AddStable(0, yay_tmp);
  AddStable(0, barbed_tmp);
  AddStable(1, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = reset_tmp;

  deck.size -= 4; // to accomodate for the added stable cards and reset button
  int tmp_size = deck.size;

  int ret;
  assert(player[0].stable.size == 2);
  assert(player[1].stable.size == 1);
  ret = Base_ConditionalEffects(0, reset_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty stable test (player[1]): turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// Reset Button
//
// Each player (including you) must SACRIFICE all Upgrade and Downgrade cards
// in their Stable. Shuffle the discard pile into the deck.
int reset_button_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Reset Button tests...\n");

    num_fails += reset_button_basic_check();
    num_fails += reset_button_empty_upgrade_check();
    num_fails += reset_button_empty_downgrade_check();
    num_fails += reset_button_empty_other_check();
  }
  return num_fails;
}
