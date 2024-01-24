// Unstable Unicorns - Online Edition!!
//
// http://collabedit.com/p9gs6
//
// Full inventory list -
// http://unstablegameswiki.com/index.php?title=Unstable_Unicorns_Base_Deck_-_Inventory_List
//
// Full card description list -
// http://unstablegameswiki.com/index.php?title=Unstable_Unicorns_-_1st_%26_2nd_Edition_Comparisons
//
// Conflicts and other things to look out for!
// - which should take priority when cards contradict each other (e.g. in an
// expansion pack, dragon slayer unicorn and dragon skies directly oppose each
// other, so would dragon slayer unicorn take priority or whichever came first?
// leaning towards the former in that case)
// - some cards have passive effects that may occur when it's not your turn
// (e.g. "barbed wire" activates anytime a card enters or leaves your stable);
// so how do you deal with that
// - loophole between the card "tiny stable" and any card that has an effect
// that negates sacrifices; tiny stable actually goes into effect immediately so
// it cannot be bypassed
// - synergy between unicorn shrinkray and barbed wire; you discard however many
// were in your stable
// - what happens if there are multiple cards with the condition that no one
// else can have [xx] (e.g. Queen Bee Unicorn); this is only if the deck is like
// doubled for whatever reason
// - should check for unicorn count every time something enters a stable instead
// of by the end of someone's turn; e.g. if somoene uses unicorn swap against
// someone with 6 unicorns, then they'd technically win the game in the middle
// of their turn. also this would get rid of the need to remove cards
// potentially at the end of your turn if you happen to have 7 unicorns anyways
// - what happens when you mix tiny stable (sacrifice a card if at any time you
// have more than 5 unicorns) and unicorn lasso or puppicorn where the acquired
// card is technically not yours; sacrificing someone elses card would be lame
// and puppicorn can't be sacrificed or destroyed
// - for glitter tornado, the user chooses the cards to return to each player's
// hand
//
// *** Don't forget to optimize and rewrite the heck out of this!
// *** Looks like effect #22 is blank
// *** Combine some functions together and maybe use more define variables to
// make the code clearer
// *** TODO (maybe?): move the potentially unplayable cards (i.e. the ones that
// increase turncount and move the card back to the bottom of the player's hand
// after being removed from the stable) to a different function that gets
// called before the hand is rearranged; this is so that it doesn't need to
// leave the hand and briefly enter the stable, and that may also cause a bug
// if it's a unicorn card and that would bring the number of unicorns up to 7
// if I ever put that in the addStable function
// *** TODO: Change all the "i = DESC_SIZE" or other type of constant
// assignments to breaks or returns instead
// *** TODO (maybe): replace "isvalid++" with "isvalid = 1" in most places tbh
// *** TODO: find a way to deal with edge cases in unicorn lasso where the
// beginning effect or action phase card has some way to sacrifice the stolen
// card, thus making it unable to be given back to the original owner (currently
// it would probably bug out and pass over an incorrect card from the "original
// index")

#include "client.h"
#include "server.h"
#include "unittests.h"

