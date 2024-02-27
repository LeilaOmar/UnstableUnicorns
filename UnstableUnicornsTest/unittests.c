#include <conio.h>
#include <networkfuncs.h>
#include <networkstates.h>
#include "Tests/MagicalUnicornTests.h"
#include "Tests/MagicTests.h"
#include "Tests/UpgradeTests.h"
#include "Tests/DowngradeTests.h"
#include "Tests/InstantTests.h"
#include "resetfns.h"

int num_fails = 0;

// just to avoid including windowsapp.c and client/server for now
char partyMems[296] = { 0 };
RECT pselect[MAX_PLAYERS];

// ********************************************************************************
// **************************** Utility Function Tests ****************************
// ********************************************************************************

void check_type_tests(void) {
  rainbow_error("\nStarting CheckType tests...\n");

  // test for ANY cards
  if (CheckType(ANY, BABYUNICORN) == 0) num_fails++;
  if (CheckType(ANY, BASICUNICORN) == 0) num_fails++;
  if (CheckType(ANY, MAGICALUNICORN) == 0) num_fails++;
  if (CheckType(ANY, UPGRADE) == 0) num_fails++;
  if (CheckType(ANY, DOWNGRADE) == 0) num_fails++;
  if (CheckType(ANY, MAGIC) == 0) num_fails++;
  if (CheckType(ANY, INSTANT) == 0) num_fails++;

  // test for ANYUNICORN
  if (CheckType(ANYUNICORN, BABYUNICORN) == 0) num_fails++;
  if (CheckType(ANYUNICORN, BASICUNICORN) == 0) num_fails++;
  if (CheckType(ANYUNICORN, MAGICALUNICORN) == 0) num_fails++;

  // test for compatible classes
  if (CheckType(BASICUNICORN, BASICUNICORN) == 0) num_fails++;
  if (CheckType(BASICUNICORN, UPGRADE) == 1) num_fails++;

  if (num_fails > 0) {
    fprintf(stderr, "    CheckType failed?!?!!! wow lol\n");
  }
}

// a lot of potential checks here are actually checked for in the code surrounding
// the function call
void rearrange_pile_tests(void) {
  int tmp_size = nursery.size;
  int start = 5;

  rainbow_error("\nStarting RearrangePile tests...\n");

  // sanity check
  RearrangePile(&nursery, start);

  if ((tmp_size - 1) != nursery.size) {
    num_fails++;
    Red();
    fprintf(stderr, "    basic sanity check test 1 failed\n");
    ResetCol();
  }

  // using the nursery here because each card is unique
  int found = 0;
  for (int i = 0; i < nursery.size; i++) {
    if (strcmp(nursery.cards[i].name, Base_DECK[start].name) == 0)
      found++;
  }

  if (found > 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    basic sanity check test 2 failed\n");
    ResetCol();
  }
  reset_nursery();

  // empty pile test (this shouldn't really happen in the first place)
  nursery.size = 0;
  tmp_size = nursery.size;

  RearrangePile(&nursery, 0);

  if (tmp_size != nursery.size) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty pile test failed\n");
    ResetCol();
  }

  reset_nursery();
}

