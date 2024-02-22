#include <networkevents.h>
#include "MagicalUnicornTests.h"

// sanity check
int puppicorn_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn basic_tmp3 = Base_DECK[20];
  struct Unicorn basic_tmp4 = Base_DECK[23];
  struct Unicorn basic_tmp5 = Base_DECK[26];
  struct Unicorn basic_tmp6 = Base_DECK[29];
  struct Unicorn puppicorn_tmp = Base_DECK[41];

  reset_players();

  // does it move?
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp2);
  AddStable(0, basic_tmp3);
  AddStable(0, puppicorn_tmp);
  AddStable(1, basic_tmp4);
  AddStable(1, basic_tmp5);

  assert(player[0].stable.size == 4);
  assert(player[1].stable.size == 2);
  assert(puppicornIndex[0] == 3);
  assert(puppicornIndex[1] == 0);
  EndOfTurn(0);

  if (player[0].stable.numUnicorns != 3 || player[0].stable.size != 3) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[0] stable size failed\n");
    ResetCol();
  }

  if (player[1].stable.numUnicorns != 3 || player[1].stable.size != 3) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[1] stable size failed\n");
    ResetCol();
  }

  if (strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[1].name, basic_tmp2.name) != 0 ||
      strcmp(player[0].stable.unicorns[2].name, basic_tmp3.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[0] stable verification failed\n");
    ResetCol();
  }

  if (strcmp(player[1].stable.unicorns[0].name, basic_tmp4.name) != 0 ||
      strcmp(player[1].stable.unicorns[1].name, basic_tmp5.name) != 0 ||
      strcmp(player[1].stable.unicorns[2].name, puppicorn_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[1] stable verification failed\n");
    ResetCol();
  }

  if (puppicornIndex[0] != 2 || puppicornIndex[1] != 1 ||
      strcmp(player[1].stable.unicorns[2].name, puppicorn_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: puppicorn index failed\n");
    ResetCol();
  }

  // cycle 2
  int events;
  ReceiveInt(&events, clientsockfd[0]);
  netStates[events].RecvServer(1, clientsockfd[0]);

  if (player[0].stable.numUnicorns != 4 || player[0].stable.size != 4) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (round 2): player[0] stable size failed\n");
    ResetCol();
  }

  if (player[1].stable.numUnicorns != 3 || player[1].stable.size != 3) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (round 2): player[1] stable size failed\n");
    ResetCol();
  }

  if (strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0 ||
      strcmp(player[0].stable.unicorns[1].name, basic_tmp2.name) != 0 ||
      strcmp(player[0].stable.unicorns[2].name, basic_tmp3.name) != 0 ||
      strcmp(player[0].stable.unicorns[3].name, puppicorn_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (round 2): player[0] stable verification failed\n");
    ResetCol();
  }

  if (strcmp(player[1].stable.unicorns[0].name, basic_tmp4.name) != 0 ||
      strcmp(player[1].stable.unicorns[1].name, basic_tmp5.name) != 0 ||
      strcmp(player[1].stable.unicorns[2].name, basic_tmp6.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (round 2): player[1] stable verification failed\n");
    ResetCol();
  }

  if (puppicornIndex[0] != 3 || puppicornIndex[1] != 0 ||
      strcmp(player[0].stable.unicorns[3].name, puppicorn_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test (round 2): puppicorn index failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

int puppicorn_sacrifice_destroy_check(void) {
  int num_fails = 0;
  struct Unicorn puppicorn_tmp = Base_DECK[41];

  AddStable(0, puppicorn_tmp);

  // destroy test
  CanBeDestroyed(0, 0, ANY, FALSE);

  if (CanBeDestroyed(0, 0, ANY, FALSE)) {
    num_fails++;
    Red();
    fprintf(stderr, "    destroy test: CanBeDestroyed failed\n");
    ResetCol();
  }

  // sacrifice test
  CanBeSacrificed(0, 0, ANY);

  if (CanBeSacrificed(0, 0, ANY)) {
    num_fails++;
    Red();
    fprintf(stderr, "    sacrifice test: CanBeSacrificed failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// win condition should be checked before it moves to the next player
int puppicorn_win_check(void) {
  int num_fails = 0;
  struct Unicorn puppicorn_tmp = Base_DECK[41];

  currentPlayers = 2;
  for (int i = 0; i < 6; i++) {
    AddStable(0, Base_DECK[i]);
  }
  AddStable(0, puppicorn_tmp);

  int win = EndOfTurn(0);

  if (win != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    win check: EndOfTurn failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// puppicornIndex[0] should be -1 once it's returned to someone's hand
int puppicorn_reset_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn puppicorn_tmp = Base_DECK[41];

  currentPlayers = 2;
  AddStable(0, puppicorn_tmp);
  AddStable(1, basic_tmp);

  EndOfTurn(0);
  ReturnCardToHand(1, 1);

  if (puppicornIndex[0] != -1) {
    num_fails++;
    Red();
    fprintf(stderr, "    reset test: puppicorn index failed\n");
    ResetCol();
  }

  if (player[0].stable.numUnicorns != 0 || player[0].stable.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    reset test: stable size failed\n");
    ResetCol();
  }


  return num_fails;
}

// puppicornIndex[0] should adjust accordingly to any stable changes
int puppicorn_rearrange_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn puppicorn_tmp = Base_DECK[41];

  AddStable(0, basic_tmp);
  AddStable(0, puppicorn_tmp);

  assert(puppicornIndex[0] == 1);
  Sacrifice(0, ANY);

  // puppicornIndex[0] should become 0 if the first index gets sacrificed or destroyed
  if (puppicornIndex[0] != 0 ||
      strcmp(player[0].stable.unicorns[puppicornIndex[0]].name, puppicorn_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    rearrange test: puppicorn index failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Puppicorn
// 
// (original print text)
// At the end of your turn, move Puppicorn to the Stable of the player on your left.
// This card cannot be sacrificed or destroyed.
//
// (2nd print text: i overlooked this when configuring unicorn swap, but now it
// will be defunct)
// Each time any player begins their turn, move this card to that player's Stable.
// This card cannot be sacrificed or destroyed.
int puppicorn_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Puppicorn tests...\n");
  if (!isClient) {
    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/line_1.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += puppicorn_basic_check();
    num_fails += puppicorn_sacrifice_destroy_check();
    num_fails += puppicorn_win_check();
    num_fails += puppicorn_reset_check();
    num_fails += puppicorn_rearrange_check();

    fclose(fp);
  }
  else {
    // basic check, no input
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // cycle 2 of basic check
    struct Unicorn basic_tmp6 = Base_DECK[29];
    AddStable(1, basic_tmp6);

    assert(player[0].stable.size == 3);
    assert(player[1].stable.size == 4);
    assert(puppicornIndex[0] == 2);
    assert(puppicornIndex[1] == 1);
    EndOfTurn(1);

    // win check
    // this should get skipped over

    // reset check, no input
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }
  return num_fails;
}