// ******************** Deck List and IDs ********************
// 1: Baby Unicorn (Red) [ID: 0]
// 2: Baby Unicorn (Pink) [ID: 1]
// 3: Baby Unicorn (Orange) [ID: 2]
// 4: Baby Unicorn (Yellow) [ID: 3]
// 5: Baby Unicorn (Green) [ID: 4]
// 6: Baby Unicorn (Blue) [ID: 5]
// 7: Baby Unicorn (Purple) [ID: 6]
// 8: Baby Unicorn (Black) [ID: 7]
// 9: Baby Unicorn (White) [ID: 8]
// 10: Baby Unicorn (Brown) [ID: 9]
// 11: Baby Unicorn (Rainbow) [ID: 10]
// 12: Baby Unicorn (Death) [ID: 11]
// 13: Baby Narwhal [ID: 12]
// 1: Basic Unicorn (Red) [ID: 13]
// 2: Basic Unicorn (Red) [ID: 14]
// 3: Basic Unicorn (Red) [ID: 15]
// 4: Basic Unicorn (Orange) [ID: 16]
// 5: Basic Unicorn (Orange) [ID: 17]
// 6: Basic Unicorn (Orange) [ID: 18]
// 7: Basic Unicorn (Yellow) [ID: 19]
// 8: Basic Unicorn (Yellow) [ID: 20]
// 9: Basic Unicorn (Yellow) [ID: 21]
// 10: Basic Unicorn (Green) [ID: 22]
// 11: Basic Unicorn (Green) [ID: 23]
// 12: Basic Unicorn (Green) [ID: 24]
// 13: Basic Unicorn (Blue) [ID: 25]
// 14: Basic Unicorn (Blue) [ID: 26]
// 15: Basic Unicorn (Blue) [ID: 27]
// 16: Basic Unicorn (Indigo) [ID: 28]
// 17: Basic Unicorn (Indigo) [ID: 29]
// 18: Basic Unicorn (Indigo) [ID: 30]
// 19: Basic Unicorn (Purple) [ID: 31]
// 20: Basic Unicorn (Purple) [ID: 32]
// 21: Basic Unicorn (Purple) [ID: 33]
// 22: Narwhal [ID: 34]
// 23: Narwhal [ID: 35]
// 24: Narwhal [ID: 36]
// 25: Rhinocorn [ID: 37]
// 26: Extremely Fertile Unicorn [ID: 38]
// 27: Magical Kittencorn [ID: 39]
// 28: Stabby the Unicorn [ID: 40]
// 29: Puppicorn [ID: 41]
// 30: Rainbow Unicorn [ID: 42]
// 31: Zombie Unicorn [ID: 43]
// 32: Extremely Destructive Unicorn [ID: 44]
// 33: Chainsaw Unicorn [ID: 45]
// 34: Llamacorn [ID: 46]
// 35: Americorn [ID: 47]
// 36: Ginormous Unicorn [ID: 48]
// 37: Seductive Unicorn [ID: 49]
// 38: Angel Unicorn [ID: 50]
// 39: Queen Bee Unicorn [ID: 51]
// 40: Greedy Flying Unicorn [ID: 52]
// 41: Annoying Flying Unicorn [ID: 53]
// 42: Magical Flying Unicorn [ID: 54]
// 43: Swift Flying Unicorn [ID: 55]
// 44: Majestic Flying Unicorn [ID: 56]
// 45: Unicorn Phoenix [ID: 57]
// 46: Unicorn on the Cob [ID: 58]
// 47: Black Knight Unicorn [ID: 59]
// 48: Shark With a Horn [ID: 60]
// 49: Shabby the Narwhal [ID: 61]
// 50: Narwhal Torpedo [ID: 62]
// 51: Alluring Narwhal [ID: 63]
// 52: Mermaid Unicorn [ID: 64]
// 53: Classy Narwhal [ID: 65]
// 54: The Great Narwhal [ID: 66]
// 55: Unicorn Poison [ID: 67]
// 56: Unicorn Poison [ID: 68]
// 57: Unicorn Poison [ID: 69]
// 58: Back Kick [ID: 70]
// 59: Back Kick [ID: 71]
// 60: Back Kick [ID: 72]
// 61: Change of Luck [ID: 73]
// 62: Change of Luck [ID: 74]
// 63: Glitter Tornado [ID: 75]
// 64: Glitter Tornado [ID: 76]
// 65: Unicorn Swap [ID: 77]
// 66: Unicorn Swap [ID: 78]
// 67: Re-Target [ID: 79]
// 68: Re-Target [ID: 80]
// 69: Unfair Bargain [ID: 81]
// 70: Unfair Bargain [ID: 82]
// 71: Two-For-One [ID: 83]
// 72: Two-For-One [ID: 84]
// 73: Unicorn Shrinkray [ID: 85]
// 74: Targeted Destruction [ID: 86]
// 75: Mystical Vortex [ID: 87]
// 76: Good Deal [ID: 88]
// 77: Shake Up [ID: 89]
// 78: Blatant Thievery [ID: 90]
// 79: Reset Button [ID: 91]
// 80: Rainbow Mane [ID: 92]
// 81: Rainbow Mane [ID: 93]
// 82: Rainbow Mane [ID: 94]
// 83: Extra Tail [ID: 95]
// 84: Extra Tail [ID: 96]
// 85: Extra Tail [ID: 97]
// 86: Glitter Bomb [ID: 98]
// 87: Glitter Bomb [ID: 99]
// 88: Yay [ID: 100]
// 89: Yay [ID: 101]
// 90: Unicorn Lasso [ID: 102]
// 91: Rainbow Aura [ID: 103]
// 92: Double Dutch [ID: 104]
// 93: Summoning Ritual [ID: 105]
// 94: Barbed Wire [ID: 106]
// 95: Pandamonium [ID: 107]
// 96: Sadistic Ritual [ID: 108]
// 97: Slowdown [ID: 109]
// 98: Nanny Cam [ID: 110]
// 99: Broken Stable [ID: 111]
// 100: Blinding Light [ID: 112]
// 101: Tiny Stable [ID: 113]
// 102: Neigh [ID: 114]
// 103: Neigh [ID: 115]
// 104: Neigh [ID: 116]
// 105: Neigh [ID: 117]
// 106: Neigh [ID: 118]
// 107: Neigh [ID: 119]
// 108: Neigh [ID: 120]
// 109: Neigh [ID: 121]
// 110: Neigh [ID: 122]
// 111: Neigh [ID: 123]
// 112: Neigh [ID: 124]
// 113: Neigh [ID: 125]
// 114: Neigh [ID: 126]
// 115: Neigh [ID: 127]
// 116: Super Neigh [ID: 128]