void add_nursery_tests(void) {
  struct Unicorn corn = Base_DECK[12];
  int tmp_size = nursery.size;

  rainbow_error("\nStarting AddNursery tests...\n");

  // capacity check; this really shouldn't be called erroneously in the first place,
  // but at least it wouldn't completely break if sometihng else duplicated or went
  // wrong in some other fashion
  AddNursery(corn);
  if (nursery.size != tmp_size) {
    num_fails++;
    Red();
    fprintf(stderr, "    full nursery size check test failed\n");
    ResetCol();
  }
  reset_nursery();

  // basic check
  nursery.size = 5;
  tmp_size = nursery.size;

  AddNursery(corn);

  if (nursery.size != (tmp_size + 1)) {
    num_fails++;
    Red();
    fprintf(stderr, "    basic sanity check test 1 failed\n");
    ResetCol();
  }
  if (strcmp(nursery.cards[nursery.size - 1].name, corn.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    basic sanity check test 2 failed\n");
    ResetCol();
  }

  reset_nursery();
}

void add_stable_tests(void) {
  struct Unicorn narwhal_tmp = Base_DECK[34];
  struct Unicorn broken_stable_tmp = Base_DECK[111];
  struct Unicorn basic_tmp = Base_DECK[13];

  rainbow_error("\nStarting AddStable tests...\n");

  // adding unicorn (technically a Narwhal :V )
  AddStable(0, narwhal_tmp);

  if (player[0].stable.numUnicorns != 1 || player[0].stable.size != 1 || 
      strcmp(player[0].stable.unicorns[0].name, narwhal_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    adding unicorn test failed\n");
    ResetCol();
  }
  reset_players();

  // adding magic/upgrade/downgrade card [broken stable]
  AddStable(0, broken_stable_tmp);

  if (player[0].stable.numUnicorns != 0 || player[0].stable.size != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    adding non-unicorn test failed\n");
    ResetCol();
  }
  reset_players();

  // barbed wire test with adding a non-unicorn card (it shouldn't activate)
  player[0].hand.cards[player[0].hand.numCards++] = narwhal_tmp;
  player[0].flags |= BARBED_WIRE;
  AddStable(0, broken_stable_tmp);

  if (player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire test w/ non-unicorn failed\n");
    ResetCol();
  }
  reset_players();

  // barbed wire test with adding a unicorn card (it should activate)
  player[0].hand.cards[player[0].hand.numCards++] = narwhal_tmp;
  player[0].flags |= BARBED_WIRE;

  FILE *fp;
  fopen_s(&fp, "Tests/Input/line_1.txt", "r");
  if (fp == NULL) {
    num_fails++;
    Magenta();
    fprintf(stderr, "    file input failed :(\n");
    ResetCol();
    return;
  }
  fpinput = fp;

  AddStable(0, narwhal_tmp);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire test w/ unicorn failed\n");
    ResetCol();
  }
  reset_players();
  fclose(fp);

  // barbed wire + pandamonium test (it should activate because it enters as a unicorn)
  player[0].hand.cards[player[0].hand.numCards++] = narwhal_tmp;
  player[0].flags |= BARBED_WIRE;
  player[0].flags |= PANDAMONIUM;

  fopen_s(&fp, "Tests/Input/line_1.txt", "r");
  if (fp == NULL) {
    num_fails++;
    Magenta();
    fprintf(stderr, "    file input failed :(\n");
    ResetCol();
    return;
  }
  fpinput = fp;

  AddStable(0, narwhal_tmp);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire test w/ pandamonium failed\n");
    ResetCol();
  }
  reset_players();

  // tiny stable sanity check w/ 5 unicorns and 1 "other"
  player[0].flags |= TINY_STABLE;
  for (int i = 0; i < 5; i++) {
    AddStable(0, narwhal_tmp);
  }
  AddStable(0, broken_stable_tmp);

  if (player[0].stable.size != 6 || player[0].stable.numUnicorns != 5) {
    num_fails++;
    Red();
    fprintf(stderr, "    tiny stable pre-limit check failed\n");
    ResetCol();
  }
  fclose(fp);

  // tiny stable cont. check
  fopen_s(&fp, "Tests/Input/line_1.txt", "r");
  if (fp == NULL) {
    num_fails++;
    Magenta();
    fprintf(stderr, "    file input failed :(\n");
    ResetCol();
    return;
  }
  fpinput = fp;

  player[0].stable.unicorns[1] = basic_tmp; // replace the second unicorn since the first will be sacrificed
  AddStable(0, narwhal_tmp);

  // check if the previous second becomes the first alongside the basic size/count checks
  if (player[0].stable.size != 6 || player[0].stable.numUnicorns != 5 ||
      strcmp(player[0].stable.unicorns[0].name, basic_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    tiny stable sacrifice test failed\n");
    ResetCol();
  }
  reset_players();
  fclose(fp);

  fpinput = stdin; // to prevent an infinite loop due to some error

  // tiny stable w/ pandamonium
  player[0].flags |= TINY_STABLE;
  player[0].flags |= PANDAMONIUM;
  for (int i = 0; i < 7; i++) {
    AddStable(0, narwhal_tmp);
  }

  if (player[0].stable.size != 7 || player[0].stable.numUnicorns != 7) {
    num_fails++;
    Red();
    fprintf(stderr, "    tiny stable test w/ pandamonium failed\n");
    ResetCol();
  }
  reset_players();
}

void rearrange_stable_tests(void) {
  struct Unicorn narwhal_tmp = Base_DECK[34];
  struct Unicorn broken_stable_tmp = Base_DECK[111];
  struct Unicorn basic_tmp = Base_DECK[13];

  rainbow_error("\nStarting RearrangeStable tests...\n");

  // barbed wire w/o pandamonium (general test pt 1)
  AddStable(0, basic_tmp);
  AddStable(0, narwhal_tmp);
  AddStable(0, broken_stable_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = narwhal_tmp;
  player[0].flags |= BARBED_WIRE;

  RearrangeStable(0, 2); // index 2 represents the 3rd card added [i.e. broken stable]

  if (player[0].stable.numUnicorns != 2 || player[0].stable.size != 2 || player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire basic non-unicorn check failed\n");
    ResetCol();
  }
  reset_players();

  // barbed wire w/o pandamonium (general test pt 2)
  AddStable(0, basic_tmp);
  AddStable(0, narwhal_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = narwhal_tmp;
  player[0].flags |= BARBED_WIRE;

  FILE *fp;
  fopen_s(&fp, "Tests/Input/line_1.txt", "r");
  if (fp == NULL) {
    num_fails++;
    Magenta();
    fprintf(stderr, "    file input failed :(\n");
    ResetCol();
    return;
  }
  fpinput = fp;
  RearrangeStable(0, 0); // take out the first card

  if (player[0].stable.numUnicorns != 1 || player[0].stable.size != 1 || player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire basic unicorn check failed\n");
    ResetCol();
  }
  reset_players();
  fclose(fp);

  // barbed wire w/ pandamonium
  AddStable(0, basic_tmp);
  AddStable(0, narwhal_tmp);
  player[0].hand.cards[player[0].hand.numCards++] = narwhal_tmp;
  player[0].flags |= BARBED_WIRE;
  player[0].flags |= PANDAMONIUM;

  fopen_s(&fp, "Tests/Input/line_1.txt", "r");
  if (fp == NULL) {
    num_fails++;
    Magenta();
    fprintf(stderr, "    file input failed :(\n");
    ResetCol();
    return;
  }
  fpinput = fp;

  RearrangeStable(0, 0); // take out the first card

  if (player[0].stable.numUnicorns != 1 || player[0].stable.size != 1 || player[0].hand.numCards != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    barbed wire w/ pandamonium check failed\n");
    ResetCol();
  }
  reset_players();

  fclose(fp);
}

void tiebreaker_tests(void) {

  rainbow_error("\nStarting Tiebreaker tests...\n");
  
  // the colors actually count for the name count lol
  int ret;
  struct Unicorn basic_tmp = Base_DECK[13];
  struct Unicorn basic_tmp2 = Base_DECK[17];
  struct Unicorn panda_tmp = Base_DECK[107];
  struct Unicorn ginormous_tmp = Base_DECK[48];
  struct Unicorn blinding_tmp = Base_DECK[112];

  // test for unicorn majority
  currentPlayers = 3;
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp2);

  AddStable(1, basic_tmp);
  AddStable(1, panda_tmp);
  AddStable(1, panda_tmp);
  AddStable(1, panda_tmp);

  AddStable(2, basic_tmp);
  AddStable(2, basic_tmp);
  
  ret = Tiebreaker();

  // player[0] wins
  if (ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    max number of unicorns test failed\n");
    ResetCol();
  }
  reset_players();

  // test for the longest total unicorn card character length
  currentPlayers = 3;
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, blinding_tmp); // just to pad character count
  AddStable(0, blinding_tmp);

  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);

  AddStable(2, basic_tmp);
  AddStable(2, basic_tmp);
  AddStable(2, basic_tmp2);

  ret = Tiebreaker();

  // player[2] wins
  if (ret != 2) {
    num_fails++;
    Red();
    fprintf(stderr, "    longest name test failed\n");
    ResetCol();
  }
  reset_players();

  // check for a complete tie breaker
  currentPlayers = 3;
  AddStable(0, basic_tmp2);
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp2);
  AddStable(0, basic_tmp);

  AddStable(1, basic_tmp);
  AddStable(1, basic_tmp);

  AddStable(2, basic_tmp);
  AddStable(2, basic_tmp2);
  AddStable(2, basic_tmp);
  AddStable(2, basic_tmp2);

  ret = Tiebreaker();

  // nobody wins
  if (ret != -1) {
    num_fails++;
    Red();
    fprintf(stderr, "    equal name test failed\n");
    ResetCol();
  }
  reset_players();

  // check to see if pandamonium affects it or not
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);
  AddStable(0, panda_tmp);

  AddStable(1, basic_tmp);

  assert(player[0].flags == PANDAMONIUM);
  ret = Tiebreaker();

  // player[0] has pandas, so player[1] should win
  if (ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    pandamonium test failed\n");
    ResetCol();
  }
  reset_players();

  // check to see if ginormous unicorn works
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);

  AddStable(1, basic_tmp);
  AddStable(1, ginormous_tmp);

  ret = Tiebreaker();

  // ginormous unicorn has less characters than "basic unicorn (red)," but
  // its effect counts for 2 unicorns, so player[1] should win
  if (ret != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    ginormous unicorn test failed\n");
    ResetCol();
  }
  reset_players();

  // check to see if blinding light disables ginormous unicorn
  currentPlayers = 2;
  AddStable(0, basic_tmp);
  AddStable(0, basic_tmp);

  AddStable(1, basic_tmp);
  AddStable(1, ginormous_tmp);
  AddStable(1, blinding_tmp);

  assert((player[1].flags & BLINDING_LIGHT) == BLINDING_LIGHT);
  ret = Tiebreaker();

  // player[0] should win because blinding light makes ginormous unicorn act like a basic unicorn,
  // and "ginormous unicorn" has less characters than "basic unicorn (red)"
  if (ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    ginormous unicorn w/ blinding light test failed\n");
    ResetCol();
  }
  reset_players();
}

