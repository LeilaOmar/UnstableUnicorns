#pragma once
#define WINDOWS_IGNORE_PACKING_MISMATCH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib") // Winsock library

#define NAME_SIZE 33
#define DESC_SIZE 256
#define LINE_MAX 32
#define BUF_SIZE 1024
#define HAND_START 5
#define HAND_SIZE 16
#define MAX_PLAYERS 8
#define STABLE_SIZE 25

// in the future, these would be changed to global ints composed of all chosen decks;
// the packet structures would have to allocate space at runtime
#define DECK_SIZE 116
#define NURSERY_SIZE 13

enum UnicornDatatypes {ANY = -1, BABYUNICORN, BASICUNICORN, MAGICUNICORN, UPGRADE, DOWNGRADE, MAGIC, INSTANT, ANYUNICORN, UNICORN, NARWHAL, NOSPECIES};

struct Unicorn {
  unsigned char cType;    // 0 = Baby Unicorn
                          // 1 = Basic Unicorn
                          // 2 = Magic Unicorn
                          // 3 = Upgrade
                          // 4 = Downgrade
                          // 5 = Magic
                          // 6 = Instant
  unsigned char species;  // 8 = Unicorn
                          // 9 = Narwhal
                          // 10 = Other or N/A [for now...]
  char name[NAME_SIZE];
  char description[DESC_SIZE];
  short effect;
  int id;
};

struct Deck {
  struct Unicorn* cards;
  int size; // this should never exceed the max, so no there's point in a max variable for reallocating
};

struct CardsInHand {
  struct Unicorn cards[HAND_SIZE];
  int num_cards;
};

struct Stable {
  struct Unicorn unicorns[STABLE_SIZE];
  int num_unicorns;
  int size;
};

struct Player {
  struct CardsInHand hand;
  struct Stable stable;
  char username[NAME_SIZE];
  enum BabySelection icon;
  short flags;  // enum baseflag
  short filler; // padding for prev variable
};

SOCKET clientsockfd[MAX_PLAYERS - 1];
SOCKET sockfd;
extern int isclient; // 0 = server, 1 = client
extern int network_events;

extern struct Deck deck;
extern struct Deck nursery;
extern struct Deck discardpile;
extern struct Player player[MAX_PLAYERS];
extern int current_players;
extern int turn_count;
extern int WIN_CONDITION;

// toggles whether the deck piles are printed out or not
extern unsigned char deck_flag;
extern unsigned char discard_flag;
extern unsigned char nursery_flag;

extern FILE* fpinput; // stand-in for stdin in case it needs to read from a file instead
int numinput(char*, char**, int);
char charinput(char*, int);

void init_deck(struct Deck* nursery, struct Deck* deck, struct Deck* discard);

// closes sockets and frees memory
void cleanup(void);

// ********************************************************************************
// **************************** Pretty Color Functions ****************************
// ********************************************************************************

// https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
// add: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f
// undo: reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000000 /f
void red(void);
void yellow(void);
void green(void);
void cyan(void);
void blue(void);
void magenta(void);
void reset_col(void);
void rainbow(char* str);
