#include <networkevents.h>
#include "MagicalUnicornTests.h"

// sanity check
int black_knight_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn knight_tmp = Base_DECK[59];

  currentPlayers = 2;
  AddStable(1, knight_tmp);
  AddStable(1, basic_tmp);

  assert(discardpile.size == 0);
  assert(player[1].stable.numUnicorns == 2);
  assert(player[1].flags == BLACK_KNIGHT_UNICORN);

  Destroy(0, ANY, FALSE);

  // this test assumes that the 1st player initially tries to destroy
  // the basic unicorn; this is reflected in the input file
  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, knight_tmp.name) != 0 ||
      strcmp(player[1].stable.unicorns[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: sacrifice failed\n");
    ResetCol();
  }

  if ((player[1].flags & BLACK_KNIGHT_UNICORN) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: toggle flags failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no other unicorns in the stable to sacrifice for
int black_knight_empty_check(void) {
  int num_fails = 0;
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn knight_tmp = Base_DECK[59];

  currentPlayers = 2;
  AddStable(0, knight_tmp);
  AddStable(0, yay_tmp);

  assert(discardpile.size == 0);
  assert((player[0].flags & BLACK_KNIGHT_UNICORN) == BLACK_KNIGHT_UNICORN);

  Base_SacrificeDestroyEffects(0, 1, knight_tmp.effect);

  // this test assumes that there's no stdout prompting the self-sacrifice
  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, yay_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    non-unicorn test: destroy failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// no other "unicorns" in the stable to sacrifice for
int black_knight_pandamonium_check(void) {
  int num_fails = 0;
  struct Unicorn panda_tmp = Base_DECK[107];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn knight_tmp = Base_DECK[59];

  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(0, panda_tmp);
  AddStable(0, knight_tmp);

  assert(discardpile.size == 0);
  assert((player[0].flags & PANDAMONIUM) == PANDAMONIUM);
  assert((player[0].flags & BLACK_KNIGHT_UNICORN) == BLACK_KNIGHT_UNICORN);

  Base_SacrificeDestroyEffects(0, 0, knight_tmp.effect);

  // this test assumes that there's no stdout prompting the self-sacrifice
  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: destroy failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// blinding light disables the black knight check
int black_knight_blinding_light_check(void) {
  int num_fails = 0;
  struct Unicorn blinding_tmp = Base_DECK[112];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn knight_tmp = Base_DECK[59];

  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(0, blinding_tmp);
  AddStable(0, knight_tmp);

  assert(discardpile.size == 0);
  assert((player[0].flags & BLINDING_LIGHT) == BLINDING_LIGHT);

  Base_SacrificeDestroyEffects(0, 0, knight_tmp.effect);

  // this test assumes that there's no stdout prompting the self-sacrifice
  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test: destroy failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Black Knight Unicorn
//
// If a Unicorn card in your Stable would be destroyed, you may SACRIFICE this card instead.
int black_knight_unicorn_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Black Knight Unicorn tests...\n");
  FILE *fp;

  if (!isClient) {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/blackknightunicorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // the test is set up to prepare for the fail scenario with y inputs
    num_fails += black_knight_basic_check();
    num_fails += black_knight_empty_check();
    num_fails += black_knight_pandamonium_check();
    num_fails += black_knight_blinding_light_check();

  }
  else {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/line_y_1.txt", "r"); // technically only the y part is necessary
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // basic check
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }

  fclose(fp);
  return num_fails;
}