// ********************************************************************************
// **************************** Basic Card Effect Tests ***************************
// ********************************************************************************

void draw_tests(void) {

  rainbow_error("\nStarting draw tests...\n");

  // check if it ends the game when the deck is about to be empty
  deck.size = 1;
  int tmp_hand_size = player[0].hand.numCards;
  int ret = Draw(0, 1);

  if (player[0].hand.numCards != tmp_hand_size || ret != -1) {
    num_fails++;
    Red();
    fprintf(stderr, "    empty deck end game test failed\n");
    ResetCol();
  }
  reset_players();
  reset_deck();

  // normal draw
  int tmp_deck_size = deck.size;
  tmp_hand_size = player[0].hand.numCards;
  ret = Draw(0, 1);

  if (player[0].hand.numCards != (tmp_hand_size + 1) || deck.size != (tmp_deck_size - 1) || ret != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test 1 failed\n");
    ResetCol();
  }

  // it isn't shuffled, so the player is drawing from the end
  if (strcmp(player[0].hand.cards[0].name, Base_DECK[NURSERY_SIZE + tmp_deck_size - 1].name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test 2 failed\n");
    ResetCol();
  }
  reset_players();
  reset_deck();
  reset_discard();
}

void discard_tests(void) {

  rainbow_error("\nStarting discard tests...\n");

  // simple check
  Draw(0, 1);

  FILE *fp;
  fopen_s(&fp, "Tests/Input/line_1.txt", "r");
  if (fp == NULL) {
    num_fails++;
    Magenta();
    fprintf(stderr, "    file input failed :(\n");
    ResetCol();
    return;
  }
  fpinput = fp;

  assert(discardpile.size == 0);
  assert(player[0].hand.numCards == 1);
  Discard(0, 1, ANY);

  if (player[0].hand.numCards != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: hand size failed\n");
    ResetCol();
  }

  if (discardpile.size != 1) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard size failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
}

void sacrifice_tests(void) {
  struct Unicorn ginormous_tmp = Base_DECK[48];
  rainbow_error("\nStarting sacrifice tests...\n");

  // simple check
  AddStable(0, ginormous_tmp);
  Base_EnterStableEffects(0, ginormous_tmp.effect);

  FILE *fp;
  fopen_s(&fp, "Tests/Input/line_1.txt", "r");
  if (fp == NULL) {
    num_fails++;
    Magenta();
    fprintf(stderr, "    file input failed :(\n");
    ResetCol();
    return;
  }
  fpinput = fp;

  assert(discardpile.size == 0);
  assert(player[0].stable.size == 1);
  assert(player[0].flags == GINORMOUS_UNICORN);
  Sacrifice(0, ANY);

  if (player[0].stable.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: stable size failed\n");
    ResetCol();
  }

  if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, ginormous_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: discard verification failed\n");
    ResetCol();
  }

  if (player[0].flags == GINORMOUS_UNICORN) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: toggle flags failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
}

