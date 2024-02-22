#include "MagicTests.h"

// sanity check for upgrade cards
int re_target_upgrade_check(void) {
  int num_fails = 0;
  struct Unicorn target_tmp = Base_DECK[79];
  struct Unicorn yay_tmp = Base_DECK[100];

  currentPlayers = 3;
  AddStable(1, yay_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = target_tmp;

  int ret;
  assert(player[1].stable.size == 1);
  assert(player[1].flags == YAY);
  assert(player[2].stable.size == 0);
  ret = Base_ConditionalEffects(0, target_tmp, 0, 0);
  if (ret) Base_MagicEffects(0, target_tmp.effect);

  if (turnCount != 1 || ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    upgrade test: turn count failed\n");
    ResetCol();
  }

  if ((player[2].flags & YAY) != YAY ||
      player[1].flags != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    upgrade test: toggle flags failed\n");
    ResetCol();
  }

  if (player[2].stable.size != 1 || player[2].stable.numUnicorns != 0 ||
      player[1].stable.size != 0 || player[1].stable.numUnicorns != 0 ||
      strcmp(player[2].stable.unicorns[0].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    upgrade test: stable verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// sanity check for downgrade cards
int re_target_downgrade_check(void) {
  int num_fails = 0;
  struct Unicorn target_tmp = Base_DECK[79];
  struct Unicorn barbed_tmp = Base_DECK[106];

  currentPlayers = 2;
  AddStable(1, barbed_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = target_tmp;

  int ret;
  assert(player[0].stable.size == 0);
  assert(player[1].stable.size == 1);
  assert(player[1].flags == BARBED_WIRE);
  ret = Base_ConditionalEffects(0, target_tmp, 0, 0);
  if (ret) Base_MagicEffects(0, target_tmp.effect);

  if (turnCount != 1 || ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    downgrade test: turn count failed\n");
    ResetCol();
  }

  if ((player[0].flags & BARBED_WIRE) != BARBED_WIRE ||
      player[1].flags != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    downgrade test: toggle flags failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 0 || player[1].stable.numUnicorns != 0 ||
      strcmp(player[1].stable.unicorns[0].name, barbed_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    downgrade test: stable verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no upgrades or downgrades at all
int re_target_empty_check(void) {
  int num_fails = 0;
  struct Unicorn target_tmp = Base_DECK[79];
  struct Unicorn basic_tmp = Base_DECK[13];

  // no unicorn card in player[0]'s stable
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(1, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = target_tmp;

  int ret;
  assert(player[0].stable.size == 1);
  assert(player[1].stable.size == 1);
  ret = Base_ConditionalEffects(0, target_tmp, 0, 0);

  if (turnCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}


// Re-Target
//
// Move an Upgrade or Downgrade card from any player's Stable
// to any other player's Stable.
int re_target_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Re-Target tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/retarget.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += re_target_upgrade_check();
    num_fails += re_target_downgrade_check();
    num_fails += re_target_empty_check();

    fclose(fp);
  }
  return num_fails;
}
