#include "MagicalUnicornTests.h"

// sanity check
int alluring_basic_check(void) {
  int num_fails = 0;
  struct Unicorn alluring_tmp = Base_DECK[63];
  struct Unicorn yay_tmp = Base_DECK[100];

  currentPlayers = 2;
  AddStable(0, yay_tmp);

  assert(player[0].stable.size == 1);
  assert(player[1].stable.numUnicorns == 0);
  assert(player[1].stable.size == 0);
  AddStable(1, alluring_tmp);

  if (player[0].stable.size != 0 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 2 || player[1].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  // sacrifices in this case are in reverse order
  if (strcmp(player[1].stable.unicorns[1].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: unicorn verification failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// no available upgrade cards to steal
int alluring_empty_check(void) {
  int num_fails = 0;
  struct Unicorn alluring_tmp = Base_DECK[63];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn basic_tmp = Base_DECK[13];

  currentPlayers = 2;
  AddStable(0, yay_tmp);
  AddStable(1, basic_tmp);

  assert(player[0].stable.numUnicorns == 0);
  assert(player[0].stable.size == 1);
  assert(player[1].stable.size == 1);
  AddStable(0, alluring_tmp);

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1 ||
      player[1].stable.size != 1 || player[1].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-upgrade test: stable size failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Alluring Narwhal
//
// When this card enters your Stable, you may STEAL an Upgrade card.
int alluring_narwhal_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Alluring Narwhal tests...\n");

    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += alluring_basic_check();
    num_fails += alluring_empty_check();

    fclose(fp);
  }
  return num_fails;
}
