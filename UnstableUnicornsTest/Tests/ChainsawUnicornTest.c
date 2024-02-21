#include "MagicalUnicornTests.h"

// upgrade check
int chainsaw_upgrade_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn chainsaw_tmp = Base_DECK[45];

  currentPlayers = 2;
  AddStable(0, chainsaw_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, yay_tmp);

  int ncorn0 = player[0].stable.numUnicorns;
  int nsize0 = player[0].stable.size;
  int ncorn1 = player[1].stable.numUnicorns;
  int nsize1 = player[1].stable.size;

  Base_EnterStableEffects(0, player[0].stable.unicorns[0].effect);

  if (player[0].stable.numUnicorns != ncorn0 || player[0].stable.size != nsize0 ||
      player[1].stable.numUnicorns != ncorn1 || player[1].stable.size != (nsize1 - 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    upgrade test: stable size failed\n");
    ResetCol();
  }

  if (strcmp(discardpile.cards[0].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    upgrade test: card validation failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// downgrade check
int chainsaw_downgrade_check(void) {
  int num_fails = 0;
  struct Unicorn barbed_wire_tmp = Base_DECK[106];
  struct Unicorn chainsaw_tmp = Base_DECK[45];

  AddStable(0, chainsaw_tmp);
  AddStable(0, barbed_wire_tmp);

  int ncorn0 = player[0].stable.numUnicorns;
  int nsize0 = player[0].stable.size;

  Base_EnterStableEffects(0, player[0].stable.unicorns[0].effect);

  if (player[0].stable.numUnicorns != ncorn0 || player[0].stable.size != (nsize0 - 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    downgrade test: stable size failed\n");
    ResetCol();
  }

  if (strcmp(discardpile.cards[0].name, barbed_wire_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    downgrade test: card validation failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// empty check
int chainsaw_empty_check(void) {
  int num_fails = 0;
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn barbed_wire_tmp = Base_DECK[106];
  struct Unicorn chainsaw_tmp = Base_DECK[45];

  AddStable(0, chainsaw_tmp);
  AddStable(0, yay_tmp);
  AddStable(1, barbed_wire_tmp);

  int ncorn0 = player[0].stable.numUnicorns;
  int nsize0 = player[0].stable.size;
  int ncorn1 = player[1].stable.numUnicorns;
  int nsize1 = player[1].stable.size;

  // shouldn't require any input, but this keeps it from getting borked
  fpinput = stdin;

  Base_EnterStableEffects(0, player[0].stable.unicorns[0].effect);

  if (player[0].stable.numUnicorns != ncorn0 || player[0].stable.size != nsize0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: player[0] stable size failed\n");
    ResetCol();
  }

  if (player[1].stable.numUnicorns != ncorn1 || player[1].stable.size != nsize1) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: player[1] stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: discard pile size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
  return num_fails;
}

// Chainsaw Unicorn
//
// When this card enters your Stable, you may DESTROY an Upgrade card
// or SACRIFICE a Downgrade card.
int chainsaw_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Chainsaw Unicorn tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/chainsawunicorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += chainsaw_upgrade_check();
    num_fails += chainsaw_downgrade_check();
    num_fails += chainsaw_empty_check();

    fclose(fp);
  }
  return num_fails;
}
