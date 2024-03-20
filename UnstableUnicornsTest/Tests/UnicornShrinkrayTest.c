#include "MagicTests.h"

// sanity check
int shrinkray_basic_check(void) {
  int num_fails = 0;
  struct Unicorn shrinkray_tmp = Base_DECK[85];
  struct Unicorn basic_tmp = Base_DECK[13]; // [emojis]
  struct Unicorn tail_tmp = Base_DECK[95];  // non-unicorn card
  struct Unicorn ginormous_tmp = Base_DECK[48]; // special effect 1
  struct Unicorn queen_tmp = Base_DECK[51]; // special effect 2

  // no cards to sacrifice
  currentPlayers = 3;
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, ginormous_tmp);
  AddStable(1, tail_tmp);
  AddStable(1, queen_tmp);
  Base_EnterStableEffects(1, ginormous_tmp.effect);
  Base_EnterStableEffects(1, queen_tmp.effect);

  nursery.size = 4;
  int tmp_size = nursery.size;
  player[0].hand.cards[player[0].hand.numCards++] = shrinkray_tmp;

  int ret;
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[1].stable.size == 5);
  assert(player[1].stable.numUnicorns == 4);
  assert(player[2].stable.size == 0);

  assert(player[0].flags == QUEEN_BEE_UNICORN);
  assert((player[1].flags & GINORMOUS_UNICORN) == GINORMOUS_UNICORN);
  assert(player[2].flags == QUEEN_BEE_UNICORN);
  ret = Base_ConditionalEffects(0, shrinkray_tmp, 0, 0);
  if (ret) Base_MagicEffects(0, shrinkray_tmp.effect);

  if (moveCount != 1 || ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[1].stable.size != 5 || player[1].stable.numUnicorns != 4 ||
      player[1].stable.unicorns[0].cType != BABYUNICORN ||
      strcmp(player[1].stable.unicorns[3].name, tail_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable verification failed\n");
    ResetCol();
  }

  // discardpile.size = player[1].stable.numUnicorns
  // normally it would include the shrinkray card, but magicEffects is simpler
  if (discardpile.size != 4 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, basic_tmp.name) != 0 ||
      strcmp(discardpile.cards[2].name, ginormous_tmp.name) != 0 ||
      strcmp(discardpile.cards[3].name, queen_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard verification failed\n");
    ResetCol();
  }

  // nursery.size = tmp_size - player[1].stable.numUnicorns
  if (nursery.size != (tmp_size - 4)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: nursery size failed\n");
    ResetCol();
  }

  // flags should be disabled
  if (player[0].flags != 0 ||
      player[1].flags != 0 ||
      player[2].flags != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: toggle flags failed\n");
    ResetCol();
  }

  reset_players();
  reset_nursery();
  reset_discard();
  return num_fails;
}

// not enough baby unicorns to swap
int shrinkray_empty_check(void) {
  int num_fails = 0;
  struct Unicorn shrinkray_tmp = Base_DECK[85];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn yay_tmp = Base_DECK[100];

  // no cards to sacrifice
  currentPlayers = 2;
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, yay_tmp);

  nursery.size = 2;
  int tmp_size = nursery.size;
  player[0].hand.cards[player[0].hand.numCards++] = shrinkray_tmp;

  int ret;
  assert(player[1].stable.size == 4);
  assert(player[1].stable.numUnicorns == 3);
  ret = Base_ConditionalEffects(0, shrinkray_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty nursery test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_nursery();
  reset_discard();
  return num_fails;
}

// checks if the puppicorn index gets adjusted
int shrinkray_puppicorn_check(void) {
  int num_fails = 0;
  struct Unicorn shrinkray_tmp = Base_DECK[85];
  struct Unicorn puppicorn_tmp = Base_DECK[41];
  struct Unicorn basic_tmp = Base_DECK[13];

  currentPlayers = 2;
  int tmp_size = nursery.size;
  AddStable(1, puppicorn_tmp);
  AddStable(1, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = shrinkray_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].stable.size == 0);
  assert(player[1].stable.size == 2);
  assert(player[1].stable.numUnicorns == 2);
  Base_MagicEffects(0, shrinkray_tmp.effect);

  if (player[1].stable.size != 2 || player[1].stable.numUnicorns != 2 ||
      player[1].stable.unicorns[0].cType != BABYUNICORN ||
      player[1].stable.unicorns[1].cType != BABYUNICORN) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: stable verification failed\n");
    ResetCol();
  }

  if (puppicornIndex[0] != -1) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: puppicorn index failed\n");
    ResetCol();
  }

  // does it move?
  EndOfTurn(1);

  if (player[0].stable.size != 0 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 2 || player[1].stable.numUnicorns != 2 ||
      player[1].stable.unicorns[0].cType != BABYUNICORN ||
      player[1].stable.unicorns[1].cType != BABYUNICORN) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: end of turn stable verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_nursery();
  reset_discard();
  return num_fails;
}