int main(int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "deck") == 0) deck_flag = 1;
    if (strcmp(argv[i], "discard") == 0) discard_flag = 1;
    if (strcmp(argv[i], "nursery") == 0) nursery_flag = 1;
    if (strcmp(argv[i], "debug") == 0) {
      // run a bunch of tests!
      // https://cmocka.org/talks/cmocka_unit_testing_and_mocking.pdf

      // FILE *fp = freopen("test.txt", "r", stdin);

      exit(0);
    }
  }

  // unit testing :')
  int result;
  char buf[LINE_MAX];
  
  do {
    printf("Enter \"test\" or \"1\" if you would like to perform unit tests, or\n"
      "Enter \"game\" or \"2\" if you wish to start a real game\n"
      "Option: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
  } while (strncmp(buf, "test", 4) != 0 && strncmp(buf, "game", 4) != 0 &&
    strncmp(buf, "1", 1) != 0 && strncmp(buf, "2", 1) != 0);

  if (strncmp(buf, "test", 4) == 0 || strncmp(buf, "1", 1) == 0) {
    if ((result = test_main()) == 0) {
      green();
      printf("\nAll tests have successfully passed! :D\n");
      reset_col();
    }
    else {
      printf("\nA total of %d tests have failed :(\n", result);
    }

    exit(0);
  }

  // create a log file for all input
  /*char filename[40];
  struct tm* timenow;
  time_t now = time(NULL);
  timenow = gmtime(&now);

  strftime(filename, sizeof(filename), "/log/GAME_%Y-%m-%d_%H:%M:%S", timenow);

  fopen(filename, "w");*/

  //HANDLE new_stdout = CreateFileA("");
  //SetStdHandle(STD_OUTPUT_HANDLE, new_stdout);
  //int fd = _open_osfhandle(new_stdout, O_WRONGLY | O_TEXT);
  //dup2(fd, STDOUT_FILENO);
  //close(fd);

  // seed for randomized pulls
  srand((unsigned int)time(NULL));

  // *****************************************************
  // ***************** Networking Set-up *****************
  // *****************************************************
  WSADATA wsa;

  // set up winsock with TDP communication (SOCK_STREAM) on IPv4 (AF_INET)
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("Failed. Error Code : %d", WSAGetLastError());
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    printf("Could not create socket : %d", WSAGetLastError());
    exit(1);
  }

  rainbow("Welcome to Unstable Unicorns for the command line!\n");

  do {
    printf("\nEnter \"host\" or \"1\" if you would like to host your own game, or\n"
           "Enter \"join\" or \"2\" if you would like to join an existing host\n"
           "Option: ");
    fgets(buf, sizeof buf, stdin);
    buf[strlen(buf) - 1] = 0;
  } while (strncmp(buf, "host", 4) != 0 && strncmp(buf, "join", 4) != 0 &&
           strncmp(buf, "1", 1) != 0 && strncmp(buf, "2", 1) != 0);

  if (strncmp(buf, "host", 4) == 0 || strncmp(buf, "1", 1) == 0) {
    if ((result = serverMain()) != 0) {
      exit(result);
    }
  } else {
    if ((result = clientMain()) != 0) {
      exit(result);
    }
  }

  closesocket(sockfd);
  WSACleanup();

  exit(0);
}
