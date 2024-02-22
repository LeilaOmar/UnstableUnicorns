#include "UpgradeTests.h"

// sanity test
int summoning_basic_check(void) {
  int num_fails = 0;
  struct Unicorn summon_tmp = Base_DECK[105];
  struct Unicorn rainbow_tmp = Base_DECK[42];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17]; // basic unicorn
  struct Unicorn ginormous_tmp = Base_DECK[48]; // magical unicorn

  AddStable(0, summon_tmp);
  AddDiscard(rainbow_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp2;
  player[0].hand.cards[player[0].hand.numCards++] = ginormous_tmp;

  assert(discardpile.size == 1);
  assert(player[0].hand.numCards == 3);
  assert(player[0].stable.size == 1);
  Base_BeginningTurnEffects(0, summon_tmp);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 3 || player[0].stable.numUnicorns != 2 ||
      strcmp(player[0].stable.unicorns[0].name, summon_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[1].name, rainbow_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[2].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 2 ||
      strcmp(discardpile.cards[0].name, basic_tmp2.name) != 0 ||
      strcmp(discardpile.cards[1].name, ginormous_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard pile verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no cards in discard pile; this should still work because the cards you discard
// are eventually included
int summoning_empty_discard_check(void) {
  int num_fails = 0;
  struct Unicorn summon_tmp = Base_DECK[105];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn ginormous_tmp = Base_DECK[48]; // magical unicorn

  AddStable(0, summon_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = ginormous_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 2);
  assert(player[0].stable.size == 1);
  assert(player[0].flags == 0);
  Base_BeginningTurnEffects(0, summon_tmp);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty discard test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1 ||
      strcmp(player[0].stable.unicorns[0].name, summon_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[1].name, ginormous_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty discard test: stable verification failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty discard test: discard pile verification failed\n");
    ResetCol();
  }

  if (player[0].flags != GINORMOUS_UNICORN) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty discard test: toggle flags failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// not enough unicorn cards in hand
int summoning_empty_hand_check(void) {
  int num_fails = 0;
  struct Unicorn summon_tmp = Base_DECK[105];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn yay_tmp = Base_DECK[100];

  AddStable(0, summon_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp;
  player[0].hand.cards[player[0].hand.numCards++] = yay_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 2);
  assert(player[0].stable.size == 1);
  Base_BeginningTurnEffects(0, summon_tmp);

  if (player[0].hand.numCards != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty hand test: hand size failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty hand test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty hand test: discard pile size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Summoning Ritual
//
// If this card is in your Stable at the beginning of your turn, you may DISCARD 2 Unicorn cards.
// If you do, bring a Unicorn card directly from the discard pile into your Stable.
// (note) you can choose to directly bring one of the two discarded cards into your stable
int summoning_ritual_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Summoning Ritual tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/summoningritual.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += summoning_basic_check();
    num_fails += summoning_empty_discard_check();
    num_fails += summoning_empty_hand_check();

    fclose(fp);
  }
  return num_fails;
}
