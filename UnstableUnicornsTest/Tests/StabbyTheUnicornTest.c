#include <networkevents.h>
#include "MagicalUnicornTests.h"

// sanity check
int stabby_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13]; // unicorn card
  struct Unicorn barbed_wire_tmp = Base_DECK[106]; // non-unicorn card
  struct Unicorn stabby_tmp = Base_DECK[40];

  // destroy worked
  currentPlayers = 2;
  AddStable(1, basic_tmp);
  Base_SacrificeDestroyEffects(0, 0, stabby_tmp.effect);

  if (player[1].stable.numUnicorns != 0 || player[1].stable.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: destroy failed\n");
    ResetCol();
  }
  reset_players();

  // no valid cards to destroy
  currentPlayers = 2;
  AddStable(1, barbed_wire_tmp);

  Base_SacrificeDestroyEffects(0, 0, stabby_tmp.effect);
  if (player[1].stable.size != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: destroy failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Stabby the Unicorn
//
// If this card is sacrificed or destroyed, you may DESTROY a Unicorn card
int stabby_the_unicorn_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Stabby the Unicorn tests...\n");
  if (!isClient) {
    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/stabbytheunicorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += stabby_basic_check();

    fclose(fp);
  }
  else {
    // basic check, no input
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }
  return num_fails;
}
