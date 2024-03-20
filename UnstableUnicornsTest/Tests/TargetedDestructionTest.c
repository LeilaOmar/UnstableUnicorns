#include "MagicTests.h"

// destroying an upgrade card
int targeted_destruction_upgrade_check(void) {
  int num_fails = 0;
  struct Unicorn targeted_tmp = Base_DECK[86];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn yay_tmp = Base_DECK[100];

  currentPlayers = 3;
  AddStable(1, yay_tmp);
  AddStable(2, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = targeted_tmp;

  int ret;
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[1].stable.size == 1);
  assert(player[1].stable.numUnicorns == 0);
  assert(player[2].stable.size == 1);
  assert(player[1].flags == YAY);
  ret = Base_ConditionalEffects(0, targeted_tmp, 0, 0);
  if (ret) Base_MagicEffects(0, targeted_tmp.effect);

  if (moveCount != 1 || ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    upgrade test: turn count failed\n");
    ResetCol();
  }

  if (player[1].stable.size != 0 || player[1].stable.numUnicorns != 0 ||
      player[2].stable.size != 1 || player[2].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    upgrade test: stable size failed\n");
    ResetCol();
  }

  // targeted destruction isn't included because it's only testing magicEffects
  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    upgrade test: discard verification failed\n");
    ResetCol();
  }

  // flags should be disabled
  if (player[1].flags != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: toggle flags failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// sacrificing a downgrade card
int targeted_destruction_downgrade_check(void) {
  int num_fails = 0;
  struct Unicorn targeted_tmp = Base_DECK[86];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn barbed_tmp = Base_DECK[106];

  currentPlayers = 3;
  AddStable(0, barbed_tmp);
  AddStable(2, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = targeted_tmp;

  int ret;
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].stable.size == 1);
  assert(player[0].stable.numUnicorns == 0);
  assert(player[2].stable.size == 1);
  assert(player[0].flags == BARBED_WIRE);
  ret = Base_ConditionalEffects(0, targeted_tmp, 0, 0);
  if (ret) Base_MagicEffects(0, targeted_tmp.effect);

  if (moveCount != 1 || ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    downgrade test: turn count failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 0 || player[0].stable.numUnicorns != 0 ||
      player[2].stable.size != 1 || player[2].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    downgrade test: stable size failed\n");
    ResetCol();
  }

  // targeted destruction isn't included because it's only testing magicEffects
  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, barbed_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    downgrade test: discard verification failed\n");
    ResetCol();
  }

  // flags should be disabled
  if (player[0].flags != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    downgrade test: toggle flags failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}


// no upgrade/downgrade cards to destroy/sacrifice respectively
int targeted_destruction_empty_check(void) {
  int num_fails = 0;
  struct Unicorn targeted_tmp = Base_DECK[86];
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn yay_tmp = Base_DECK[100];

  currentPlayers = 2;
  AddStable(0, yay_tmp);
  AddStable(1, barbed_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = targeted_tmp;

  int ret;
  assert(player[0].stable.size == 1);
  assert(player[1].stable.size == 1);
  ret = Base_ConditionalEffects(0, targeted_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Targeted Destruction
//
// DESTROY an Upgrade card or SACRIFICE a Downgrade card.
int targeted_destruction_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Targeted Destruction tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/targeteddestruction.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += targeted_destruction_upgrade_check();
    num_fails += targeted_destruction_downgrade_check();
    num_fails += targeted_destruction_empty_check();

    fclose(fp);
  }
  return num_fails;
}