// essentially discard ALL of the hand
int shrinkray_barbed_wire_check(void) {
  int num_fails = 0;
  struct Unicorn shrinkray_tmp = Base_DECK[85];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn barbed_tmp = Base_DECK[106];

  // no cards to sacrifice
  currentPlayers = 2;
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, barbed_tmp);

  Draw(1, 6);
  player[1].hand.cards[player[1].hand.numCards++] = yay_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = shrinkray_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[1].stable.size == 7);
  assert(player[1].stable.numUnicorns == 6);
  assert(player[1].hand.numCards == 7);
  Base_MagicEffects(0, shrinkray_tmp.effect);

  if (player[1].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire test: hand size failed\n");
    ResetCol();
  }

  // discardpile.size = 1 (unicorn shrinkray) + player[1].stable.numUnicorns + min((player[1].stable.numUnicorns * 2), player[1].hand.numCards)
  // discardpile.size = 1 + 6 + min(12, 7) = 1 + 6 + 7 = 14
  //
  // since this is excluding unicorn shrinkray, it should check for 13 cards
  if (discardpile.size != 13) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire test: discard size failed\n");
    ResetCol();
  }
  reset_players();
  reset_deck();
  reset_nursery();
  reset_discard();

  // test w/ cards still in hand by the end
  currentPlayers = 2;
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);

  Draw(1, 4);
  player[1].hand.cards[player[1].hand.numCards++] = yay_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = shrinkray_tmp;
  Base_ToggleFlags(1, barbed_tmp.effect);

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[1].stable.size == 2);
  assert(player[1].stable.numUnicorns == 2);
  assert(player[1].hand.numCards == 5);
  Base_MagicEffects(0, shrinkray_tmp.effect);

  if (player[1].hand.numCards != 1 ||
      strcmp(player[1].hand.cards[0].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire test 2: hand verification failed\n");
    ResetCol();
  }

  // discardpile.size = 1 (unicorn shrinkray) + player[1].stable.numUnicorns + min((player[1].stable.numUnicorns * 2), player[1].hand.numCards)
  // discardpile.size = 1 + 2 + min(4, 5) = 1 + 2 + 4 = 7
  //
  // as explained above, just subtract 1 due to excluding unicorn shrinkray
  if (discardpile.size != 6) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire test 2: discard size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_nursery();
  reset_discard();
  return num_fails;
}

// compatibility w/ PANDAMONIUM
int shrinkray_pandamonium_check(void) {
  int num_fails = 0;
  struct Unicorn shrinkray_tmp = Base_DECK[85];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn panda_tmp = Base_DECK[107];

  currentPlayers = 2;
  AddStable(1, panda_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);

  int tmp_size = nursery.size;
  player[0].hand.cards[player[0].hand.numCards++] = shrinkray_tmp;

  int ret;
  assert(player[1].stable.size == 4);
  assert(player[1].stable.numUnicorns == 3);
  assert(player[1].flags == PANDAMONIUM);
  ret = Base_ConditionalEffects(0, shrinkray_tmp, 0, 0);

  if (moveCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_nursery();
  reset_discard();
  return num_fails;
}

// Unicorn Shrinkray
//
// Choose any player. Move all of that player's Unicorn cards to the discard pile
// without triggering any of their effects, then bring the same number of Baby Unicorn
// cards from the Nursery directly into that player's Stable.
int unicorn_shrinkray_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Unicorn Shrinkray tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/unicornshrinkray.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += shrinkray_basic_check();
    num_fails += shrinkray_empty_check();
    num_fails += shrinkray_puppicorn_check();
    num_fails += shrinkray_barbed_wire_check();
    num_fails += shrinkray_pandamonium_check();

    fclose(fp);
  }
  return num_fails;
}