void destroy_tests(void) {
  struct Unicorn ginormous_tmp = Base_DECK[48];
  rainbow_error("\nStarting destroy tests...\n");

  // simple check
  currentPlayers = 2;
  AddStable(0, ginormous_tmp);
  Base_EnterStableEffects(0, ginormous_tmp.effect);

  // the client is the destroyer, and the server receives the request and takes action
  if (isClient) {
    FILE *fp;
    fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
    if (fp == NULL) {
      num_fails++;
      Magenta();
      fprintf(stderr, "    file input failed :(\n");
      ResetCol();
      return;
    }
    fpinput = fp;

    assert(discardpile.size == 0);
    assert(player[0].stable.size == 1);
    assert(player[0].flags == GINORMOUS_UNICORN);
    Destroy(1, ANY, FALSE);

    if (player[0].stable.size != 0) {
      num_fails++;
      Red();
      fprintf(stderr, "    sanity test: stable size failed\n");
      ResetCol();
    }

    if (discardpile.size != 1 ||
      strcmp(discardpile.cards[0].name, ginormous_tmp.name) != 0) {
      num_fails++;
      Red();
      fprintf(stderr, "    sanity test: discard verification failed\n");
      ResetCol();
    }

    if (player[0].flags == GINORMOUS_UNICORN) {
      num_fails++;
      Red();
      fprintf(stderr, "    sanity test: toggle flags failed\n");
      ResetCol();
    }
    fclose(fp);
  }
  else {
    // basic check, no input
    int events;
    ReceiveInt(&events, clientsockfd[0]);
    netStates[events].RecvServer(1, clientsockfd[0]);
  }

  reset_players();
  reset_deck();
  reset_discard();
}

