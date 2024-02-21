#include <networkevents.h>
#include "MagicTests.h"

// sanity check
int unicorn_swap_basic_check(void) {
  int num_fails = 0;
  struct Unicorn swap_tmp = Base_DECK[77];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];

  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(1, basic_tmp2);
  AddStable(1, basic_tmp2);
  AddStable(1, basic_tmp2);
  player[0].hand.cards[player[0].hand.numCards++] = swap_tmp;

  int ret;
  assert(player[0].stable.size == 1);
  assert(player[1].stable.size == 3);
  ret = Base_ConditionalEffects(0, swap_tmp, 0, 0);
  if (ret) Base_MagicEffects(0, swap_tmp.effect);

  if (turnCount != 1 || ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: turn count failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 1 ||
      player[1].stable.size != 3 || player[1].stable.numUnicorns != 3 ||
      strcmp(player[0].stable.unicorns[0].name, basic_tmp2.name) != 0 ||
      strcmp(player[1].stable.unicorns[2].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// check to see if enter stable effects get triggered when cards swapped stables
int unicorn_swap_special_check(void) {
  int num_fails = 0;
  struct Unicorn swap_tmp = Base_DECK[77];
  struct Unicorn rainbow_tmp = Base_DECK[42]; // brings basic unicorn from hand to stable
  struct Unicorn americorn_tmp = Base_DECK[47]; // picks random card from another player's hand
  struct Unicorn neigh_tmp = Base_DECK[128];
  struct Unicorn basic_tmp = Base_DECK[13];

  currentPlayers = 2;
  AddStable(0, rainbow_tmp);
  AddStable(1, americorn_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = swap_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = neigh_tmp;
  player[1].hand.cards[player[1].hand.numCards++] = basic_tmp;

  assert(player[0].hand.numCards == 1);
  assert(player[1].hand.numCards == 2);
  Base_MagicEffects(0, swap_tmp.effect);

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 1 ||
      player[1].stable.size != 2 || player[1].stable.numUnicorns != 2 ||
      strcmp(player[0].stable.unicorns[0].name, americorn_tmp.name) != 0 ||
      strcmp(player[1].stable.unicorns[0].name, rainbow_tmp.name) != 0 ||
      strcmp(player[1].stable.unicorns[1].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    special test: stable verification failed\n");
    ResetCol();
  }

  // numCards would be 1 instead of 2 in the real game, but magicEffects
  // doesn't discard cards or rearrange hands
  if (player[0].hand.numCards != 2 || player[1].hand.numCards != 0 ||
      strcmp(player[0].hand.cards[1].name, neigh_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    special test: hand verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// check to see if flags get toggled when cards swapped stables
int unicorn_swap_toggle_check(void) {
  int num_fails = 0;
  struct Unicorn swap_tmp = Base_DECK[77];
  struct Unicorn queen_tmp = Base_DECK[51];
  struct Unicorn ginormous_tmp = Base_DECK[48];

  currentPlayers = 2;
  AddStable(0, queen_tmp);
  AddStable(1, ginormous_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = swap_tmp;

  assert((player[0].flags & QUEEN_BEE_UNICORN) == 0); // this flag is set backwards
  assert((player[1].flags & QUEEN_BEE_UNICORN) == QUEEN_BEE_UNICORN);
  assert((player[1].flags & GINORMOUS_UNICORN) == GINORMOUS_UNICORN);
  assert(player[0].hand.numCards == 1);
  Base_MagicEffects(0, swap_tmp.effect);

  if ((player[0].flags & QUEEN_BEE_UNICORN) != QUEEN_BEE_UNICORN || (player[0].flags & GINORMOUS_UNICORN) != GINORMOUS_UNICORN ||
      (player[1].flags & QUEEN_BEE_UNICORN) != 0 || (player[1].flags & GINORMOUS_UNICORN) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    special test: toggle flags failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// either no unicorn cards in your own stable or no unicorns is any other stable;
// unicorn swap cannot be played if yours is empty,
// but it can be played with other empty stables
int unicorn_swap_empty_check(void) {
  int num_fails = 0;
  struct Unicorn swap_tmp = Base_DECK[77];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn barbed_tmp = Base_DECK[106];
  struct Unicorn basic_tmp = Base_DECK[13];

  // no unicorn card in player[0]'s stable
  currentPlayers = 2;
  AddStable(0, barbed_tmp);
  AddStable(1, basic_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = swap_tmp;

  int ret;
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  ret = Base_ConditionalEffects(0, swap_tmp, 0, 0);

  if (turnCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty stable test (player[0]): turn count failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // no unicorn card in player[1]'s stable
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(1, yay_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = swap_tmp;

  assert(discardpile.size == 0);
  assert(player[0].stable.size == 1);
  assert(player[1].stable.size == 1);
  assert(player[0].hand.numCards == 1);
  ret = Base_ConditionalEffects(0, swap_tmp, 0, 0);
  if (ret) Base_MagicEffects(0, swap_tmp.effect);

  if (turnCount != 1 || ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty stable test (player[1]): turn count failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 1 || player[0].stable.numUnicorns != 1 ||
      player[1].stable.size != 1 || player[1].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty stable test (player[1]): stable size failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// PANDAMONIUM
int unicorn_swap_masquerade_check(void) {
  int num_fails = 0;
  struct Unicorn swap_tmp = Base_DECK[77];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn panda_tmp = Base_DECK[107];

  // player[0] is under a masquerade effect
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(1, basic_tmp2);
  AddStable(0, panda_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = swap_tmp;

  int ret;
  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[0].flags == PANDAMONIUM);
  ret = Base_ConditionalEffects(0, swap_tmp, 0, 0);

  if (turnCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test (player[0]): turn count failed\n");
    ResetCol();
  }
  reset_players();
  reset_discard();

  // player[0] is under a masquerade effect
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(1, basic_tmp2);
  AddStable(1, panda_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = swap_tmp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  assert(player[1].flags == PANDAMONIUM);
  ret = Base_ConditionalEffects(0, swap_tmp, 0, 0);

  if (turnCount != 2 || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    PANDAMONIUM test (player[0]): turn count failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// compatibility w/ unicorn lasso & swapping stolen cards
int unicorn_swap_unicorn_lasso_check(void) {
  int num_fails = 0;
  struct Unicorn swap_tmp = Base_DECK[77];
  struct Unicorn queen_tmp = Base_DECK[51];
  struct Unicorn ginormous_tmp = Base_DECK[48];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn lasso_tmp = Base_DECK[102];

  // playing hot potato with the stolen lasso card
  // i.e. player[0] takes card (ginormous_tmp) from player[2]
  // player[0] activates unicorn swap and moves card (ginormous_tmp) to player[1]'s stable
  // player[0] then steals some random card (queen_tmp) from player[1]'s stable
  // player[2] gets nothing back at the end of player[0]'s turn
  currentPlayers = 3;
  player[0].hand.cards[player[0].hand.numCards++] = swap_tmp;

  AddStable(0, lasso_tmp);
  AddStable(0, basic_tmp);
  AddStable(2, ginormous_tmp);
  AddStable(1, basic_tmp2);
  AddStable(1, queen_tmp);

  assert(player[0].hand.numCards == 1);
  assert(player[0].stable.size == 2);
  assert(player[0].stable.numUnicorns == 1);
  assert(player[0].flags == QUEEN_BEE_UNICORN);

  assert(player[2].stable.size == 1);
  assert(player[2].stable.numUnicorns == 1);
  assert((player[2].flags & GINORMOUS_UNICORN) == GINORMOUS_UNICORN);
  assert((player[2].flags & QUEEN_BEE_UNICORN) == QUEEN_BEE_UNICORN);

  assert(player[1].stable.size == 2);
  assert(player[1].stable.numUnicorns == 2);
  assert(player[1].flags == 0);

  Base_BeginningTurnEffects(0, lasso_tmp);
  assert(uniLassoIndex[0] != -1);
  Base_MagicEffects(0, swap_tmp.effect);

  // check if the lasso flag has been reset before the turn is over
  if (uniLassoIndex[0] != -1) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: flag reset failed\n");
    ResetCol();
  }

  // player[0]'s size and number of unicorns should go up by 1 because of unicorn lasso
  // player[1]'s size and number of unicorns stay the same since unicorn swap is an
  // equivalent exchange number-wise
  // player[2]'s size and number of unicorns should go down by 1 because of unicorn lasso
  if (player[0].stable.size != 3 || player[0].stable.numUnicorns != 2 ||
      player[2].stable.size != 0 || player[2].stable.numUnicorns != 0 ||
      player[1].stable.size != 2 || player[1].stable.numUnicorns != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: stable size pre-end of turn failed\n");
    ResetCol();
  }

  // checks the last unicorn in player[0] (index 2, 3rd card) against the swapped
  // card from player[1] (queen_tmp)
  // the stolen/swapped card (ginormous_tmp) is the last card of player[1] at index 1
  if (strcmp(player[0].stable.unicorns[2].name, queen_tmp.name) != 0 ||
      strcmp(player[1].stable.unicorns[0].name, basic_tmp2.name) != 0 ||
      strcmp(player[1].stable.unicorns[1].name, ginormous_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: stable unicorn check pre-end of turn failed\n");
    ResetCol();
  }
  
  // player[0] has queen bee unicorn, so that flag should be set for every /other/ player but them
  // player[1] has ginormous unicorn, so that flag should be set for themself
  if ((player[0].flags & GINORMOUS_UNICORN) != 0 || (player[0].flags & QUEEN_BEE_UNICORN) != 0 ||
      (player[1].flags & GINORMOUS_UNICORN) != GINORMOUS_UNICORN || (player[1].flags & QUEEN_BEE_UNICORN) != QUEEN_BEE_UNICORN ||
      (player[2].flags & GINORMOUS_UNICORN) != 0 || (player[2].flags & QUEEN_BEE_UNICORN) != QUEEN_BEE_UNICORN) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: toggle flags check pre-end of turn failed\n");
    ResetCol();
  }

  EndOfTurn(0);

  // the unicorn locations, stable sizes, and flags should all stay the same at the end of the turn
  if (player[0].stable.size != 3 || player[0].stable.numUnicorns != 2 ||
      player[1].stable.size != 2 || player[1].stable.numUnicorns != 2 ||
      player[2].stable.size != 0 || player[2].stable.numUnicorns != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: stable size post-end of turn failed\n");
    ResetCol();
  }
  
  if (strcmp(player[0].stable.unicorns[2].name, queen_tmp.name) != 0 ||
      strcmp(player[1].stable.unicorns[0].name, basic_tmp2.name) != 0 ||
      strcmp(player[1].stable.unicorns[1].name, ginormous_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: stable unicorn check post-end of turn failed\n");
    ResetCol();
  }
  
  // queen bee unicorn is in the same spot as the stolen unicorn from unicorn lasso,
  // so if the end of turn phase mistakenly swapped unicorns again when the original lasso'd unicorn is gone,
  // then everyone's flags should change as if player[1] has queen bee unicorn now
  if ((player[0].flags & GINORMOUS_UNICORN) != 0 || (player[0].flags & QUEEN_BEE_UNICORN) != 0 ||
      (player[1].flags & GINORMOUS_UNICORN) != GINORMOUS_UNICORN || (player[1].flags & QUEEN_BEE_UNICORN) != QUEEN_BEE_UNICORN ||
      (player[2].flags & GINORMOUS_UNICORN) != 0 || (player[2].flags & QUEEN_BEE_UNICORN) != QUEEN_BEE_UNICORN) {
    num_fails++;
    Red();
    fprintf(stderr, "    unicorn lasso test: toggle flags check post-end of turn failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

int unicorn_swap_puppicorn_check(void) {
  int num_fails = 0;
  struct Unicorn swap_tmp = Base_DECK[77];
  struct Unicorn yay_tmp = Base_DECK[100];
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn puppicorn_tmp = Base_DECK[41];

  currentPlayers = 2;
  player[0].hand.cards[player[0].hand.numCards++] = swap_tmp;

  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, puppicorn_tmp);
  AddStable(0, basic_tmp);
  AddStable(1, yay_tmp);
  AddStable(1, basic_tmp2);

  assert(player[0].hand.numCards == 1);
  assert(player[0].stable.size == 6);
  assert(player[1].stable.size == 2);
  assert(puppicornIndex[0] == 4); // card index spot
  assert(puppicornIndex[1] == 0); // player number index
  Base_MagicEffects(0, swap_tmp.effect);

  // puppicorn should have moved from player[0][4] to player[1][1]
  if (puppicornIndex[0] != 1 || puppicornIndex[1] != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: puppicorn index failed\n");
    ResetCol();
  }

  if (player[0].stable.size != 6 || player[0].stable.numUnicorns != 6 ||
      player[1].stable.size != 2 || player[1].stable.numUnicorns != 1 ||
      strcmp(player[0].stable.unicorns[5].name, basic_tmp2.name) != 0 ||
      strcmp(player[1].stable.unicorns[0].name, yay_tmp.name) != 0 ||
      strcmp(player[1].stable.unicorns[puppicornIndex[0]].name, puppicorn_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    puppicorn test: stable verification failed\n");
    ResetCol();
  }

  reset_players();
  reset_discard();
  return num_fails;
}

// Unicorn Swap
//
// Move a Unicorn card in your Stable to any other player's Stable,
// then STEAL a Unicorn card from that player's Stable.
int unicorn_swap_tests(void) {
  int num_fails = 0;

  rainbow_error("\nStarting Unicorn Swap tests...\n");
  FILE *fp;
  if (!isClient) {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/unicornswap.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    num_fails += unicorn_swap_basic_check();
    num_fails += unicorn_swap_special_check();
    num_fails += unicorn_swap_toggle_check();
    num_fails += unicorn_swap_empty_check();
    num_fails += unicorn_swap_masquerade_check();
    num_fails += unicorn_swap_unicorn_lasso_check();
    num_fails += unicorn_swap_puppicorn_check();

  }
  else {
    // file input stream setup
    fopen_s(&fp, "Tests/Input/unicornswapclient.txt", "r");
    if (fp == NULL) {
      Magenta();
      fprintf(stderr, "    file input failed :(");
      ResetCol();
      return 1;
    }
    fpinput = fp;

    // basic check, no input
    int events;
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // special check
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // toggle check
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // empty check; the second test within that function requires input
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // masquerade check
    // should get no prompt

    // unicorn lasso check; should require no input
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);

    // puppicorn check
    ReceiveInt(&events, sockfd);
    netStates[events].RecvClient(1, sockfd);
  }

  fclose(fp);
  return num_fails;
}
