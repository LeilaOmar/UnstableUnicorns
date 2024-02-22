#include "MagicalUnicornTests.h"

// sanity check
int fertile_basic_check(void) {
  int num_fails = 0;
  struct Unicorn fertile_tmp = Base_DECK[38];

  AddStable(0, fertile_tmp);
  Draw(0, 1);
  int tmp_hand_size = player[0].hand.numCards;
  int tmp_nursery_size = nursery.size;

  Base_BeginningTurnEffects(0, player[0].stable.unicorns[0]);

  if (nursery.size != (tmp_nursery_size - 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: nursery size failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != (tmp_hand_size - 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: num cards failed\n");
    ResetCol();
  }

  // the magic number 12 is part of the file i/o; it represents the baby unicorn id,
  // but in the file it's actually 1 number higher because of the way input is retrieved
  if (strcmp(player[0].stable.unicorns[1].name, Base_DECK[12].name) != 0 ||
      player[0].stable.numUnicorns != 2 || player[0].stable.size != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_nursery();
  return num_fails;
}

// empty hand check
int fertile_empty_hand() {
  int num_fails = 0;
  struct Unicorn fertile_tmp = Base_DECK[38];

  AddStable(0, fertile_tmp);
  int tmp_size = nursery.size;
  player[0].hand.numCards = 0;

  Base_BeginningTurnEffects(0, player[0].stable.unicorns[0]);

  if (nursery.size != tmp_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty hand test: nursery size failed\n");
    ResetCol();
  }

  // the player should stay the same because there were no babies to get
  if (player[0].stable.numUnicorns != 1 || player[0].stable.size != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty hand test: stable size failed\n");
    ResetCol();
  }

  reset_players();
  reset_nursery();
  return num_fails;
}

// empty nursery check
int fertile_empty_nursery() {
  int num_fails = 0;
  struct Unicorn fertile_tmp = Base_DECK[38];

  nursery.size = 0;
  int tmp_size = 0;

  AddStable(0, fertile_tmp);

  Base_BeginningTurnEffects(0, player[0].stable.unicorns[0]);

  if (nursery.size != tmp_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty nursery test: nursery size failed\n");
    ResetCol();
  }

  // the player should stay the same because there were no babies to get
  if (player[0].stable.numUnicorns != 1 || player[0].stable.size != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty nursery test: stable size failed\n");
    ResetCol();
  }

  reset_players();
  reset_nursery();
  return num_fails;
}

// Extremely Fertile Unicorn
//
// If this card is in your Stable at the beginning of your turn, you may DISCARD a card.
// If you do, bring a Baby Unicorn card from the Nursery directly into your Stable.
int extremely_fertile_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Extremely Fertile Unicorn tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/extremelyfertileunicorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += fertile_basic_check();
    num_fails += fertile_empty_hand();
    num_fails += fertile_empty_nursery();

    fclose(fp);
  }
  return num_fails;
}