void steal_tests(void) {
  struct Unicorn ginormous_tmp = Base_DECK[48];
  rainbow_error("\nStarting steal tests...\n");

  // simple check
  currentPlayers = 2;
  AddStable(0, ginormous_tmp);
  Base_EnterStableEffects(0, ginormous_tmp.effect);

  FILE *fp;
  fopen_s(&fp, "Tests/Input/line_1_1.txt", "r");
  if (fp == NULL) {
    num_fails++;
    Magenta();
    fprintf(stderr, "    file input failed :(\n");
    ResetCol();
    return;
  }
  fpinput = fp;

  assert(discardpile.size == 0);
  assert(player[0].stable.size == 1);
  assert(player[0].flags == GINORMOUS_UNICORN);
  Steal(1, ANY);

  if (player[0].stable.size != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[0] stable size failed\n");
    ResetCol();
  }

  if (player[1].stable.size != 1 || player[1].stable.numUnicorns != 1 ||
      strcmp(player[1].stable.unicorns[0].name, ginormous_tmp.name) != 0) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: player[1] stable size failed\n");
    ResetCol();
  }

  if (player[0].flags == GINORMOUS_UNICORN) {
    num_fails++;
    Red();
    fprintf(stderr, "    sanity test: toggle flags failed\n");
    ResetCol();
  }

  reset_players();
  reset_deck();
  reset_discard();
}

