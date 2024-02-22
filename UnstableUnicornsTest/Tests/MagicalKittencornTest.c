#include <networkevents.h>
#include "MagicalUnicornTests.h"

// sanity check
int kittencorn_basic_check(void) {
  int num_fails = 0;
  struct Unicorn kitten_tmp = Base_DECK[39];

  currentPlayers = 2;
  AddStable(1, kitten_tmp);

  // destroyed by a magic card; should not work
  if (CanBeDestroyed(1, 0, ANY, TRUE)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: CanBeDestroyed failed\n");
    ResetCol();
  }
  reset_players();

  // destroyed by a non-magic card
  currentPlayers = 2;
  AddStable(1, kitten_tmp);

  Destroy(0, ANY, FALSE);

  if (player[1].stable.size != 0 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: destruction by non-magic card failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Magical Kittencorn
//
// This card cannot be destroyed by Magic cards.
int magical_kittencorn_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Magical Kittencorn tests...\n");
  if (!isClient) {

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/line_2_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += kittencorn_basic_check();

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
