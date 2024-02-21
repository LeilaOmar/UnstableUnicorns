#include <networkevents.h>
#include "MagicalUnicornTests.h"

// sanity check
int annoying_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn annoying_tmp = Base_DECK[53];

  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = basic_tmp2;
  player[1].hand.cards[player[1].hand.numCards++] = basic_tmp;

  assert(player[0].hand.numCards == 1);
  assert(player[1].hand.numCards == 1);
  assert(discardpile.size == 0);
  AddStable(0, annoying_tmp);

  if (player[0].hand.numCards != 1 || player[1].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }
  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard size failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // no players to discard; a proper test would check if stdout is empty
  currentPlayers = 2;
  player[0].hand.numCards = 0;
  player[1].hand.numCards = 0;

  AddStable(0, annoying_tmp);

  if (player[1].hand.numCards != 0 || discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard skip failed\n");
    ResetCol();
  }
  reset_players();

  // check the sacrifice/destroy effects

  player[0].stable.unicorns[0] = annoying_tmp;

  assert(player[0].hand.numCards == 0);
  Base_SacrificeDestroyEffects(0, 0, annoying_tmp.effect);

  if (player[0].hand.numCards != 1 ||
      strcmp(player[0].hand.cards[0].name, annoying_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand return failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Annoying Flying Unicorn
//
// When this card enters your Stable, you may force another player to DISCARD a card.
// If this card is sacrificed or destroyed, return it to your hand.
int annoying_flying_unicorn_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Annoying Flying Unicorn tests...\n");
  FILE *fp;

  if (!isClient) {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/annoyingflyingunicorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += annoying_basic_check();

  }
  else {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/line_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // input = 1; card index is actually 0
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }

  fclose(fp);
  return num_fails;
}