// ********************************************************************************
// ********************************* Main Function ********************************
// ********************************************************************************

int main(int argc, char *argv[]) {

  // just in case file i/o fails
  fpinput = stdin;

  // client/server variables
  isClient = 0;
  WSADATA wsa;
  short portno = 1234;
  struct sockaddr_in addr;
  struct sockaddr_in server;

  // additional information
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;

  // set the size of the structures
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  // set up winsock with TDP communication (SOCK_STREAM) on IPv4 (AF_INET)
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("Failed. Error Code : %d", WSAGetLastError());
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    printf("Could not create socket : %d", WSAGetLastError());
    exit(1);
  }

  if (argc > 1 && strcmp(argv[1], "client") == 0) {

    // ********************************************************************************
    // ********************************* Client Set-up ********************************
    // ********************************************************************************

    server.sin_family = AF_INET;
    server.sin_port = htons(portno);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

    while (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Attempt to connect to the server was unsuccessful. Error code : %d\n", WSAGetLastError());
    }
    
    isClient = 1;

  }
  else {

    // ********************************************************************************
    // ********************************* Server Set-up ********************************
    // ********************************************************************************

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof addr) == SOCKET_ERROR) {
      fprintf(stderr, "Bind failed with error code : %d.", WSAGetLastError());
      return 1;
    }

    listen(sockfd, MAX_PLAYERS);

    // start the program up
    CreateProcessA (
      "..\\Debug\\UnstableUnicornsTest.exe",   // the path
      "..\\Debug\\UnstableUnicornsTest.exe client",		// Full command line (includes argv[0])
      NULL,                 // Process handle not inheritable
      NULL,                 // Thread handle not inheritable
      FALSE,                // Set handle inheritance to FALSE
      CREATE_NO_WINDOW,			// Process Create Flags; could do CREATE_NEW_CONSOLE to show separately
      NULL,									// Use parent's environment block
      NULL,									// Use parent's starting directory 
      &si,									// Pointer to STARTUPINFO structure
      &pi										// Pointer to PROCESS_INFORMATION structure
    );

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    clientsockfd[0] = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);

  }

  // ********************************************************************************
  // **************************** Unit/Integration Tests ****************************
  // ********************************************************************************

  // redirect general output to make the cmd line cleaner and easier to parse through
  FILE *fp;
  freopen_s(&fp, "NUL", "w", stdout);

  // initialize the deck
  InitDeck(&nursery, &deck, &discardpile);
  srand(0);

  // initialize the network states too
  InitNetworkStates();

  // wheeeee~
  currentPlayers = 1;
  strcpy_s(player[0].username, sizeof player[0].username, "one");
  strcpy_s(player[1].username, sizeof player[1].username, "two");
  strcpy_s(player[2].username, sizeof player[2].username, "three");

  fprintf(stderr, "Commencing testing for the ");
  rainbow_error("Unstable Unicorns");
  fprintf(stderr, " application demo! :D\n");

  if (!isClient) {
    // utility function test
    check_type_tests();
    rearrange_pile_tests();
    add_nursery_tests();
    add_stable_tests();
    rearrange_stable_tests();
    tiebreaker_tests();

    // basic card effect tests
    draw_tests();
    discard_tests();
    sacrifice_tests();
    destroy_tests();
    steal_tests();
  }
  else {
    destroy_tests();
  }

  // specific card functionality tests

  // magical unicorn cards
  num_fails += rhinocorn_tests();
  num_fails += extremely_fertile_unicorn_tests();
  num_fails += magical_kittencorn_tests();
  num_fails += stabby_the_unicorn_tests();
  num_fails += puppicorn_tests();
  num_fails += rainbow_unicorn_tests();
  num_fails += zombie_unicorn_tests();
  num_fails += extremely_destructive_unicorn_tests();
  num_fails += chainsaw_unicorn_tests();
  num_fails += llamacorn_tests();
  num_fails += americorn_tests();
  num_fails += ginormous_unicorn_tests();
  num_fails += seductive_unicorn_tests();
  num_fails += angel_unicorn_tests();
  num_fails += queen_bee_unicorn_tests();
  num_fails += greedy_flying_unicorn_tests();
  num_fails += annoying_flying_unicorn_tests();
  num_fails += magical_flying_unicorn_tests();
  num_fails += swift_flying_unicorn_tests();
  num_fails += majestic_flying_unicorn_tests();
  num_fails += unicorn_phoenix_tests();
  num_fails += unicorn_on_the_cob_tests();
  num_fails += black_knight_unicorn_tests();
  num_fails += shark_with_a_horn_tests();
  num_fails += shabby_the_narwhal_tests();
  num_fails += narwhal_torpedo_tests();
  num_fails += alluring_narwhal_tests();
  num_fails += mermaid_unicorn_tests();
  num_fails += classy_narwhal_tests();
  num_fails += the_great_narwhal_tests();

  // magic cards; note that some tests use magicEffects instead of PlayCard,
  // so the magic cards don't always get discarded
  num_fails += unicorn_poison_tests();
  num_fails += back_kick_tests();
  num_fails += change_of_luck_tests();
  num_fails += glitter_tornado_tests();
  num_fails += unicorn_swap_tests();
  num_fails += re_target_tests();
  num_fails += unfair_bargain_tests();
  num_fails += two_for_one_tests();
  num_fails += unicorn_shrinkray_tests();
  num_fails += targeted_destruction_tests();
  num_fails += mystical_vortex_tests();
  num_fails += good_deal_tests();
  num_fails += shake_up_tests();
  num_fails += blatant_thievery_tests();
  num_fails += reset_button_tests();
  
  // upgrade cards
  num_fails += rainbow_mane_tests();
  num_fails += extra_tail_tests();
  num_fails += glitter_bomb_tests();
  num_fails += yay_tests();
  num_fails += unicorn_lasso_tests();
  num_fails += rainbow_aura_tests();
  num_fails += double_dutch_tests();
  num_fails += summoning_ritual_tests();
  
  // downgrade cards
  num_fails += barbed_wire_tests();
  num_fails += pandamonium_tests();
  num_fails += sadistic_ritual_tests();
  num_fails += slowdown_tests();
  num_fails += nanny_cam_tests();
  num_fails += broken_stable_tests();
  num_fails += blinding_light_tests();
  num_fails += tiny_stable_tests();
  
  // instant cards
  num_fails += neigh_tests();
  num_fails += super_neigh_tests();

  // ********************************************************************************
  // ***************************** Results and Clean-up *****************************
  // ********************************************************************************

  if (isClient) {
    char data[1024];
    int rc;
    while (rc = recv(sockfd, data, sizeof data, 0) > 0) {
      // loop until socket closes or server disconnects in some way
    }
    
    closesocket(sockfd);
    exit(1);
  }

  if (num_fails == 0) {
    Green();
    fprintf(stderr, "\nAll tests have successfully passed! :D\n");
    ResetCol();
  }
  else {
    Red();
    fprintf(stderr, "\nA total of %d tests have failed :(\n", num_fails);
    ResetCol();
  }

  // Close process and thread handles. 
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  closesocket(clientsockfd[0]);
  closesocket(sockfd);
  if (fp != NULL)
    fclose(fp);
  WSACleanup();

  (void)_getch();
  exit(0);
}
