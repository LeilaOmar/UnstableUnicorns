#include "MagicalUnicornTests.h"

// sanity check
int torpedo_basic_check(void) {
  int num_fails = 0;
  struct Unicorn torpedo_tmp = Base_DECK[62];
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn pandamonium_tmp = Base_DECK[107];
  struct Unicorn sadistic_tmp = Base_DECK[108];

  // this also checks to see if it works when
  // adding downgrade cards in the first and last indices,
  // even though the last should theoretically be impossible
  AddStable(0, barbed_tmp);
  AddStable(0, pandamonium_tmp);
  AddStable(0, sadistic_tmp);

  assert(discardpile.size == 0);
  assert(player[0].stable.numUnicorns == 0);
  assert(player[0].stable.size == 3);
  AddStable(0, torpedo_tmp);

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  // sacrifices in this case are in reverse order
  if (discardpile.size != 3 ||
      strcmp(discardpile.cards[0].name, sadistic_tmp.name) != 0 ||
      strcmp(discardpile.cards[1].name, pandamonium_tmp.name) != 0 ||
      strcmp(discardpile.cards[2].name, barbed_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: sacrifice downgrade cards failed\n");
    ResetCol();
  }

  // torpedo should be moved over to the first index in the stable
  if (strcmp(player[0].stable.unicorns[0].name, torpedo_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable unicorn[0] verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no available downgrade cards to destroy
int torpedo_empty_check(void) {
  int num_fails = 0;
  struct Unicorn torpedo_tmp = Base_DECK[62];
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn yay_tmp = Base_DECK[100];

  currentPlayers = 2;
  AddStable(0, yay_tmp);
  AddStable(1, barbed_tmp);

  assert(discardpile.size == 0);
  assert(player[0].stable.numUnicorns == 0);
  assert(player[1].stable.size == 1);
  AddStable(0, torpedo_tmp);

  if (player[0].stable.size != 2 || player[0].stable.numUnicorns != 1 ||
      player[1].stable.size != 1 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-downgrade test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-downgrade test: discard size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Narwhal Torpedo
//
// When this card enters your Stable, SACRIFICE all Downgrade cards in your Stable.
int narwhal_torpedo_tests(void) {
  int num_fails = 0;

  if (!isClient) {
    rainbow_error("\nStarting Narwhal Torpedo tests...\n");

    num_fails += torpedo_basic_check();
    num_fails += torpedo_empty_check();
  }
  return num_fails;
}
