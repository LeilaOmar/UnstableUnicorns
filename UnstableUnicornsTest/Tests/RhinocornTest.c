#include <networkevents.h>
#include "MagicalUnicornTests.h"

int rhinocorn_basic_check(void) {
  int num_fails = 0;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn rhinocorn_tmp = Base_DECK[37];

  moveCount = 1;
  currentPlayers = 2;
  AddStable(0, rhinocorn_tmp);
  AddStable(1, basic_tmp);

  Base_BeginningTurnEffects(0, player[0].stable.unicorns[0]);

  // destroy unicorn test
  if (player[1].stable.size != 0 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: destroy unicorn failed\n");
    ResetCol();
  }

  // assert end of turn phase
  if (moveCount > 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: end of turn failed\n");
    ResetCol();
  }

  reset_players();
  return num_fails;
}

// Rhinocorn
// 
// If this card is in your Stable at the beginning of your turn, you may DESTROY a Unicorn card.
// If you do, immediately skip to your End of Turn phase.
int rhinocorn_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Rhinocorn tests...\n");
  if (!isClient) {
    // file input stream setup
    FILE *fp;
    fopen_s(&fp, "Tests/Input/rhinocorn.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += rhinocorn_basic_check();

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
