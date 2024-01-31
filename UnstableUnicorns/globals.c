#include "globals.h"
#include "basedeck.h"

unsigned int isclient; // 0 = server, 1 = client

struct Deck deck;
struct Deck nursery;
struct Deck discardpile;
struct Player player[MAX_PLAYERS];
int current_players = 0; // TODO: change to unsigned someday.. there are too many comparisons to type cast
int turn_count = 0;      // number of moves during the action phase
int WIN_CONDITION = 7; // usually 7, but can be 6 with 6-8 players

unsigned char deck_flag = 0;    // toggles whether deck is printed out or not
unsigned char discard_flag = 0;
unsigned char nursery_flag = 0;

FILE* fpinput; // stand-in for stdin in case it needs to read from a file instead

int numinput(char* buf, char** end, int size) {
  int index;

  fgets(buf, size, fpinput);
  buf[strlen(buf) - 1] = 0;
  index = strtol(buf, end, 10);

  return index;
}

char charinput(char* buf, int size) {
  char c;

  fgets(buf, size, fpinput);
  c = buf[0];

  return c;
}

// in the future, use a flag for decks chosen
void init_deck(struct Deck *nursery, struct Deck *deck, struct Deck *discard) {
  int counter = 0;

  nursery->cards = malloc(NURSERY_SIZE * sizeof(struct Unicorn));
  nursery->size = NURSERY_SIZE;

  if (nursery->cards == NULL) {
    perror(NULL);
    exit(1);
  }

  for (int i = 0; i < NURSERY_SIZE; i++) {
    nursery->cards[i] = basedeck[i];
    nursery->cards[i].id = counter;
    counter++;
  }

  deck->cards = malloc(DECK_SIZE * sizeof(struct Unicorn));
  deck->size = DECK_SIZE;

  if (deck->cards == NULL) {
    perror(NULL);
    exit(1);
  }

  for (int i = 0; i < DECK_SIZE; i++) {
    deck->cards[i] = basedeck[i + NURSERY_SIZE];
    deck->cards[i].id = counter;
    counter++;
  }

  discard->cards = malloc(DECK_SIZE * sizeof(struct Unicorn));
  discard->size = 0;

  if (discard->cards == NULL) {
    perror(NULL);
    exit(1);
  }
}

// ********************************************************************************
// **************************** Pretty Color Functions ****************************
// ********************************************************************************

// https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
// add: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f
// undo: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000000 /f
void red(void) { printf("\033[1;31m"); }
void yellow(void) { printf("\033[1;33m"); }
void green(void) { printf("\033[1;32m"); }
void cyan(void) { printf("\033[1;36m"); }
void blue(void) { printf("\033[1;34m"); }
void magenta(void) { printf("\033[1;35m"); }
void reset_col(void) { printf("\033[0m"); }

void rainbow(char* str) {
  unsigned int color_index = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    if (color_index % 6 == 0)
      red();
    else if (color_index % 6 == 1)
      yellow();
    else if (color_index % 6 == 2)
      green();
    else if (color_index % 6 == 3)
      cyan();
    else if (color_index % 6 == 4)
      blue();
    else
      magenta();

    printf("%c", str[i]);

    if (str[i] != '\n' && str[i] != ' ') color_index++;
  }
  reset_col();
}
