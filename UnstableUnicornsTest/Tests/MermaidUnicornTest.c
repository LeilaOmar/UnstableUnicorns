#include "MagicalUnicornTests.h"

// sanity check; this doubles as a check for toggled flags
int mermaid_basic_check(void) {
  int num_fails = 0;
  struct Unicorn mermaid_tmp = Base_DECK[64];
  struct Unicorn yay_tmp = Base_DECK[100];

  currentPlayers = 2;
  AddStable(1, mermaid_tmp);
  AddStable(0, yay_tmp);

  assert(player[0].stable.size == 1);
  assert(player[0].hand.numCards == 0);
  assert((player[0].flags & YAY) == YAY);

  assert(player[1].stable.size == 1);
  assert(player[1].stable.numUnicorns == 1);
  Base_EnterStableEffects(1, mermaid_tmp.effect);

  if (player[0].stable.size != 0 || player[0].stable.numUnicorns != 0 ||
      player[1].stable.size != 1 || player[1].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "\n    sanity test: stable size failed\n");
    ResetCol();
  }

  if (player[0].hand.numCards != 1 ||
      strcmp(player[0].hand.cards[0].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand return failed\n");
    ResetCol();
  }

  // check to make sure that the YAY flag was disabled for player[0]
  if ((player[0].flags & YAY) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: toggle flag check failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Mermaid Unicorn
//
// When this card enters your Stable, return a card in another player's Stable to their hand.
int mermaid_unicorn_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Mermaid Unicorn tests...\n");

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

    num_fails += mermaid_basic_check();

    fclose(fp);
  }
  return num_fails;
}
