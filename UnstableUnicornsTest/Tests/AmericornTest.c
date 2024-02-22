#include "MagicalUnicornTests.h"

// basic check
int americorn_basic_check(void) {
  int num_fails = 0;
  struct Unicorn americorn_tmp = Base_DECK[47];

  currentPlayers = 2;
  Draw(0, 1); 

  int nhand0 = player[0].hand.numCards;
  int nhand1 = player[1].hand.numCards;
  struct Unicorn tmp_card = player[0].hand.cards[0];

  AddStable(1, americorn_tmp);

  if (player[0].hand.numCards != (nhand0 - 1) ||
      player[1].hand.numCards != (nhand1 + 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: num cards failed\n");
    ResetCol();
  }

  // since the deck is unshuffled and being picked from the back, tmp_card
  // should be unique (Super Neigh)
  assert(tmp_card.effect == SUPER_NEIGH);
  if (strcmp(player[1].hand.cards[0].name, tmp_card.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: unicorn verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  return num_fails;
}

// empty check
int americorn_empty_check(void) {
  int num_fails = 0;
  struct Unicorn americorn_tmp = Base_DECK[47];

  currentPlayers = 2;

  int nhand0 = player[0].hand.numCards;
  int nhand1 = player[1].hand.numCards;

  AddStable(0, americorn_tmp);

  if (player[0].hand.numCards != nhand0 ||
      player[1].hand.numCards != nhand1) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty test: num cards failed\n");
    ResetCol();
  }


  reset_players();
  reset_deck();
  return num_fails;
}

// check if the player can choose the card to pull when the other person
// is under the nanny cam effect (all cards in hand are visible at all times)
int americorn_nanny_cam_check(void) {
  int num_fails = 0;
  struct Unicorn americorn_tmp = Base_DECK[47];

  currentPlayers = 2;
  struct Unicorn corn = deck.cards[deck.size - 1];
  Draw(1, 7); // the first card drawn is "Super Neigh" since it's the last card when unshuffled
  Base_ToggleFlags(1, NANNY_CAM_EFFECT);

  assert(player[0].hand.numCards == 0);
  assert((player[1].flags & NANNY_CAM) == NANNY_CAM);
  AddStable(0, americorn_tmp);

  // a random pick would take the 6th card w/ seed(0), so
  // an incorrect result should actually be different from corn.name/Super Neigh
  if (strcmp(player[0].hand.cards[0].name, corn.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    nanny cam test: hand pick failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  return num_fails;
}

// Americorn
//
// When this card enters your Stable, you may pull a card at random
// from another player's hand.
int americorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Americorn tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/americorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += americorn_basic_check();
    num_fails += americorn_empty_check();
    num_fails += americorn_nanny_cam_check();

    fclose(fp);
  }
  return num_fails;
}
