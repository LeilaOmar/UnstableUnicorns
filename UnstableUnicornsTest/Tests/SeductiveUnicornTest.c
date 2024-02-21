#include "MagicalUnicornTests.h"

// stealing a regular card
int seductive_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn seductive_tmp = Base_DECK[49];

  currentPlayers = 2;
  AddStable(0, seductive_tmp);
  Draw(0, 1);
  AddStable(1, basic_tmp);

  int nhand0 = player[0].hand.numCards;
  int nsize0 = player[0].stable.size;
  int ncorn0 = player[0].stable.numUnicorns;

  int nsize1 = player[1].stable.size;
  int ncorn1 = player[1].stable.numUnicorns;

  Base_EnterStableEffects(0, player[0].stable.unicorns[0].effect);

  assert(player[0].stable.numUnicorns == player[0].stable.size);
  assert(player[1].stable.numUnicorns == player[1].stable.size);

  if (player[0].stable.numUnicorns != (ncorn0 + 1) ||
      player[0].stable.size != (nsize0 + 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[0] stable size failed\n");
    ResetCol();
  }

  if (player[1].stable.numUnicorns != (ncorn1 - 1) ||
      player[1].stable.size != (nsize1 - 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[1] stable size failed\n");
    ResetCol();
  }

  assert(discardpile.size == 1);
  if (player[0].hand.numCards != (nhand0 - 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[0] hand size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// stealing a magical unicorn w/ an "entering your stable" effect
// this should probably fall under the steal tests...
int seductive_special_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn rainbow_tmp = Base_DECK[42]; // brings basic unicorn from hand to stable
  struct Unicorn seductive_tmp = Base_DECK[49];

  currentPlayers = 2;
  AddStable(0, seductive_tmp);
  AddStable(1, rainbow_tmp);

  Draw(0, 1);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  int nhand0 = player[0].hand.numCards;
  int nsize0 = player[0].stable.size;
  int ncorn0 = player[0].stable.numUnicorns;

  Base_EnterStableEffects(0, player[0].stable.unicorns[0].effect);

  assert(player[0].stable.numUnicorns == player[0].stable.size);

  if (player[0].stable.numUnicorns != (ncorn0 + 2) ||
      player[0].stable.size != (nsize0 + 2)) {
    num_fails++;
    Red();
    fprintf(stderr, "    special test: rainbow unicorn effect failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// unable to steal anything because the other player has non-unicorns
int seductive_empty_check(void) {
  int num_fails = 0;
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn barbed_wire_tmp = Base_DECK[106];
  struct Unicorn seductive_tmp = Base_DECK[49];

  currentPlayers = 2;
  Draw(0, 1);
  AddStable(0, seductive_tmp);
  AddStable(1, barbed_wire_tmp);
  AddStable(1, yay_tmp);

  int nhand0 = player[0].hand.numCards;
  int nsize0 = player[0].stable.size;
  int ncorn0 = player[0].stable.numUnicorns;

  Base_EnterStableEffects(0, player[0].stable.unicorns[0].effect);

  assert(player[0].stable.numUnicorns == player[0].stable.size);
  assert(discardpile.size == 0);

  if (player[0].stable.numUnicorns != ncorn0 ||
      player[0].stable.size != nsize0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: stable size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// Seductive Unicorn
//
// When this card enters your Stable, you may DISCARD a card,
// then STEAL a Unicorn card.
int seductive_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Seductive Unicorn tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/seductiveunicorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += seductive_basic_check();
    num_fails += seductive_special_check();
    num_fails += seductive_empty_check();

    fclose(fp);
  }
  return num_fails;
}
