#include "MagicalUnicornTests.h"

// stealing a regular card
int angel_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn angel_tmp = Base_DECK[50];

  AddStable(0, angel_tmp);
  AddDiscard(basic_tmp);

  int nsize0 = player[0].stable.size;
  int ncorn0 = player[0].stable.numUnicorns;

  assert(discardpile.size == 1);

  Base_BeginningTurnEffects(0, player[0].stable.unicorns[0]);

  assert(player[0].stable.numUnicorns == player[0].stable.size);

  if (player[0].stable.numUnicorns != ncorn0 ||
      player[0].stable.size != nsize0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player size failed\n");
    ResetCol();
  }

  // the angel unicorn card is discarded after being sacrificed
  if (discardpile.size != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard pile size failed\n");
    ResetCol();
  }

  // angel and basic basically swapped places, so the basic unicorn should
  // now be in the player's stable, and angel unicorn should be in the discard pile
  if (strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0 ||
      strcmp(discardpile.cards[0].name, angel_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: unicorn verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// taking a magical unicorn w/ an "entering your stable" effect
int angel_special_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn rainbow_tmp = Base_DECK[42]; // brings basic unicorn from hand to stable
  struct Unicorn angel_tmp = Base_DECK[50];

  AddStable(0, angel_tmp);
  AddDiscard(rainbow_tmp);

  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;

  int nhand0 = player[0].hand.numCards;
  int nsize0 = player[0].stable.size;
  int ncorn0 = player[0].stable.numUnicorns;

  Base_BeginningTurnEffects(0, player[0].stable.unicorns[0]);

  assert(player[0].stable.numUnicorns == player[0].stable.size);

  if (player[0].stable.numUnicorns != (ncorn0 + 1) ||
      player[0].stable.size != (nsize0 + 1) ||
      player[0].hand.numCards != (nhand0 - 1)) {
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

// unable to sacrifice because there are no unicorns in the discard pile
int angel_empty_check(void) {
  int num_fails = 0;
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn barbed_wire_tmp = Base_DECK[106];
  struct Unicorn angel_tmp = Base_DECK[50];

  AddStable(0, angel_tmp);
  AddDiscard(barbed_wire_tmp);
  AddDiscard(yay_tmp);

  int nhand0 = player[0].hand.numCards;
  int nsize0 = player[0].stable.size;
  int ncorn0 = player[0].stable.numUnicorns;

  Base_BeginningTurnEffects(0, player[0].stable.unicorns[0]);

  assert(player[0].stable.numUnicorns == player[0].stable.size);

  if (player[0].stable.numUnicorns != ncorn0 ||
      player[0].stable.size != nsize0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: stable size failed\n");
    ResetCol();
  }

  if (strcmp(player[0].stable.unicorns[0].name, angel_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: unicorn verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// Angel Unicorn
//
// If this card is in your Stable at the beginning of your turn, you may SACRIFICE this card.
// If you do, choose a Unicorn card from the discard pile and bring it directly into your Stable.
int angel_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Angel Unicorn tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/angelunicorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += angel_basic_check();
    num_fails += angel_special_check();
    num_fails += angel_empty_check();

    fclose(fp);
  }
  return num_fails;
}
